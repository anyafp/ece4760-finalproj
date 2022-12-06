# Follow These Steps to Run the Project

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
1. ```git clone -b master https://github.com/anyafp/ece4760-finalproj.git```

### Build Project
1. ```cd ~/Pico/ece4760-finalproj```
2. ```mkdir build```
3. ```cd build```
4. ```export PICO_SDK_PATH=../../pico-sdk```
5. ```cmake ..```
6. ```make```

### Run Project
1. Comment in lines 217 and 218 in ```digitrec.c```
2. Open up Terminal on Mac
3. ```ls /dev/tty.*``` and note the usbserial numbers
    * e.g. ```/dev/tty.usbserial-0001``` and ```/dev/tty.usbserial-3```
4. Open two terminal windows and open the serial terminal for the two usbserial ports -- ```screen <usbserial numbers> 115200```
    * e.g. ```screen /dev/tty.usbserial-0001 115200``` and ```screen /dev/tty.usbserial-3 115200``` on the two windows
    * 115200 is the baud rate
5. Note which usbserial corresponds to which uart, close the terminals, open a new terminal and type ```killall screen```
6. Comment out lines 217 and 218 in ```digitrec.c```
7. ```cd ~/Pico/ece4760-finalproj/build```
8. ```make```
9. Save ```serial_write.py``` somewhere on your computer (not in ~/Pico) -- I recommend your Documents folder 
10. In ```serial_write.py```, change line ___ to specify the usbserial number for **uart0**
    * e.g. ```ser = serial.Serial('/dev/tty.usbserial-3', 115200, timeout=0.05)```
11. Open up a paint application (I downloaded Paintbrush)
12. Specify your canvas to be of any **square** size (200x200 recommended)
13. Using the largest brush thickness (stroke size 10), draw your digit
14. Save the digit in **bmp format** to the same directory as ```serial_write.py```
15. Run ```serial_write.py``` and enter your image file name
16. Your drawn image and the predicted digit will appear on the VGA screen
