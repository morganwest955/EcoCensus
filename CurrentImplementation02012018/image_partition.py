from PIL import Image

import os
import cv2
import numpy as np
import struct
import scipy
import scipy.misc
import scipy.cluster

def main():
    rootdir = '/Users/bound_to_love/Downloads/T3'
    directory = os.path.dirname('/Users/bound_to_love/Downloads/T3/Partitions/')
    if os.path.exists(directory):
        print "Directory already exists"
        #return

    if not os.path.exists(directory):
        os.makedirs(directory)
        print "Directory made for partitions"

    files = os.listdir(rootdir)
    for file in files:
        if ".JPG" in file:
            imgPartition = cv2.imread(rootdir + "/" + file)
            x, y, c = imgPartition.shape
            xp = len(str(x))
            yp = len(str(y))
            i, j = 0, 0
            #while i < (x/10)*10:
                #while j < (y/10)*10:
                    #partition = imgPartition.crop((i, j, (i + x / 10), (j + y / 10)))
            while i < x:
                i2 = i + 100
                while j < y:
                    j2 = j + 100
                    newfile = directory + "/" + str(j).zfill(yp) + "_" + str(i).zfill(xp) + "_" + file
                    partition = cv2.resize(imgPartition[i:i2, j:j2],(x, y), interpolation = cv2.INTER_AREA)
                    cv2.imwrite(newfile, partition)
                    #j += y / 10
                    j += 100
                #i += x / 10
                i += 100
                j = 0

if __name__ == "__main__":
    main()
