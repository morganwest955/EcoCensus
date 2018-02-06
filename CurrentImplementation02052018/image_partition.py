from PIL import Image
import get_lat_lon_exif_pil as gll
import utm
import os
import cv2
import numpy as np
import struct
import scipy
import scipy.misc
import scipy.cluster

def main():
    rootdir = '/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M'
    directory = os.path.dirname('/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M/Partitions/')
    f = open(str(rootdir) + "/Drone_coords.txt", "w+")
    if os.path.exists(directory):
        print "Directory already exists"

    if not os.path.exists(directory):
        os.makedirs(directory)
        print "Directory made for partitions"

    files = os.listdir(rootdir)
    for file in files:
        if ".JPG" in file:
            image = Image.open(str(rootdir + '/' + file))
            exif_data = gll.get_exif_data(image)
            lat, lon = gll.get_lat_lon(exif_data)
            f.write((file + " " + str(lat) + " " + str(lon) + " " + str(image.size[0]) + " " + str(image.size[1])) + str("\n"))
            imgPartition = cv2.imread(rootdir + "/" + file)
            x, y, c = imgPartition.shape
            xp = len(str(x))
            yp = len(str(y))
            i, j = 0, 0
            #while i < (x/10)*10:
                #while j < (y/10)*10:
                    #partition = imgPartition.crop((i, j, (i + x / 10), (j + y / 10)))
            while i < x:
                i2 = i + 300
                while j < y:
                    j2 = j + 300
                    newfile = directory + "/" + str(j).zfill(yp) + "_" + str(i).zfill(xp) + "_" + file
                    partition = cv2.resize(imgPartition[i:i2, j:j2],(x, y), interpolation = cv2.INTER_LINEAR)
                    cv2.imwrite(newfile, partition)
                    #j += y / 10
                    j += 300
                #i += x / 10
                i += 300
                j = 0
    f.close()

if __name__ == "__main__":
    main()
