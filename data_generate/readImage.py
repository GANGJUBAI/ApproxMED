#readImage.py
#Author:Fenfang Li
#Time:5/1/2021

from PIL import Image
import numpy as np
import struct

image =Image.open(r'lena_512_512.png').convert('L')
print(image)
print(image.size)
images = np.asarray(image)
print(images)

Value = images.flatten()

copyTime=1
Value =[int(val) for _ in range(copyTime) for val in Value]
#print(Value)

fp =open('lena.bin','wb+')
data = struct.pack( ('%di' % len(Value)), *Value)
fp.write(data)
fp.close()


