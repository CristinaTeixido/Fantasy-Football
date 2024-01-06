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
        for (int i = 1; i < 4; ++i){
            for (uint j = 0; j<members[i].size(); ++j){
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
            for (uint j=1; j<members[i].size(); ++j) fout << ';' << members[i][j].name;
            fout << endl;
        }
        fout << "Punts: " << points << endl;
        fout << "Preu: " << price << endl;
    }
    

};


// Declarem variables globals 
int nDef, nMig, nDav;
int maxTotalPrice, maxIndivPrice; 
int actual_max_points, t_start;
string output;

vector<Player> players;
vector<vector<Player>> players_by_position;
vector<vector<Player>> fake_players;

vector<int> num_pl_position;


int randInt(int b){
    assert(b >= 0);
    if(b == 0) return 0;
    return rand() % (b+1);
}

//jugador de l'equip
Player getPlayer(const Team& equip, int pos, int jugador) {
    if (pos == 0) return equip.members[0][0];
    return equip.members[pos][jugador];
}

bool repeated_player(const Team& team, const Player& pl, int g){
    for(uint i = 0; i< team.members[g].size(); ++i){
        if (team.members[g][i].name == pl.name) return true;
    }
    return false;
}


bool exchangePlayer (int pos, int jugador, Player& newP, Player& oldP, Team& equip){
    if(repeated_player(equip, newP, pos)) return false;
    equip.price += (newP.price - oldP.price);
    equip.points += (newP.points -  oldP.points);
    equip.members[pos][jugador] = newP;
    return true;
    
}


bool accepted_player(Team& team, Player& pl, int g, bool repeated){
    if (pl.price > maxIndivPrice) return false;
    //if (pl.price + team.price > maxTotalPrice) return false;
    if (repeated) if (repeated_player(team, pl, g)) return false;
    //if (int(team.members[g].size()) == num_pl_position[g]) return false;
    return true;
}



//jugador random
Player randPlayer(Team& team, int g){
    int n = players_by_position[g].size();
    Player pl = players_by_position[g][randInt(n-1)];
    int tries = 0;
    while(!accepted_player(team, pl, g, true)){
        if (tries < 200){
            pl = players_by_position[g][randInt(n-1)];
            ++tries;
        }

        else{
            int n = fake_players[g].size();
            for(int k = 0; k < n; ++k){
                pl = fake_players[g][k];
                if (!repeated_player(team, pl, g)) return pl;
            }
        }
    }

    return pl; 
}


bool fake_player(string name){
    if (name.substr(0,4) == "Fake") return true;
    return false;
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


vector<Team> get_neighbors(Team& solution){ 
    vector<Team> neighbours;   
	for (int i = 0; i < 400; ++i){       
        Team equip = solution; 
        bool selected = false;
        while (!selected){
            int pos = randInt(3);
            int jugador = randInt(solution.members[pos].size()-1);
            Player oldP = getPlayer(solution, pos, jugador);
            Player newP = randPlayer(solution, pos);
            if((solution.price - oldP.price + newP.price) <= maxTotalPrice){
                selected = exchangePlayer(pos, jugador, newP, oldP, equip);
                neighbours.push_back(equip);
            }
        }
    }

    return neighbours;
}

         
// Algoritme Tabu Search
void tabu_search(Team& initial_solution, int max_iterations, int max_size){
	Team best_solution = initial_solution;
	Team current_solution = initial_solution;
	vector<Team> tabu_list;

	for (int iter = 0; iter < max_iterations; iter++){
		vector<Team> neighbors = get_neighbors(current_solution);
		Team best_neighbor = Team();
		int actual_max_points = 0;

		for (Team& neighbor: neighbors) {
            if (find(tabu_list.begin(), tabu_list.end(), neighbor) == tabu_list.end()) {
                if (neighbor.points >= actual_max_points) {
                    best_neighbor = neighbor;
                    actual_max_points = neighbor.points;
                }
            }
		}

		if (best_neighbor.num_members == 0) break;

		current_solution = best_neighbor;
        
        if (int(tabu_list.size()) > max_size) tabu_list.erase(tabu_list.begin());
        tabu_list.push_back(best_neighbor);
		if (best_neighbor.points > best_solution.points) {
            best_solution = best_neighbor;
            write_solution(best_solution);

	    }

    }

    cout << "metaheuristic terminated" << endl;

}


void tactica_greedy(Team& selected_team){
    int pl = 0;
    while(pl < int(players.size()) && selected_team.num_members < 11){
        Player p = players[pl];

        if(accepted_player(selected_team, p, p.position_num(), false) && p.price + selected_team.price < maxTotalPrice && int(selected_team.members[p.position_num()].size()) != num_pl_position[p.position_num()]) selected_team.add_member(p, p.position_num());
        ++pl;

        if (selected_team.num_members == 11){
            write_solution(selected_team);
            break;
        }
    }
    
    cout << "greedy terminated" << endl;
    cout << selected_team.points << endl;
    tabu_search(selected_team, 10000, 100);
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
        Player player = Player(name, position, price, club, p, efficiency);
        if (fake_player(name)) add_player_list(fake_players, player);
        else add_player_list(players_by_position, player);
        
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

    players_by_position = {{},{},{},{}};
    fake_players= {{},{},{},{}};


    read(argc, argv);

    //ordena els jugadors en ordre ascendent respecte els punts/log(preu)
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
