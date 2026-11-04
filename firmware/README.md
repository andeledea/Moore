# Moore v2 firmware (NUCLEO-G491RE)

The STM32 generates the step signals for the three axes at the speed sent by the
PC, and handles the safety inputs. The position loop stays on the PC (see
`docs/next_steps.md`).

- X, Y: new Oriental Motor 5-phase motors, CVD drivers (PLS, DIR, AWO, CS)
- Z: old motor and driver (step, dir, enable), same signals as the v1 firmware

Code: `Core/Src/motion.c` (step generation, ramps, safety), `Core/Src/protocol.c`
(serial commands), hooks in the `USER CODE` sections of `Core/Src/main.c`.

## Pinout

All pins are on the Arduino headers, except LIM_Z_POS and the two Cary inputs
(morpho CN10 / CN7).
Labels must match exactly: the code uses the `<LABEL>_Pin` / `<LABEL>_GPIO_Port`
macros generated in `main.h`.

### Outputs

| Label   | Pin  | Header | CubeMX mode                  | Function |
|---------|------|--------|------------------------------|----------|
| X_STEP  | PA0  | A0     | TIM2_CH1 (Output Compare)    | X pulse (PLS) |
| Y_STEP  | PA1  | A1     | TIM2_CH2 (Output Compare)    | Y pulse (PLS) |
| Z_STEP  | PB10 | D6     | TIM2_CH3 (Output Compare)    | Z step |
| X_DIR   | PA4  | A2     | GPIO_Output                  | X direction |
| X_AWO   | PB0  | A3     | GPIO_Output                  | X all windings off |
| X_CS    | PC1  | A4     | GPIO_Output                  | X step resolution select |
| Y_DIR   | PA10 | D2     | GPIO_Output                  | Y direction |
| Y_AWO   | PB5  | D4     | GPIO_Output                  | Y all windings off |
| Y_CS    | PB4  | D5     | GPIO_Output                  | Y step resolution select |
| Z_DIR   | PA8  | D7     | GPIO_Output                  | Z direction |
| Z_ENA   | PA9  | D8     | GPIO_Output                  | Z enable (windings off when high, as v1) |
| LED     | PA5  | D13    | GPIO_Output                  | Green LED LD2: on = idle, slow blink = moving, fast blink = fault |

GPIO outputs: push-pull, no pull, low speed, initial level low.

### Inputs

| Label     | Pin  | Header   | CubeMX mode | Function |
|-----------|------|----------|-------------|----------|
| BTN       | PC13 | B1       | GPIO_Input, no pull | Blue button: stops all axes while pressed |
| ESTOP     | PC7  | D9       | GPIO_Input, pull-up | Red emergency button |
| LIM_X_POS | PB6  | D10      | GPIO_Input, pull-up | X limit, positive end |
| LIM_X_NEG | PA7  | D11      | GPIO_Input, pull-up | X limit, negative end |
| LIM_Y_POS | PA6  | D12      | GPIO_Input, pull-up | Y limit, positive end |
| LIM_Y_NEG | PB9  | D14      | GPIO_Input, pull-up | Y limit, negative end |
| LIM_Z_POS | PC8  | CN10 (2) | GPIO_Input, pull-up | Z limit, positive end (not PB8: it is BOOT0 at reset) |
| LIM_Z_NEG | PC0  | A5       | GPIO_Input, pull-up | Z limit, negative end |
| CARY_A    | PC2  | CN7 (35) | GPIO_Input, pull-up | Cary safety stop 1 |
| CARY_B    | PC3  | CN7 (37) | GPIO_Input, pull-up | Cary safety stop 2 |

Safety contacts are wired **normally closed to GND**: closed = OK (pin low), open
or broken cable = tripped (pin high). Inputs are polled every 1 ms, so they don't
need EXTI. Until they are wired, they are ignored: enable each one in
`SAFETY_INPUTS_ENABLED` in `motion.c` once its switch is connected.

Free pins kept for later: PB3 (D3, SWO), PC4/PC5 (D1/D0), PB11..PB15, PC6, PC9..PC12.
Avoid PB8 (D15) for inputs: it is sampled as BOOT0 at reset.

Check every pin against CubeMX (it flags conflicts) and the Nucleo user manual
UM2505 when wiring. In CubeMX, ctrl+click on a pin shows the alternatives if a
TIM2 channel is placed on a different pin by default.

### Signal levels

The Nucleo outputs are 3.3 V push-pull. **Check the CVD driver input
specification before connecting:** if the inputs need 5 V to switch the
photocouplers reliably, put a 5 V buffer in between (for example a 74HCT541
powered at 5 V). Don't use open-drain with pull-ups to 5 V on these pins: several
of them (PA0, PA1, PA4, ...) are not 5 V tolerant.

Active levels are defined at the top of `motion.c` (`XY_DIR_POS_LEVEL`,
`XY_AWO_OFF_LEVEL`, `XY_CS_ON_LEVEL`, `Z_DIR_POS_LEVEL`, `Z_ENA_OFF_LEVEL`) and are
set once the wiring is known.

## CubeMX (.ioc) settings

1. **BSP**: disable the NUCLEO-G491RE BSP component (LD2, BUTTON, VCP). The LED,
   button and LPUART1 are configured as plain peripherals below, so the code
   doesn't depend on the BSP.
2. **LPUART1**: Asynchronous on PA2 (TX) / PA3 (RX), 115200 baud, 8N1, no flow
   control. NVIC: LPUART1 global interrupt enabled.
3. **TIM2**: clock source internal. Channel 1, 2, 3 = *Output Compare CH1/2/3*.
   - Prescaler 0, Counter Period 4294967295, auto-reload preload disabled.
   - Each channel: Mode *Toggle on match*, Pulse 0, output compare preload
     disabled, polarity High.
   - NVIC: TIM2 global interrupt enabled.
4. **TIM6**: activated, Prescaler 169, Counter Period 999 (1 kHz at 170 MHz).
   NVIC: TIM6 global interrupt (TIM6_DAC) enabled.
5. **GPIO**: outputs and inputs as in the tables above, with the labels.
6. **NVIC priorities** (preemption): TIM2 = 0, TIM6 = 1, LPUART1 = 2.
7. Clock: keep SYSCLK at 170 MHz (already set).

Then generate code and build. Only the `USER CODE` sections of `main.c` contain
project code, so regenerating is safe.

## Serial protocol

ST-LINK virtual COM port, 115200 8N1. One command per line, terminated by CR
and/or LF. Axis is `x`, `y` or `z`; speeds are step pulses per second, signed
(sign = direction).

**Set commands are silent.** The board doesn't answer them, so the PC can send
speed updates in a stream without waiting. It only answers queries, and it
reports problems on its own:

- `!E <ERROR> <command>`: a command was rejected (the command is echoed)
- `!F <mask>`: the fault mask changed (hex)

| Command | Meaning |
|---------|---------|
| `V <axis> <hz>` | Set the step rate of one axis, e.g. `V x 1500` or `V z -200.5` |
| `V <hx> <hy> <hz>` | Set all three axes in one line |
| `E <axis> <0\|1>` | Windings off (0, AWO / old enable) or on (1). Refused while moving |
| `C <axis> <0\|1>` | CS off (0) or on (1). Refused while moving. X and Y only |
| `A <axis> <steps/s²>` | Acceleration limit applied by the firmware, 0 = off (speed changes immediately, as v1) |
| `S` | Stop all axes immediately |
| `R` | Clear latched faults (refused while the e-stop is pressed) |
| `Z <axis>` | Reset the step counter |
| `?` | Status, answer: `S <faults> <inputs> <hz> <steps> <flags>` × X, Y, Z |
| `I` | Identification, answer: `I MOORE-FW 2.0.0` (also sent at boot) |

Status flags per axis: 1 = windings on, 2 = CS on, 4 = moving.

Fault / input bits: 0 ESTOP, 1 LIM_X_POS, 2 LIM_X_NEG, 3 LIM_Y_POS, 4 LIM_Y_NEG,
5 LIM_Z_POS, 6 LIM_Z_NEG, 7 CARY_A, 8 CARY_B, 9 WATCHDOG.

Errors: `SYNTAX`, `AXIS`, `BUSY`, `FAULT`, `LIMIT`, `DISABLED`, `RANGE`, `ESTOP`,
`UNSUPPORTED`.

### Behaviour

- **Speed range**: up to 40 000 steps/s per axis; below 0.5 steps/s the output
  stops. The timer runs at 170 MHz with 32 bits, so the resolution is better than
  0.03 % even at the top speed.
- **Direction change**: the axis goes through zero speed, DIR changes while
  stopped, and stepping restarts after 1 ms.
- **Safety**: an input that trips latches a fault and stops all axes immediately.
  Speeds other than 0 are refused until `R`. While a limit input is active, motion
  towards it is refused, but moving away is allowed.
- **Watchdog**: if no line arrives for 300 ms while an axis is moving, all axes
  stop and the `WATCHDOG` fault latches. A PC that streams speeds or polls `?`
  keeps it alive.
- **Bandwidth**: `V x 12345.67` is 13 bytes, about 1.1 ms at 115200. One `V` line
  with all three axes every 10 ms uses about a quarter of the link. If more is
  needed, raise the LPUART1 baud rate (the ST-LINK V3 supports higher rates).
