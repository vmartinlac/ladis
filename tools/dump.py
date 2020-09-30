
import pymongo

client = pymongo.MongoClient("mongodb://192.168.1.2/")
database = client['ladis']
collection = database['matches']

with collection.find() as c:
    for x in c:
        print(x['result'])
        print(x['start_timestamp'])
        print()

