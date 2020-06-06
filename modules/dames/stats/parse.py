#!/usr/bin/python3

import numpy

wins = dict()
total = dict()

def addTournament(player, opponent, win):

    if player not in total:
        total[player] = dict()
    if opponent not in total[player]:
        total[player][opponent] = 0
    total[player][opponent] += 1

    if player not in wins:
        wins[player] = dict()
    if opponent not in wins[player]:
        wins[player][opponent] = 0
    wins[player][opponent] += win

with open("log.txt") as f:
    for row in f:
        toks = row.split()
        if toks[2] == '1':
            if toks[3] == '0':
                addTournament(toks[0]+'_as_first', toks[1]+'_as_second', 1)
                addTournament(toks[1]+'_as_second', toks[0]+'_as_first', 0)
            else:
                addTournament(toks[0]+'_as_first', toks[1]+'_as_second', 0)
                addTournament(toks[1]+'_as_second', toks[0]+'_as_first', 1)


for k0 in total:
    for k1 in total[k0]:
        alpha = 100.0 * float(wins[k0][k1]) / float(total[k0][k1])
        print("Scores for {} against {}: {:.02f}%".format(k0, k1, alpha))

