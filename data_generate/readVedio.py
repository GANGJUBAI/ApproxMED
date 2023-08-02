import numpy as np
import cv2
import os
import pickle


filehat = "/path/to/data_generator/MinstMoving/"
data=np.load('mnist_test_seq.npy')
data = data.transpose(1, 0, 2, 3)
print(data.shape)

for index in range(len(data[0:2, 0, 0, 0])):
    #print(data[index,...].shape)
    
    #save one sequence into a binary file
    filename = filehat+'sequence-%05d.bin'%(index)
    images = data[index,...].astype(np.int32)
    with open(filename, 'wb') as f:
        images.tofile(f)  
    
    #scan each sequence and save each image into one figure
    count=0
    for j in data[index,...]:
        img=np.expand_dims(j,-1)
        #print(img.shape)
        filename='MinstMoving/video_%04d/frame_%d.jpg'%(index,count)
        count+=1
        savepath='MinstMoving/video_%04d'%(index)
        if not os.path.exists(savepath):
            os.mkdir(savepath)
        cv2.imwrite(filename,img)
   

