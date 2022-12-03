# Follow These Steps

## MacOS

### Install Homebrew
1. ```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"```
2. ```which brew``` # tells you where homewbrew lives on machine
3. ```echo 'eval "$([path to homebrew from command above] shellenv)"' >> /Users/<username>/.zprofile```
4. ```eval "$(/opt/homebrew/bin/brew shellenv)"```

### Install Toolchain
1. ```brew install cmake```
2. ```brew tap ArmMbed/homebrew-formulae```
3. ```brew install arm-none-eabi-gcc```
4. ```brew install wget```
5. ```brew install libpng```
6. ```brew install zlib```

### Install the SDK
1. ```mkdir Pico``` (I did this in ```~/Pico```)
2. ```cd ./Pico```
3. ```git clone -b master https://github.com/raspberrypi/pico-sdk.git```


### Install libpng
Skip the first 2 steps since the folder is already in the repo
1. ```wget https://download.sourceforge.net/libpng/libpng-1.6.37.tar.gz```
2. ```tar xvfz libpng-1.6.37.tar.gz```
3. ```cd libpng-1.6.37```
4. ```./configure --prefix=/usr/local/libpng/1_6_37```
5. ```make```
6. ```sudo make install```

### Make
1. cmake .. -DZLIB_LIBRARY=zlib -DZLIB_INCLUDE_DIR=/usr/local/Cellar/zlib/1.2.13 -DPNG_LIBRARY=libpng -DPNG_PNG_INCLUDE_DIR=/usr/local/Cellar/libpng/1.6.38

## What are these demos?

This is a collection of RP2040 examples created and assembled for ECE 4760 at Cornell. ECE 4760 students are asked to clone this repository and add their own lab assignments and projects. Some links to course materials are provided below.

> - [Course website](https://ece4760.github.io)
> - [Lab 1](https://vanhunteradams.com/Pico/Cricket/Crickets.html): Cricket Chirp Synthesis and Synchronization
> - [Lab 2](https://vanhunteradams.com/Pico/Animal_Movement/Animal_Movement.html): Modeling Decision Making in Animal Groups on the Move
> - [Lab 3](https://vanhunteradams.com/Pico/ReactionWheel/ReactionWheel.html): PID Control of an Inverted Pendulum with a Reaction Wheel

This repository is modeled off of the [pico-examples](https://github.com/raspberrypi/pico-examples) repository from Raspberry Pi. To build the examples in this repository, please install the RP2040 toolchain and SDK as described in the [Getting Started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) document. The two pages linked below provide explicit instructions for Windows and Mac users. For Linux users, setup is easy (see chapters 1 and 2 of the Getting Started document linked previously). 

> - [Building these examples on Windows](https://vanhunteradams.com/Pico/Setup/PicoSetup.html)
> - [Building these examples on Mac](https://vanhunteradams.com/Pico/Setup/PicoSetupMac.html)

## How do I use them?

If you are an ECE 4760 student, you will clone this repository into the same directory which contains your copy of the [pico sdk](https://github.com/raspberrypi/pico-sdk). 
