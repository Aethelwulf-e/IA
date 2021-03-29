/* Dupla: Carlos Eduardo da Silva Ferreira
 *        Alcides Ribeiro Sampaio Neto
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Perception {
public:
    bool location;
    bool isDirty;

    Perception(bool _location, bool _isDirty) {
        location = _location;
        isDirty = _isDirty;
    }
};

class Action {
public:
    string name;

    Action(string _name) {
        name = _name;
    }
};

class Agent {
public:
    Perception *perception;
    vector<Action*> actions;
};

class Environment {
public:
    bool isDirtyA, isDirtyB, agentLocation;

    Environment(bool _agentLocation, bool _isDirtyA, bool _isDirtyB) {
        this->isDirtyA = _isDirtyA;
        this->isDirtyB = _isDirtyB;
        this->agentLocation = _agentLocation;
    }

    void showEnvironmentState() {
        cout << "Final state of the environment:" << endl;
        cout << "State of the room 'A': ";

        if(this->isDirtyA) {
            cout << "Dirty!" << endl;
        } else {
            cout << "Clean." << endl;
        }

        cout << "State of the room 'B': ";
        if(this->isDirtyB) {
            cout << "Dirty!" << endl;
        } else {
            cout << "Clean." << endl;
        }

        cout << "Current location of the agent: ";
        if(this->agentLocation) {
            cout << "Room 'A'." << endl << endl;
        } else {
            cout << "Room 'B'." << endl << endl;
        }
    }
};

string chave(vector<Perception*> vetor){
    string s;
    s = "";
    int i;

    for(i = 0; i < (int)vetor.size(); i++){
        s += vetor[i]->location*2 + vetor[i]->isDirty + 1 + 'a';
    }

    return s;
}

map<string, Action*> loadTable() {
    map<string, Action*> table;

    Perception *pp1 = new Perception(true, true);
    Perception *pp2 = new Perception(true, false);
    Perception *pp3 = new Perception(false, true);
    Perception *pp4 = new Perception(false, false);

    Action *a1 = new Action("Aspirate.\n");
    Action *a2 = new Action("Go left.\n");
    Action *a3 = new Action("Go right.\n");

    vector<Perception*> p1;
    vector<Perception*> p2;
    vector<Perception*> p3;
    vector<Perception*> p4;
    vector<Perception*> p5;
    vector<Perception*> p6;
    vector<Perception*> p7;
    vector<Perception*> p8;
    vector<Perception*> p9;
    vector<Perception*> p10;
    vector<Perception*> p11;
    vector<Perception*> p12;
    vector<Perception*> p13;
    vector<Perception*> p14;
    vector<Perception*> p15;
    vector<Perception*> p16;
    vector<Perception*> p17;
    vector<Perception*> p18;
    vector<Perception*> p19;
    vector<Perception*> p20;

    p1.push_back(pp1);
    p2.push_back(pp2);
    p3.push_back(pp3);
    p4.push_back(pp4);

    p5.push_back(pp1);
    p5.push_back(pp1);
    p6.push_back(pp2);
    p6.push_back(pp2);
    p7.push_back(pp3);
    p7.push_back(pp3);
    p8.push_back(pp4);
    p8.push_back(pp4);

    p9.push_back(pp1);
    p9.push_back(pp2);
    p10.push_back(pp1);
    p10.push_back(pp3);
    p11.push_back(pp1);
    p11.push_back(pp4);

    p12.push_back(pp2);
    p12.push_back(pp1);
    p13.push_back(pp2);
    p13.push_back(pp3);
    p14.push_back(pp2);
    p14.push_back(pp4);

    p15.push_back(pp3);
    p15.push_back(pp1);
    p16.push_back(pp3);
    p16.push_back(pp2);
    p17.push_back(pp3);
    p17.push_back(pp4);

    p18.push_back(pp4);
    p18.push_back(pp1);
    p19.push_back(pp4);
    p19.push_back(pp2);
    p20.push_back(pp4);
    p20.push_back(pp3);

    table = {
        {chave(p1), a1},
        {chave(p2), a3},
        {chave(p3), a1},
        {chave(p4), a2},
        {chave(p5), a1},
        {chave(p6), a3},
        {chave(p7), a1},
        {chave(p8), a2},
        {chave(p9), a3},
        {chave(p10), a1},
        {chave(p11), a2},
        {chave(p12), a1},
        {chave(p13), a1},
        {chave(p14), a2},
        {chave(p15), a1},
        {chave(p16), a3},
        {chave(p17), a2},
        {chave(p18), a1},
        {chave(p19), a3},
        {chave(p20), a1}
    };

    return table;
}

class TableDrivenAgent : public Agent {
public:
    vector<Perception*> perceptions;
    map<string, Action*> table;

    TableDrivenAgent() {
        table = loadTable();
    }

    void setNewPerception(Environment *e) {
        bool local = e->agentLocation;

        if(local && e->isDirtyA) {
            perception = new Perception(local, true);
        } else if(!local && e->isDirtyB) {
            perception = new Perception(local, true);
        } else {
            perception = new Perception(local, false);
        }
    }

    Environment* updateEnvironment(Environment *e, Action *a) {
        bool AgentLocal = e->agentLocation;
        bool DirtyA = e->isDirtyA;
        bool DirtyB = e->isDirtyB;

        if((a->name.compare("Aspirate.\n") == 0) && AgentLocal) {
            e = new Environment(true, false, DirtyB);
        } else if((a->name.compare("Aspirate.\n") == 0) && !AgentLocal) {
            e = new Environment(false, DirtyA, false);
        } else if(a->name.compare("Go right.\n") == 0) {
            e = new Environment(false, DirtyA, DirtyB);
        } else {
            e = new Environment(true, DirtyA, DirtyB);
        }

        return e;
    }

    Environment* act(Environment *e, int n) {
        for(int i = 0; i < n; i++) {
            setNewPerception(e);

            if(perceptions.size() == 2) {
                perceptions.erase(perceptions.begin());
                perceptions.push_back(perception);
            } else {
                perceptions.push_back(perception);
            }

            Action *a = table.at(chave(perceptions));
            actions.push_back(a);

            e = updateEnvironment(e, a);
        }

        return e;
    }

    void showActions() {
        cout << "Table driven agent actions:" << endl;
        for(int i = 0; i < (int)actions.size(); i++) {
            cout << actions[i]->name;
        }
        cout << endl;
    }
};

class SimpleReflexAgent : public Agent{
public:
    SimpleReflexAgent(Environment *e) {
        bool local = e->agentLocation;
        if(local && e->isDirtyA) {
            perception = new Perception(local, true);
        }
        else if(!local && e->isDirtyB) {
            perception = new Perception(local, true);
        }
        else {
            perception = new Perception(local, false);
        }
    }

    Environment* act(Environment *e, int n) {
        Action *a1 = new Action("Aspirate.\n");
        Action *a2 = new Action("Go left.\n");
        Action *a3 = new Action("Go right.\n");

        bool DirtyA = e->isDirtyA;
        bool DirtyB = e->isDirtyB;
        bool AgentLocation = e->agentLocation;

        for(int i = 0; i < n; i++) {
            if(perception->isDirty && perception->location) {
                actions.push_back(a1);
                perception = new Perception(AgentLocation, false);
                DirtyA = false;
            } else if(!perception->isDirty && perception->location) {
                actions.push_back(a3);
                perception = new Perception(false, DirtyB);
                AgentLocation = false;
            } else if(perception->isDirty && !perception->location) {
                actions.push_back(a1);
                perception = new Perception(AgentLocation, false);
                DirtyB = false;
            } else {
                actions.push_back(a2);
                perception = new Perception(true, DirtyA);
                AgentLocation = true;
            }
        }

        e = new Environment(AgentLocation, DirtyA, DirtyB);

        return e;
    }

    void showActions() {
        cout << "Simple reflex agent actions:" << endl;
        for(int i = 0; i < (int)actions.size(); i++) {
            cout << actions[i]->name;
        }
        cout << endl;
    }
};

void clearScreen() {
    cout << "Enter to continue..." << endl;
    getchar();
    getchar();
    system("clear");
}

int main()
{
    int n;
    bool roomA, roomB, start;
    int option;
    char room;

    cout << "Enter the amount of actions: ";
    cin >> n;

    cout << "Enter with the state of the first room (1 - Dirty / 0 - Clean): ";
    cin >> roomA;

    cout << "Enter with the state of the second room (1 - Dirty / 0 - Clean): ";
    cin >> roomB;

    cout << "Tell which room you want to start with (A/B): ";
    cin >> room;

    if((room == 'a') || (room == 'A')) {
        start = true;
    } else if((room == 'b') || (room == 'B')) {
        start = false;
    } else {
        cout << "Invalid room." << endl;
        return 0;
    }

    cout << endl << "Tell us which agent you want to start with:" << endl;
    cout << "(1 - Table driven / 2 - Simple reflex)" << endl;
    cout << "Option: ";
    cin >> option;

    system("clear");

    if(option == 1) {
        Environment *amb = new Environment(start, roomA, roomB);
        TableDrivenAgent *agt = new TableDrivenAgent();

        amb = agt->act(amb, n);
        agt->showActions();

        amb->showEnvironmentState();

        clearScreen();

        SimpleReflexAgent *agt2 = new SimpleReflexAgent(amb);

        amb = agt2->act(amb, n);
        agt2->showActions();

        amb->showEnvironmentState();

    } else if(option == 2) {
        Environment *amb = new Environment(start, roomA, roomB);
        SimpleReflexAgent *agt = new SimpleReflexAgent(amb);

        amb = agt->act(amb, n);
        agt->showActions();

        amb->showEnvironmentState();

        clearScreen();

        TableDrivenAgent *agt2 = new TableDrivenAgent();

        amb = agt2->act(amb, n);
        agt2->showActions();

        amb->showEnvironmentState();

    } else {
        cout << "Invalid option!" << endl;
    }

    return 0;
}
