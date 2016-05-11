#coding: utf-8
import cv2

cap = cv2.VideoCapture(0)

while 1:
    stat, image = cap.read()
    cv2.imshow("capture",image)
    key = cv2.waitKey(1)
    print key
    if key == 1048603:
        break
cap.release()
