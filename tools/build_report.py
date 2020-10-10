
import datetime
import os
import PIL.Image
import PIL.ImageDraw
import pymongo

result_codes = dict()
result_codes[0] = 'AGENT_WON'
result_codes[1] = 'AGENT_LOST'
result_codes[2] = 'AGENT_GAVE_UP'
result_codes[3] = 'AGENT_ILLEGAL_MOVE'
result_codes[4] = 'CONTROLLER_ERROR'
result_codes[5] = 'AGENT_ERROR'

class ReportBuilder:

    def __init__(self):
        self.overwrite = False
        self.report_directory = 'report'
        self.client = pymongo.MongoClient("mongodb://192.168.1.2/")
        self.database = self.client['ladis']
        self.collection = self.database['matches']

        try:
            os.mkdir(self.report_directory)
        except FileExistsError:
            pass

    def begin_document(self, path):
        f = open(path, 'w')
        f.write('<html>')
        f.write('<head>')
        f.write('<style type="text/css">')
        f.write('table th { background: #AAA; padding:1em; }')
        f.write('table td { background: #CCC; padding:1em; }')
        f.write('</style>')
        f.write('</head>')
        f.write('<body>')
        return f

    def end_document(self, f):
        f.write('</body>')
        f.write('</html>')
        f.close()

    def build(self):
        index = self.begin_document(os.path.join(self.report_directory, 'index.html'))
        index.write('<table>')
        index.write('<tr>')
        index.write('<th>Object ID</th>')
        index.write('<th>Date</th>')
        index.write('<th>Difficulty</th>')
        index.write('<th>Agent</th>')
        index.write('<th>Number of moves</th>')
        index.write('<th>Result</th>')
        index.write('</tr>')

        pipeline = list()
        pipeline.append({'$sort':{'start_timestamp':pymongo.DESCENDING}})
        #pipeline.append({'$limit':5})

        with self.collection.aggregate(pipeline) as cursor:
            for match in cursor:

                print("Processing match {}...".format(match['_id']))

                object_id = match['_id']
                match_directory = 'match_{}'.format(object_id)
                match_path = os.path.join(self.report_directory, match_directory)
                if self.overwrite or (not os.path.exists(match_path)):
                    try:
                        os.mkdir(match_path)
                    except FileExistsError:
                        pass
                    self.write_log_page(match_path, match)

                index.write('<tr>')
                index.write('<td><a href="{}/index.html">{}</td>'.format(match_directory, match['_id']))
                index.write('<td>{}</td>'.format(match['start_timestamp'].replace(tzinfo=datetime.timezone.utc).astimezone(tz=None)))
                index.write('<td>{}</td>'.format(match['difficulty']))
                index.write('<td>{}</td>'.format(match['agent']))
                index.write('<td>{}</td>'.format(len(match['agent_moves'])))
                index.write('<td>{}</td>'.format(result_codes[match['result']]))
                index.write('</tr>')

        index.write('</table>')
        self.end_document(index)

    def write_log_page(self, directory, log):
        page = self.begin_document(os.path.join(directory, 'index.html'))
        page.write('<h1>Match log</h1>')

        page.write('<table>')
        page.write('<tr><th>Object ID</th><td>{}</td></tr>'.format(log['_id']))
        page.write('<tr><th>Date</th><td>{}</td></tr>'.format(log['start_timestamp'].replace(tzinfo=datetime.timezone.utc).astimezone(tz=None)))
        page.write('<tr><th>Difficulty</th><td>{}</td></tr>'.format(log['difficulty']))
        page.write('<tr><th>Agent</th><td>{}</td></tr>'.format(log['agent']))
        page.write('<tr><th>Number of moves</th><td>{}</td></tr>'.format(len(log['agent_moves'])))
        page.write('<tr><th>Result</th><td>{}</td></tr>'.format(result_codes[log['result']]))
        page.write('</table>')

        page.write('<table>')
        page.write('<tr>')
        page.write('<th>Index</th>')
        page.write('<th>Move</th>')
        page.write('</tr>')
        count = 0

        image_size = 400
        def cell_to_rect(index):
            assert( 0 <= index and index < 50 )
            index2 = 49 - index
            u = 2*(index2 % 5) + (1 - (index2//5)%2)
            v = index2 // 5
            return u*image_size//10, v*image_size//10, (u+1)*image_size//10-1, (v+1)*image_size//10-1

        for move in log['agent_moves']:
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
            filename = 'move_{}.png'.format(count)
            im.save(os.path.join(directory, filename))
            page.write('<tr>')
            page.write('<td>{}</td>'.format(count))
            page.write('<td><img src="{}" /></td>'.format(filename))
            page.write('</tr>')
            count += 1
        page.write('</table>')
        self.end_document(page)

r = ReportBuilder()
r.build()

