"""
EcoCensus Image Reader
Copyright 2018: Morgan West, Rebekah Loving
"""

from PIL.ExifTags import TAGS
from PIL import Image

# This class is capable of reading a .jpg image and returning the following:
#   Image dimensions, drone coordinates, and drone yaw
# Much more information is provided in the image, however it is not relevant
# to EcoCensus' use and therefore not extracted and returned.
class imageReader:

    def __init__(self, image):
        self.image = image
        return

    # Call this function to read the defined image
    # Returns in tuple:
    #   Image dimensions
    #   Drone Coordinates (geological coords)
    #   Drone yaw (rotation relative to North) in degrees
    def readImage():
        # open image
        img = Image.open(self.image)

        # extracting exif information (for the coordinates)
        exif = {
            TAGS[k]: v
            for k, v in img._getexif().items()
            if k in TAGS
        }

        # size of image (dimensions)
        size = img.size

        # getting the coordinates of the drone
        gps = exif['GPSInfo']
        x = gps[2]
        y = gps[4]
        coords = (x,y)

        # getting the yaw (rotation) of the drone
        with open(self.image, "rb") as imageFile:
          f = imageFile.read()
          b = str(f)
        yaw_start = b.find('FlightYawDegree=')
        yaw_end = b.find('drone-dji:FlightPitchDegree')
        yaw = b[yaw_start+18:yaw_end-6]

        # return all relevant information
        return(size, coords, yaw)
