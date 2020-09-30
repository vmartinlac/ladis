
import datetime
import re
import os
import pymongo
import bson.json_util

client = pymongo.MongoClient("mongodb://192.168.1.2/")
database = client['ladis']
collection = database['matches']

for directory in 'abc':
    for filename in os.listdir(directory):
        r = re.match('^log_([0-9]+).json$', filename)
        if r:
            print("Processing {}:{}".format(directory, r.group(1)))
            with open(os.path.join(directory, filename), 'r') as f:
                data = bson.json_util.loads(f.read())
            del data['start_utc_datetime']
            data['start_timestamp'] = datetime.datetime.utcfromtimestamp(data['start_timestamp'])
            for move in data['agent_moves']:
                time_offset = datetime.timedelta(seconds=move['time_offset'])
                computation_time = datetime.timedelta(seconds=move['computation_time'])
                move['timestamp_before_computation'] = data['start_timestamp'] + time_offset - computation_time
                move['timestamp_after_computation'] = data['start_timestamp'] + time_offset
                del move['computation_time']
                del move['time_offset']
            data['imported'] = True
            collection.insert_one(data)

