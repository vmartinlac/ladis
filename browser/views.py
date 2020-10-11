
import os
import bson
import pymongo
import pyramid.view

client = pymongo.MongoClient("mongodb://192.168.1.2")
database = client.ladis
collection = database.matches

static_directory = os.path.abspath('static')
overwrite = False
match_limit = None

@pyramid.view.view_config(route_name='index', renderer='index.jinja2')
def view_index(request):

    pipeline = list()
    pipeline.append({'$sort':{'start_timestamp':pymongo.DESCENDING}})

    with collection.aggregate(pipeline) as cursor:
        matches = list(cursor)

    ret = dict()
    ret['matches'] = matches

    return ret

@pyramid.view.view_config(route_name='match', renderer='match.jinja2')
def view_match(request):
    match = database['matches'].find_one({'_id':bson.ObjectId(request.matchdict['match'])})
    return {}

