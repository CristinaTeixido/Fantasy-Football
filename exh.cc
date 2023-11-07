" Ha d’implementar una cerca exhaustiva. Cada vegada que obtingui una 
  solució millor, l’ha de sobreescriure a l’arxiu de sortida. És a dir, 
  volem que si avortem el programa, dins l’arxiu de sortida hi hagi la millor 
  solució trobada fins al moment. No es pot combinar l’algorisme de cerca 
  exhaustiva amb el greedy o la metaheurística. És a dir, ha de ser una 
  cerca exhaustiva pura. "

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>

using namespace std;

class Player {
    public:
    int    id;
    string name;
    string position;
    int    price;
    string club;
    int    points;

    Player(int ident, const string& n, const string& pos, int pr, const string& c, int p):
        id(ident), name(n), position(pos), price(pr), club(c), points(p){}
};

vector<Player> id2player;

void tactica_exh(const string& sortida, uint nDef, uint nMig, 
              uint nDav, int maxTotalPrice, int maxIndivPrice){

    ofstream sortida(sortida);
            
}

int main(int argc, char* argv[]) {
    // Comprova que l'entrada sigui correcta
    if (argc != 4) {
        cout << "Entrada incorrecta. Es necessiten 3 arguments: <fitxer_jugadors> <fitxer_consulta> <fitxer_sortida>" << endl;
        exit(1);
    }

    // Llegeix les dades dels jugadors
    ifstream jugadors(argv[1]);
    int nextId = 0;
    while (not jugadors.eof()) {
        string name, club, position;
        int p;
        getline(jugadors,name,';');    if (name == "") break;
        getline(jugadors,position,';');
        int price; jugadors >> price;
        char aux; jugadors >> aux;
        getline(jugadors,club,';');
        jugadors >> p;
        string aux2;
        getline(jugadors,aux2);
        id2player.push_back(Player(nextId++, name, position, price, club, p));
    }
    jugadors.close();

    // Llegeix fitxer_consulta
    ifstream consulta(argv[2]);
    uint nDef, nMig, nDav;
    int maxTotalPrice, maxIndivPrice; 
    consulta >> nDef >> nMig >> nDav >> maxTotalPrice >> maxIndivPrice;
    consulta.close();

    // Crida la tactica de cerca exhaustiva
    tactica_exh(argv[3], nDef, nMig, nDav, maxTotalPrice, maxIndivPrice);

}
