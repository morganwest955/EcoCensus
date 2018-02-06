"""
Image Reader
Copyright 2017: Morgan West
"""

# TODO: Find a library to read in metadata/exifdata that puts the data in a usable format.
#   ExifRead puts the data into an unusable format that cannot be changed or manipulated and can
#   only be printed.
import os
import coordFinder as crdfdr # class to calculate coordinates of selected plants in images
import fnmatch
pixelCoords = "/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M/Drone_coords.txt"
realCoords = "/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M/Real_coords.txt"
positiveDirectory = os.path.dirname('/Users/bound_to_love/Downloads/20171227_Panaewa_Track_Miconia_I2_X5S_50M/Positive/')

c = crdfdr.CoordFinder(94,0,30) # 94 FOV, 0 direction, 30 meter height
realC = open(realCoords, 'w+')
droneCoords = [0, 0]
plantCoords = [0, 0]
with open(pixelCoords) as f:
    for line in f:
        image_name, droneCoords[0], droneCoords[1], sizex, sizey = line.split()
        for root, dirs, files in os.walk(positiveDirectory):
            for filename in fnmatch.filter(files, str("*"+image_name+"*")):
                plantCoords[0], plantCoords[1], rest = filename.split('_', 2)
                plant = c.processCoords((float(droneCoords[0]), float(droneCoords[1])), (float(plantCoords[0]), float(plantCoords[1])), (float(sizex), float(sizey)))
                realC.write(str(plant[0]) + str(plant[1]) + str("\n"))

realC.close()
