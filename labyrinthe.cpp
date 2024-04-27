#include <iostream>
#include<vector>
#include <unordered_map>
#include <algorithm> // pour la fonction shuffle dans générerAleatoire
#include <random> // pour pouvoir faire fonctionner shuffle
#include <fstream>
using namespace std;

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
    if ((c.x*m + c.y >= m * n || c.x*m + c.y) < 0) {
        return -1;
    }
    else {
        return c.x*m + c.y;
    }
    /*
    // recherche la cellule c dans le vecteur de cellules
    for (int i = 0; i < cells.size(); i++) {
        if (cells[i]->x == c.x && cells[i]->y == c.y) {
            return i;
        }
    }
    return -1;
    */
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
        // modifier dans adj aussi
        if (pc1->y < pc2->y) {
            //adj[c1][index(adj[c1], c2)].Murs["left"] = false;
            //adj[c2][index(adj[c1], c1)].Murs["right"] = false;
            pc1->Murs["up"] = false;
            pc2->Murs["down"] = false;
        } else {
            //adj[c1][index(adj[c1], c2)].Murs["right"] = false;
            //adj[c2][index(adj[c1], c1)].Murs["left"] = false;
            pc1->Murs["down"] = false;
            pc2->Murs["up"] = false;
        }
    } else {
        if (pc1->x < pc2->x) {
            //adj[c1][index(adj[c1], c2)].Murs["up"] = false;
            //adj[c2][index(adj[c1], c1)].Murs["down"] = false;
            pc1->Murs["right"] = false;
            pc2->Murs["left"] = false;
        } else {
            //adj[c1][index(adj[c1], c2)].Murs["down"] = false;
            //adj[c2][index(adj[c1], c1)].Murs["up"] = false;
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
    shuffle(neighbors.begin(), neighbors.end(), default_random_engine(time(0))); // remplacer shuffle par random_shuffle si on utilise c++11
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
    int width = 4;
    int height = 6;
    Labyrinthe laby(width, height);
    laby.genererAleatoire(Cell(rand() % width, rand() % height));
    ofstream file = laby.write("laby.txt");
    file.close();
    return 0;
}