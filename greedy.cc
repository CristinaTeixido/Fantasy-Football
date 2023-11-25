#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <iomanip>
#include <cmath>

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
        id(ident), name(n), position(pos), price(pr), club(c), points(p){};
};

// Declarem dades globals de la consulta
int nDef, nMig, nDav, sp;
int maxTotalPrice, maxIndivPrice, nTotalPlayersDataset; 
int actual_max_points, t_start, t_end;

vector<Player> id2player;
vector<Player> pors;
vector<Player> defs;
vector<Player> migs;
vector<Player> davs;


vector<string> selected_players;

void write_solution(const vector<string>& selected_players, 
                    const string& output, int points, int price){
    ofstream fout(output);   

    // Comprovem que el fitxer s'obre correctament
    if (!fout.is_open()) {
        cout << "No s'ha pogut obrir el fitxer de sortida correctament." << endl;
        exit(1); 
    }

    t_end = clock();
    double time = (double(t_end-t_start)/CLOCKS_PER_SEC);
    fout << fixed << setprecision(1) << time << endl;

    fout << "POR: " << selected_players[0] << endl;
    
    fout << "DEF: "; bool primer = true;
    for (int i = 1; i < 11; ++i){
        if (i == nDef + 1){
            fout << endl;
            fout << "MIG: "; 
            primer = true;
            }

        if (i == nDef+nMig+1){
            fout << endl;
            fout << "DAV: "; 
            primer = true;
            }
        
        if (primer) {
            fout << selected_players[i]; 
            primer = false;
        }
        else fout << ';' << selected_players[i];
    }

    fout << endl;
    fout << "Punts: " << points << endl;
    fout << "Preu: " << price << endl;

    fout.close();

}


void tactica_greedy(const vector<Player>& players, vector<string>& selected_players, 
                   int num, int& sum_points, int& sum_price){
    for(uint p = 0; p < players.size(); p++){
        if (num == 0) break;
        else{
            if(sum_price + players[p].price <= maxTotalPrice && players[p].points <= maxIndivPrice){
                selected_players.push_back(players[p].name); 
                sum_points += players[p].points;
                sum_price += players[p].price;
                num -= 1;
            }
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


    // ordena els jugadors en ordre descendent respecte els punts
    sort(id2player.begin(), id2player.end(), [](const Player& a, const Player& b) {
        return a.points > b.points; 
    });
    
    // Classifiquem tots els jugadors segona la seva posició
    for (Player& p: id2player){
        if (p.price > maxIndivPrice) continue;
        if (p.points == 0 && p.price > 0) continue;
        
        if (p.position == "por") pors.push_back(p);
        else if (p.position == "def") defs.push_back(p);
        else if (p.position == "mig") migs.push_back(p);
        else davs.push_back(p);
    }

    // Inicialitzem el cronòmetre
    t_start = clock();

    // Crida la tactica de cerca exhaustiva
    vector<string> selected_players;

    int actual_price = 0;
    int actual_points = 0;

    tactica_greedy(pors, selected_players, 1, actual_points, actual_price); //porters
    tactica_greedy(defs, selected_players, nDef, actual_points, actual_price); //defenses
    tactica_greedy(migs, selected_players, nMig, actual_points, actual_price); //migcampistes
    tactica_greedy(davs, selected_players, nDav, actual_points, actual_price); //davanters

    write_solution(selected_players, argv[3], actual_points, actual_price);
}
