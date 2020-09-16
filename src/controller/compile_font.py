
import numpy
import os.path
import PIL.Image

root_directory = '/DATA/victor/developpement/ladis/data//fonts/'
content = dict()
content['a.png'] = "MODE : JEU"
content['b.png'] = "Taper F1 pour"
content['c.png'] = "le mode problème"
content['d.png'] = "F3 pour exercices"
content['e.png'] = "DA2020 (jaune) COMMENCE"
content['f.png'] = "Taper F2 pour changer"
content['g.png'] = "ENTREE OU ESPACE"
content['h.png'] = "POUR DEBUTER"
content['i.png'] = "Taper le N° du niveau"
content['j.png'] = "choisi pour changer"
content['k.png'] = "(0 à 5)"
content['l.png'] = "NIVEAU :"
content['m.png'] = "2"
content['n.png'] = "VOUS (jaune) COMMENCEZ"
content['o.png'] = "4"
content['p.png'] = "A VOUS DE JOUER"
content['q.png'] = "JE REFLECHIS..."
content['r.png'] = "JE JOUE"

letters = dict()

for filename in content:
    print("Processing {}".format(filename))
    sentence = content[filename]
    path = os.path.join(root_directory, filename)
    im = PIL.Image.open(path)
    im = numpy.asarray(im)

    assert(im.shape[1] == 8*len(sentence))
    assert(im.shape[0] == 8)
    assert( (im[:,:,3] == 255).all() )
    assert( (im[:,:,1] == im[:,:,0]).all() )
    assert( (im[:,:,2] == im[:,:,0]).all() )

    for n in range(len(sentence)):
        compiled = sum((im[:,8*n:8*n+8,0].flatten()==255) * 2**numpy.arange(8*8))
        if sentence[n] in letters:
            assert( (letters[sentence[n]] == compiled).all() )
        else:
            letters[sentence[n]] = compiled

for letter in letters:
    print("    myFont[{}] = \"{}\";".format(letters[letter], letter))

