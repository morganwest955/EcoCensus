from PIL import Image

import os
import cv2
import numpy as np
import struct
import scipy
import scipy.misc
import scipy.cluster

def main():
    rootdir ='/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M'
    directory = os.path.dirname('/Users/bound_to_love/Downloads/Miconia_Partitions20171227_Panaewa_Track_Miconia_I2_X5S_50M/')

    if not os.path.exists(directory):
        os.makedirs(directory)
        print "Directory made for partitions"

    files = os.listdir(rootdir)
    for file in files[1:]:
        if ".JPG" in file:
            imgPartition = Image.open(rootdir + "/" + file)
            x, y = imgPartition.size
            i, j = 0, 0
            while i < (x/10)*10:
                while j < (y/10)*10:
                    partition = imgPartition.crop((i, j, (i + x / 10), (j + y / 10)))
                    partition = partition.resize((x, y), Image.ANTIALIAS)
                    partition.save(directory+"/"+str(i)+"_"+str(j)+"_"+file)
                    j += y / 10
                i += x / 10
                j = 0



if __name__ == "__main__":
    main()
