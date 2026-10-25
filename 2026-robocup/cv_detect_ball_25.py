import cv2
import math
import serial
import numpy as np 
import time
import threading
from collections import defaultdict
import struct

""" 
    dectect ball based OpenCV
"""

class DetectBall:
    def __init__(self):
        """
            init...
        """
        # init capture
        self.capture = cv2.VideoCapture("/dev/cam2", cv2.CAP_V4L2) 
        # set
        self.capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

        # init ser
        #self.ser = serial.Serial(port= '/dev/ttyACM0', baudrate= 115200, timeout= 1)
        #self.ser = serial.Serial(port= '/dev/pts/2', baudrate= 9600, timeout= 1)
        # color defiend
        self.color_ranges = {
        'yellow': [
        (np.array([13, 30, 14]), np.array([41, 210, 255]))
    ],
        'green': [
        (np.array([48, 155, 6]), np.array([81, 255, 255]))
    ],
         'blue': [
        (np.array([104, 79, 19]), np.array([179, 255, 255]))
    ],
        'red': [
        (np.array([0, 218, 27]), np.array([179, 255, 255]))
    ],
        'white': [
        (np.array([6, 0, 66]), np.array([101, 89, 255]))
    ]
}
        self.color_map = {"red": 0x01, "white": 0x02, "yellow": 0x03, "blue": 0x04, "green": 0x05}

        # state control
        self.detection_active = True
        self.min_contour_area = 500

    
   #def server(self):
     #   """ 
      #      ser connect...
      #  """
    #    if not self.ser.is_open():
     #       self.ser.open()
      #  print("already connect", self.ser.port)
    #def send(self,color_info):
     #   """
     #       send info... 
     #   """
     #   if color_info is None:
      #      print("color is None , no send")
       #     return
      #  color_id = self.color_map.get(color_info["name"], 0)
       # msg = struct.pack('Biiii', 0xFF, color_id, color_info["position"][0], color_info["position"][1], color_info["radius"]) 
        #self.ser.write(msg)
       # msg = f"{color_id},{color_info['position'][0]},{color_info['position'][1]},{color_info['radius']}\n"
       # self.ser.write(msg.encode())
       # print("send info...", msg)
    def recognize_color(self):
        """ 
            dectect color... return the most common color in 5 result 
        """
        ans_color = [] # store 5 result
        for i in range(5): 

            # read frame
            if self.capture.isOpened():
                for x in range(10):
                    ret ,frame = self.capture.read() 
                    if ret:
                        break
                if not ret:
                    print("can not read the frame... return None")
                    return None
            else:
                print("can not open the camera... return None")
                return None
            
            # pre
            filtered = cv2.medianBlur(frame, 7) # zzlb
            hsv = cv2.cvtColor(filtered, cv2.COLOR_BGR2HSV) # HSV
            x,y,w,h = 190,132,146,166
            hsv = hsv[y:y + h,x :x+w]
            
            # create mask
            for color_name, ranges in self.color_ranges.items():
                mask = np.zeros_like(hsv[:, :, 0])
                for lower, upper in ranges:
                    mask_range = cv2.inRange(hsv, lower, upper)
                    mask = cv2.bitwise_or(mask, mask_range)
        
            
                
                kernel = np.ones((5,5), np.uint8)
                mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

                # find contours
                contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                
                if contours:
                    
                    largest_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > 80]
                    if largest_contours:
                        largest_contour = max(contours, key= cv2.contourArea)
                    
                        (x, y), radius = cv2.minEnclosingCircle(largest_contour)
                        center = (int(x), int(y))
                        radius = int(radius)
                        cv2.circle(frame, center, radius, (0,255,0),2)
                        
                        ans_color.append({
                                            "name": color_name,
                                                "position": (int(x), int(y)),
                                                "radius": int(radius)
                                            })
                        # print(color_name)
                        # print(detected_colors)
                        #break   

        
        if ans_color:
            color_counts = defaultdict(int)
            for color_info in ans_color:
                color_counts[color_info["name"]] += 1
            
        
            most_common_color = max(color_counts.items(), key=lambda x: x[1])[0]
            most_common_color_info = max([color for color in ans_color if color["name"] == most_common_color], key=lambda x: x["radius"])

            print("color:", most_common_color_info["name"])
            cv2.imshow("frame:",frame)
            return most_common_color_info
        else:
            print("no color ...")
            return None

            
#if __name__ == "__main__":

   # detector = DetectBall()
    #detector.server()
   # while True:
      #  time.sleep(0.1)
