# MyoSyn

A control API for a novel modular synthetic muscle, MyoSyn is a portmanteau of Synthetic (as in man-made) and Myosin (a protein fundamental to muscle function, which is also how you pronounce MyoSyn). This C++ API allows us to actuate this muscle with closed-loop control using our custom QuickDAQ software.

## Dependencies

MyoSyn requires the following libraries:
- NI-DAQmx: 
- QuickDAQ: A custom data acquisition library able to utilize National Instruments DAQ hardware (included as submodule)
- MiniPID: A PID control and tuning library (included as submodule)
- cLinkedList: A small C library that implements a simple double linked list (included as submodule)

## Installation

In addition to the API, the dependencies are also included in this repo as git submodules.

To clone this repo, use the `git clone --recursive-submodules` command.

## Documentation

You can find the DoxyGen-generated documentation [here](https://solaremperor.github.io/myosyn/), complete with an [example](https://solaremperor.github.io/myosyn/myosyn__test_8cpp_source.html).

## LICENSE

This library is distributed under the GNU Lesser GPL v3. The license file is located in the root directory of this repo.
