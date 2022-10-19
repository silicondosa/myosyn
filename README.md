# MyoSyn

A control API for a novel modular synthetic muscle, MyoSyn is a portmanteau of Synthetic (as in man-made) and Myosin (a protein fundamental to muscle function, which is also how you pronounce MyoSyn). This C++ API allows us to actuate this muscle with closed-loop control using our custom QuickDAQ software.

## Dependencies

MyoSyn requires the following libraries:

- QuickDAQ: A custom data acquisition library able to utilize National Instruments DAQ hardware
- MiniPID: A PID control and tuning library
- cLinkedList: A small C library that implements a simple double linked list

## Installation

In addition to the API, the dependencies are also included in this repo as git submodules.

To clone this repo, use the `git clone --recursive` command.
