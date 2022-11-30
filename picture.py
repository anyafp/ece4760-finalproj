from PIL import Image
import PIL
import os
import glob
base_width = 28
image = Image.open('2-bmp.bmp').convert('L')
width_percent = (base_width / float(image.size[0]))
hsize = int((float(image.size[1]) * float(width_percent)))
image = image.resize((base_width, hsize), PIL.Image.ANTIALIAS)
p = np.array(image)

# for row in p:
#     print(' '.join('{:3}'.format(value) for value in row))

# finalArray=np.concatenate(p)
# print(finalArray)

from PIL import Image
import PIL
import os
import glob
base_width = 28
image = Image.open('2-bmp.bmp').convert('L')
width_percent = (base_width / float(image.size[0]))
hsize = int((float(image.size[1]) * float(width_percent)))
image = image.resize((base_width, hsize), PIL.Image.ANTIALIAS)
smallImage = np.array(image)
finalArray=np.concatenate(smallImage)
# print(finalArray)


# print(repr(finalArray))




# import serial
# ser = serial.Serial('/dev/ttyACM0', 9600)
# for(i in finalArray)
#     ser.write(finalArray[i])
