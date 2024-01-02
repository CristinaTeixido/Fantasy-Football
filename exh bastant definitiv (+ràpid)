#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <iomanip>

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

    int position_num(){ // Retorna 0, 1 , 2 o 3 segons la posició del jugador
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

    // Afageix un jugador a l'equip augmentant els punts, el preu i el nombre de membres
    void add_member(Player p, int g){
        members[g].push_back(p);
        price += p.price;
        points += p.points;
        num_members += 1;
    }

    // Elimina un jugador de l'equip disminuint els punts, el preu i el nombre de membres
    void remove_member(Player p, int g){
        members[g].pop_back();
        price -= p.price;
        points -= p.points;
        num_members -= 1;
    }

};


// Declarem variables globals 
int maxTotalPrice, maxIndivPrice; 
int actual_max_points, t_start;
vector<Player> players;
vector<int> num_pl_position = {1, 0, 0, 0};       // nombre de jugadors de cada categoria segons la consulta det
vector<int> max_points(4, 0);


void max_punts_pos(Player& p){
    if(p.points > max_points[p.position_num()]) max_points[p.position_num()] = p.points;
}


/* Donada la suma parcial del preu d'un equip en construcció,
retorna true si utilitzant els hipòtetics millors jugadors de cada posició la suma de punts de l'equip sobrepassa,
la millor puntuació trobada fins el moment.
En canvi, retorna fals si l'alineació que s'està construint, no arriba a la millor puntuació ni amb els 
millors jugadors de cada categoria. */

bool future(Team& selected_team){
    int points = selected_team.points;
    for(int i = 0; i < 4; i++) points += (num_pl_position[i] - selected_team.members[i].size()) * max_points[i];
    if(points <= actual_max_points) return false;
    return true;
}


// Retrona true si pl es pot afegir a l'equip i les condicions de l'input es segueixen complint 
bool accepted_player(Team& team, Player& pl, int g){
    if (int(team.members[g].size()) == num_pl_position[g]) return false;
    if (pl.price > maxIndivPrice) return false;
    if (pl.price + team.price > maxTotalPrice) return false;
    return true;
}

// Escriu la solució en el format correcte
void write_solution(Team& selected_team, const string& output){
    ofstream fout(output);   

    // Comprovem que el fitxer s'obre correctament
    if (!fout.is_open()) {
        cout << "No s'ha pogut obrir el fitxer de sortida correctament." << endl;
        exit(1); 
    }

    int t_end = clock();
    double time = (double(t_end-t_start)/CLOCKS_PER_SEC);
    fout << fixed << setprecision(1) << time << endl;

    vector<string> positions = {"POR: ", "DEF: ", "MIG: ", "DAV: "};
    for (int i = 0; i < 4; ++i){
        fout << positions[i] << selected_team.members[i][0].name;
        for (uint j = 1; j < selected_team.members[i].size(); ++j) fout << ';' << selected_team.members[i][j].name;
        fout << endl;
    }

    fout << "Punts: " << selected_team.points << endl;
    fout << "Preu: " << selected_team.price << endl;

    fout.close();

}


void tactica_exh(const string& output, Team& selected_team, int id){
    if(!future(selected_team)) return; // Si no té futur no continuem amb la cerca

    // Si ja hem explorat tots els jugadors acabem la cerca
    if(id >= int(players.size())) return;

    Player p = players[id];

    // Si ja tenim 11 jugadors:
    if (selected_team.num_members == 11){
        // Si aquesta nova alineació té més punts que l'anterior amb més punts:
        if (selected_team.points > actual_max_points){ 
            actual_max_points = selected_team.points;  // Actualitzem els actual_max_points
            write_solution(selected_team, output); // Escrivim l'alineació 
        }
        return;
    }

    // Si l'alineació compleix les condicions de l'input:
    if(accepted_player(selected_team, p, p.position_num())){
        selected_team.add_member(p, p.position_num()); 
        tactica_exh(output, selected_team, id+1); 
        selected_team.remove_member(p, p.position_num()); 
        }

    tactica_exh(output, selected_team, id+1);
}


int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Entrada incorrecta. Es necessiten 3 arguments: <fitxer_jugadors> <fitxer_consulta> <fitxer_sortida>" << endl;
        exit(1);
    }
     // Llegeix el fitxer_consulta

    ifstream consulta(argv[2]);
    consulta >> num_pl_position[1] >> num_pl_position[2] >> num_pl_position[3] >> maxTotalPrice >> maxIndivPrice;
    consulta.close();
    
    // Llegim les dades dels jugadors
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
        if(price <= maxIndivPrice){
            Player player = Player(nextId++, name, position, price, club, p);
            max_punts_pos(player); // Calcula el max de punts que podem obtenir de cada posició
            players.push_back(player);
        }
    }
    
    jugadors.close();

    // Inicialitza el cronòmetre
    t_start = clock();

    // Inicialitza els actuals punts a 0 i l'equip buit
    actual_max_points = 0;
    Team selected_team = Team();

    // Crida la cerca exhaustiva
    tactica_exh(argv[3], selected_team, 0);

    cout << "Exhaustive finished" << endl;

}
