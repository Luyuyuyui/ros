import cv2
import math
import serial
import numpy as np 
import time
import threading
from collections import defaultdict
import struct
from pyzbar import pyzbar


if __name__ == "__main__":
    # init capture
    capture = cv2.VideoCapture("/dev/cam0", cv2.CAP_V4L2)
    capture.set(cv2.CAP_PROP_FRAME_WIDTH,640)
    capture.set(cv2.CAP_PROP_FRAME_HEIGHT,480)
    # color difined
    color = (b"r", b"w", b"y", b"b", b"g", b"o")
    img = []
    if capture.isOpened():
        for x in range(10): 
            ret, img = capture.read() 
            if ret:
                break
        if not ret:
            print("not find frame...")
            
    else: 
        print("not find capture...")


    
    min_y = 500 
    smin_y = 500
    min_y_index = -1
    smin_y_index = -1
    flag = 0


    barcodes = pyzbar.decode(img)
    for barcode in barcodes:
        flag = 1

        detect_color = barcode.data
        data_index_arr = [i + 1 for i,x in enumerate(color) if x == detect_color ]
        if data_index_arr:
            color_data_index = data_index_arr[0]
        else:
            print("it is not color defined...")
            print(detect_color)
            break
        
        points = barcode.polygon
    
        point_middle_x = sum(point[0] for point in points) / 4
        point_middle_y = sum(point[1] for point in points) / 4
        print(point_middle_x, point_middle_y)

        point_middle_x = sum(point[0] for point in points) / 4
        point_middle_y = sum(point[1] for point in points) / 4

        img_h, img_w = img.shape[:2]
        offset_x = int(point_middle_x - img_w / 2) + 65
        offset_y = int(point_middle_y - img_h / 2)
        print("offset_x:", offset_x)
        print("offset_y:", offset_y)
        lm = - offset_x / 18.6
        data_arr = [color_data_index,lm]
        print(data_arr)
    
    
    cv2.imshow("RESULT",img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    
    


