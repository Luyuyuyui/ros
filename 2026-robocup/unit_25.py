import cv2
import math
import serial
import numpy as np 
import time
import threading
from collections import defaultdict
import struct
from pyzbar import pyzbar
from cv_detect_ball_25 import DetectBall
from cv_qrcode_25 import Qrcode

class MainController:
    def __init__(self):
        """
            init...
        """
        
        # init ser
        self.ser = serial.Serial(port= '/dev/ttyAMA0', baudrate= 9600, timeout= 1)
        self.ball = DetectBall()
        self.qrcode = Qrcode()
        
    def server(self):
        """
            ser connect... 
        """
        if not self.ser.is_open:
            self.ser.open()
        print("already connect", self.ser.port)
        
    def send_ball_info(self,color_info):
        """
            send collor info... 
        """
        if color_info is None:
            print("color is None , no send")
            return
        color_id = self.ball.color_map.get(color_info["name"], 0)
        #msg = str(color_id).encode("utf-8") + b"\n"
       # msg = struct.pack('Biiii', 0xFF, color_id, color_info["position"][0], color_info["position"][1], color_info["radius"]) 
        #self.ser.write(msg)
        #msg = f"{color_id}\n"
        msg = struct.pack('B', color_id)
        self.ser.write(msg)
        print("send color info...", msg)
        
    
    def send_qrcode_info(self,info):
        """ 
            send qrcode info...
        """
        # sendinfo
        if info is None:
            print("no result...")
            return
        try:
            msg = struct.pack('if', int(info[0]), float(info[1]))
            self.ser.write(msg)
            print("already send...", msg)
        except Exception as e:
            print("worse ser(send)", e)
    
    def main_deal(self):
        self.server()
        while True:
            # wait for instr...
            if self.ser.in_waiting > 0:
                command = self.ser.read(1)
                if command == b'\x31':
                    print("receive the color instr...")
                    color_info = self.ball.recognize_color()
                    self.send_ball_info(color_info)
                elif command == b'\xA1':
                    info = self.qrcode.recognize_qrcode()
                    print("receive the qrcode instr...")
                    self.send_qrcode_info(info)

if __name__ == "__main__":
    Controller = MainController()
    Controller.main_deal()
    