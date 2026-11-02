# Moore v2 — next phase

The v1 project (C++ / wxWidgets, described in `docs/thesis.pdf`) proved that every
part works: motors, interferometer, scales, Cary, environmental instruments, camera,
and the approach / caryFlex / findMax routines. It is still not a tool that can do a
full, unattended calibration measurement. v2 is a restart. It keeps v1 as a working
reference and changes the language split and the code structure.

**v2 goal:** measure a ring and a sphere diameter end to end (setup → probe
calibration on the gauge block → n repetitions → report with uncertainty) with
reproducible results, recovery after an interruption, and simple testing of each
component.

Baseline from v1 (thesis, ch. 4), which v2 must at least match:

| Quantity | v1 result |
|---|---|
| X / Y positioning error (max) | 5.5 µm / 1.7 µm |
| Z positioning error (max) | 30 µm (belt backlash) |
| Diameter uncertainty, 20 mm sample | U ≈ 100 nm, CMC Q[0.1; 0.5·10⁻³ L] µm |

### Decisions taken

| Topic | Decision |
|---|---|
| Restart | Same repo, v1 tagged and moved to `legacy/` (§1) |
| Position / speed control | Stays on the PC, in a C++ DLL (§2) |
| Everything else | Python |
| GUI | PySide6 + pyqtgraph |
| PCB | Unchanged for now, not part of this phase |
| Homing at startup | No: the scales have no reference marks, and moving to the origin is too slow. Use saved position instead (§7.2) |
| Pinouts (Z driver, switches, e-stop) | Provided during development |
| Old GPIB laser (`Laser` class, `XPOS?`) | Dismantled, not ported |
| Machine position at startup | Saved position is only an initial guess; the exact position is always re-established by touching the block or workpiece side (§7.2) |
| Calibration certificate output | Out of scope for now, last issue to address |

---

## 1. Restarting without losing history

1. Tag the current state so it can always be checked out:
   `git tag v1-thesis` (and push the tag).
2. Create a branch `v2`. On it, `git mv` the existing code into `legacy/`
   (`legacy/src`, `legacy/tests`, `legacy/firmware`, ...). `git log --follow`
   still shows the full history of every file.
3. Build the new structure (section 4) next to `legacy/`. Old code stays one click
   away while porting, and many C++ classes are moved straight into the new DLL.
4. Merge `v2` into `master` once v2 can move the machine safely (end of Phase 3).
5. Delete `legacy/` when nothing references it any more. The tag keeps it forever.

Housekeeping to do during the restart:
- Remove `dllbkp/` and the generated `docs/html/` from git. Vendor DLLs go in a
  documented install step, and Doxygen output is a build product. The wx libraries
  are no longer needed.
- Decide whether `docs/thesis.pdf` (21 MB) is committed or kept outside the repo.

---

## 2. Language split

The position loop needs the laser and the optical scales, and those can only be read
through vendor DLLs on the PC. So **speed and position control stay on the PC**, as in
v1 (which already moved three axes at once). The STM32 stays a step generator with
safety added.

| Layer | Language | Responsibility |
|---|---|---|
| Firmware (STM32) | C | Step/dir generation at the commanded speed, safety inputs, watchdog, step counters |
| **Motion core DLL** (`moore_core.dll`) | C++ (std + Win32 + vendor DLLs only) | Owns the STM32 serial port and the position instruments (laser, scales, Cary, CHRocodile). Position polling thread, ramps, closed-loop moves per axis, approach on probe reading |
| Python package | Python | Wraps the DLL with `ctypes`. Environment instruments, frames and maths, measurement procedures, storage |
| GUI | PySide6 + pyqtgraph | Client of the Python package only |

### Why the position instruments live in the DLL
- The control loop reads them every few milliseconds, so the reads must happen in the
  same C++ thread, without going through Python.
- A device should have **one owner**. Python never opens the laser or the IK220
  directly. It asks the DLL (`mc_get_position`, `mc_read_probe`), so readings for
  measurements and for control come from the same source.
- These are the v1 classes that already work: `Asse`, `Moore` (motion part only),
  `keysight`, `Scale` / `IkOptical`, `cary`, `CHRocodile`, `SimpleSerial`. They are
  ported, not rewritten.

### DLL rules
- **C API** (`extern "C"`), so Python can call it with `ctypes`. C++ classes stay
  inside. No external C++ libraries (no wx, no OpenCV, no matplotlibcpp).
- **64-bit build**, to match 64-bit Python. All vendor DLLs exist in 64-bit
  (`E1735A.dll`, `IK220Dll64.dll`, `CHRocodileDLL.dll`).
- **Non-blocking moves.** `mc_move_to` starts the axis threads and returns, and Python
  polls `mc_status` or calls `mc_wait`. This keeps the GUI responsive and allows a
  stop at any time.
- Errors are returned as codes plus `mc_last_error(buf)`, never as C++ exceptions
  crossing the DLL boundary.

API sketch (to be refined during development):

```c
int    mc_open(const char* config_path);       // connects STM32 + position instruments
void   mc_close(void);

int    mc_get_position(double xyz[3]);         // latest value from the polling thread [mm]
int    mc_set_position(const double xyz[3]);   // preset counters (used for state restore, §7.2)
int    mc_set_env(double T, double P, double RH); // laser wavelength compensation

int    mc_set_ramp(int axis, double acc, double v_start, double v_max, double v_stop);
int    mc_move_to(const double xyz[3]);        // all three axes in parallel, returns at once
int    mc_move_axis(int axis, double target, int sensor); // sensor: ENCODER / CARY / CHR
int    mc_jog(int axis, double speed);         // manual / keyboard control
int    mc_stop(int axis);                      // -1 = all axes
int    mc_wait(int timeout_ms);
int    mc_status(mc_status_t* s);              // moving flags, faults, safety inputs

double mc_read_probe(int sensor, int n_avg);   // averaged Cary / CHR / laser reading
int    mc_last_error(char* buf, int len);
```

### Python instrument interfaces (outside the DLL)

| Instrument | v2 |
|---|---|
| Fluke Super-DAQ (LAN): temperatures | `pyvisa` TCPIP socket (or plain `socket`) |
| Agilent 3458A (GPIB): pressure, dew point | `pyvisa` with the NI-VISA backend |
| Nilox webcam | `opencv-python` |

These are slow (readings every few minutes), so Python is fine for them.

---

## 3. Firmware v2

The firmware keeps the v1 model: **the PC sets the speed and the micro generates the
steps**. What changes is what it does when something goes wrong.

### Protocol
- Raise the baud rate from 9600 to **115200**. At 9600 one command (~9 characters)
  takes ~9 ms, almost the whole 10 ms update period, and three axes queue up. At
  115200 it takes <1 ms. This is a firmware setting plus one line in the DLL.
- Keep it **ASCII and line-based**, so it can be debugged with any serial terminal.
- **Every command gets a reply** (`OK` / `ERR <code>`). A reply costs ~0.3 ms at 115200.
- Example command set:

```
SPD <axis> <±speed>       set speed (sign = direction), 0 = stop
EN  <axis> <0|1>          enable / free-wheel (AWO)
RES <axis> <0|1>          step resolution select (CS input)
STOP <axis|ALL>           stop immediately
CNT? <axis>               step counter since last reset
STAT?                     per axis: moving / idle / fault, limits, e-stop
RESET                     clear a latched fault (explicit, from the PC)
PING                      heartbeat
```

### Safety and robustness (handled in firmware, independent of the PC)
- **Limit switches** (rewired from the old setup), **Cary over-travel pins** and the
  **red e-stop button** go to EXTI interrupt inputs.
- Any trigger immediately stops step generation (e-stop: all axes) and **latches a
  fault**. `SPD` is refused until the PC sends `RESET` and the input has cleared.
  Moving *away* from a limit is allowed so the axis can be freed.
- Wire the switches **normally closed**, so a broken cable reads as "triggered".
- The e-stop should also act **in hardware** (for example cutting driver power or
  forcing AWO), not only through firmware.
- **Watchdog:** if the PC sends no command for more than ~200 ms while an axis is
  moving (program crash, USB unplugged), the board stops all axes.
- **Acceleration limit:** if the PC asks for a speed jump larger than a configured
  maximum (for example after a PC hiccup), the firmware ramps to it instead of
  jumping. This prevents lost steps when the PC loop is late.
- **Step counter** per axis. The DLL compares steps with the encoder movement to
  detect **stalls or lost steps** (for example the new motors lacking torque).

### Board
- No PCB changes in this phase. The firmware is adapted to the existing board.
- **Per-axis configuration table** (pins, polarity, step resolution, max speed):
  Z keeps the old motor and driver with a different pinout, while X and Y use the new
  Oriental drivers. Pinouts will be provided during development.
- Later improvement (not now): a 32-bit timer for a better speed range and resolution
  (thesis §2.4.2).

---

## 4. Code structure

```
moore/                      repository root
├── firmware/               STM32 project (C)
├── core/                   C++ motion core → moore_core.dll
│   ├── include/moore_core.h   the C API (the only header Python needs)
│   ├── src/                axis, machine, serial, keysight, ik220/scale, cary, chrocodile
│   ├── vendor/             vendor headers (E1735A.h, DllFunc.h, ...)
│   ├── tests/              small C++ exe per device, same idea as v1 tests
│   └── Makefile            64-bit MinGW build
├── moore/                  Python package
│   ├── hw/
│   │   ├── core.py         ctypes wrapper of moore_core.dll
│   │   ├── fluke.py  voltmeter.py  camera.py
│   │   ├── base.py         interfaces: MotionCore, EnvSensor, Camera
│   │   └── sim.py          simulated MotionCore / sensors for working without the machine
│   ├── geometry/           frames and transforms, fits (max finding, circle/sphere fit)
│   ├── env/                environment monitor, Edlén refractive index, thermal correction
│   ├── measure/            procedures: base Measurement, gauge block, sphere, ring, cylinder, confocal scan
│   ├── store/              result files, measurement journal, machine state, database
│   └── gui/                PySide6 + pyqtgraph
├── config/
│   └── machine.toml        COM ports, IPs, steps/mm, signal periods, ramp defaults,
│                           calibration constants (PRESSURE_A/B, HUMIDITY_A/B, ...), safe Z
├── tests/
│   ├── unit/               pytest, runs anywhere using hw/sim.py
│   └── hw/                 one script per instrument / feature on the real machine
├── docs/
└── legacy/                 v1 code (temporary)
```

Design rules:
- **Every instrument can be tested on its own.** There are C++ test executables in
  `core/tests/` for each device inside the DLL, and Python scripts in `tests/hw/` for
  each instrument and each feature (one axis move, approach, caryFlex, findMax, ...).
  When something breaks, test it piece by piece.
- **Simulator.** `hw/sim.py` implements the same interface as `hw/core.py` (fake
  axes that move over time, fake Cary contact). Procedures and the GUI can be
  developed at the desk, and `pytest` runs without the machine.
- **No hard-coded constants.** Everything machine-specific goes in
  `config/machine.toml`, read by both Python and the DLL (`mc_open(path)`). v1 has
  these as `#define`s spread across headers.
- **GUI is a client only.** Every procedure can run from a plain Python script. The
  GUI calls the same functions. Long procedures run in a worker thread and send
  progress to the GUI through Qt signals.
- **Procedures are lists of steps.** A measurement is a sequence of named steps
  (approach, scan, read, ...) whose results are written as they happen. This is what
  makes resuming possible (§7.2) and should be built in from the start.
- **Units:** millimetres and seconds everywhere. Convert only at the instrument
  boundary (steps, µm, laser counts) and in the GUI.

---

## 5. Maths

`pos.h` is replaced by **numpy** on the Python side. The DLL only needs plain
`double[3]` for targets and positions, so no C++ maths library is needed.
- Points are `np.ndarray` of shape `(3,)`. Point sets (grids, scans) are `(N, 3)`, so
  operations are vectorised.
- **Coordinate frames** are 4×4 homogeneous matrices in a small `Frame` class
  (`to_machine(p)`, `from_machine(p)`, compose, invert). The machine frame is the
  encoder frame, and the DLL only ever sees machine coordinates. Workpiece frames are
  defined by probing or by the camera.
- Rotations use `scipy.spatial.transform.Rotation`.
- Fits use numpy/scipy: linear regression for caryFlex (`np.polyfit` plus residuals,
  which v1 computed but didn't use), parabola or peak fit for findMax (more robust
  than averaging the samples at the max reading), least-squares circle/sphere fit for
  ring and cylinder measurements.
- Uncertainty: keep the GUM budget as a function, and use `monaco` (already used in the
  thesis) for Monte Carlo.

---

## 6. Hardware

### New motors (X, Y): bench test before mounting
Expected figures (to verify):
- Resolution: 0.72° full step / 30 = 0.024° per step → 15 000 full steps/rev; with a
  3 mm screw pitch → **0.2 µm per full step** (finer with microstepping).
- Speed: the 30:1 gearbox makes rapid moves slow. For example, 1000 rpm at the motor →
  33 rpm at the screw → **1.7 mm/s**. Measure the actual top speed under load and
  check whether long travels (~300 mm) are acceptable.

Test plan:
1. Print the adapter and mount one motor on X.
2. Check torque margin: find the speed at which it stalls, in both directions, across
   the full travel. The firmware step counter compared with the laser shows lost steps.
3. Check steps/mm against the interferometer over a long move.
4. Check backlash: approach the same point from both sides and read the laser.

### Wiring tasks
- Limit switches: move them from the old setup to the STM32 (NC wiring).
- Cary safety-stop pins → STM32.
- Red e-stop button → STM32 *and* a hardware cut (see §3).
- Z axis: keep the old motor and driver, and add it to the firmware axis table.

---

## 7. Higher-level features (after the basics work)

### 7.1 Database
Use **SQLite** (built into Python, a single file, no server).
- Tables: `artefacts` (gauge blocks, spheres, rings, with nominal values, material and
  CTE), `probes`, `calibrations` (date, values, uncertainty, linked artefact or probe,
  linked measurement), `measurements` (procedure, operator, environment summary,
  result, path to the raw data).
- Raw data (scans, readings) stays in files. The DB only indexes them.
- Use case: pick the gauge block and probe from a list and reuse the last probe
  calibration if it is recent enough.

### 7.2 State saving and recovery
The scales have no reference marks and moving to an origin at every start is too
slow. The motors are not kept enabled while the program is off, so a saved position
cannot be trusted as exact. The approach is therefore: **the saved position is an
initial guess, and the exact position is always re-established by touching the
block or the workpiece side.** The point of the guess is that the operator does not
have to drive the machine there by hand every time.

- **Machine state file** (`state.json`): last position (xyz), the time it was saved,
  defined workpiece frames (block, workpiece), current probe calibration. It is
  written after every completed move and periodically while idle. It is written
  atomically (write a temp file, then rename), so a crash never leaves it half written.
- **Startup:** call `mc_set_position(saved)` to preset the laser and scale counters
  with the last known position. Frames and contact points from the state file are
  now approximately right.
- **Re-referencing routine:** move to a safe Z, go to the saved contact point on the
  block (or workpiece side) with a margin, then approach until contact with the Cary.
  The contact gives the exact X. Y/Z are refined the same way (findMax on the
  workpiece, or contact on the reference faces). Frames are then corrected by the
  measured offset. The margin must cover how far the axes can drift while the motors
  are off (to be measured: hand-turned knobs, belt relaxation on Z).
- **Laser beam interruption during a run:** the laser count is lost but Y/Z are not.
  The DLL detects it (E1735A error / NaN) and stops the measurement. X is recovered
  with the same re-referencing routine on the block, then the measurement resumes.
- **Resuming a measurement:** each completed step is appended to a journal (JSON
  lines) with its results. On restart the program offers to continue from the last
  completed step instead of starting a multi-hour run from scratch.

### 7.3 Webcam click-to-move
Replace ArUco size-based scaling with:
1. **Intrinsic calibration once** (chessboard), saved to a file (`camera_params_*.yml`
   already exists from v1).
2. **Pixel → machine XY mapping by moving the machine.** Track one feature in the
   undistorted image while moving X and Y by known amounts, then fit scale and rotation
   (or a homography with `cv2.findHomography`). This needs no marker of known size and
   is redone in seconds if the camera is moved.
3. **Live view in the GUI** (a Qt widget showing the OpenCV frames): click on a
   point → compute the XY offset from the reference pixel (probe position) → move.
   Like ArUco, this is only a coarse positioning; precise contact is still made with
   the approach routine.

### 7.4 Other future work (from the thesis)
- Cylinder measurement routines.
- Optical microscope instead of the Cary for optical scale calibration.
- Z belt retensioning routine (or approach Z always from the same direction to avoid
  backlash).
- 32-bit timer / PCB revision.
- Calibration certificate output (format to be defined). This is planned as the last item.
- Time a full measurement and compare it with the old system.

---

## 8. Roadmap

Each phase ends with a check that can be run, so progress is measurable.

| Phase | Content | Done when |
|---|---|---|
| 0. Restart | Tag, `v2` branch, `legacy/`, folder skeleton, `machine.toml`, Python simulator | `pytest` runs on simulated hardware |
| 1. Firmware + new motors | 115200 protocol with replies, safety inputs, watchdog, acceleration limit, step counters; bench test of the new motors | All three axes run with `SPD` from a serial terminal; every safety input and the watchdog stop motion and latch |
| 2. Motion core DLL | Port v1 device classes + Asse/Moore motion into `core/`, C API, C++ device tests | Each device reads correctly from its own test exe; one axis moves to a target from a C++ test |
| 3. Python link | `hw/core.py` (ctypes), `tests/hw/` scripts, env instruments, Edlén | 125-point grid test repeated from Python: X/Y ≤ v1 values |
| 4. Measurement core | Frames, approach, caryFlex, findMax, probe calibration on gauge block, sphere, ring; journal per step; state file | Calibrated sphere and a ring reproduce known values within uncertainty |
| 5. GUI | PySide6: position/jog panel, environment panel, measurement setup, pyqtgraph live plots | A measurement runs from the GUI |
| 6. Extras | Resume after crash, database, webcam click-to-move, cylinders, microscope | Each feature tested on its own |

---

## 9. Open questions

None at the moment. The earlier ones (old laser, holding current, certificate
output) are answered in the "Decisions taken" table at the top.
