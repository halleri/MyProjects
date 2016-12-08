from google.appengine.ext import ndb
import datetime

class Person(ndb.Model):
    name = ndb.StringProperty(required=True)
    pw = ndb.StringProperty(required=True)
    pId = ndb.IntegerProperty(required=True)

class Book(ndb.Model):
    def to_dict(self): #shamelessly stolen from lecture vid for debugging
        d = super(Book, self).to_dict()
        d['key'] = self.key.id()
        return d

    user = ndb.IntegerProperty(required=True)
    title = ndb.StringProperty()

