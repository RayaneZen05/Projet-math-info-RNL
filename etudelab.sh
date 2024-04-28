#!/bin/bash

# Ce script a pour objectif de faire l'étude du nombre de labyrinthes unique de largeur donnée et de hauteur donnée
# On commence par lire la largeur et la hauteur voulue, puis on compile labyrinthe.cpp avec les args donnés
# Ensuite, une fois le programme executé, on execute hashlab et on renvoie le nombre de labyrinthes uniques

echo "Entrez la largeur du labyrinthe"
read largeur
echo "Entrez la hauteur du labyrinthe"
read hauteur
echo "Entrez le nombre de labyrinthes à générer"
read nb_laby
rm -r media
mkdir media
g++ labyrinthe.cpp -o labyrinthe -std=c++11 -O3
./labyrinthe $largeur $hauteur $nb_laby
rm labyrinthe
./hashlab.sh
cd media/labuniques
echo "Il y a $(ls | wc -l) labyrinthes uniques"