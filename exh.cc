#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>

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

// Declarem dades globals de la consulta
uint nDef, nMig, nDav, N;
int maxTotalPrice, maxIndivPrice, nTotalPlayersDataset; 

vector<Player> id2player;
vector<Player> pors;
vector<Player> defs;
vector<Player> migs;
vector<Player> davs;


vector<string> selected_players;



void tactica_exh(const string& output, vector<bool>& used, vector<string>& selected_players, int sum_points, int sum_price, int sp, int last_selected){

    if (sp == N) //escriu sortida al arxiu si sum_points > punts_max_moment;

    else {

    if (sp == 0)
        for (uint p = 0; p < pors.size();++p){
            // si el p + sum_price > maxTotalPrice  >> continue

            if (not used[pors[p].id]){
                used[pors[p].id] = true;
                selected_players[0] = pors[p].name;
                tactica_exh(argv[3], used, selected_players, sum_points+pors[p].points, sum_price+pors[p].price, 1, -1)
                used[pors[p].id] = false;
            }
        }
    
    else if (sp > 0 && sp <= nDef){
        for (uint de = last_selected+1; de < defs.size(); ++de){
            used[defs[de].id] = true;
            selected_players[sp] = defs[de].name;
            tactica_exh(argv[3], used, selected_players, sum_points+defs[de].points, sum_price+defs[de].price, sp+1, de)
            used[defs[de].id] = false;
        }

    else if (sp > nDef && sp <= nMig){
        for (uint m = 0; m < migs.size(); ++m){
            used[migs[m].id] = true;
            selected_players[sp] = migs[m].name;
            tactica_exh(argv[3], used, selected_players, sum_points+migs[m].points, sum_price+migs[m].price, sp+1)
            used[migs[m].id] = false;
        }
    }
    
    else {
        for (uint da = 0; da < davs.size(); ++da){
            used[davs[da].id] = true;
            selected_players[sp] = davs[da].name;
            tactica_exh(argv[3], used, selected_players, sum_points+davs[da].points, sum_price+davs[da].price, sp+1)
            used[davs[da].id] = false;
            }
        }
    }

    }

    ofstream sortida(output);   
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

    nTotalPlayersDataset = nextId;
    jugadors.close();

    // Llegeix fitxer_consulta
    ifstream consulta(argv[2]);
    consulta >> nDef >> nMig >> nDav >> maxTotalPrice >> maxIndivPrice;
    consulta.close();

    N = 1+nDef+nMig+nDav;

    // Classifiquem tots els jugadors segona la seva posició
    for (Player& p: id2player){
        if (p.price > maxIndivPrice) continue;
        if (p.position == "por") pors.push_back(p);
        else if (p.position == "def") defs.push_back(p);
        else if (p.position == "mig") migs.push_back(p);
        else davs.push_back(p)
    }

    // Crida la tactica de cerca exhaustiva
    vector<bool> used = vector<bool>(N + 1, false);
    selected_players = vector<string>(N);

    tactica_exh(argv[3], used, selected_players, 0, 0, 0, 0);

    cout << "POR:" << endl;
    cout << "DEF:" << endl;

    cout << "MIG: " << endl;
    cout << "Punts:" << endl;
    cout << "Preu:" << endl;

}
