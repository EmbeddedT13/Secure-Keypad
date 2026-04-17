# Secure Keypad (STM32)

This project implements a secure keypad system governed by a precise Mealy State Machine. The system is built from scratch using bare-metal C programming on an STM32 microcontroller, featuring modular peripheral drivers and asynchronous event handling via hardware interrupts.

## 🛠️ Hardware Components
- **Microcontroller**: STM32F401 (ARM Cortex-M4)
- **Input**: 
  - 4x4 Matrix Keypad (GPIO polling)
- **External Buttons (EXTI)**:
  - **Emergency Reset**: A high-priority external interrupt that forces the system out of the alarm state and resets the terminal.
  - **Doorbell**: An external interrupt that triggers a momentary action (toggling a doorbell LED) without disrupting the current state of the keypad lock.
- **Output Displays**:
  - **7-Segment Display**: Used strictly to display the current number of failed unlock attempts (0 to 10).
  - **LED Bar**: Visualizes the progress of the 4-digit unlocking sequence.
  - **Status LEDs**: Dedicated indicators for `Success` (Unlocked) and `Alarm` states.

## ⚙️ Software Architecture
The system's core behavior is strictly controlled by a **Mealy State Machine**, meaning outputs and transitions depend on both the current state and current inputs. 

### System States
1. **LOCKED (Initial State)**: 
   - Awaits the correct 4-digit sequence.
   - Updates the LED progress bar upon each valid keypress.
   - If an invalid sequence is detected, the failure count on the 7-segment display increments. Reaching the lockout threshold (10 failures) transitions the system to the `ALARM` state.

2. **UNLOCKED**: 
   - Entered upon successful sequence input. 
   - Pressing the lock command (`*`) transitions the system back to `LOCKED`.

3. **ALARM**: 
   - System locks up and ignores all keypad input. 
   - Only the physical Emergency Reset button can clear the sequence progress, reset the failure history, and transition the system back to `LOCKED`.

## 🧱 Codebase Structure
The project is built with a layered architecture to ensure modularity:

```
core/        -> Standard types and bit-math macros
mcal/        -> Microcontroller Abstraction Layer (RCC, GPIO, NVIC, EXTI)
hal/         -> Hardware Abstraction Layer (Keypad, 7-Segment, LED)
app/         -> Core application logic (State Machine)
startup.c    -> Custom startup code
stm32_ls.ld  -> Linker script for bare-metal memory initialization
```

## 🚀 Build Instructions
The project uses a custom `Makefile` and requires the `arm-none-eabi-gcc` toolchain.

1. Ensure the ARM GNU Toolchain is installed and added to your system path.  
2. Open a terminal in the root directory of the project.  
3. Run the build command to compile the source code and generate the executable:

```bash
make
```

The output `stm32-sec6.hex` will be generated in the root directory, ready to be flashed to the physical board or simulated in Proteus.

To clean up the build artifacts:

```bash
make clean
```

## 👥 Authors

Developed by our two-person team as part of the Systems and Biomedical Engineering program at Cairo University:

- Ahmed Mohamed Abdelsalam
- Ziad Ashraf Abdo
