# -*- coding:utf-8 -*-

from flask_script import Manager, Server
from app import create_app, socketio
from flask_login import LoginManager, UserMixin, login_user, logout_user, login_required, current_user
from app.User_Access import users

app = create_app('DevConfig')

# 設定你的app
manager = Manager(app)

# 設定 python3 manage.py runserver 為啟動 server 指令
manager.add_command('runserver', socketio.run(app = app, host = '0.0.0.0', port = 8080))
#manager.add_command('runserver', socketio.run(app = app, host = '0.0.0.0', port = 8080, keyfile='server.key', certfile='server.crt'))

# 設定 python3 manage.py shell 為啟動互動式指令 shell 的指令
@manager.shell
def make_shell_context():
    return dict(app=app)

if __name__ == '__main__':
    manager.run()
