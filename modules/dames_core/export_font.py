
import cv2
import sys

im = cv2.imread(sys.argv[1])
print(im.dtype)
print( im[:,:,0] )
