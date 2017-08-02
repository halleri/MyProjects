import webapp2
from google.appengine.ext import ndb
from cgi import escape
import json
import db_defs
import random

class Book(webapp2.RequestHandler):
    def post(self):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        
        newBook = db_defs.Book()
        user = self.request.get('pId', default_value=None)
        title = self.request.get('title', default_value=None)
 
        if not user:
            return
        else:
            newBook.user = int(user)

        if not title:
            return
        else:
            newBook.title = escape(title)

        newBook.put()

        self.response.write(json.dumps({'message': "Book added!"}))
        return
  
    def get(self, **kwargs):
        self.response.headers.add_header('Access-Control-Allow-Origin', '*')
        self.response.headers['Content-Type'] = 'application/json'
        pId = self.request.get('pId', default_value=None)

        if not pId:
            return
        elif 'id' not in kwargs:
            res = [{
            'key'       : x.key.integer_id(),
            'title'     : x.title,
            } for x in db_defs.Book.query(db_defs.Book.user==int(pId)).order(-db_defs.Book.title).fetch()]
            self.response.write(json.dumps(res))
            return

        else:
            res= ndb.Key(db_defs.Book, int(kwargs['id'])).get().to_dict()
            if res['user'] != int(pId):
                return
            self.response.write(json.dumps(res))


    def delete(self, **kwargs):

        if "id" not in kwargs:
            return

        else:
            book_key = ndb.Key(db_defs.Book, int(kwargs['id'])).get()
            book_key.key.delete();
            self.response.write(json.dumps({'message': 'Deleted', 'success': 'true'}))
            return


