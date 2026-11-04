"""Bench GUI for the Moore v2 motor firmware.

Sets the step rate of the three axes over the ST-LINK virtual COM port and shows
the resulting motor, screw and axis speed using config/machine.toml.

    python tests/hw/stm32_speed_gui.py          real board
    python tests/hw/stm32_speed_gui.py --sim    simulated board, no hardware

The protocol is described in firmware/README.md.
"""
from __future__ import annotations

import argparse
import html
import math
import sys
import time
import tomllib
from collections import deque
from dataclasses import dataclass, field
from pathlib import Path

from PySide6 import QtCore, QtGui, QtWidgets
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo
import pyqtgraph as pg

ROOT = Path(__file__).resolve().parents[2]
CONFIG_PATH = ROOT / "config" / "machine.toml"

AXES = ("x", "y", "z")
AXIS_COLORS = {"x": "#ef5350", "y": "#66bb6a", "z": "#42a5f5"}
FAULT_BITS = ("ESTOP", "LIM X+", "LIM X-", "LIM Y+", "LIM Y-", "LIM Z+", "LIM Z-",
              "CARY A", "CARY B", "WATCHDOG")
FLAG_WINDINGS, FLAG_CS, FLAG_MOVING = 1, 2, 4
FIRMWARE_MAX_HZ = 40000.0

STATUS_PERIOD_MS = 100   # status poll, also keeps the firmware watchdog alive
SEND_PERIOD_MS = 25      # speed updates are sent at most this often
PLOT_SECONDS = 30
SLIDER_STEPS = 1000


# ---- Configuration and status ---------------------------------------------------

@dataclass
class AxisConfig:
    name: str
    max_step_hz: float
    ppr_cs_off: float
    ppr_cs_on: float
    gear_ratio: float
    mm_per_rev: float
    has_cs: bool

    @classmethod
    def from_dict(cls, name: str, d: dict) -> "AxisConfig":
        return cls(
            name=name,
            max_step_hz=min(float(d["max_step_hz"]), FIRMWARE_MAX_HZ),
            ppr_cs_off=float(d["pulses_per_rev_cs_off"]),
            ppr_cs_on=float(d.get("pulses_per_rev_cs_on", d["pulses_per_rev_cs_off"])),
            gear_ratio=float(d.get("gear_ratio", 1.0)),
            mm_per_rev=float(d.get("mm_per_rev", 0.0)),
            has_cs=bool(d.get("has_cs", False)),
        )

    def ppr(self, cs: bool) -> float:
        return self.ppr_cs_on if cs else self.ppr_cs_off

    def motor_rpm(self, hz: float, cs: bool) -> float:
        return hz / self.ppr(cs) * 60.0

    def screw_rpm(self, hz: float, cs: bool) -> float:
        return self.motor_rpm(hz, cs) / self.gear_ratio

    def mm_per_step(self, cs: bool) -> float:
        if self.mm_per_rev <= 0:
            return math.nan
        return self.mm_per_rev / (self.ppr(cs) * self.gear_ratio)


def load_config(path: Path) -> tuple[dict, dict[str, AxisConfig]]:
    with open(path, "rb") as f:
        data = tomllib.load(f)
    return data.get("stm32", {}), {a: AxisConfig.from_dict(a, data["axis"][a]) for a in AXES}


@dataclass
class AxisStatus:
    hz: float = 0.0
    steps: int = 0
    flags: int = FLAG_WINDINGS

    @property
    def windings(self) -> bool:
        return bool(self.flags & FLAG_WINDINGS)

    @property
    def cs(self) -> bool:
        return bool(self.flags & FLAG_CS)

    @property
    def moving(self) -> bool:
        return bool(self.flags & FLAG_MOVING)


@dataclass
class BoardStatus:
    faults: int
    inputs: int
    axes: dict[str, AxisStatus] = field(default_factory=dict)


def parse_status(line: str) -> BoardStatus | None:
    """Parses 'S <faults> <inputs>' followed by '<hz> <steps> <flags>' per axis."""
    t = line.split()
    if len(t) != 3 + 3 * len(AXES) or t[0] != "S":
        return None
    try:
        axes = {a: AxisStatus(float(t[3 + 3 * i]), int(t[4 + 3 * i]), int(t[5 + 3 * i]))
                for i, a in enumerate(AXES)}
        return BoardStatus(int(t[1], 16), int(t[2], 16), axes)
    except ValueError:
        return None


def bit_names(mask: int) -> list[str]:
    return [name for i, name in enumerate(FAULT_BITS) if mask >> i & 1]


# ---- Links: real serial port and simulated board ----------------------------------

class SerialLink(QtCore.QObject):
    line_received = QtCore.Signal(str)
    error = QtCore.Signal(str)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._port = QSerialPort(self)
        self._port.readyRead.connect(self._on_ready_read)
        self._port.errorOccurred.connect(self._on_error)
        self._buf = b""

    def open(self, name: str, baudrate: int) -> bool:
        self._port.setPortName(name)
        self._port.setBaudRate(baudrate)
        self._buf = b""
        if not self._port.open(QtCore.QIODevice.OpenModeFlag.ReadWrite):
            self.error.emit(f"Cannot open {name}: {self._port.errorString()}")
            return False
        return True

    def close(self):
        if self._port.isOpen():
            self._port.close()

    def is_open(self) -> bool:
        return self._port.isOpen()

    def send(self, line: str):
        if self._port.isOpen():
            self._port.write((line + "\n").encode("ascii"))

    def _on_ready_read(self):
        self._buf += bytes(self._port.readAll().data())
        *lines, self._buf = self._buf.split(b"\n")
        for raw in lines:
            text = raw.decode("ascii", "replace").strip()
            if text:
                self.line_received.emit(text)

    def _on_error(self, err):
        if err == QSerialPort.SerialPortError.NoError:
            return
        self.error.emit(self._port.errorString())
        if err == QSerialPort.SerialPortError.ResourceError:  # cable unplugged
            self.close()


class SimBoard:
    """Python model of the firmware protocol, for working without hardware."""

    WATCHDOG_S = 0.3

    def __init__(self, axes_cfg: dict[str, AxisConfig]):
        self.has_cs = {a: c.has_cs for a, c in axes_cfg.items()}
        self.target = dict.fromkeys(AXES, 0.0)
        self.current = dict.fromkeys(AXES, 0.0)
        self.accel = dict.fromkeys(AXES, 0.0)
        self.steps = dict.fromkeys(AXES, 0.0)
        self.windings = dict.fromkeys(AXES, True)
        self.cs = dict.fromkeys(AXES, False)
        self.faults = 0
        self._reported = 0
        self._idle = 0.0

    def _moving(self, a: str) -> bool:
        return self.target[a] != 0 or self.current[a] != 0

    def handle(self, line: str) -> list[str]:
        self._idle = 0.0
        t = line.split()
        if not t:
            return []
        try:
            reply = self._dispatch(t[0].upper(), t[1:])
        except (ValueError, IndexError, KeyError):
            reply = "SYNTAX"
        if isinstance(reply, list):
            return reply
        return [f"!E {reply} {line}"] if reply else []

    def _axis(self, args: list[str], n: int) -> str:
        if len(args) != n:
            raise ValueError
        a = args[0].lower()
        if a not in AXES:
            raise KeyError
        return a

    def _set_speed(self, a: str, hz: float) -> str | None:
        if not math.isfinite(hz) or abs(hz) > FIRMWARE_MAX_HZ:
            return "RANGE"
        if hz and self.faults:
            return "FAULT"
        if hz and not self.windings[a]:
            return "DISABLED"
        self.target[a] = hz
        return None

    def _dispatch(self, cmd: str, args: list[str]):
        if cmd == "V":
            if args and args[0].lower() in AXES:
                a = self._axis(args, 2)
                return self._set_speed(a, float(args[1]))
            if len(args) != 3:
                raise ValueError
            errors = [self._set_speed(a, float(v)) for a, v in zip(AXES, args)]
            return next((e for e in errors if e), None)
        if cmd in ("E", "C"):
            a = self._axis(args, 2)
            if cmd == "C" and not self.has_cs[a]:
                return "UNSUPPORTED"
            if self._moving(a):
                return "BUSY"
            (self.windings if cmd == "E" else self.cs)[a] = int(args[1]) != 0
            return None
        if cmd == "A":
            a = self._axis(args, 2)
            acc = float(args[1])
            if acc < 0:
                return "RANGE"
            self.accel[a] = acc
            return None
        if cmd == "Z":
            self.steps[self._axis(args, 1)] = 0.0
            return None
        if args:
            raise ValueError
        if cmd == "S":
            self._stop_all()
            return None
        if cmd == "R":
            self.faults = 0
            return None
        if cmd == "?":
            return [self.status_line()]
        if cmd == "I":
            return ["I MOORE-FW SIMULATOR"]
        return "SYNTAX"

    def _stop_all(self):
        for a in AXES:
            self.target[a] = self.current[a] = 0.0

    def tick(self, dt: float) -> list[str]:
        self._idle += dt
        if any(self._moving(a) for a in AXES) and self._idle > self.WATCHDOG_S:
            self.faults |= 1 << FAULT_BITS.index("WATCHDOG")
            self._stop_all()
        for a in AXES:
            tgt = 0.0 if self.faults or not self.windings[a] else self.target[a]
            cur = self.current[a]
            if cur and tgt and (cur > 0) != (tgt > 0):
                tgt = 0.0
            if self.accel[a] > 0:
                dv = self.accel[a] * dt
                cur = min(max(tgt, cur - dv), cur + dv)
            else:
                cur = tgt
            self.current[a] = cur
            if abs(cur) >= 0.5:
                self.steps[a] += cur * dt
        if self.faults != self._reported:
            self._reported = self.faults
            return [f"!F {self.faults:04X}"]
        return []

    def status_line(self) -> str:
        parts = [f"S {self.faults:04X} 0000"]
        for a in AXES:
            flags = (FLAG_WINDINGS * self.windings[a] | FLAG_CS * self.cs[a]
                     | FLAG_MOVING * (abs(self.current[a]) >= 0.5))
            parts.append(f"{self.current[a]:.2f} {int(self.steps[a])} {flags}")
        return " ".join(parts)


class SimLink(QtCore.QObject):
    line_received = QtCore.Signal(str)
    error = QtCore.Signal(str)

    def __init__(self, axes_cfg: dict[str, AxisConfig], parent=None):
        super().__init__(parent)
        self._board = SimBoard(axes_cfg)
        self._open = False
        self._last = time.monotonic()
        self._timer = QtCore.QTimer(self, interval=10)
        self._timer.timeout.connect(self._tick)

    def open(self, name: str, baudrate: int) -> bool:
        self._open = True
        self._last = time.monotonic()
        self._timer.start()
        self._emit_later(["I MOORE-FW SIMULATOR"])
        return True

    def close(self):
        self._open = False
        self._timer.stop()

    def is_open(self) -> bool:
        return self._open

    def send(self, line: str):
        if self._open:
            self._emit_later(self._board.handle(line.strip()))

    def _tick(self):
        now = time.monotonic()
        lines = self._board.tick(now - self._last)
        self._last = now
        self._emit_later(lines)

    def _emit_later(self, lines: list[str]):
        for text in lines:
            QtCore.QTimer.singleShot(1, lambda t=text: self.line_received.emit(t))


# ---- Widgets -------------------------------------------------------------------------

def fmt(value: float, decimals: int = 1) -> str:
    if value is None or not math.isfinite(value):
        return "—"
    return f"{value:,.{decimals}f}".replace(",", " ")


class AxisPanel(QtWidgets.QFrame):
    speed_requested = QtCore.Signal(str, float)
    windings_requested = QtCore.Signal(str, bool)
    cs_requested = QtCore.Signal(str, bool)
    accel_requested = QtCore.Signal(str, float)
    stop_requested = QtCore.Signal(str)
    zero_requested = QtCore.Signal(str)

    def __init__(self, cfg: AxisConfig, parent=None):
        super().__init__(parent)
        self.cfg = cfg
        self.color = AXIS_COLORS[cfg.name]
        self.status = AxisStatus()
        self.travel_mm = 0.0
        self._last_steps: int | None = None
        self._syncing = False
        self.deadman = True
        self.setObjectName("axisCard")

        title = QtWidgets.QLabel(cfg.name.upper(), objectName="axisTitle")
        title.setStyleSheet(f"color: {self.color}")
        self.badge = QtWidgets.QLabel("—", objectName="badge")
        self.badge.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
        header = QtWidgets.QHBoxLayout()
        header.addWidget(title)
        header.addStretch()
        header.addWidget(self.badge, 0, QtCore.Qt.AlignmentFlag.AlignVCenter)

        grid = QtWidgets.QGridLayout()
        grid.setHorizontalSpacing(10)
        grid.setVerticalSpacing(2)
        self.v_rate = self._readout(grid, 0, 0, "Step rate", "steps/s", big=True)
        self.v_axis = self._readout(grid, 0, 1, "Axis speed", "mm/s", big=True)
        self.v_motor = self._readout(grid, 2, 0, "Motor", "rpm")
        self.v_screw = self._readout(grid, 2, 1, "Output", "rpm")
        self.v_steps = self._readout(grid, 4, 0, "Step counter", "steps")
        self.v_travel = self._readout(grid, 4, 1, "Travel", "mm")
        self.v_res = QtWidgets.QLabel(objectName="caption")

        self.slider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal)
        self.slider.setRange(-SLIDER_STEPS, SLIDER_STEPS)
        self.slider.setStyleSheet(
            f"QSlider::handle:horizontal {{ background: {self.color}; width: 18px;"
            f" margin: -7px 0; border-radius: 9px; }}"
            f"QSlider::sub-page:horizontal {{ background: #2a2f38; border-radius: 3px; }}")
        self.slider.valueChanged.connect(self._on_slider)
        self.slider.sliderReleased.connect(self._on_slider_released)

        self.spin = QtWidgets.QDoubleSpinBox()
        self.spin.setRange(-cfg.max_step_hz, cfg.max_step_hz)
        self.spin.setDecimals(1)
        self.spin.setSingleStep(10)
        self.spin.setSuffix(" steps/s")
        self.spin.setKeyboardTracking(False)
        self.spin.valueChanged.connect(self._on_spin)
        btn_zero_speed = QtWidgets.QPushButton("0")
        btn_zero_speed.setFixedWidth(36)
        btn_zero_speed.clicked.connect(lambda: self.set_command(0.0))
        speed_row = QtWidgets.QHBoxLayout()
        speed_row.addWidget(self.spin, 1)
        speed_row.addWidget(btn_zero_speed)

        self.btn_windings = QtWidgets.QPushButton("Windings", checkable=True)
        self.btn_windings.setToolTip("On: motor holds position. Off: shaft free (AWO / enable)")
        self.btn_windings.clicked.connect(lambda on: self.windings_requested.emit(cfg.name, on))
        self.btn_cs = QtWidgets.QPushButton("CS", checkable=True)
        self.btn_cs.setEnabled(cfg.has_cs)
        self.btn_cs.setToolTip("Selects the CS step resolution (only while stopped)")
        self.btn_cs.clicked.connect(lambda on: self.cs_requested.emit(cfg.name, on))
        self.accel = QtWidgets.QDoubleSpinBox()
        self.accel.setRange(0, 1e6)
        self.accel.setDecimals(0)
        self.accel.setSingleStep(100)
        self.accel.setSpecialValueText("no ramp")
        self.accel.setSuffix(" steps/s²")
        self.accel.setToolTip("Acceleration limit applied by the firmware")
        self.accel.editingFinished.connect(
            lambda: self.accel_requested.emit(cfg.name, self.accel.value()))
        toggles = QtWidgets.QHBoxLayout()
        toggles.addWidget(self.btn_windings)
        toggles.addWidget(self.btn_cs)
        toggles.addWidget(self.accel, 1)

        btn_stop = QtWidgets.QPushButton("Stop", objectName="axisStop")
        btn_stop.clicked.connect(lambda: self.stop_requested.emit(cfg.name))
        btn_reset = QtWidgets.QPushButton("Zero counter")
        btn_reset.clicked.connect(lambda: self.zero_requested.emit(cfg.name))
        actions = QtWidgets.QHBoxLayout()
        actions.addWidget(btn_stop, 1)
        actions.addWidget(btn_reset, 1)

        lay = QtWidgets.QVBoxLayout(self)
        lay.setContentsMargins(16, 12, 16, 14)
        lay.setSpacing(8)
        lay.addLayout(header)
        lay.addLayout(grid)
        lay.addWidget(self.v_res)
        lay.addSpacing(4)
        lay.addWidget(self.slider)
        lay.addLayout(speed_row)
        lay.addLayout(toggles)
        lay.addLayout(actions)
        self.update_status(self.status)

    @staticmethod
    def _readout(grid, row, col, caption, unit, big=False):
        grid.addWidget(QtWidgets.QLabel(caption, objectName="caption"), row, col)
        value = QtWidgets.QLabel("—", objectName="valueBig" if big else "value")
        box = QtWidgets.QHBoxLayout()
        box.setSpacing(4)
        box.addWidget(value)
        box.addWidget(QtWidgets.QLabel(unit, objectName="unit"),
                      0, QtCore.Qt.AlignmentFlag.AlignBottom)
        box.addStretch()
        grid.addLayout(box, row + 1, col)
        return value

    # slider is quadratic: fine control near zero, full range at the ends
    def _slider_to_hz(self, v: int) -> float:
        x = v / SLIDER_STEPS
        return math.copysign(self.cfg.max_step_hz * x * x, x)

    def _hz_to_slider(self, hz: float) -> int:
        x = math.sqrt(min(abs(hz) / self.cfg.max_step_hz, 1.0))
        return int(round(math.copysign(x * SLIDER_STEPS, hz)))

    def _show_command(self, hz: float):
        self._syncing = True
        self.slider.setValue(self._hz_to_slider(hz))
        self.spin.setValue(hz)
        self._syncing = False

    def set_command(self, hz: float, emit: bool = True):
        self._show_command(hz)
        if emit:
            self.speed_requested.emit(self.cfg.name, hz)

    def _on_slider(self, v: int):
        if self._syncing:
            return
        hz = round(self._slider_to_hz(v), 1)
        self._syncing = True
        self.spin.setValue(hz)
        self._syncing = False
        self.speed_requested.emit(self.cfg.name, hz)

    def _on_slider_released(self):
        if self.deadman:
            self.set_command(0.0)

    def _on_spin(self, hz: float):
        if self._syncing:
            return
        self._syncing = True
        self.slider.setValue(self._hz_to_slider(hz))
        self._syncing = False
        self.speed_requested.emit(self.cfg.name, hz)

    def reset_travel(self):
        self.travel_mm = 0.0
        self._last_steps = None

    def update_status(self, st: AxisStatus):
        cfg = self.cfg
        if self._last_steps is not None:
            step_mm = cfg.mm_per_step(st.cs)
            if math.isfinite(step_mm):
                self.travel_mm += (st.steps - self._last_steps) * step_mm
        self._last_steps = st.steps
        self.status = st

        self.v_rate.setText(fmt(st.hz, 1))
        self.v_axis.setText(fmt(st.hz * cfg.mm_per_step(st.cs), 3))
        self.v_motor.setText(fmt(cfg.motor_rpm(st.hz, st.cs), 1))
        self.v_screw.setText(fmt(cfg.screw_rpm(st.hz, st.cs), 2))
        self.v_steps.setText(fmt(st.steps, 0))
        self.v_travel.setText(fmt(self.travel_mm if cfg.mm_per_rev > 0 else math.nan, 4))
        step_um = cfg.mm_per_step(st.cs) * 1000
        res = f"{cfg.ppr(st.cs):g} P/rev · gear {cfg.gear_ratio:g}:1"
        if math.isfinite(step_um):
            res += f" · {step_um:.3f} µm/step"
        self.v_res.setText(res + (" · CS on" if st.cs else ""))

        self.btn_windings.setChecked(st.windings)
        self.btn_windings.setText("Windings ON" if st.windings else "Windings OFF")
        self.btn_cs.setChecked(st.cs)
        if not st.windings:
            text, bg = "FREE", "#4a4f5a"
        elif st.moving:
            text, bg = "MOVING", self.color
        else:
            text, bg = "HOLD", "#2a2f38"
        self.badge.setText(text)
        self.badge.setStyleSheet(f"background: {bg};")


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, link, stm_cfg: dict, axes_cfg: dict[str, AxisConfig], sim: bool):
        super().__init__()
        self.link = link
        self.stm_cfg = stm_cfg
        self.axes_cfg = axes_cfg
        self.sim = sim
        self._pending: dict[str, float] = {}
        self._t0 = time.monotonic()
        self.setWindowTitle("Moore · STM32 motor bench" + (" (simulator)" if sim else ""))
        self.resize(1280, 860)

        # top bar
        self.port_combo = QtWidgets.QComboBox()
        self.port_combo.setMinimumWidth(320)
        btn_refresh = QtWidgets.QPushButton("↻")
        btn_refresh.setFixedWidth(36)
        btn_refresh.setToolTip("Refresh port list")
        btn_refresh.clicked.connect(self._refresh_ports)
        self.btn_connect = QtWidgets.QPushButton("Connect", objectName="connect")
        self.btn_connect.clicked.connect(self._toggle_connection)
        self.fw_label = QtWidgets.QLabel("not connected", objectName="caption")
        self.chk_deadman = QtWidgets.QCheckBox("Sliders return to 0 on release")
        self.chk_deadman.setChecked(True)
        self.chk_deadman.toggled.connect(self._set_deadman)
        self.fault_chip = QtWidgets.QLabel(objectName="chip")
        btn_reset = QtWidgets.QPushButton("Reset faults")
        btn_reset.clicked.connect(lambda: self._send("R"))
        self.btn_stop_all = QtWidgets.QPushButton("STOP ALL  (Esc)", objectName="stopAll")
        self.btn_stop_all.clicked.connect(self.stop_all)

        top = QtWidgets.QHBoxLayout()
        top.addWidget(self.port_combo)
        top.addWidget(btn_refresh)
        top.addWidget(self.btn_connect)
        top.addSpacing(8)
        top.addWidget(self.fw_label)
        top.addStretch()
        top.addWidget(self.chk_deadman)
        top.addSpacing(12)
        top.addWidget(self.fault_chip)
        top.addWidget(btn_reset)
        top.addWidget(self.btn_stop_all)

        # axis cards
        self.panels: dict[str, AxisPanel] = {}
        cards = QtWidgets.QHBoxLayout()
        cards.setSpacing(12)
        for a in AXES:
            p = AxisPanel(axes_cfg[a])
            p.speed_requested.connect(self._queue_speed)
            p.windings_requested.connect(lambda ax, on: self._send(f"E {ax} {int(on)}"))
            p.cs_requested.connect(lambda ax, on: self._send(f"C {ax} {int(on)}"))
            p.accel_requested.connect(lambda ax, v: self._send(f"A {ax} {v:.0f}"))
            p.stop_requested.connect(self._stop_axis)
            p.zero_requested.connect(self._zero_axis)
            self.panels[a] = p
            cards.addWidget(p, 1)

        # live plot of the motor speed
        self.plot = pg.PlotWidget()
        self.plot.setBackground("#1d2128")
        self.plot.showGrid(x=True, y=True, alpha=0.15)
        self.plot.setLabel("left", "Motor speed", units="rpm")
        self.plot.setLabel("bottom", "Time", units="s")
        self.plot.addLegend(offset=(10, 10))
        self.plot.setMouseEnabled(x=False, y=True)
        maxlen = PLOT_SECONDS * 1000 // STATUS_PERIOD_MS + 10
        self._t = deque(maxlen=maxlen)
        self._rpm = {a: deque(maxlen=maxlen) for a in AXES}
        self.curves = {a: self.plot.plot(pen=pg.mkPen(AXIS_COLORS[a], width=2), name=a.upper())
                       for a in AXES}

        # console
        self.log = QtWidgets.QPlainTextEdit(readOnly=True)
        self.log.setMaximumBlockCount(3000)
        self.cmd_line = QtWidgets.QLineEdit(placeholderText="Raw command, e.g.  V x 1000   or   ?")
        self.cmd_line.returnPressed.connect(self._send_raw)
        self.chk_status_log = QtWidgets.QCheckBox("Show status polling")
        cmd_row = QtWidgets.QHBoxLayout()
        cmd_row.addWidget(self.cmd_line, 1)
        cmd_row.addWidget(self.chk_status_log)
        console = QtWidgets.QWidget()
        console_lay = QtWidgets.QVBoxLayout(console)
        console_lay.setContentsMargins(0, 0, 0, 0)
        console_lay.addWidget(self.log)
        console_lay.addLayout(cmd_row)

        splitter = QtWidgets.QSplitter(QtCore.Qt.Orientation.Vertical)
        splitter.addWidget(self.plot)
        splitter.addWidget(console)
        splitter.setSizes([300, 180])

        central = QtWidgets.QWidget()
        lay = QtWidgets.QVBoxLayout(central)
        lay.setContentsMargins(14, 12, 14, 12)
        lay.setSpacing(12)
        lay.addLayout(top)
        lay.addLayout(cards)
        lay.addWidget(splitter, 1)
        self.setCentralWidget(central)

        # timers and shortcuts
        self.status_timer = QtCore.QTimer(self, interval=STATUS_PERIOD_MS)
        self.status_timer.timeout.connect(lambda: self._send("?", log=False))
        self.send_timer = QtCore.QTimer(self, interval=SEND_PERIOD_MS)
        self.send_timer.timeout.connect(self._flush_speeds)
        QtGui.QShortcut(QtGui.QKeySequence(QtCore.Qt.Key.Key_Escape), self,
                        activated=self.stop_all, context=QtCore.Qt.ShortcutContext.ApplicationShortcut)

        self.link.line_received.connect(self._on_line)
        self.link.error.connect(lambda msg: self._log(msg, "#ef5350"))
        self._show_faults(0, 0)
        self._refresh_ports()
        self._set_connected(False)

    # ---- connection

    def _refresh_ports(self):
        self.port_combo.clear()
        if self.sim:
            self.port_combo.addItem("Simulated board", "SIM")
            return
        wanted = self.stm_cfg.get("port", "")
        best = -1
        for info in QSerialPortInfo.availablePorts():
            label = f"{info.portName()} — {info.description() or 'serial port'}"
            self.port_combo.addItem(label, info.portName())
            desc = f"{info.description()} {info.manufacturer()}".lower()
            if info.portName() == wanted or (best < 0 and ("stlink" in desc or "stmicro" in desc)):
                best = self.port_combo.count() - 1
        if best >= 0:
            self.port_combo.setCurrentIndex(best)
        if self.port_combo.count() == 0:
            self.port_combo.addItem("no serial ports found", None)

    def _toggle_connection(self):
        if self.link.is_open():
            self._send("S")
            self.link.close()
            self._set_connected(False)
            self._log("disconnected", "#8a93a3")
            return
        port = self.port_combo.currentData()
        if not port:
            return
        if self.link.open(port, int(self.stm_cfg.get("baudrate", 115200))):
            self._log(f"connected to {port}", "#8a93a3")
            self._set_connected(True)
            self._send("I")
            for a, p in self.panels.items():
                p.reset_travel()
                self._send(f"A {a} {p.accel.value():.0f}")

    def _set_connected(self, on: bool):
        self.btn_connect.setText("Disconnect" if on else "Connect")
        self.btn_connect.setProperty("connected", on)
        self.btn_connect.style().polish(self.btn_connect)
        self.port_combo.setEnabled(not on)
        for p in self.panels.values():
            p.setEnabled(on)
        if on:
            self.status_timer.start()
            self.send_timer.start()
        else:
            self.status_timer.stop()
            self.send_timer.stop()
            self.fw_label.setText("not connected")
            for p in self.panels.values():
                p.set_command(0.0, emit=False)

    # ---- sending

    def _send(self, line: str, log: bool = True):
        if not self.link.is_open():
            return
        self.link.send(line)
        if log or self.chk_status_log.isChecked():
            self._log(f"→ {line}", "#8ab4f8")

    def _queue_speed(self, axis: str, hz: float):
        self._pending[axis] = hz

    def _flush_speeds(self):
        pending, self._pending = self._pending, {}
        for axis, hz in pending.items():
            self._send(f"V {axis} {hz:.2f}")

    def _stop_axis(self, axis: str):
        self._pending.pop(axis, None)
        self.panels[axis].set_command(0.0, emit=False)
        self._send(f"V {axis} 0")

    def _zero_axis(self, axis: str):
        self._send(f"Z {axis}")
        self.panels[axis].reset_travel()

    def stop_all(self):
        self._pending.clear()
        for p in self.panels.values():
            p.set_command(0.0, emit=False)
        self._send("S")

    def _send_raw(self):
        text = self.cmd_line.text().strip()
        if text:
            self._send(text)
            self.cmd_line.clear()

    def _set_deadman(self, on: bool):
        for p in self.panels.values():
            p.deadman = on

    # ---- receiving

    def _on_line(self, line: str):
        if line.startswith("S "):
            st = parse_status(line)
            if st:
                self._apply_status(st)
            if self.chk_status_log.isChecked():
                self._log(f"← {line}", "#8a93a3")
        elif line.startswith("!E"):
            self._log(f"← {line}", "#ef5350")
            self.statusBar().showMessage(line, 5000)
        elif line.startswith("!F"):
            self._log(f"← {line}", "#ffb74d")
            try:
                self._show_faults(int(line.split()[1], 16), None)
            except (IndexError, ValueError):
                pass
        elif line.startswith("I "):
            self.fw_label.setText(line[2:])
            self._log(f"← {line}", "#c5e1a5")
        else:
            self._log(f"← {line}", "#e6e9ef")

    def _apply_status(self, st: BoardStatus):
        self._show_faults(st.faults, st.inputs)
        t = time.monotonic() - self._t0
        self._t.append(t)
        for a, p in self.panels.items():
            ax = st.axes[a]
            p.update_status(ax)
            self._rpm[a].append(self.axes_cfg[a].motor_rpm(ax.hz, ax.cs))
            self.curves[a].setData(list(self._t), list(self._rpm[a]))
        self.plot.setXRange(max(0.0, t - PLOT_SECONDS), max(t, PLOT_SECONDS), padding=0)

    def _show_faults(self, faults: int, inputs: int | None):
        if faults and not self.fault_chip.property("fault"):
            # the board has stopped everything: don't leave stale commands on screen
            self._pending.clear()
            for p in self.panels.values():
                p.set_command(0.0, emit=False)
        if faults:
            self.fault_chip.setText("FAULT · " + ", ".join(bit_names(faults)))
            self.fault_chip.setProperty("fault", True)
        else:
            self.fault_chip.setText("NO FAULTS")
            self.fault_chip.setProperty("fault", False)
        if inputs is not None:
            active = bit_names(inputs)
            self.fault_chip.setToolTip("Active inputs: " + (", ".join(active) if active else "none"))
        self.fault_chip.style().polish(self.fault_chip)

    def _log(self, text: str, color: str):
        stamp = time.strftime("%H:%M:%S")
        self.log.appendHtml(f'<span style="color:#59606d">{stamp}</span> '
                            f'<span style="color:{color}">{html.escape(text)}</span>')

    def closeEvent(self, event):
        if self.link.is_open():
            self._send("S")
            self.link.close()
        super().closeEvent(event)


STYLE = """
QWidget { background: #15181d; color: #e6e9ef; font-family: "Segoe UI"; font-size: 10pt; }
QMainWindow, QStatusBar { background: #15181d; }
QFrame#axisCard { background: #1d2128; border: 1px solid #2a2f38; border-radius: 12px; }
QFrame#axisCard QLabel, QFrame#axisCard QWidget { background: transparent; }
QLabel#axisTitle { font-size: 28pt; font-weight: 700; }
QLabel#badge { padding: 3px 12px; border-radius: 10px; font-weight: 600; font-size: 9pt; }
QLabel#caption { color: #8a93a3; font-size: 9pt; }
QLabel#unit { color: #8a93a3; font-size: 9pt; padding-bottom: 3px; }
QLabel#value { font-family: "Consolas"; font-size: 14pt; }
QLabel#valueBig { font-family: "Consolas"; font-size: 20pt; font-weight: 600; }
QLabel#chip { padding: 6px 12px; border-radius: 8px; font-weight: 600; background: #1b5e20; }
QLabel#chip[fault="true"] { background: #b71c1c; }
QPushButton { background: #2a2f38; border: 1px solid #353b46; border-radius: 6px; padding: 6px 12px; }
QPushButton:hover { background: #333944; }
QPushButton:pressed { background: #3b4250; }
QPushButton:checked { background: #2e7d32; border-color: #43a047; }
QPushButton:disabled { color: #59606d; background: #20242b; }
QPushButton#connect[connected="true"] { background: #1565c0; border-color: #1e88e5; }
QPushButton#axisStop { background: #3e2723; border-color: #6d4c41; }
QPushButton#axisStop:hover { background: #4e342e; }
QPushButton#stopAll { background: #c62828; border-color: #e53935; font-weight: 700;
                      font-size: 11pt; padding: 8px 22px; }
QPushButton#stopAll:hover { background: #d32f2f; }
QSlider::groove:horizontal { height: 6px; background: #2a2f38; border-radius: 3px; }
QDoubleSpinBox, QComboBox, QLineEdit, QPlainTextEdit {
    background: #11141a; border: 1px solid #2a2f38; border-radius: 6px; padding: 4px; }
QPlainTextEdit { font-family: "Consolas"; font-size: 9pt; }
QComboBox QAbstractItemView { background: #1d2128; selection-background-color: #2a2f38; }
QSplitter::handle { background: #15181d; height: 8px; }
QToolTip { background: #1d2128; color: #e6e9ef; border: 1px solid #353b46; }
"""


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--sim", action="store_true", help="use a simulated board")
    parser.add_argument("--config", type=Path, default=CONFIG_PATH, help="machine.toml path")
    args = parser.parse_args()

    stm_cfg, axes_cfg = load_config(args.config)
    app = QtWidgets.QApplication(sys.argv)
    app.setStyle("Fusion")
    app.setStyleSheet(STYLE)
    pg.setConfigOptions(antialias=True, foreground="#8a93a3")
    link = SimLink(axes_cfg) if args.sim else SerialLink()
    win = MainWindow(link, stm_cfg, axes_cfg, sim=args.sim)
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
