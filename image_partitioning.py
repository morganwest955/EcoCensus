from PIL import Image

import os
import cv2
import numpy as np

rootdir ='/Users/bound_to_love/Downloads/Miconia'

directory1 = os.path.dirname('/Users/bound_to_love/Downloads/Miconia_Kmeans/')

directory2 = os.path.dirname('/Users/bound_to_love/Downloads/Miconia_Partitioned/')
if not os.path.exists(directory1):
    os.makedirs(directory1)
    print "Directory 1 made"

if not os.path.exists(directory2):
    os.makedirs(directory2)
    print "Directory 2 made"

files = os.listdir(rootdir)
for file in files[1:]:
    img = cv2.imread(rootdir+"/"+file)
    Z = img.reshape((-1, 3))
    # convert to np.float32
    Z = np.float32(Z)
    # define criteria, number of clusters(K) and apply kmeans()
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)

    K = 20
    ret, label, center = cv2.kmeans(Z, K, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)
    # Now convert back into uint8, and make original image

    center = np.uint8(center)
    res = center[label.flatten()]
    imgRes = res.reshape((img.shape))
    cv2.imwrite(directory1 + "/" + file, imgRes)

    imgPartition = Image.open(directory1+"/"+file)
    x, y = imgPartition.size
    i, j = 0, 0
    while i < x:
        while j < y:
            partition = imgPartition.crop((i, j, (i + x/100), (j + y/100)))
            partition.save(directory2+"/"+str(i)+"_"+str(j)+"_"+file)
            j += y / 100
        i += x / 100