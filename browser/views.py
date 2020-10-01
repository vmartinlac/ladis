
import bson
import pymongo
import pyramid.view

client = pymongo.MongoClient("mongodb://192.168.1.2")
database = client['ladis']

@pyramid.view.view_config(route_name='index', renderer='index.jinja2')
def v_index(request):

    matches = database['matches'].find({}, {'_id':1, 'result':1, 'agent':1, 'start_timestamp':1})
    matches = list(matches)

    ret = dict()
    ret['matches'] = matches

    return ret

@pyramid.view.view_config(route_name='match', renderer='match.jinja2')
def v_match(request):
    match = database['matches'].find_one({'_id':bson.ObjectId(request.matchdict['match'])})
    return {}

