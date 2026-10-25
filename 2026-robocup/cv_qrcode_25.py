import cv2
import math
import serial
import numpy as np 
import time
import threading
from collections import defaultdict
import struct
from pyzbar import pyzbar

"""
    dectect qrcode based OpenCV 
"""

class Qrcode:
    def __init__(self):
        """ 
            init...
        """
        # init capture
        self.capture = cv2.VideoCapture("/dev/cam0", cv2.CAP_V4L2) # open the camera
        # set 
        self.capture.set(cv2.CAP_PROP_FRAME_WIDTH,320)
        self.capture.set(cv2.CAP_PROP_FRAME_HEIGHT,240)
        # init ser
        # self.ser = serial.Serial(port= '/dev/ttyAMA2', baudrate= 9600, timeout= 1)
        #self.ser = serial.Serial(port= '/dev/pts/2', baudrate= 9600, timeout= 1)
        # color difined
        self.color = (b"r", b"w", b"y", b"b", b"g", b"o")
        
        
    
            
    def recognize_qrcode(self):
        """ 
            dectect qrcode...
        """

        if self.capture.isOpened():
            for x in range(10): # read 10 frame
                ret, img = self.capture.read() 
                if ret:
                    break
            if not ret:
                print("can not read frame... return None")
                return None
        else: 
            print("can not open the camera... return None")
            return  None
        
        # init param
        min_y = 500 
        smin_y = 500
        min_y_index = -1
        smin_y_index = -1
        flag = 0
        # dectect qrcode
        barcodes = pyzbar.decode(img)
        for barcode in barcodes:
            flag = 1

            detect_color = barcode.data
            data_index_arr = [i + 1 for i,x in enumerate(self.color) if x == detect_color ]
            if data_index_arr:
                color_data_index = data_index_arr[0]
            else:
                print("check the color unexpect...")
                print(detect_color)
                break
            
            points = barcode.polygon
        
            
            point_middle_x = sum(point[0] for point in points) / 4
            point_middle_y = sum(point[1] for point in points) / 4
            img_h, img_w = img.shape[:2]
            offset_x = int(point_middle_x - img_w / 2) + 65
            offset_y = int(point_middle_y - img_h / 2)
            print("offset_x:", offset_x)
            print("offset_y:", offset_y)
            lm = offset_x / 18.6
            data_arr = [color_data_index, lm]
            print(data_arr)
            return data_arr
        return [0,0]
    #def main_deal(self):
     #   while True:
      #      if self.receive():
       #         info = self.recognize_qrcode()
        #        self.send(info)
         #   else: 
          #      continue

def get_fresh_frame(self):
    for _ in range(10):
        self.capture.read()  
    
    ret, fresh_frame = self.capture.read()
    if ret:
        return fresh_frame
    else:
        return None