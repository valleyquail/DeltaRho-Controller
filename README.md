This is the repository that houses the code for an Omni Wheel Delta Robot. The robot uses a Pico W along with a motor
shield. The documentation for the shield can be found at:

* https://www.waveshare.com/wiki/Pico-Motor-Driver

## Building

To set up the repository, the following libraries are needed:
FreeRTOS SMP Branch
Pico-SDK

The FreeRTOS library is included through a Git Submodule and can be added to the project using the following commands

```
 cd include 
 git submodule add https:/github.com/FreeRTOS/FreeRTOS-Kernel.git --init-recursive 
 cd FreeRTOS
 git checkout smp
 git submodule update --recursive
```

The Pico-SDK can be added using the following guide:

* https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf

The following relative paths also need to be set:

* PICO_SDK_PATH
* FREERTOS_KERNEL_PATH

## Structure

The project makes use of FreeRTOS task scheduling instead of a state machine for controlling the robot since it allows
for more flexibility during future feature development, including the implementation of a state machine in
the ` main__task()` if desired.

Lower level functions, such as I2C and GPIO, and configuration files are stored in the `/lib` directory.

In the `/src` directory, all functions used by FreeRTOS are defined in the `multucore_management.h` header file.
These functions are then launched in `main__task()`.

In regard to the run time structure, most, if not all, of the control of the physical robot is handled on a single core,
while the second core is dedicated to sending, receiving, and parsing data from an MQTT Broker. 

