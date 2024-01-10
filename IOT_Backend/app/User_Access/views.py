# -*- coding:utf-8 -*-

import cv2
from flask import  request, render_template, redirect, jsonify, send_file, Response
from . import user_access
from .. import socketio
from . import Device
from . import users
from time import sleep

from flask import render_template, request, redirect, url_for, flash
from flask_login import login_user, logout_user, current_user, login_required

# Index
@user_access.route('/', methods = ['GET', 'POST'])
def direct_to_login():
    return redirect(url_for('user_access.login'))

# Login
@user_access.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        #flash('HELLO WORLD!!')
        return render_template('login.html')
    
    user_id = request.form['user_id']
    password = request.form['password']

    
    if (user_id in users.users_list):
        if(password == users.users_list[user_id]['password']):
            
            user = users.User()
            user.id = user_id
            login_user(user)
            #flash(f'{user_id}！歡迎加入草泥馬訓練家的行列！')
            if(password == 'admin'):
                return redirect(url_for('user_access.index'))


    flash('登入失敗了...')
    return render_template('login.html')

# Check
@user_access.route('/protected')
@login_required
def protected():
    return 'Logged in as: ' + current_user.id

# 註冊
@user_access.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        name = request.form['name']
        email = request.form['email']
        password = request.form['password']
        user = register_user(name, email, password)
        if user is not None:
            login_user(user)
            return redirect(url_for('dashboard'))
        else:
            flash('Failed to create user')
    return render_template('register.html')

# Logout
@user_access.route('/logout')
def logout():
    user_id = current_user.get_id()
    logout_user()
    flash(f'{user_id}！歡迎下次再來！')
    return render_template('login.html')

# Admin
@user_access.route('/admin', methods = ['GET', 'POST'])
@login_required
def index():
    global Device
    print(Device)
    return render_template('index.html', Device=Device)


@user_access.route("/status", methods=['GET'])
def upload():
    socketio.emit('status_response', {'data': 123})
    
    return jsonify(
        {"response": "ok"}
    )

@user_access.route('rgb/<string:rgb>/<int:time>')
def rgb(rgb, time):
    filename = 'User_Access/static/images/' + rgb
    return send_file(filename, mimetype='image/jpg')

def gen_frames():
    while True:
        img = cv2.imread('/home/bryan/IOT/app/User_Access/static/images/1_rgb.jpg')
        ret, buffer = cv2.imencode('.jpg', img)
        frame = buffer.tobytes()
        yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
        sleep(0.1)

@user_access.route('video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')