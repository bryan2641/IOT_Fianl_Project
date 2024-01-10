# -*- coding:utf-8 -*-

from flask_socketio import emit
from . import Device
from .. import socketio
import base64
from time import sleep
from flask_login import login_user, logout_user, current_user, login_required


@socketio.on('connect')
def connect():
    socketio.emit('update',Device)
    print('connect')

@socketio.on('disconnect')
def disconnect():
    global Device
    Device = {'RPI': []}
    socketio.emit('update',Device)
    socketio.emit('keep_alive')
    
    print('Client disconnected')
    

@socketio.on('new_device')
def new_dev(device_info):
    global Device
    
    if device_info is not None:
        if device_info['type'] == 'RPI' and device_info not in Device['RPI']:
            Device['RPI'].append(device_info)

        socketio.emit('update',Device)
        print("=======================")
        print(Device)


@socketio.on('ack')
def ack(msg):
    print(msg)

@socketio.on('connect', namespace='/test')
def test():
    print('namespace is connect')
    socketio.emit('test', 'this is test.', namespace='/test')
    print('is emit')



@socketio.on('update_device')
def update_location(data):
    global Device
    #print(data)
    Device = data
    socketio.emit('update', Device)


@socketio.on('get_rgb_image')
def get_rgb_image(data):
    #print(data)
    Device = data['Device']

    with open('app/User_Access/static/images/' + Device['rgb'], "wb") as img:
        img.write(base64.b64decode(data['image_data']))
        #print (data['image_data'])

# Streaming
## Open Streaming
@socketio.on('dostreaming')
def dostreaming(__dostop):
    socketio.emit('dostreaming', __dostop)

@socketio.on('now_streaming')
def now_streaming(__device, open_or_not):
    if open_or_not:
        print('----------- now streaming ', __device)
    else:
        print('----------- stop streaming ', __device)
    socketio.emit('now_streaming', open_or_not)# to js
