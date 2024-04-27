#include <iostream>
#include<vector>
#include <unordered_map>
#include <algorithm> // pour la fonction shuffle dans générerAleatoire
#include <random> // pour pouvoir faire fonctionner shuffle
#include <fstream>
using namespace std;
auto rng = std::default_random_engine {time(0)};
struct Cell {
    int x, y;
    unordered_map<string, bool> Murs;
    bool marked = false;

    Cell(int x, int y);

    struct CellHash {
        size_t operator()(const Cell& c) const {
            return hash<int>()(c.x) ^ hash<int>()(c.y);
        }
    };
    bool operator==(const Cell& c) const {
        return x == c.x && y == c.y;
    }
};

Cell::Cell(int x, int y) {
    this->x = x;
    this->y = y;
    Murs["up"] = true;
    Murs["down"] = true;
    Murs["right"] = true;
    Murs["left"] = true;
}

struct Labyrinthe {
    int n, m;
    vector<Cell*> cells;
    unordered_map<Cell, vector<Cell>, Cell::CellHash> adj;

    // constructeur
    Labyrinthe(int n, int m);

    // methodes
    void afficher();
    int recherche(Cell c);
    /** Renvoit l'indice d'une cellule dans un des vecteurs de la matrice d'adjacence donné */
    int index(vector<Cell> cellules,Cell c);
    void deconnecte(Cell c1, Cell c2);
    void genererAleatoire(Cell c);
    /** écrit le labyrinthe dans un fichier texte 
     *  @param filename le nom du fichier
     * @return un objet ofstream pour écrire dans le fichier
    */
    ofstream write(string filename);
};

Labyrinthe::Labyrinthe(int n, int m) {
    this->n = n;
    this->m = m;
    this->cells = vector<Cell*>(n * m); // Allocate memory for all cells
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cells[i*m + j] = new Cell(i, j); // Create the cells
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Cell *c = cells[i*m + j];
            adj[*c] = vector<Cell>{};
            if (i > 0) {
                adj[*c].push_back(*cells[(i-1)*m + j]);
            }
            if (i < n-1) {
                adj[*c].push_back(*cells[(i+1)*m + j]);
            }
            if (j > 0) {
                adj[*c].push_back(*cells[i*m + (j-1)]);
            }
            if (j < m-1) {
                adj[*c].push_back(*cells[i*m + (j+1)]);
            }
        }
    }
}
void Labyrinthe::afficher() {
    // affiche la liste d'adjacence du labyrinthe
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Cell *c = cells[i*m + j];
            cout << "Cell (" << c->x << ", " << c->y << ") : ";
            for (Cell c2 : adj[*c]) {
                cout << "(" << c2.x << ", " << c2.y << ") ";
            }
            cout << endl;
        }
    }
}
int Labyrinthe::recherche(Cell c) {
    // calcul de l'indice
    int index = c.x * m + c.y;

    // verifications
    if (index >= 0 && index < n * m) {
        return index;
    } else {
        return -1; // Cell not found
    }
}
void Labyrinthe::deconnecte(Cell c1, Cell c2) {
    // supprime le mur entre les cellules c1 et c2
    int pos1 = recherche(c1);
    int pos2 = recherche(c2);
    if (pos1 == -1 || pos2 == -1) {
        throw invalid_argument("Cellule non trouvée");
    }
    Cell* pc1 = cells[recherche(c1)];
    Cell* pc2 = cells[recherche(c2)];
    // evaluation d'une erreur de lecture de pointeur
    if (pc1 == nullptr || pc2 == nullptr) {
        throw invalid_argument("renvoi de pointeur null");
    }
    // evaluation de la proximité des cellules
    else if ((pc1->x == pc2->x && pc1->y == pc2->y) || (abs(pc1->x - pc2->x) + abs(pc1->y - pc2->y) != 1)) {
        throw invalid_argument("Les cellules ne sont pas adjacentes");
    }
    if (pc1->x == pc2->x) {
        if (pc1->y < pc2->y) {
            pc1->Murs["up"] = false;
            pc2->Murs["down"] = false;
        } else {
            pc1->Murs["down"] = false;
            pc2->Murs["up"] = false;
        }
    } else { // pc1->y == pc2->y
    if (pc1->x < pc2->x) {
        pc1->Murs["right"] = false;
        pc2->Murs["left"] = false;
    } else {
        pc1->Murs["left"] = false;
        pc2->Murs["right"] = false;
    }
}
}
int Labyrinthe::index(vector<Cell> cellules,Cell c) {
    for (int i = 0; i < cellules.size(); i++) {
        if (cellules[i].x == c.x && cellules[i].y == c.y) {
            return i;
        }
    }
    return -1;
}
void Labyrinthe::genererAleatoire(Cell c) {
    Cell* current = cells[c.x*m + c.y];
    current->marked = true;
    vector<Cell> neighbors = adj[c];
    shuffle(neighbors.begin(), neighbors.end(), rng); // remplacer shuffle par random_shuffle si on utilise c++11
    for (Cell& c2 : neighbors) {
        if ((cells[recherche(c2)]->marked) == false) {
            deconnecte(c, c2);
            genererAleatoire(c2);
        }
    }
}
ofstream Labyrinthe::write(string filename) {
    ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw invalid_argument("Impossible d'ouvrir le fichier");
    }
    // voici le format : (x,y) : up down right left avec up down right et left des booléens
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Cell *c = cells[i*m + j];
            file << "(" << c->x << ", " << c->y << ") : ";
            if (c->Murs["up"]) {
                file << 1;
            }
            else {
                file << 0;
            }
            if (c->Murs["down"]) {
                file << 1;
            }
            else {
                file << 0;
            }
            if (c->Murs["right"]) {
                file << 1;
            }
            else {
                file << 0;
            }
            if (c->Murs["left"]) {
                file << 1;
            }
            else {
                file << 0;
            }
            file << endl;
        }
    }
    return file;
}
int main() {
    for (int i = 0; i < 100; i++) {
        int width = 100;
        int height = 100;
        Labyrinthe laby(width, height);
        laby.genererAleatoire(Cell(rand() % width, rand() % height));
    }
    int width = 100;
    int height = 100;
    Labyrinthe laby(width, height);
    laby.genererAleatoire(Cell(rand() % width, rand() % height));
    ofstream file = laby.write("laby.txt");
    file.close();
    return 0;
}