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

### Install the SDK
1. ```mkdir Pico``` (I did this in ```~/Pico```)
2. ```cd ./Pico```
3. ```git clone -b master https://github.com/raspberrypi/pico-sdk.git```
4. ```cd pico-sdk```
5. ```git submodule update --init```
6. ```cd ..```

### Clone Repo
1. ```git clone https://github.com/anyafp/ece4760-finalproj.git```

### Build Project
1. ```cd ~/Pico/ece4760-finalproj```
2. ```mkdir build```
3. ```cd build```
4. ```export PICO_SDK_PATH=../../pico-sdk```
5. ```cmake ..```
6. ```make```