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
    int                     num_members;
    vector<vector<string>>  members;
    int                     points;
    int                     price;

    Team(int n, vector<vector<string>> m, int po, int pr):
        num_members(n), members(m), points(po), price(pr){};

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

    vector<string> positions = {"POR: ", "DEF: ", "MIG: ", "DAV: "};

    for (int i = 0; i<4; ++i){
        fout << positions[i] << selected_team.members[i][0];
        for (uint j=1; j<selected_team.members[i].size(), ++j) fout << ';' << selected_team.defenses[i][j];
        fout << endl;
    }

    fout << "Punts: " << selected_team.points << endl;
    fout << "Preu: " << selected_team.price << endl;

    fout.close();

}


// Define the objective function
int objective_function(const Team& solution) return solution.points;

// Define the neighborhood function
vector<Team> get_neighbors(const Team& solution){ // Added const qualifier
    vector<Team> neighbors;   
	for (i = 0; i < 4; i++) {
        Team neighbor = solution;
        neighbor.members[i] = {}; 			
		neighbors.push_back(neighbor);
	}
	return neighbors;
}

// Define the Tabu Search algorithm
std::vector<int>
tabu_search(const std::vector<int>& initial_solution, int max_iterations, int tabu_list_size){
	Team best_solution = initial_solution;
	Team current_solution = initial_solution;
	std::vector<std::vector<int>> tabu_list;

	for (int iter = 0; iter < max_iterations; iter++) {
		vector<Team> neighbors = get_neighbors(current_solution);
		std::vector<int> best_neighbor;
		int best_neighbor_fitness = std::numeric_limits<int>::max();

		for (const std::vector<int>& neighbor : neighbors) {
			if (std::find(tabu_list.begin(), tabu_list.end(), neighbor) == tabu_list.end()) {
				int neighbor_fitness = objective_function(neighbor);
				if (neighbor_fitness < best_neighbor_fitness) {
					best_neighbor = neighbor;
					best_neighbor_fitness= neighbor_fitness;
				}
			}
		}

		if (best_neighbor.empty()) {
			// No non-tabu neighbors found,
			// terminate the search
			break;
		}

		current_solution = best_neighbor;
		
        tabu_list.push_back(best_neighbor);
		if (tabu_list.size() > tabu_list_size) {
			// Remove the oldest entry from the
			// tabu list if it exceeds the size
			tabu_list.erase(tabu_list.begin());
		}

		if (objective_function(best_neighbor)
			< objective_function(best_solution)) {
			// Update the best solution if the
			// current neighbor is better
			best_solution = best_neighbor;
		}
	}

	return best_solution;
}

///int main()
{
	// Example usage
	// Provide an initial solution
	std::vector<int> initial_solution = { 1, 2, 3, 4, 5 };
	int max_iterations = 100;
	int tabu_list_size = 10;

	std::vector<int> best_solution = tabu_search(
		initial_solution, max_iterations, tabu_list_size);
	std::cout << "Best solution:";
	for (int val : best_solution) {
		std::cout << " " << val;
	}
	std::cout << std::endl;
	std::cout << "Best solution fitness: "
			<< objective_function(best_solution)
			<< std::endl;

	return 0;
}

///////

void tactica_greedy(Team& selected_team, string out){
    int pos = -1;

    while(pl < players.size() && selected_team.num_members < 11){
        Player p = players[pl];
        if(selected_team.price + p.price <= maxTotalPrice && p.points <= maxIndivPrice){
            if(p.position == "por" && selected_team.members[0].size() == 0) pos = 0;    
 
            else if (p.position == "def" && selected_team.members[1].size() < uint(nDef)) pos = 1;
            
            else if (p.position == "mig" && selected_team.members[2].size() < uint(nMig)) pos = 2;
            
            else if(p.position == "dav" && selected_team.members[3].size() < uint(nDav)) pos = 3;

            if (pos > -1){
                selected_team.points += p.points;
                selected_team.price += p.price;
                selected_team.num_members += 1;
                selected_team.members[pos].push_back(p.name)
                pos = -1;
            }
        }
        ++pl;

        if (selected_team.num_members == 11){
            if (selected_team.points > actual_max_points){
                actual_max_points = selected_team.points;
                write_solution(selected_team, output);
            }
            local_search(selected_team);
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
    Team selected_team = Team(0, {{}, {}, {}, {}} 0, 0);

    tactica_greedy(selected_team, argv[3]); 
}