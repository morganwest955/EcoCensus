"""
EcoCensus Coordinate converter
Copyright 2017: Morgan West, Rebekah Loving, Morgan Friend
"""

import math
import utm

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
        self.phi = math.radians(direction) # polar rotation coordinate converted to radians
        self.droneHeight = height # in meters
        return

    # Converts coordinates from geographic to decimal degrees
    # Used because coordinates stored in image metadata are in geographic form
    def toDecimalDegrees(self,droneLat,droneLong):
        # Geographic coords stored as tuple of lists ([hours,minutes,seconds],[hours,minutes,seconds])
        # Conversion formula is (hr + (min/60) + (sec/3600))
        latitude = droneLat
        longitude = droneLong
        newLatitude = float(latitude[0] + float(latitude[1] / 60) + float(latitude[2] / 3600))
        newLongitude = float(longitude[0] + float(longitude[1] / 60) + float(longitude[2] / 3600))
        ##print(newLatitude, newLongitude)
        return (newLatitude,newLongitude)

    # Converts decimal degrees to universal transverse mercador in zone 5Q (Big Island).
    # Returns a coordinate tuple in UTM.
    def toUTM(self,coords):
        latitude = coords[0]
        longitude = coords[1]
        return utm.from_latlon(latitude,longitude,5,'Q')

    # Takes the coordinate given from the image data and calculates the edge
    def getEdges(self,droneLat,droneLong,imageRatio):
        droneCoords = self.toDecimalDegrees(droneLat,droneLong)
        edges = [] # list of edges [top, right, bottom, left]
        distToEdge = float(self.droneHeight*math.tan(self.theta))
        # convert from meters to decimal degrees: 1.0247 meter = 0.00001 dd
        # 1 m = 0.0004858 dd
        distToEdge = float(distToEdge * 0.0004858) # conversion from meters to decimal degrees
        # edges assuming orientation is north-up
        # directional calculation & distance
        # x' = xcos(phi)-ysin(phi)
        # y' = ycos(phi)+xsin(phi)
        # TODO: fix the math because it ain't right
        NSdistToEdge = distToEdge * imageRatio # since the image is wider than it is tall
        edges.append((droneLat + 0 - (NSdistToEdge * math.sin(self.phi)), \
                      (droneLong + NSdistToEdge * math.cos(self.phi) + 0))) # N
        edges.append((droneLat + -distToEdge * math.cos(self.phi) - 0, \
                      (droneLong + 0 + (-distToEdge * math.sin(self.phi))))) # E
        edges.append((droneLat + 0 - (-NSdistToEdge * math.sin(self.phi)), \
                      (drongLong + -NSdistToEdge * math.cos(self.phi) + 0))) # S
        edges.append((droneLat + distToEdge * math.cos(self.phi) - 0, \
                      (droneLong + 0 + (distToEdge * math.sin(self.phi))))) # W
        for i in range(edges):
            edge[i] = self.toUTM(edge[i])        
        return edges

    # Finds the midpoint of the coordinates calculated for multiple images that contain the same
        # geographic location
    # TODO: this funciton
    def triangulate(self,coordinates):

        return
    
    # Function for processing real life UTM coordinates given image and image coordinates of
        # detected plants
    def processCoords(self,droneLat,droneLong,plantCoords,imageX,imageY):
        imageRatio = float(float(imageX) / float(imageY))
        edges = self.getEdges(droneLat,droneLong,imageRatio)
        return edges
                
