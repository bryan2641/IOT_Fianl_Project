import base64
import cv2
import numpy as np
from socketIO_client import SocketIO, LoggingNamespace
import requests
import threading
from subprocess import Popen, PIPE, run
import subprocess
import os
from time import sleep
from datetime import datetime
#
import random
import sys
import time

from util.rpi import RPI

# print info
def print_info(info):
    info = '\033[1;34;40m[INFO]\033[0m ' + str(info)
    print(info)

def print_data(data):
    data = '\033[1;36;40m[DATA]\033[0m ' + '\033[1;35;40m' + str(data) + '\033[0m'
    print(data)

def print_command(command):
    command = '\033[1;32;40m[COMMAND]\033[0m ' + str(command)
    print(command)

def print_warning(warning):
    warning = '\033[1;33;40m[WARN]\033[0m ' + str(warning)
    #print(warning)

# main func
class System(RPI):

    def __init__(self, id, socket_host, socket_port, type="UAV"):
        print_info("Initializing...")
        super().__init__(id, type)
        self.__stop = False
        self.__streaming = False
        print_info("Connecting backend...")
        
        self.__socketIO = SocketIO('http://192.168.79.8', 8080, LoggingNamespace)
        print_info("Backend connect")

        self.__device = {
            'type': "RPI",
            'ID': id,
            'host':'192.168.79.8:8080',
            'rgb': str(id) + '_rgb.jpg',
        }
        self.__socketIO.emit('new_device', self.__device)
        self.__socketIO.on('keep_alive', self.__keep_alive)
        self.__socketIO.on('dostreaming', self.__dostreaming)
        print_info("\033[1;32;1mComplete\033[0m")
        print_info(self.__device)

    def __update(self):
        self.__socketIO.emit('update_device', {'RPI': self.__device})
    
    def __repeat_update(self):
        while not self.__stop:
            try:
                self.__update()
            except:
                pass
            #print_info("Update!!")
            sleep(1)

    # keep_alive 的函式
    ## 後端閒置太久會傳送keep_alive的socketIO封包，這邊接收到後會重新new一個RPI
    def __keep_alive(self):
        print_info("[I] keep alive")
        self.__socketIO.emit('new_RPI', self.__device)
        
    # ???
    def response(self):
        self.__socketIO.emit('get_target', {'RPI':self.__device})
    
    def __dostreaming(self, data):
        dostop = data['dostop']
        number = data['number']
        #print(data)
        #print(dostop)
        if(str(number) == str(self.__device['ID'])):
            if dostop:
                self.__streaming = True
                self.__socketIO.emit('now_streaming', self.__device, 1)
            else:
                self.__streaming = False
                self.__socketIO.emit('now_streaming', self.__device, 0)
    
    def __stream(self):
        while True:
            sleep(0.1)
            if(self.__streaming):
                video = cv2.VideoCapture(-1)
                video.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
                video.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
                print_info("Stream Start!!")

                if not video.isOpened():
                    print_warning("no camara")
                    return None
                while self.__streaming:
                    ret, frame = video.read()
                    
                    if ret:
                        self.send_rgb_image(frame)
                    #print("[I] take picture")
                    #sleep(0.1)
                    if self.__streaming == False:
                        break
                video.release()
                print_info("Stream Stop!!")

    def send_rgb_image(self, frame):
        image_data = cv2.imencode('.jpg', frame)[1]
        image_data = base64.b64encode(image_data).decode()
        self.__socketIO.emit('get_rgb_image', {'Device': self.__device, 'image_data': image_data})
          
    def start(self):
        try:
            threading.Thread(target=self.__repeat_update).start()
            threading.Thread(target=self.__stream).start()
            print_info("System Start...")
            print('')

            self.__socketIO.wait()
        except KeyboardInterrupt:
            self.__stop = True
