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
    def toDecimalDegrees(self,droneCoords):
        # Geographic coords stored as tuple of lists ([hours,minutes,seconds],[hours,minutes,seconds])
        # Conversion formula is (hr + (min/60) + (sec/3600))
        latitude = droneCoords[0]
        longitude = droneCoords[1]
        newLatitude = float(latitude[0] + float(latitude[1] / 60) + float(latitude[2] / 3600))
        newLongitude = float(longitude[0] + float(longitude[1] / 60) + float(longitude[2] / 3600))
        ##print(newLatitude, newLongitude)
        return (newLatitude,newLongitude)

    # Converts decimal degrees to universal transverse mercador in zone 5Q (Big Island).
    # Returns a coordinate tuple in UTM.
    def toUTM(self,coords):
        latitude = coords[0]
        longitude = coords[1]
        return utm.from_latlon(latitude,longitude) # see documentation for utm library

    # Takes the coordinate given from the image data and calculates the edge
    def getEdges(self,droneCoords,imageRatio):
        droneDD = self.toDecimalDegrees(droneCoords)
        droneLat = droneDD[0]
        droneLong = droneDD[1]
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
        NSdistToEdge = float(distToEdge * imageRatio) # since the image is wider than it is tall
        print(droneLat)
        northx = droneLat - (NSdistToEdge * math.sin(self.phi))
        northy = droneLong + NSdistToEdge * math.cos(self.phi)
        westx = droneLat + -distToEdge * math.cos(self.phi)
        westy = droneLong + (-distToEdge * math.sin(self.phi))
        southx = droneLat - (-NSdistToEdge * math.sin(self.phi))
        southy = droneLong + -NSdistToEdge * math.cos(self.phi)
        eastx = droneLat + distToEdge * math.cos(self.phi)
        easty = droneLong + (distToEdge * math.sin(self.phi))
        edges.append((northx,northy)) # N
        edges.append((westx,westy)) # E
        edges.append((southx,southy)) # S
        edges.append((eastx,easty)) # W
        return edges

    # This is a function to test the edge function for accuracy.
    def testEdges(self, edges, droneCoords):
        droneDD = self.toDecimalDegrees(droneCoords)
        dronex = droneDD[0]
        droney = droneDD[1]
        distances = []
        for edge in edges:
            distances.append(math.sqrt((dronex - edge[0])**2 + (droney - edge[1])**2))
        for distance in distances:
            print(distance)
        return

    # Finds the midpoint of the coordinates calculated for multiple images that contain the same
    #   geographic location.
    def midpoint(self,coordinates):
        xsum = 0
        ysum = 0
        for coord in coordinates:
            xsum += coord[0]
            ysum += coord[1]
        xcoord = xsum / len(coordinates)
        ycoord = ysum / len(coordinates)
        return (xcoord,ycoord)
    
    # Function for processing real life UTM coordinates given image and image coordinates of
    #   detected plants.
    def processCoords(self,droneCoords,plantCoords,imageDims):

        #defining and setting values for variables
        imageRatio = float(imageDims[0] / imageDims[1])        
        edges = self.getEdges(droneCoords,imageRatio)
        edgeN = edges[0]
        edgeE = edges[1]
        edgeS = edges[2]
        edgeW = edges[3]
        realCoordsX = 0 # these should not be 0 when returned
        realCoordsY = 0 # /
        imageOrigin = (imageDims[0]/2, imageDims[1]/2) # middle of the picture in image coords

        #calculating distances and ratios
        distWE = math.abs(edgeW[0] - edgeE[0])
        distNS = math.abs(edgeN[1] - edgeS[1])
        xratio = imageCoords[0]/imageDims[0]
        yratio = imageCoords[1]/imageDims[1]
        ratioWE = distWE * xratio
        ratioNS = distNS * yratio

        #calculating coordinates with ratios
        if plantCoords[0] < imageOrigin[0]:
            realCoordsX = droneCoords[0] + ((distWE/2) - (distWE * xratio))
        if plantCoords[0] > imageOrigin[0]:
            realCoordsX = edgeW[0] - (distWE * xratio)
        if plantCoords[0] == imageOrigin[0]:
            realCoordsX = droneCoords[0]
        realCoordsY = edgeN[1] - (distNS * yratio)
        
        for edge in edges:
            print(edge)
        print("COORDS: ", xcoord, ycoord)
        UTMcoords = self.toUTM((xcoord,ycoord))
        return UTMcoords
                
