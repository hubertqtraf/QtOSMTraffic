import os, errno
import math
import sys

def deg2num(lat_deg, lon_deg, zoom):
    lat_rad = math.radians(lat_deg)
    n = 1 << zoom
    xtile = int((lon_deg + 180.0) / 360.0 * n)
    ytile = int((1.0 - math.asinh(math.tan(lat_rad)) / math.pi) / 2.0 * n)
    return xtile, ytile

def num2deg(xtile, ytile, zoom):
    n = 1 << zoom
    lon_deg = xtile / n * 360.0 - 180.0
    lat_rad = math.atan(math.sinh(math.pi * (1 - 2 * ytile / n)))
    lat_deg = math.degrees(lat_rad)
    return lat_deg, lon_deg

print(sys.argv)
os.mkdir(sys.argv[1])
os.chdir(sys.argv[1])
print(deg2num(float(sys.argv[2]), float(sys.argv[3]), 15))
print(deg2num(float(sys.argv[4]), float(sys.argv[5]), 15))
for z in range(0, 20):
    os.mkdir(str(z))
    os.chdir(str(z))
    print(os.getcwd())
    x_min,y_min = deg2num(float(sys.argv[2]), float(sys.argv[3]), z)
    x_max,y_max = deg2num(float(sys.argv[4]), float(sys.argv[5]), z)
    #print(x_min, x_max)
   
    for x in range(x_min, x_max+1):
        #print(num2deg(x, y_min, z))
        os.mkdir(str(x))

    os.chdir("..")

