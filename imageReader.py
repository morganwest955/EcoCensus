"""
Image Reader
Copyright 2017: Morgan West
"""

##import exifread # reads file metadata
import coordFinder as crdfdr # class to calculate coordinates of selected plants in images

# We want latitude, longitude, width of picture, length of picture
# GPS GPSLatitude, GPS GPSLongitude, Image XResolution, Image YResolution

# TODO: Figure out why trying to create tuples turns into IfdTags
##f = open(str('DJI_0074.jpg'), 'rb')
##tags = exifread.process_file(f,strict=True)
##for entry in tags:
##    print entry
##print tags['GPS GPSLatitude']
##print tags['GPS GPSLongitude']
##print tags['EXIF ExifImageWidth']
##print tags['EXIF ExifImageLength']
##droneLat = tags['GPS GPSLatitude']
##droneLong = tags['GPS GPSLongitude']
##imageX = tags['EXIF ExifImageWidth']
##imageY = tags['EXIF ExifImageLength']
##print droneLat
##print droneLong
##print imageX
##print imageY
##droneCoords = (droneLat, droneLong)
##print droneCoords
##imageResolution = (imageX, imageY)
##print "Res",imageResolution
##print "Coords",droneCoords
c = crdfdr.CoordFinder(94,0,30) # 94 FOV, 0 direction, 30 meter hight
edges = c.processCoords([19,28,float(267777/5000)],[154,56,float(36523/625)],0,4000,3000)
print("N",edges[0])
print("E",edges[1])
print("S",edges[2])
print("W",edges[3])
