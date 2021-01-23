import numpy as np
import math
import cv2
from enum import Enum
from grip import FindArrows
import plotly.graph_objects as go

#import matplotlib
#matplotlib.use("macOSX")
#import matplotlib.pyplot as plt
import plotly.express as px
arrow  = FindArrows()
img = cv2.imread('Screen Shot 2019-07-19 at 9.34.59 AM.png')
#cv2.imshow('image',img)
arrow.process(img)
cv2.imshow('filtered',arrow.rgb_threshold_output)
angles =[]
for line in arrow.filter_lines_output:
	print(line.angle())
	angles.append(line.angle())
print(angles)
a = np.asarray(angles)
fig = go.Figure(data=[go.Histogram(x=a,nbinsx=10)])
#fig.show()

rows,cols = img.shape[:2]
for cnt in arrow.filter_contours_output:
	[vx,vy,x,y] = cv2.fitLine(cnt, cv2.DIST_L2,0,0.01,0.01)
	lefty = int((-x*vy/vx) + y)
	righty = int(((cols-x)*vy/vx)+y)
	#cv2.line(img,(cols-1,righty),(0,lefty),(0,255,0),2)
	x,y,w,h = cv2.boundingRect(cnt)
	cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)
	rect = cv2.minAreaRect(cnt)
	box = cv2.boxPoints(rect)
	box = np.int0(box)
	cv2.drawContours(img,[box],0,(0,0,255),2)
	# leftmost = tuple(cnt[cnt[:,:,0].argmin()][0])
	# rightmost = tuple(cnt[cnt[:,:,0].argmax()][0])
	# topmost = tuple(cnt[cnt[:,:,1].argmin()][0])
	# bottommost = tuple(cnt[cnt[:,:,1].argmax()][0])


	#min enclosing triagle
	retval,triangle=cv2.minEnclosingTriangle(cnt)
	#formatting hell
	Tlist = triangle.tolist()
	Tlist_flat = [item for sublist in Tlist for item in sublist]
	#should be good now 
	cv2.circle(img, tuple([int(i) for i in Tlist_flat[0]]),1,(255,0,0),10);
	cv2.circle(img, tuple([int(i) for i in Tlist_flat[1]]),1,(255,0,0),10);
	cv2.circle(img, tuple([int(i) for i in Tlist_flat[2]]),1,(255,0,0),10);
cv2.imshow('image',img)

cv2.waitKey(0)