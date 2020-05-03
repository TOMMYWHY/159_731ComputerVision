
import cv2
import numpy as np

filename="./images/2Dempty.jpg"


qrcode_input = cv2.imread(filename,1)
qrcode_gray = cv2.cvtColor(qrcode_input,cv2.COLOR_BGR2GRAY)
ret,binary= cv2.threshold(qrcode_gray,127,255,cv2.THRESH_BINARY)
# print(ret)

contours,hierachy = cv2.findContours(binary,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)# todo
# print(contours,hierachy)
level =0
contours_sign = []
# print(hierachy)
for i in range(len(hierachy[0])):
    hie = hierachy[0][i]
    if hie[2] !=-1:
        level+=1
        pass
    elif hie[2] == -1 and level !=0:
        level+=1
        if level ==3:
            print('level3~!')
            contours_sign.append(contours[i])
        level=0
    else:
        level = 0

cv2.drawContours(qrcode_input,contours_sign,-1,(0,0,255),2)

cv2.imshow('qrcode_input',qrcode_input)
# cv2.imshow('qrcode_gray',qrcode_gray)
# cv2.imshow('binary',binary)

cv2.waitKey(0)
cv2.destroyAllWindows()
