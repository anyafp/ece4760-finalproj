import serial
import time
import numpy as np
import PIL
from PIL import Image
import os
import glob

img_array = []

# For testing with no training data

# img_array = [
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  26, 235, 235, 235,  31,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  83, 219, 254, 254, 254, 220, 214, 117,  29,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  94, 238, 254, 254, 254, 254, 254, 254, 254, 126,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  34, 187, 254, 254, 254, 254, 254, 254, 254, 254, 254, 248,  80,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  79, 252, 254, 254, 252, 150, 189, 254, 254, 254, 254, 254, 254, 117,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,  79, 195, 254, 254, 254, 134,   0,  71, 124, 124, 124, 227, 254, 254, 117,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,  16, 197, 254, 254, 254, 127,   2,   0,   0,   0,   0,   0, 200, 254, 254, 251,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,  20, 187, 254, 254, 254, 126,   5,   0,   0,   0,   0,   0,   0, 200, 254, 254, 206,   0,   0,   0,   0,
# 0,   0,   0,   0, 113,   0,   0,  94, 254, 254, 254, 127,   5,   0,   0,   0,   0,   0,   0, 106, 245, 254, 254, 117,   0,   0,   0,   0,
# 0,   0,   0,   0,  37,   0,  83, 238, 254, 254, 229,   5,   0,   0,   0,   0,   0,   0,  20, 228, 254, 254, 213,  32,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,  11, 219, 254, 254, 230,  43,   0,   0,   0,   0,   0,   0,  14, 195, 254, 254, 250,  86,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0, 101, 254, 254, 254, 100,   0,   0,   0,   0,   0,   1,  11, 199, 254, 254, 253, 132,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,  58, 245, 254, 254, 152,   0,   0,   0,   0,   0,   0,  96, 254, 254, 254, 254, 192,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0, 118, 254, 254, 232,  49,   0,   0,   0,  37, 104, 205, 250, 254, 254, 254, 254,  61,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0, 118, 254, 254, 217,  84,  84,  84, 218, 233, 254, 254, 255, 254, 214, 171, 127,   8,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0, 118, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 248, 188,  28,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,  35, 241, 254, 254, 254, 254, 254, 254, 254, 221, 213, 115,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0, 141, 234, 234, 234, 234, 234, 173,  96,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
# 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
# ]

img_file  = input("Image file name: ")

image = Image.open(img_file)
new_image = image.resize((28,28))
new_image.save('small-image.bmp')
rgb_im = new_image.convert('RGB')

for i in range(28):
    for j in range(28):
        img_array.append( 255 - (rgb_im.getpixel((j,i)))[0] )

ser = serial.Serial('/dev/tty.usbserial-3', 115200, timeout=0.05)

for i in range(784):
    if img_array[i] < 10:
        ser.write(str(1).encode())
    elif img_array[i] < 100:
        ser.write(str(2).encode())
    else:
        ser.write(str(3).encode())
    time.sleep(0.001)
    ser.write(str(img_array[i]).encode())
    time.sleep(0.001)