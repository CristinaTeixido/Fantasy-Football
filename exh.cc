#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>

using namespace std;

#include <iomanip>

int main() {
    double numero = 123.45678;
    std::cout << std::fixed << std::setprecision(1) << numero << std::endl;
    return 0;
}


class Player {
    public:
    int    id;
    string name;
    string position;
    int    price;
    string club;
    int    points;

    Player(int ident, const string& n, const string& pos, int pr, const string& c, int p):
        id(ident), name(n), position(pos), price(pr), club(c), points(p){};
};

// Declarem dades globals de la consulta
int nDef, nMig, nDav, sp;
int maxTotalPrice, maxIndivPrice, nTotalPlayersDataset; 
int actual_max_points;

vector<Player> id2player;
vector<Player> pors;
vector<Player> defs;
vector<Player> migs;
vector<Player> davs;


vector<string> selected_players;

void write_solution(const vector<string>& selected_players, const string& output, int points, int price){
    ofstream fout(output);   
    fout << "a" << endl;

    // Comprovem que el fitxer s'obre correctament
    if (!fout.is_open()) {
        std::cerr << "No s'ha pogut obrir el fitxer de sortida correctament." << std::endl;
        exit(1); 
    }

    fout << "POR:" << selected_players[0] << endl;
    fout << "DEF: ";

    for (int i = 1; i<=nDef; ++i) fout << selected_players[i] << ' ';
    fout << endl;

    fout << "MIG: ";
    for (int i = nDef+1; i<=1+nDef+nMig; ++i) fout << selected_players[i] << ' ';

    fout << "DAV: ";
    for (int i = 2+nDef+nMig; i< 11; ++i) fout << selected_players[i] << ' ';

    fout << "Punts:" << points << endl;
    fout << "Preu:" << price << endl;

    fout.close();

}


void tactica_exh(const string& output, vector<string>& selected_players, int sum_points, int sum_price, int last_selected){
    if (sp == 11) {
        if (sum_points > actual_max_points){
            cout << sum_points << endl; 
            actual_max_points = sum_points;
        }
            
    }
     
    //write_solution(selected_players, output, sum_points, sum_price);

    if (sp == 0){
        for (uint p = 0; p < pors.size();++p){              // busquem totes les possibles linies per tots els porters
            selected_players[0] = pors[p].name; 
            sp += 1;
            tactica_exh(output, selected_players, sum_points+pors[p].points, sum_price+pors[p].price, -1);
            sp -= 1;
        }
    }
    
    else if (sp > 0 && sp <= nDef){
        for (uint de = last_selected+1; de < defs.size(); ++de){
            if (defs[de].price + sum_price > maxTotalPrice) continue;
            selected_players[sp] = defs[de].name; 
            sp += 1;
            tactica_exh(output, selected_players, sum_points+defs[de].points, sum_price+defs[de].price, de);
            sp -= 1;
        }
    }

    else if (sp > nDef && sp <= nDef+nMig){
        if (sp == nDef+1) last_selected = -1;
        for (uint m = last_selected+1; m < migs.size(); ++m){
            if (migs[m].price + sum_price > maxTotalPrice) continue;
            selected_players[sp] = migs[m].name; 
            sp += 1;
            tactica_exh(output, selected_players, sum_points+migs[m].points, sum_price+migs[m].price,  m);
            sp -= 1;
        }
    }
    
    else if (sp > nDef+nMig && sp < 11){
        if (sp == nDef+nMig+1) last_selected = -1;
        for (uint da = last_selected+1; da < davs.size(); ++da){
            if (davs[da].price + sum_price > maxTotalPrice) continue;
            selected_players[sp] = davs[da].name; 
            sp += 1;
            tactica_exh(output, selected_players, sum_points+davs[da].points, sum_price+davs[da].price, da);
            sp -= 1;
        }
    }
}

int main(int argc, char** argv) {
    // Comprova que l'entrada sigui correcta
    if (argc != 4) {
        cout << "Entrada incorrecta. Es necessiten 3 arguments: <fitxer_jugadors> <fitxer_consulta> <fitxer_sortida>" << endl;
        exit(1);
    }

    // Llegeix les dades dels jugadors
    ifstream jugadors(argv[1]);
    int nextId = 0;
    while (!jugadors.eof()) {
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


    // Classifiquem tots els jugadors segona la seva posició
    for (Player& p: id2player){
        if (p.price > maxIndivPrice) continue;
        if (p.position == "por") pors.push_back(p);
        else if (p.position == "def") defs.push_back(p);
        else if (p.position == "mig") migs.push_back(p);
        else davs.push_back(p);
    }

    // Crida la tactica de cerca exhaustiva
    selected_players = vector<string>(12);
    cout << 'a' << nTotalPlayersDataset << endl;

    actual_max_points = 0;

    ofstream fout(argv[3]);   
    fout << "a";

    sp = 0; 
    tactica_exh(argv[3], selected_players, 0, 0, 0);
}
