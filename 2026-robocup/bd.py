import cv2
import numpy as np
import glob

board_width = 11   
board_height = 8  
square_size = 3  

objp = np.zeros((board_height * board_width, 3), np.float32)
objp[:, :2] = np.mgrid[0:board_width, 0:board_height].T.reshape(-1, 2) * square_size


objpoints = []  
imgpoints = []  

images = glob.glob('./your_calibration_images/*.jpg') 

for fname in images:
    img = cv2.imread(fname)
    if img is None:
        print(f" can not read img: {fname}")
        continue
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, corners = cv2.findChessboardCorners(gray, (board_width, board_height), None)

    if ret:
        objpoints.append(objp)
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        imgpoints.append(corners2)


        img = cv2.drawChessboardCorners(img, (board_width, board_height), corners2, ret)
        cv2.imshow('Chessboard Corners', img)
        cv2.waitKey(500) 

cv2.destroyAllWindows()


if len(objpoints) > 0:
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

    print("ncjz:")
    print(mtx)
    print("\njbxs(k1, k2, p1, p2, k3):")
    print(dist)


    mean_error = 0
    for i in range(len(objpoints)):
        imgpoints2, _ = cv2.projectPoints(objpoints[i], rvecs[i], tvecs[i], mtx, dist)
        error = cv2.norm(imgpoints[i], imgpoints2, cv2.NORM_L2) / len(imgpoints2)
        mean_error += error
    print("wc: {}".format(mean_error / len(objpoints)))
    
    np.savez('camera_calib_params.npz', mtx=mtx, dist=dist)
    print("bzcs:camera_calib_params.npz'")
else:
    print("No corners were found in any image. Calibration failed.")