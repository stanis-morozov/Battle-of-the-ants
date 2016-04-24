#include <iostream>
#include "IAntLogic.hpp"
#include "RealAnt.hpp"
#include "Field.hpp"
#include <dlfcn.h>
#include <vector>
#include <functional>
#include <set>
#include <sstream>
#include <fstream>

enum {NTEAMS = 4, NANTS = 10, NSTEPS = 1001};
const int N = 31, M = 31;
antlogic::AntAction (*GetAction[NTEAMS])(const antlogic::Ant&, antlogic::AntSensor[3][3]);

int prepare()
{
    void *handle1 = dlopen("lib1.so", RTLD_LAZY);
    if (handle1 == nullptr) {
        std::cout << "no lib1" << std::endl;
        return 0;
    }
    GetAction[0] = (antlogic::AntAction (*)(const antlogic::Ant&, antlogic::AntSensor[3][3]))dlsym(handle1, "GetAction");
    if (GetAction[0] == nullptr) {
        std::cout << "no GetAction1" << std::endl;
        return 0;
    }
    
    void *handle2 = dlopen("lib2.so", RTLD_LAZY);
    if (handle2 == nullptr) {
        std::cout << "no lib2" << std::endl;
        return 0;
    }
    GetAction[1] = (antlogic::AntAction (*)(const antlogic::Ant&, antlogic::AntSensor[3][3]))dlsym(handle2, "GetAction");
    if (GetAction[1] == nullptr) {
        std::cout << "no GetAction2" << std::endl;
        return 0;
    }

    void *handle3 = dlopen("lib3.so", RTLD_LAZY);
    if (handle3 == nullptr) {
        std::cout << "no lib3" << std::endl;
        return 0;
    }
    GetAction[2] = (antlogic::AntAction (*)(const antlogic::Ant&, antlogic::AntSensor[3][3]))dlsym(handle3, "GetAction");
    if (GetAction[2] == nullptr) {
        std::cout << "no GetAction3" << std::endl;
        return 0;
    }

    void *handle4 = dlopen("lib4.so", RTLD_LAZY);
    if (handle4 == nullptr) {
        std::cout << "no lib4" << std::endl;
        return 0;
    }
    GetAction[3] = (antlogic::AntAction (*)(const antlogic::Ant&, antlogic::AntSensor[3][3]))dlsym(handle4, "GetAction");
    if (GetAction[3] == nullptr) {
        std::cout << "no GetAction4" << std::endl;
        return 0;
    }
    return 1;
}

void getsensors(antlogic::AntSensor s[3][3], const antlogic::Field &f, antlogic::RealAnt ant)
{
    int x = ant.getX();
    int y = ant.getY();
    s[1][1] = f.at(x, y).getSensor(ant.getTeamId());
    s[0][0] = f.at(x - 1, y - 1).getSensor(ant.getTeamId());
    s[1][0] = f.at(x - 1, y).getSensor(ant.getTeamId());
    s[2][0] = f.at(x - 1, y + 1).getSensor(ant.getTeamId());
    s[0][1] = f.at(x, y - 1).getSensor(ant.getTeamId());
    s[2][1] = f.at(x, y + 1).getSensor(ant.getTeamId());
    s[0][2] = f.at(x + 1, y - 1).getSensor(ant.getTeamId());
    s[1][2] = f.at(x + 1, y).getSensor(ant.getTeamId());
    s[2][2] = f.at(x + 1, y + 1).getSensor(ant.getTeamId());
}

void get(antlogic::RealAnt &ant, antlogic::Field &f)
{
    if (!ant.hasFood() && f.at(ant.getX(), ant.getY()).foodAmount()) {
        ant.setFood();
        f.foodTaken(ant.getX(), ant.getY());
    }
}

void put(antlogic::RealAnt &ant, antlogic::Field &f)
{
    if (ant.hasFood()) {
        ant.putFood();
        f.foodDropped(ant.getX(), ant.getY());
    }
}

void move_up(antlogic::RealAnt &ant, antlogic::Field &f)
{
    int x = ant.getX();
    int y = ant.getY();
    x--;
    if (f.isCorrectPos(x, y)) {
        ant.goUp();
    }
}

void move_left(antlogic::RealAnt &ant, antlogic::Field &f)
{
    int x = ant.getX();
    int y = ant.getY();
    y--;
    if (f.isCorrectPos(x, y)) {
        ant.goLeft();
    }
}

void move_down(antlogic::RealAnt &ant, antlogic::Field &f)
{
    int x = ant.getX();
    int y = ant.getY();
    x++;
    if (f.isCorrectPos(x, y)) {
        ant.goDown();
    }
}

void move_right(antlogic::RealAnt &ant, antlogic::Field &f)
{
    int x = ant.getX();
    int y = ant.getY();
    y++;
    if (f.isCorrectPos(x, y)) {
        ant.goRight();
    }
}

void bite(int t, int x, int y, antlogic::Field &f, std::vector <antlogic::RealAnt> ants[NTEAMS])
{
    std::vector <std::pair <int, int> > lst;
    for (int i = 0; i < NTEAMS; i++) {
        if (i == t) {
            continue;
        }
        for (int j = 0; j < (int)ants[i].size(); j++) {
            if (ants[i][j].getX() == x && ants[i][j].getY() == y) {
                lst.push_back(std::make_pair(i, j));
            }
        }
    }
    int mn = NSTEPS;
    for (auto it: lst) {
        mn = std::min(mn, ants[it.first][it.second].getWait()); 
    }
    std::vector <std::pair <int, int> > lst2;
    for (auto it: lst) {
        if (ants[it.first][it.second].getWait() == mn) {
            lst2.push_back(it);
        }
    }
    if (lst2.empty()) {
        return;
    }
    int num = rand() % (int)lst2.size();
    put(ants[lst2[num].first][lst2[num].second], f);
    ants[lst2[num].first][lst2[num].second].bitten();
}

void bite_up(antlogic::RealAnt &ant, antlogic::Field &f, std::vector <antlogic::RealAnt> ants[NTEAMS])
{
    int t = ant.getTeamId();
    int x = ant.getX();
    int y = ant.getY();
    x--;
    bite(t, x, y, f, ants);
}

void bite_left(antlogic::RealAnt &ant, antlogic::Field &f, std::vector <antlogic::RealAnt> ants[NTEAMS])
{
    int t = ant.getTeamId();
    int x = ant.getX();
    int y = ant.getY();
    y--;
    bite(t, x, y, f, ants);
}

void bite_down(antlogic::RealAnt &ant, antlogic::Field &f, std::vector <antlogic::RealAnt> ants[NTEAMS])
{
    int t = ant.getTeamId();
    int x = ant.getX();
    int y = ant.getY();
    x++;
    bite(t, x, y, f, ants);
}

void bite_right(antlogic::RealAnt &ant, antlogic::Field &f, std::vector <antlogic::RealAnt> ants[NTEAMS])
{
    int t = ant.getTeamId();
    int x = ant.getX();
    int y = ant.getY();
    y++;
    bite(t, x, y, f, ants);
}

void updateantspositions(const std::vector <antlogic::RealAnt> ant[NTEAMS], antlogic::Field &f)
{
    f.cleanAnts();
    for (int i = 0; i < NTEAMS; i++) {
        for (auto it: ant[i]) {
            f.addAnt(it);
        }
    }
}

std::string antspositions(const std::vector <antlogic::RealAnt> ant[NTEAMS])
{
    std::string ans;
    ans = "[";
    std::set <std::string> st;
    for (int i = 0; i < NTEAMS; i++) {
        for (auto it: ant[i]) {
            std::string tmp;
            std::ostringstream oss;
            oss << "[" << it.getX() + 1 << "," << it.getY() + 1 << "," << it.getTeamId() << "," << it.hasFood() << "," << it.isBitten() << "]";
            tmp = oss.str();
            st.insert(tmp);
        }
    }
    for (auto it: st) {
        if (ans != "[") {
            ans += ",";
        }
        ans += it;
    }
    ans += "]";
    return ans;
}

std::string foodchanges(const antlogic::Field &old, const antlogic::Field &nw)
{
    std::string ans;
    ans = "[";
    int n = old.getN();
    int m = old.getM();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (old.at(i, j).foodAmount() != nw.at(i, j).foodAmount()) {
                if (ans != "[") {
                    ans += ",";
                }
                std::ostringstream oss;
                oss << "[" << i + 1 << "," << j + 1 << "," << nw.at(i, j).foodAmount() << "]";
                ans += oss.str();
            }
        }
    }
    ans += "]";
    return ans;
}

std::string getScore(const antlogic::Field &f)
{
    int n = f.getN();
    int m = f.getM();
    std::ostringstream oss;
    oss << "[" << f.at(0, 0).foodAmount() << "," << f.at(n - 1, 0).foodAmount() << "," << f.at(0, m - 1).foodAmount() << "," << f.at(n - 1, m - 1).foodAmount() << "]";
    return oss.str();
}

void fill(std::string s, antlogic::Field &f)
{
    std::ifstream fin(s);
    int a, b, c;
    std::vector <std::tuple <int, int, int> > lst;
    while (fin >> a >> b >> c) {
        lst.push_back(std::make_tuple(a, b, c));
    }
    f.fill(lst);
    fin.close();
}

int main()
{
    if (!prepare()) {
        return 0;
    }
    std::string result = "[";
    std::vector <antlogic::RealAnt> ant[NTEAMS];
    antlogic::Field f(N, M);
    for (int s = 0; s < NSTEPS; s++) {
        for (int i = 0; i < NTEAMS; i++) {
            for (int j = 0; j < (int)ant[i].size(); j++) {
                ant[i][j].newStep();
            }
        }
        antlogic::Field prev(f);
        if (s == 0) {
            fill("foodmap.txt", f);
        }
        for (int i = 0; i < NTEAMS; i++) {
            if (s < NANTS) {
                ant[i].push_back(antlogic::RealAnt(i, N, M));
            }
            for (int j = 0; j < (int)ant[i].size(); j++) {
                if (ant[i][j].isBitten()) {
                    continue;
                }
                int x = ant[i][j].getX();
                int y = ant[i][j].getY();
                antlogic::AntSensor s[3][3];
                getsensors(s, f, ant[i][j]);
                antlogic::AntAction act = GetAction[i](ant[i][j], s);
                
                if (act.putSmell) {
                    f.at(x, y).setSmell(act.smell);
                }
                switch (act.actionType) {
                    case antlogic::MOVE_UP:
                        move_up(ant[i][j], f);
                        break;
                    case antlogic::MOVE_LEFT:
                        move_left(ant[i][j], f);
                        break;
                    case antlogic::MOVE_DOWN:
                        move_down(ant[i][j], f);
                        break;
                    case antlogic::MOVE_RIGHT:
                        move_right(ant[i][j], f);
                        break;
                    case antlogic::BITE_UP:
                        bite_up(ant[i][j], f, ant);
                        break;
                    case antlogic::BITE_LEFT:
                        bite_left(ant[i][j], f, ant);
                        break;
                    case antlogic::BITE_DOWN:
                        bite_down(ant[i][j], f, ant);
                        break;
                    case antlogic::BITE_RIGHT:
                        bite_right(ant[i][j], f, ant);
                        break;
                    case antlogic::GET:
                        get(ant[i][j], f);
                        break;
                    case antlogic::PUT:
                        put(ant[i][j], f);
                        break;
                }
                updateantspositions(ant, f);
            }
        }

        std::string log;
        log = "[";
        log += getScore(f);
        log += ",";
        log += antspositions(ant);
        log += ",";
        log += foodchanges(prev, f);
        log += "]";
        if (result != "[") {
            result += ",";
        }
        result += log;
    }
    result += "]";
    std::ofstream fout("res.json");
    fout << result << std::endl;
    fout.close();
}
