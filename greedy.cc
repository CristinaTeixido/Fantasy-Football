#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <iomanip>
#include <numeric> // Added to include accumulate
#include <cmath>

using namespace std;

class Player {
    public:
    string  name;
    string  position;
    int     price;
    string  club;
    int     points;
    float   efficiency;

    Player(const string& n, const string& pos, int pr, const string& c, int p, float e):
        name(n), position(pos), price(pr), club(c), points(p), efficiency(e){};

    bool operator==(const Player& b){
        return (name == b.name);
    }

    bool operator!=(const Player& b){
        return (name != b.name);
    }

    int position_num(){
        if (position == "por") return 0;    
        if (position == "def") return 1;
        if (position == "mig") return 2;
        else return 3;
    }

};

class Team {
    public:
    int                     num_members;
    vector<vector<Player>>  members;
    int                     points;
    int                     price;

    Team():
        num_members(0), members({{},{},{},{}}), points(0), price(0){};

    Team(int n, vector<vector<Player>> m, int po, int pr):
        num_members(n), members(m), points(po), price(pr){};
    
    bool operator==(const Team& b){
        if (num_members != b.num_members || points != b.points || price != b.price || members[0][0].name != b.members[0][0].name) return false;
        for (int i = 1; i<4; ++i){
            for (uint j=0; j<members[i].size(); ++j){
                if (members[i][j].name != b.members[i][j].name) return false;
            }
        }
        return true;
    }

    void add_member(Player p, int g){
        members[g].push_back(p);
        price += p.price;
        points += p.points;
        num_members += 1;
    }

    void write(int t_start, string output){
        ofstream fout(output);   
        int t_end = clock();
        double time = (double(t_end-t_start)/CLOCKS_PER_SEC);
        fout << fixed << setprecision(1) << time << endl;

        vector<string> positions = {"POR: ", "DEF: ", "MIG: ", "DAV: "};

        for (int i = 0; i<4; ++i){
            fout << positions[i] << members[i][0].name;
            for (uint j=1; j <members[i].size(); ++j) fout << ';' << members[i][j].name;
            fout << endl;
        }
        fout << "Punts: " << points << endl;
        fout << "Preu: " << price << endl;
    }
    

};

// Declarem dades globals de la consulta
int nDef, nMig, nDav;
int maxTotalPrice, maxIndivPrice; 
int actual_max_points, t_start;
string output;

vector<Player> players;
vector<int> num_pl_position;

bool accepted_player(Team& team, Player& pl, int g){
    if (int(team.members[g].size()) == num_pl_position[g]) return false;
    if (pl.price > maxIndivPrice) return false;
    if (pl.price + team.price > maxTotalPrice) return false;
    return true;
}

void write_solution(Team& selected_team){
    ofstream fout(output);   

    // Comprovem que el fitxer s'obre correctament
    if (!fout.is_open()) {
        cout << "No s'ha pogut obrir el fitxer de sortida correctament." << endl;
        exit(1); 
    }

    selected_team.write(t_start, output);

    fout.close();

}


void tactica_greedy(Team& selected_team){
    int pl = 0;
    while(pl < int(players.size()) && selected_team.num_members < 11){
        Player p = players[pl];

        if(accepted_player(selected_team, p, p.position_num())) selected_team.add_member(p, p.position_num());
        ++pl;

        if (selected_team.num_members == 11){
            write_solution(selected_team);
            break;
        }
    }
    
    cout << "greedy terminated" << endl;
}

void add_player_list(vector<vector<Player>>& lists, Player p){
    lists[p.position_num()].push_back(p);
}

void read(int argc, char** argv){
    // Llegeix les dades dels jugadors
    ifstream jugadors(argv[1]);
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

        //Classifiquem els jugadors segons posició i separem els fakes
        players.push_back(Player(name, position, price, club, p, efficiency));
    }

    jugadors.close();

    // Llegeix fitxer_consulta
    ifstream consulta(argv[2]);
    consulta >> nDef >> nMig >> nDav >> maxTotalPrice >> maxIndivPrice;
    consulta.close();
}

int main(int argc, char** argv) {
    // Comprova que l'entrada sigui correcta
    if (argc != 4) {
        cout << "Entrada incorrecta. Es necessiten 3 arguments: <fitxer_jugadors> <fitxer_consulta> <fitxer_sortida>" << endl;
        exit(1);
    }

    read(argc, argv);

    // ordena els jugadors en ordre descendent respecte els punts/preu
    sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.efficiency > b.efficiency; 
    });    

    // Inicialitzem el cronòmetre
    t_start = clock();

    // Inicialitzem l'equip
    Team selected_team = Team();
    num_pl_position = {1, nDef, nMig, nDav};
    output = argv[3];

    tactica_greedy(selected_team); 
}
