# -*- coding:utf-8 -*-

from flask import Flask
from flask_socketio import SocketIO
from config import DevConfig
from flask_cors import CORS
from flask_bootstrap import Bootstrap
from flask_login import LoginManager

socketio = SocketIO()
bootstrap = Bootstrap()
login_manager = LoginManager()


def create_app(config):
    """Create an application."""
    app = Flask(__name__)
    app.config.from_object(DevConfig)
    app.config['SECRET_KEY'] = 'gjr39dkjn344_!67#'

    CORS(app)

    from .User_Access import user_access
    app.register_blueprint(user_access)
    login_manager.init_app(app)
    login_manager.session_protection = "strong"
    login_manager.login_view = 'user_access.login'
    socketio.init_app(app)
    bootstrap.init_app(app)
    
    return app
