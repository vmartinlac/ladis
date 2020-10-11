
import datetime

result_codes = dict()
result_codes[0] = 'AGENT_WON'
result_codes[1] = 'AGENT_LOST'
result_codes[2] = 'AGENT_GAVE_UP'
result_codes[3] = 'AGENT_ILLEGAL_MOVE'
result_codes[4] = 'CONTROLLER_ERROR'
result_codes[5] = 'AGENT_ERROR'

first_player = dict()
first_player[True] = 'AGENT'
first_player[False] = 'OPPONENT'

def filter_datetime(t0):
    return str( t0.replace(tzinfo=datetime.timezone.utc).astimezone(tz=None) )

def filter_first_player(agent_plays_first):
    return first_player[agent_plays_first]

def filter_result(result):
    return result_codes[result]

def filter_num_moves(moves):
    return len(moves)

def filter_time_offset(delta):
    total_seconds = int(delta.total_seconds())
    total_minutes, seconds = divmod(total_seconds, 60)
    total_hours, minutes = divmod(total_minutes, 60)
    return "{}:{}:{}".format(total_hours, minutes, seconds)

