"""
Image Reader
Copyright 2017: Morgan West
"""

# TODO: Find a library to read in metadata/exifdata that puts the data in a usable format.
#   ExifRead puts the data into an unusable format that cannot be changed or manipulated and can
#   only be printed.

import coordFinder as crdfdr # class to calculate coordinates of selected plants in images

c = crdfdr.CoordFinder(94,0,30) # 94 FOV, 0 direction, 30 meter hight
coords = ([19,28,float(267777/5000)],[154,56,float(36523/625)])
imageDims = (4000,3000)
plantCoords = (1000,2000)
coords = c.processCoords(coords,plantCoords,imageDims)
print(coords)
