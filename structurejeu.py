import numpy as np 

# exemple de tableau de labyrinthe
l = [
    [[1, 0, 1], # composante horizontale du tableau
     [1, 1, 1], 
     [0, 1, 0]],
    [
     [0, 0],    # composante verticale du tableau
     [0, 0], 
     [0, 0],
     [0, 0]]
]
# comment savoir si le tableau serait un labyrinthe ?

l1 = [[1, 0, 1, 1, 1, 1, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0]] # inconvenients : toujours faire en sorte que le plus grand tableau soit le deuxième.
l2 = [[0, 0, 0, 0, 1, 0, 0, 1, 0], [1, 0, 1, 0, 1, 0, 0, 0]]
def labyoupas(l):
    """ 
    cherche à déterminer si la liste est bien un labyrinthe, en faisant une exploration 
    d'un chemin du début à la fin de la liste
    """
    cond = True
    while cond:
        for i in range(len(l[0])):
            if l[0][i] == 1:
                l = [l[1][i:len(l[1])], l[0][i:len(l[0])]]
                cond = True
                break
            cond = False
        


print(labyoupas(l1))
print(labyoupas(l2))