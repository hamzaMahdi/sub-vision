import cv2
import numpy as np
from grip import ProcessGate
def createMask(image,x,y,w,h):
    mask = np.zeros_like(image)
    mask[y:y + h, x:x + w] = 255
    return mask

print("hello")
image = cv2.imread('gate1_avi_232.jpg')
mask = createMask(image,100,190,350,180)
res = cv2.bitwise_and(image, mask)
filtered_gate = ProcessGate()
filtered_gate.process(res,90)
lines = filtered_gate.filter_lines_output
for line in lines[0]:
    x0 = int(round(line[0][0]))
    y0 = int(round(line[0][1]))
    x1 = int(round(line[0][2]))
    y1 = int(round(line[0][3]))
    cv2.line(image, (x0, y0), (x1,y1), 255, 1, cv2.LINE_AA)
cv2.imshow("mask",res)
cv2.waitKey(0)
