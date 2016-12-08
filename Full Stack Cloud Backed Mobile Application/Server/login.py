import webapp2
from google.appengine.ext import ndb
from cgi import escape
import json
import db_defs
import random


class Login(webapp2.RequestHandler):
    def get(self):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        return

    def put(self):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        return

    def delete(self):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        return

    def post(self):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        name = self.request.get('name', default_value=None)
        password = self.request.get('pw', default_value=None)
        

        #make a new user in the database
        user = db_defs.Person()

        if name:
            user_database = [x.name for x in db_defs.Person.query(db_defs.Person.name==name).fetch()]
            pId_database = [x.pId for x in db_defs.Person.query().fetch()]

            if name not in user_database:
                if password:
                    user.name = name
                    user.pw = password
        
                    while True:
                        pId = random.randint(100, 999)
                        if pId not in pId_database:
                            user.pId = pId
                            break

                    user.put()

                    self.response.write(json.dumps({'message': 'You have been added!', 'pId': pId, 'name':name}))
                    return

                else:
                    self.response.write(json.dumps({'message': 'Password is required.'}))
                    return
            elif password:
                u = db_defs.Person.query(db_defs.Person.name==name).fetch()
                if password==u[0].pw:
                    self.response.write(json.dumps({'message': 'Logged in!', 'pId': u[0].pId, 'name': u[0].name}))
                    return
                else:
                    self.response.write('Sorry. Wrong password')
                    return
            else:
                return

        else:
            self.response.write(json.dumps({'message': 'Name is required.'}))
            return
