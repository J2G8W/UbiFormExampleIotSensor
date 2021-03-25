# UbiForm Example IoT Sensor
> An application which demonstrates the possibilities of the [UbiForm platform](https://github.com/J2G8W/UbiForm).

The repository provides code for a simple application which acts as a dummy IoT sensor which makes use of the [UbiForm Middleware Platform](https://github.com/J2G8W/UbiForm).
It is especially designed to work with the [LifeHub demo](https://github.com/J2G8W/UbiFormLifeHub) and the [UbiForm skeleton Key](https://github.com/J2G8W/UbiFormSkeletonKey).
It presents two endpoints - a simple 'publisher' of dummy temperature data and a system info 'reply' endpoint, which replies to requests for info with the current status.
It is a part of my PartII Computer Science project at the University of Cambridge.

## Installation
Add the UbiForm library to the path libs/UbiForm (can be done with a soft link if UbiForm already exists elsewhere on the system).
It requires the CMake build system to be installed (available for all common platforms) and can optionally use the Ninja build system for further cross-platform usability.

Linux (starting from within the repo):
```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

## Usage example
The manifest shows the two endpoint communications available with the sensor

## Release History

* 0.1.0
    * The first proper release
* 0.0.1
    * Work in progress

## Meta
[Julian Wreford](https://www.linkedin.com/in/julian-wreford-986b34154/) – julian \[at\] ashleywreford \[dot\] com

Distributed under the MIT license. See ``LICENSE`` for more information.
