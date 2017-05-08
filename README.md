# PiOS

RTOS designed for automatics control tasks

### Development in an alpha stage

Development of the main part of the kernel is mainly finished, but it has not
been tested in real application + there are many improvements to do (see issues).

Please feel free to contribute if you want and feel, that it's worth it.

### Motivation

Kernel was created in order to improve quality of control task's code.
Main responsibilities of kernel are task cheduling and dynamic memory allocation.

Kernel is meant to be extra small and light (it's a must in a small embedded devices).

Also it is assumed, that kernel itself will use newest possible technologies at the time of implementation.
It should not influence applications (especially it will not enforce compiler used etc.),
because kernel is provided as a precompiled library. 

### Supported hardware

Currently no hardware is supported. This may or might not change soon. This means, that
adding support for various platforms is quite easy and everyone should be able to do this
on his own.

In practice three things have to be implemented Timer interruptions, exit system call and
context switch procedure (includes context structure and switch procedures), which is done
by Context class.

Adding next supported hardware is as simple as adding new directory in sources/PiOS/hardware/.
After this new entry in CMake-gui appears (drop-down list PiOS_PLATFORM_TYPE).

Example application with the usage of the kernel will be written for RaspberryPi platform.
