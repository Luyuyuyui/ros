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


ball = DetectBall()
color = ball.recognize_color()

print(color)