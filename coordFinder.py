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
        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        if (type(droneCoords) != tupleType):
            return
        if (len(droneCoords) != 2):
            return
        if (type(droneCoords[0]) != floatType or type(droneCoords[1]) != floatType):
            return
        
        # Geographic coords stored as tuple of lists ([hours,minutes,seconds],[hours,minutes,seconds])
        # Conversion formula is (hr + (min/60) + (sec/3600))
        latitude = droneCoords[0]
        longitude = droneCoords[1]
        newLatitude = float(latitude[0] + float(latitude[1] / 60) + float(latitude[2] / 3600))
        newLongitude = float(longitude[0] + float(longitude[1] / 60) + float(longitude[2] / 3600))
        return (newLatitude,newLongitude)

    # Converts decimal degrees to universal transverse mercador in zone 5Q (Big Island).
    # Returns a coordinate tuple in UTM.
    def toUTM(self,coords):
        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        if (type(coords) != tupleType):
            return
        if (len(coords) != 2):
            return
        if (type(coords[0]) != floatType or type(coords[1]) != floatType):
            return

        # Conversion
        latitude = coords[0]
        longitude = coords[1]
        return utm.from_latlon(latitude,longitude) # see documentation for utm library

    # Takes in UTM coordinates and outputs decimal degree coordinates
    def fromUTM(self,coords):
        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        if (type(coords) != tupleType):
            return
        if (len(coords) != 2):
            return
        if (type(coords[0]) != floatType or type(coords[1]) != floatType):
            return

        # Conversion
        return utm.to_latlon(coords)

    # Takes the coordinate given from the image data and calculates the edge
    def getEdges(self,droneDD,imageRatio):
        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        if (type(droneDD) != tupleType):
            return
        if (len(droneDD) != 2):
            return
        if (type(droneDD[0]) != floatType or type(droneDD[1]) != floatType):
            return
        if (type(imageRatio) != type(1.01) or type(imageRatio) != type(1)):
            return
        
        # defining and setting variables
        droneLat = droneDD[0]
        droneLong = droneDD[1]
        edges = [] # list of edges [top, right, bottom, left]
        distToEdge = float(self.droneHeight*math.tan(self.theta))
        
        # conversion from meters to decimal degrees
        # 1 m = 0.0004858 dd
        distToEdge = float(distToEdge * 0.0004858)

        # edge calculations assuming orientation is north-up
        NSdistToEdge = float(distToEdge * imageRatio) # since the image is wider than it is tall
        northx = droneLat
        northy = droneLong + NSdistToEdge 
        westx = droneLat + distToEdge
        westy = droneLong
        southx = droneLat
        southy = droneLong - NSdistToEdge
        eastx = droneLat - distToEdge
        easty = droneLong

        # add coordinates to list in NESW order
        edges.append((northx,northy))   # N
        edges.append((westx,westy))     # E
        edges.append((southx,southy))   # S
        edges.append((eastx,easty))     # W
        return edges

    # Finds the midpoint of the coordinates calculated for multiple images that contain the same
    #   geographic location.
    def midpoint(self,coordinates):
        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        if (type(coordinates) != tupleType):
            return
        if (len(coordinates) != 2):
            return
        if (type(coordinates[0]) != floatType or type(coordinates[1]) != floatType):
            return
        
        # Midpoint calculation
        xsum = 0
        ysum = 0
        for coord in coordinates:
            xsum += coord[0]
            ysum += coord[1]
        xcoord = xsum / len(coordinates)
        ycoord = ysum / len(coordinates)
        return (xcoord,ycoord)

    # Rotates the coordinate around the origin (droneCoords) of the image to match direcitonal rotation
    #       of the drone.
    def rotate(self, coords, origin):

        # Type checking
        tupleType = type((0,0))
        floatType = type(1.1)
        intType = type(1)
        if (type(coords) != tupleType):
            return
        if (len(coords) != 2):
            return
        if (type(coords[0]) != floatType or type(coords[1]) != floatType):
            return
        if (type(origin) != tupleType):
            return
        if (len(origin) != 2):
            return
        if (type(origin[0]) != intType or type(origin[1]) != intType):
            return
        
        # change image coordinates to emulate the center of the image as (0,0)
        xcoord = float(coords[0] - origin[0])
        ycoord = float(coords[1] - origin[1])

        # rotate coordinates around new origin
        # x' = xcos(phi)-ysin(phi)
        # y' = ycos(phi)+xsin(phi)       
        newCoordX = float(xcoord * math.cos(self.phi) - ycoord * math.sin(self.phi))
        newCoordY = float(ycoord * math.cos(self.phi) - xcoord * math.sin(self.phi))

        # move back the origin and return the coordinates
        newCoordX += origin[0]
        newCoordY += origin[1]      
        return (newCoordX, newCoordY)
    
    # Function for processing real life UTM coordinates given image and image coordinates of
    #   detected plants.
    def processCoords(self,droneCoords,plantCoords,imageDims):

        # type checking and performing necessary conversions for calculations
        tupleType = type((0,0))
        floatType = type(1.01)
        listType = type([1,2,3])
        if (type(droneCoords) == tupleType):
            tupleSize = len(droneCoords) # size of 2 means either geo or decimal coordinates.  4 is UTM
            if (tupleSize == 4):
                # convert the coordinates from UTM to decimal
                droneCoords = self.fromUTM(droneCoords)
            elif (tupleSize == 2):
                if (type(droneCoords[0]) == listType and len(droneCoords[0]) == 3):
                    # is geo coords.  Change to decimal for calculations
                    droneCoords = self.toDecimalDegrees(droneCoords)
                elif (type(droneCoords[0]) == floatType):
                    droneCoords = droneCoords # because a line of code needs to be here
                else:
                    return
            else:
                return
        else:
            return
                
        # defining and setting values for variables
        imageRatio = float(imageDims[0] / imageDims[1])
        edges = self.getEdges(droneCoords,imageRatio)
        imageOrigin = (imageDims[0]/2, imageDims[1]/2) # middle of the picture in image coords
        edgeN = edges[0]
        edgeE = edges[1]
        edgeS = edges[2]
        edgeW = edges[3]
        realCoordsX = 0 # these should not be 0 when returned
        realCoordsY = 0 # /
        latitude = droneCoords[0]
        longitude = droneCoords[1]

        # rotate coordinates along origin for direction
        # plantCoords = self.rotate(plantCoords, imageOrigin)
        xcoord, ycoord = self.rotate(plantCoords, imageOrigin)
        
        #calculating distances and ratios
        distWE = math.fabs(edgeW[0] - edgeE[0])
        distNS = math.fabs(edgeN[1] - edgeS[1])
        xratio = float(xcoord/imageDims[0])
        yratio = float(ycoord/imageDims[1])
        ratioWE = distWE * xratio
        ratioNS = distNS * yratio

        # calculating coordinates with ratios
        if xcoord < imageOrigin[0]:
            realCoordsX = latitude + (distWE / 2) - (distWE * xratio)
        if xcoord > imageOrigin[0]:
            realCoordsX = edgeW[0] - (distWE * xratio)
        if xcoord == imageOrigin[0]:
            realCoordsX = latitude
        realCoordsY = edgeN[1] + (distNS * yratio)        

        print(realCoordsX, realCoordsY)
        UTMcoords = self.toUTM((realCoordsX,realCoordsY))
        return (UTMcoords, (realCoordsX, realCoordsY))
                
