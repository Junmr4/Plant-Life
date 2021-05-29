from Tkinter import *
from datetime import datetime
import sys, os
import tkFont
import Tkinter as tk
import Tkinter as tkinter
import bme680
import time
import numpy as np
import cv2,io,imutils
from imutils.video import VideoStream
from PIL import Image, ImageTk

sensor = bme680.BME680()

sensor.set_humidity_oversample(bme680.OS_2X)
sensor.set_pressure_oversample(bme680.OS_4X)
sensor.set_temperature_oversample(bme680.OS_8X)
sensor.set_filter(bme680.FILTER_SIZE_3)

#bootup image
rootboot = tkinter.Tk()
w, h = rootboot.winfo_screenwidth(), rootboot.winfo_screenheight()
rootboot.overrideredirect(1)
rootboot.geometry("%dx%d+0+0" % (w, h))
rootboot.focus_set()
canvasboot = tkinter.Canvas(rootboot,width=w,height=h)
canvasboot.pack()
canvasboot.configure(background='black')


def showPIL(pilImage):
    imgWidth, imgHeight = pilImage.size
 # resize photo to full screen 
    ratio = min(w/imgWidth, h/imgHeight)
    imgWidth = int(imgWidth*ratio)
    imgHeight = int(imgHeight*ratio)
    pilImage = pilImage.resize((imgWidth,imgHeight), Image.ANTIALIAS)   
    image = ImageTk.PhotoImage(pilImage)
    imagesprite = canvasboot.create_image(w/2,h/2,image=image)
    rootboot.update_idletasks()
    rootboot.update()

names = os.listdir("/home/pi/Pimoroni/bme680/examples/Plant_Life/img/")
#print(names)
for file in names:

    #print(file)
    if file[-4:] == ".png":
        file=Image.open("/home/pi/Pimoroni/bme680/examples/Plant_Life/img/"+file)
        showPIL(file)
        time.sleep(5)
        rootboot.destroy()
        

print("Starting Camera...........")
root = Tk()
cap = cv2.VideoCapture('/dev/video0')

font = ('Helvetica', 20, 'bold')
display = tk.Label(root, font=font, fg="black")
display.pack()


while(True):
    ret, frame = cap.read()
    cv2.normalize(frame, frame, 0, 600, cv2.NORM_MINMAX)
#timestamp    
    now = datetime.now()
    timestr = now.strftime("%I:%M %p")
    cv2.putText(frame,
                timestr,
                (250, 30),
                2, 1,
                (0, 255, 255),
                2,
                cv2.LINE_4)

#GROW ROOM BEGIN ENVIORNMENT SCAN
#room
    cv2.putText(frame,
                'Room',
                (545, 30),
                2, 1,
                (0, 255, 255),
                2,
                cv2.LINE_4)
    cv2.imshow('Plant_Life',frame)
        
    if sensor.get_sensor_data():
        round(sensor.data.pressure)
        round(sensor.data.humidity)
        round(sensor.data.temperature)

#Temp        
        newtempsensor = (sensor.data.temperature * 1.8) +32
        temp = "{0:.0f}".format(newtempsensor)
        tem2str = str(temp)
        output = tem2str + " F"
        cv2.putText(frame,
                    output,
                    (580, 62),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)

#Pressure       
        press = "{0:.0f}".format(sensor.data.pressure)
        hpa2str = str(press)
        outputhpa = hpa2str + " hPa"
        cv2.putText(frame,
                    outputhpa,
                    (523, 90),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)

#Humidity       
        rh = "{0:.0f}".format(sensor.data.humidity)
        rh2str = str(rh)
        outputrh = rh2str + " RH"
        cv2.putText(frame,
                    outputrh,
                    (563, 117),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
#GROW ROOM END ENVIORNMENT SCAN

#GROW ROMM BEGIN HYDROPONIC ENVIORNqqMENT SCAN
 #Water Temperature
        newh2otempsensor = (sensor.data.temperature * 1.8) +32
        h2otemp = "{0:.0f}".format(newh2otempsensor)
        h2otempstr = str(h2otemp)
        watertemp = " F"
        outputh2otemp = h2otempstr + watertemp
        cv2.putText(frame,
                    outputh2otemp,
                    (30, 430),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        cv2.putText(frame,
                    "Water Temp",
                    (10, 460),
                    2, .5,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)

#PH
         #Water Temperature
        newphsensor = (sensor.data.temperature * 1.8) +32
        ph = "{0:.0f}".format(newphsensor)
        phstr = str(ph)
        outputph= phstr
        cv2.putText(frame,
                    outputph,
                    (162, 430),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        cv2.putText(frame,
                    "Water pH",
                    (140, 460),
                    2, .5,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        
#Water Level
        cv2.rectangle(frame,
                      (250, 440),
                      (410, 390),
                      (0, 255, 0),
                      3)
        cv2.line(frame,
                (262, 430),
                (400, 430),
                (255,0,0),
                18)
        cv2.line(frame,
                (262, 420),
                (400, 420),
                (255,0,0),
                18)
        cv2.line(frame,
                (262, 415),
                (400, 415),
                (255,0,0),
                18)
        cv2.line(frame,
                (262, 400),
                (400, 400),
                (255,0,0),
                18)
        waterlevelsensor = (sensor.data.temperature * 1.8) +32
        wl = "{0:.0f}".format(waterlevelsensor)
        wlstr = str(wl)
        outputwl= wlstr + "%"
        cv2.putText(frame,
                    outputwl,
                    (310, 430),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        cv2.putText(frame,
                    "Water Level",
                    (271, 463),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        tdssensor = (sensor.data.temperature * 1.8) +32
        tds = "{0:.0f}".format(tdssensor)
        tdsstr = str(tds)
        outputtds= tdsstr + " PPM"
        cv2.putText(frame,
                    outputtds,
                    (510, 430),
                    2, .7,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)
        cv2.putText(frame,
                    "Water Quality",
                    (500, 460),
                    2, .5,
                    (0, 255, 255),
                    2,
                    cv2.LINE_4)


#GROW ROMM END HYDROPONIC ENVIORNMENT SCAN
#open in full screen
        cv2.namedWindow ('Plant Life', cv2.WINDOW_NORMAL)
        cv2.setWindowProperty ( 'Plant Life', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
   #show app     
        cv2.imshow('Plant Life',frame)
                
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
