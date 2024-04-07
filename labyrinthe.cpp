#include <unordered_map>
#include <iostream>
#include <vector>
#include <functional>
using namespace std;
using sommet = pair<int, int>;
// Fonction de hashage pour que la map fonctionne
struct sommet_hash {
    std::size_t operator()(const sommet& s) const {
        std::hash<int> hasher;
        return hasher(s.first) ^ hasher(s.second);
    }
};

struct labyrinthe {
    int m;
    int n;
    unordered_map<sommet, vector<sommet>, sommet_hash> graph; // DEBAT : devons-nous utiliser une map ou une unordered map
    // IDEE : utiliser liste chainée à la place de vector<sommet>, pour facilité d'insertion et délétion
    // Méthode utilisée lors de l'appel d'un constructeur
    void grille();
    labyrinthe(int valm, int valn) : n(valn), m(valm) { grille(); }
    bool estConnexe() const;
    bool deconnecte(sommet node1, sommet node2);
};

void labyrinthe::grille() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            sommet node = {i, j};
            graph[node] = vector<sommet>(0);
            if (i > 0) {
                graph[node].push_back({i-1, j});
            }
            if (i < n-1) {
                graph[node].push_back({i+1, j});
            }
            if (j > 0) {
                graph[node].push_back({i, j-1});
            }
            if (j < m-1) {
                graph[node].push_back({i, j+1});
            }
        }
    }
}
bool labyrinthe::estConnexe() const {
    unordered_map<sommet, bool, sommet_hash> visited;
    int visitCount = 0;

    function<void(const sommet&)> dfs = [&](const sommet& u) {
        /**
        fonction lambda "dfs", qui prend en paramétre un sommet u par référence, toutes les
        variables présentes dans la fonction lambda sont prises par référence par rapport aux
        variables du scope de la fonction "estConnexe"
        **/
        if (visited[u]) return;
        visited[u] = true;
        visitCount++;

        for (const auto& v : graph.at(u)) {
            if (!visited[v])
                dfs(v);
        }
    };

    dfs(graph.begin()->first); // commence le dfs depuis le premier noeud dans la map

    return visitCount == m*n; // on vérifie que le nombre de visite est bien de m * n
}
ostream& operator<<(ostream& out, const labyrinthe& lab) {
    for (const auto& [key, adjacents] : lab.graph) {
        out << "(" << key.first << ", " << key.second << "): ";
        for (const auto& adj : adjacents) {
            out << "(" << adj.first << ", " << adj.second << ") ";
        }
        out << "\n";
    }
    return out;
}
bool labyrinthe::deconnecte(sommet node1, sommet node2) {
    bool a = false;
    bool b = false;
    for (auto &neigh: graph[node1]) {
        if (neigh == node2) {
            // remplacer neigh par dernier element
            neigh = graph[node1][graph[node1].size()-1];
            graph[node1].pop_back();
            a = true;
        }
    }
    for (auto &neigh: graph[node2]) {
        if (neigh == node1) {
            // remplacer neigh par dernier element
            neigh = graph[node2][graph[node2].size()-1];
            graph[node2].pop_back();
            b = true;
        }
    }
    return (a && b);
}
int main() {
    labyrinthe lab{3,2};
    cout << lab;
    cout << lab.estConnexe();
    // faire des doctest
    lab.deconnecte({1, 0}, {0, 0});
    lab.deconnecte({0, 1}, {0, 0});
    cout << lab.estConnexe();
    cout << lab;
    
    return 0;
}
