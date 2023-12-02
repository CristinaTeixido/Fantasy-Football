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
    int     id;
    string  name;
    string  position;
    int     price;
    string  club;
    int     points;
    float   efficiency;

    Player(int ident, const string& n, const string& pos, int pr, const string& c, int p, float e):
        id(ident), name(n), position(pos), price(pr), club(c), points(p), efficiency(e){};
};

class Team {
    public:
    int            members;
    vector<string> keeper;
    vector<string> defenses;
    vector<string> midfielder;
    vector<string> striker;
    int            points;
    int            price;

    Team(int m, vector<string> k, vector<string> d, vector<string> mi, vector<string> s, int po, int pr):
        members(m), keeper(k), defenses(d), midfielder(mi), striker(s), points(po), price(pr){};

};

// Declarem dades globals de la consulta
int nDef, nMig, nDav;
int maxTotalPrice, maxIndivPrice; 
int actual_max_points, t_start, t_end;

vector<Player> players;

void write_solution(Team& selected_team, const string& output){
    ofstream fout(output);   

    // Comprovem que el fitxer s'obre correctament
    if (!fout.is_open()) {
        cout << "No s'ha pogut obrir el fitxer de sortida correctament." << endl;
        exit(1); 
    }

    t_end = clock();
    double time = (double(t_end-t_start)/CLOCKS_PER_SEC);
    fout << fixed << setprecision(1) << time << endl;

    fout << "POR: " << selected_team.keeper[0] << endl;
    
    fout << "DEF: " << selected_team.defenses[0]; 
    for (int i = 1; i < nDef; ++i) fout << ';' << selected_team.defenses[i];
    fout << endl;

    fout << "MIG: "; fout << selected_team.midfielder[0]; 
    for (int i = 1; i < nMig; ++i) fout << ';' << selected_team.midfielder[i];
    fout << endl;

    fout << "DAV: "; fout << selected_team.striker[0]; 
    for (int i = 1; i < nDav; ++i) fout << ';' << selected_team.striker[i];
    fout << endl;

    fout << "Punts: " << selected_team.points << endl;
    fout << "Preu: " << selected_team.price << endl;

    fout.close();

}


void tactica_greedy(Team& selected_team, string out){
    bool afegir = false;

    for(Player p : players){
        if (selected_team.members == 11) break;
        if(selected_team.price + p.price <= maxTotalPrice && p.points <= maxIndivPrice){
            if(p.position == "por" && selected_team.keeper.size() == 0) {         
                selected_team.keeper.push_back(p.name);
                afegir = true;
            }
            
            else if (p.position == "def" && selected_team.defenses.size() < uint(nDef)){
                selected_team.defenses.push_back(p.name);
                afegir = true;
            }   
            
            else if (p.position == "mig" && selected_team.midfielder.size() < uint(nMig)){
                selected_team.midfielder.push_back(p.name);
                afegir = true;
            }
            
            else if(p.position == "dav" && selected_team.striker.size() < uint(nDav)){
                selected_team.striker.push_back(p.name);
                afegir = true;
            }

            if (afegir){
                selected_team.points += p.points;
                selected_team.price += p.price;
                selected_team.members += 1;
                afegir = false;
            }
        }
    }

    write_solution(selected_team, out);

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

        float efficiency;
        if (price != 0) efficiency = p/log(price);
        else if (p != 0) efficiency = 20;
        else efficiency = 0;
        players.push_back(Player(nextId++, name, position, price, club, p, efficiency));
    }

    jugadors.close();

    // Llegeix fitxer_consulta
    ifstream consulta(argv[2]);
    consulta >> nDef >> nMig >> nDav >> maxTotalPrice >> maxIndivPrice;
    consulta.close();


    // ordena els jugadors en ordre descendent respecte els punts
    sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.efficiency > b.efficiency; 
    });
    

    // Inicialitzem el cronòmetre
    t_start = clock();

    // Inicialitzem l'equip
    Team selected_team = Team(0, {}, {}, {}, {}, 0, 0);

    tactica_greedy(selected_team, argv[3]); 
}
