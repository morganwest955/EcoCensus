"""
EcoCensus Coordinate converter
Copyright 2017: Morgan West, Rebekah Loving, Morgan Friend
"""

import math

# This class will take the x,y coordinates of fauna found in a given
# image and output the real world coordinates.
# This class will be called per image list with identical non-metadata variables eg FOV, direciton.
# For example:
"""
find = coordFinder(94,0)
for droneCoords,droneHeight,focalLength,plantCoords in classifiedImages:
    classifiedCoordinateList.append(find.processCoords(droneCoords,droneHeight,focalLength,plantCoords))
"""

class CoordFinder:

    # if there is no defined direction, pass direction as 0 to assume north-up orientation
    def __init__(self,FOV,direction,height):
        self.theta = float(FOV / 2)
        self.direction = direction # polar coordinate
        self.droneHeight = height # in meters
        return

    # Converts coordinates from geographic to decimal degrees
    # Used because coordinates stored in image metadata are in geographic form
    def toDecimalDegrees(self,droneCoords):
        # Geographic coords stored as tuple of lists ([hours,minutes,seconds],[hours,minutes,seconds])
        # Conversion formula is (hr + (min/60) + (sec/3600))
        latitude = droneCoords[0]
        longitude = droneCoords[1]
        newLatitude = float(latitude[0] + float(latitude[1] / 60) + float(latitude[2] / 3600))
        lewLongitude = float(longitude[0] + float(longitude[1] / 60) + float(longitude[2] / 3600))
        return (newLatitude,newLongitude)

    # Takes the coordinate given from the image data and calculates the edge
    def getEdges(self,droneCoords,imageRatio):
        # TODO: implement directional calculation
        # TODO: convert distToEdge from meters to decimal degrees
            # can use haversine formula for 100% accuracy (complicated), but
            # precision of individual trees: 0.00001 DD = 1.0247m at 23 degrees N/S
        droneCoords = self.toDecimalDegrees(droneCoords)
        edges = [] # list of edges [top, right, bottom, left]
        distToEdge = float(self.droneHeight*math.tan(self.theta))
        # convert from meters to decimal degrees: 1.0247 meter = 0.00001 dd
        # 1 m = 0.0004858 dd
        distToEdge = float(distToEdge * 0.0004858)
        # edges assuming orientation is north-up
        edges.append(float((droneCoords[0] + (imageRatio * distToEdge), droneCoords[1]))) # N
        edges.append(float((droneCoords[0], droneCoords[1] - distToEdge))) # E
        edges.append(float((droneCoords[0] - (imageRatio * distToEdge), droneCoords[1]))) # S
        edges.append(float((droneCoords[0], droneCoords[1] + distToEdge))) # W
        return edges
    
    # imageResolution should be a tuple (eg (4000,4000)) with the format (width,height)
    # droneHeight is an integer, should be available in image metadata
    # droneCoords should be in the image metadata, should be passed as a tuple
    # direction is not available in the image metadata but should be passed as a polar
    # coordinate.  If it is not available, pass it as 0
    # plantCoords should be an array of tuples for the coordinates in the image
    def processCoords(self,droneCoords,plantCoords,imageResolution):
        # imageResolution[0] = width, imageResolution[1] = length
        imageRatio = float(float(imageResolution[0]) / float(imageResolution[1]))
        edges = getEdges(droneCoords,imageRatio)
        return edges
##        ratioList = [] # list of tuples that represent the ratio which coordinates are from the center to the edge
##        for x,y in plantCoords:
##            if x == 0 and y == 0: # in center
##                
##            if x == 0 and y >= 0: # center positive y axis
##
##            if x == 0 and y <= 0: # center negative y axis
##
##            if x >= 0 and y == 0: # center positive x axis
##
##            if x <= 0 and y == 0: # center negative x axis
##
##            if x > 0 and y > 0: # quadrant 1 (x,y)
##
##            if x < 0 and y > 0: # quadrant 2 (-x,y)
##
##            if x < 0 and y < 0: # quadrant 3 (-x,-y)
##
##            if x > 0 and y < 0: # quadrant 4 (x,-y)
                
