/*  Dupla: Carlos Eduardo da Silva Ferreira - 397611
 *         Alcides Ribeiro Sampaio Neto - 402138
 *
 * Todos o algoritmos de busca apresentados em aula foram implementados.
 */

#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

class State {
public:
    string city;

    State(string city_) {
        city = city_;
    }
};

class Action {
public:
    string act;

    Action(State *from, State *to) {
        act = "Drive from " + from->city + " to " + to->city;
    }
};

class Node {
public:
    State *location;
    Node *dad;
    Action *act;
    int cost;
    int heuristic;

    Node(State *c, Node *d, Action *a, int cd, int h) {
        location = c;
        dad = d;
        act = a;
        cost = cd;
        heuristic = h;
    }
};

struct Compare_Cost {
    bool operator () (const Node *a, const Node *b) {
        return a->cost >= b->cost;
    }
};

struct Compare_Heuristic {
    bool operator () (const Node *a, const Node *b) {
        return a->heuristic >= b->heuristic;
    }
};

class Problem {
public:
    State *current_location;
    State *destination;
    vector<pair<Action*, int>> actions;

    Problem(State *cl, State *d) {
        current_location = cl;
        destination = d;
    }

    void setSolution(Node *final) {
        while(final->dad != NULL) {
            actions.push_back(make_pair(final->act, final->cost));
            final = final->dad;
        }

        reverse(actions.begin(), actions.end());
    }

    void showSolution() {
        if(actions.empty()) {
            cout << endl;
            cout << "Nothing to do, you are at your destination!" << endl;
        } else {
            cout << endl << "Actions:" << endl;
            for(int i = 0; i < (int)actions.size(); i++) {
                cout << actions[i].first->act << ", ";
                cout << "Cost of: " << actions[i].second << endl;
            }
        }
    }
};

class Model {
public:
    map<string, vector<pair<string, int>>> cities;
    map<string, int> heuristic_table;

    Model() {
        cities["Arad"] = {make_pair("Zerind", 75), make_pair("Timisoara", 118), make_pair("Sibiu", 140)};
        cities["Zerind"] = {make_pair("Arad", 75), make_pair("Oradea", 71)};
        cities["Oradea"] = {make_pair("Zerind", 71), make_pair("Sibiu", 151)};
        cities["Sibiu"] = {make_pair("Arad", 140), make_pair("Fagaras", 99), make_pair("Rimnicu Vilcea", 80), make_pair("Oradea", 151)};
        cities["Fagaras"] = {make_pair("Sibiu", 99), make_pair("Bucharest", 211)};
        cities["Rimnicu Vilcea"] = {make_pair("Sibiu", 80), make_pair("Pitesti", 97), make_pair("Craiova", 146)};
        cities["Pitesti"] = {make_pair("Rimnicu Vilcea", 97), make_pair("Craiova", 138), make_pair("Bucharest", 101)};
        cities["Craiova"] = {make_pair("Drobeta", 120), make_pair("Pitesti", 138), make_pair("Rimnicu Vilcea", 146)};
        cities["Drobeta"] = {make_pair("Mehadia", 75), make_pair("Craiova", 120)};
        cities["Mehadia"] = {make_pair("Drobeta", 75), make_pair("Lugoj", 70)};
        cities["Lugoj"] = {make_pair("Mehadia", 70), make_pair("Timisoara", 111)};
        cities["Timisoara"] = {make_pair("Lugoj", 111), make_pair("Arad", 118)};
        cities["Bucharest"] = {make_pair("Giurgiu", 90), make_pair("Pitesti", 101), make_pair("Urziceni", 85), make_pair("Fagaras", 211)};
        cities["Giurgiu"] = {make_pair("Bucharest", 90)};
        cities["Urziceni"] = {make_pair("Bucharest", 85), make_pair("Hirsova", 98), make_pair("Vaslui", 142)};
        cities["Hirsova"] = {make_pair("Urziceni", 98), make_pair("Eforie", 86)};
        cities["Eforie"] = {make_pair("Hirsova", 86)};
        cities["Vaslui"] = {make_pair("Urziceni", 142), make_pair("Iasi", 92)};
        cities["Iasi"] = {make_pair("Vaslui", 92), make_pair("Neamt", 87)};
        cities["Neamt"] = {make_pair("Iasi", 87)};

        heuristic_table["Arad"] = 366;
        heuristic_table["Bucharest"] = 0;
        heuristic_table["Craiova"] = 160;
        heuristic_table["Drobeta"] = 242;
        heuristic_table["Eforie"] = 161;
        heuristic_table["Fagaras"] = 176;
        heuristic_table["Giurgiu"] = 77;
        heuristic_table["Iasi"] = 226;
        heuristic_table["Hirsova"] = 151;
        heuristic_table["Lugoj"] = 244;
        heuristic_table["Mehadia"] = 241;
        heuristic_table["Neamt"] = 234;
        heuristic_table["Oradea"] = 380;
        heuristic_table["Pitesti"] = 100;
        heuristic_table["Rimnicu Vilcea"] = 193;
        heuristic_table["Sibiu"] = 253;
        heuristic_table["Timisoara"] = 329;
        heuristic_table["Urziceni"] = 80;
        heuristic_table["Vaslui"] = 199;
        heuristic_table["Zerind"] = 374;
    }

    // THIS METHOD SHOWS ALL SUCCESSORS OF A STATE
    void transitions(State *current_location) {
        int check = 0;
        cout << endl << "Successors of " << current_location->city << ":" << endl;

        for(pair<string, int> aux : cities[current_location->city]) {
            cout << "City: " << aux.first << ", " << aux.second << " km from here." << endl;
            check++;
        }

        if(check == 0) {
            cout << "City not found!" << endl;
        }
    }

    // THIS METHOD FINDS IF A STATE IS PUSHED IN A VECTOR
    bool searchInVisited(vector<State*> v, State *s) {
        for(int i = 0; i < (int)v.size(); i++) {
            if(s->city == v[i]->city) {
                return true;
            }
        }
        return false;
    }

    // BFS SEARCH IMPLEMENTATION
    bool BFS(Problem *P) {
        queue<Node*> edges;
        vector<State*> visited;
        Node *control = new Node(P->current_location, NULL, NULL, 0, 0);

        if(P->current_location->city == P->destination->city) {
            return true;
        }

        edges.push(control);

        while(true) {
            if(edges.empty()) {
                return false;
            }

            control = edges.front();
            edges.pop();
            visited.push_back(control->location);

            for(pair<string, int> each : cities[control->location->city]) {
                State *newState = new State(each.first);
                Action *newAction = new Action(control->location, newState);

                int gn = each.second + control->cost;

                Node *son = new Node(newState, control, newAction, gn, 0);

                if(!(this->searchInVisited(visited, son->location))) {
                    if(son->location->city == P->destination->city) {
                        P->setSolution(son);
                        return true;
                    }
                    edges.push(son);
                }
            }
        }
        return false;
    }

    // DFS SEARCH IMPLEMENTATION
    bool DFS(Problem *P) {
        stack<Node*> edges;
        vector<State*> visited;
        Node *control = new Node(P->current_location, NULL, NULL, 0, 0);

        if(P->current_location->city == P->destination->city) {
            return true;
        }

        edges.push(control);

        while(true) {
            if(edges.empty()) {
                return false;
            }

            control = edges.top();
            edges.pop();
            visited.push_back(control->location);

            for(pair<string, int> each : cities[control->location->city]) {
                State *newState = new State(each.first);
                Action *newAction = new Action(control->location, newState);

                int gn = each.second + control->cost;

                Node *son = new Node(newState, control, newAction, gn, 0);

                if(!(this->searchInVisited(visited, son->location))) {
                    if(son->location->city == P->destination->city) {
                        P->setSolution(son);
                        return true;
                    }
                    edges.push(son);
                }
            }
        }
        return false;
    }

    // MINIMAL COST SEARCH IMPLEMENTATION
    bool minimalCostSearch(Problem *P) {
        priority_queue<Node*, deque<Node*>, Compare_Cost> edges;
        vector<State*> visited;
        Node *control = new Node(P->current_location, NULL, NULL, 0, 0);

        if(P->current_location->city == P->destination->city) {
            return true;
        }

        edges.push(control);

        while(true) {
            if(edges.empty()) {
                return false;
            }

            control = edges.top();
            edges.pop();
            visited.push_back(control->location);

            if(control->location->city == P->destination->city) {
                P->setSolution(control);
                return true;
            }

            for(pair<string, int> each : cities[control->location->city]) {
                State *newState = new State(each.first);
                Action *newAction = new Action(control->location, newState);

                int gn = each.second + control->cost;

                Node *son = new Node(newState, control, newAction, gn, 0);

                if(!(this->searchInVisited(visited, son->location))) {
                    edges.push(son);
                }
            }
        }
        return false;
    }

    // GREEDY HEURISTIC SEARCH IMPLEMENTATION
    bool heuristicSearch(Problem *P) {
        priority_queue<Node*, deque<Node*>, Compare_Heuristic> edges;
        vector<State*> visited;
        Node *control = new Node(P->current_location, NULL, NULL, 0, 0);

        if(P->current_location->city == P->destination->city) {
            return true;
        }

        edges.push(control);

        while(true) {
            if(edges.empty()) {
                return false;
            }

            control = edges.top();
            edges.pop();
            visited.push_back(control->location);

            if(control->location->city == P->destination->city) {
                P->setSolution(control);
                return true;
            }

            for(pair<string, int> each : cities[control->location->city]) {
                State *newState = new State(each.first);
                Action *newAction = new Action(control->location, newState);

                int gn = each.second + control->cost;
                int fn = heuristic_table[newState->city];

                Node *son = new Node(newState, control, newAction, gn, fn);

                if(!(this->searchInVisited(visited, son->location))) {
                    edges.push(son);
                }
            }
        }
        return false;
    }

    // A* HEURISTIC SEARCH IMPLEMENTATION
    bool A_search(Problem *P) {
        priority_queue<Node*, deque<Node*>, Compare_Heuristic> edges;
        vector<State*> visited;
        Node *control = new Node(P->current_location, NULL, NULL, 0, 0);

        if(P->current_location->city == P->destination->city) {
            return true;
        }

        edges.push(control);

        while(true) {
            if(edges.empty()) {
                return false;
            }

            control = edges.top();
            edges.pop();
            visited.push_back(control->location);

            if(control->location->city == P->destination->city) {
                P->setSolution(control);
                return true;
            }

            for(pair<string, int> each : cities[control->location->city]) {
                State *newState = new State(each.first);
                Action *newAction = new Action(control->location, newState);

                int gn = (each.second + control->cost);
                int fn = heuristic_table[newState->city];

                Node *son = new Node(newState, control, newAction, gn, (gn + fn));

                if(!(this->searchInVisited(visited, son->location))) {
                    edges.push(son);
                }
            }
        }
        return false;
    }
};

void ERROR() {
    cout << endl;
    cout << "An unexpected error has occurred..." << endl;
    cout << "Check the cities or contact support." << endl;
}

int main(int argc,  char** argv) {
    int opt_choice;
    State *current_state;
    State *destination_state;
    Model *model = new Model();
    Problem *problem;

    string current_city = argv[1];
    string destination_city = argv[2];

    current_state = new State(current_city);
    destination_state = new State(destination_city);
    problem = new Problem(current_state, destination_state);

//    cout << "Tell us the city you are at: ";
//    cin >> current_city;

//    cout << "Tell us the city you want to go: ";
//    cin >> destination_city;

    cout << "1 - BFS" << endl;
    cout << "2 - DFS" << endl;
    cout << "3 - Minimal cost search" << endl;
    cout << "4 - Heuristc search" << endl;
    cout << "5 - A* search" << endl;
    cout << "Tell us which search algorithm you want to apply: ";
    cin >> opt_choice;

    switch(opt_choice) {
    case 1:
    {
        bool check_bfs = model->BFS(problem);

        if(check_bfs) {
            problem->showSolution();
        } else {
            ERROR();
        }

        break;
    }
    case 2:
    {
        bool check_dfs = model->DFS(problem);

        if(check_dfs) {
            problem->showSolution();
        } else {
            ERROR();
        }

        break;
    }
    case 3:
    {
        bool check_MinCost = model->minimalCostSearch(problem);

        if(check_MinCost) {
            problem->showSolution();
        } else {
            ERROR();
        }

        break;
    }
    case 4:
    {
        bool check_HeuristicSearch = model->heuristicSearch(problem);

        if(check_HeuristicSearch) {
            problem->showSolution();
        } else {
            ERROR();
        }

        break;
    }
    case 5:
    {
        bool check_ASearch = model->A_search(problem);

        if(check_ASearch) {
            problem->showSolution();
        } else {
            ERROR();
        }

        break;
    }
    default:
    {
        cout << endl;
        cout << "Invalid Option!" << endl;
        break;
    }
    }

    return 0;
}
