
import os
import bson
import pymongo
import pyramid.view
import PIL.Image
import PIL.ImageDraw

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
    pipeline.append({'$limit':50})

    matches = list()
    with collection.aggregate(pipeline) as cursor:
        for match in cursor:
            match['num_agent_moves'] = len(match['agent_moves'])
            del match['agent_moves']
            matches.append(match)

    ret = dict()
    ret['matches'] = matches

    return ret

@pyramid.view.view_config(route_name='match', renderer='match.jinja2')
def view_match(request):

    object_id = bson.ObjectId(request.matchdict['match'])
    match = database['matches'].find_one({'_id':object_id})
    assert(match)

    match_directory = os.path.join(static_directory, "match_"+str(object_id))
    if not os.path.exists(match_directory):
        os.mkdir(match_directory)
        image_size = 400

        def cell_to_rect(index):
            assert( 0 <= index and index < 50 )
            index2 = 49 - index
            u = 2*(index2 % 5) + (1 - (index2//5)%2)
            v = index2 // 5
            return u*image_size//10, v*image_size//10, (u+1)*image_size//10-1, (v+1)*image_size//10-1

        index = 0
        for move in match['agent_moves']:
            im = PIL.Image.new('RGB', (image_size, image_size))
            draw = PIL.ImageDraw.Draw(im)
            draw.rectangle((0, 0, image_size-1, image_size-1), 'grey')
            assert(len(move['state']) == 50)
            assert(len(move['action']) >= 1)
            for i in range(50):
                cell = cell_to_rect(i)
                draw.rectangle(cell, 'green')
                draw.text(cell[:2], str(i))
                M = 5
                circle = (cell[0]+M, cell[1]+M, cell[2]-M, cell[3]-M)
                item = move['state'][i]
                if item == 'p':
                    draw.arc(circle, 0, 360, fill='yellow', width=2)
                elif item == 'o':
                    draw.arc(circle, 0, 360, fill='black', width=2)
                elif item == 'P':
                    draw.pieslice(circle, 0, 360, fill='yellow', width=2)
                elif item == 'O':
                    draw.pieslice(circle, 0, 360, fill='black', width=2)
            for cell_from, cell_to in zip(move['action'][:-1], move['action'][1:]):
                cell_from = cell_to_rect(cell_from)
                cell_to = cell_to_rect(cell_to)
                xf = 0.5 * (cell_from[0] + cell_from[2])
                yf = 0.5 * (cell_from[1] + cell_from[3])
                xt = 0.5 * (cell_to[0] + cell_to[2])
                yt = 0.5 * (cell_to[1] + cell_to[3])
                draw.line((xf, yf, xt, yt), width=2)
            filename = 'move_{}.png'.format(index)
            im.save(os.path.join(match_directory, filename))
            index += 1

    index = 0
    for m in match['agent_moves']:
        m['index'] = index
        m['image_url'] = '/static/match_' + str(object_id) + '/move_{}.png'.format(index)
        m['time_offset'] = m['timestamp_before_computation'] - match['start_timestamp']
        m['computation_time'] = m['timestamp_after_computation'] - m['timestamp_before_computation']
        index += 1

    return {'match':match}

