"""
Image Reader
Copyright 2017: Morgan West
"""

# TODO: Find a library to read in metadata/exifdata that puts the data in a usable format.
#   ExifRead puts the data into an unusable format that cannot be changed or manipulated and can
#   only be printed.

import coordFinder as crdfdr # class to calculate coordinates of selected plants in images

c = crdfdr.CoordFinder(94,30,30) # 94 FOV, 30 direction, 30 meter hight
coords = ([19,28,53.555],[154,56,58.437])
imageDims = (4000,3000)
plantCoords = (1900,2136)

noneType = type(None)
coords1 = c.processCoords(coords,plantCoords,imageDims)
coords = ([19,28,53.555],[154,56,58.437])
if (type(coords) == noneType):
    print("invalid input")
plantCoords = (1900,2136)
coords2 = c.processCoords(coords,plantCoords,imageDims)
mid = c.midpoint((coords1[1], coords2[1]))
print(mid)
point = c.toUTM(mid)
print(point)

