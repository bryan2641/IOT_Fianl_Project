from flask import request
from flask_login import UserMixin
from .. import login_manager

class User(UserMixin):
    '''
    def __init__(self, user_id, password):
        self.user_id = id
        self.password = password
    '''

    def is_active(self):
        return True if self.id else False
    
    def is_authenticated(self):
        return True
    
    def is_anonymous(self):
        return False
    
    def get_id(self):
        return str(self.id)

@login_manager.user_loader
def user_loader(user_id):
    if user_id not in users_list:
        return

    user = User()
    user.id = user_id
    return user

@login_manager.request_loader
def request_loader(request):
    user_id = request.form.get('user_id')
    if user_id not in users_list:
        return

    user = User()
    user.id = user_id

    # DO NOT ever store passwords in plaintext and always compare password
    # hashes using constant-time comparison!
    user.is_authenticated = request.form['password'] == users_list[user_id]['password']

    return user

users_list = {
    'Admin' : {'password' : 'admin'}
}