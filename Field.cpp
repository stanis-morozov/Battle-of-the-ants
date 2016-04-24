#include "Field.hpp"

antlogic::Cell::Cell(int h = -1, int fa = 0, int iw = 0): smell(0), smellintensity(0), antmask(0), hill(h), foodamount(fa), iswall(iw) {}

antlogic::AntSensor antlogic::Cell::getSensor(int teamid) const
{
    antlogic::AntSensor s;
    s.smell = smell;
    s.smellIntensity = smellintensity;
    s.isFriend = (antmask & (1 << teamid)) != 0;
    s.isEnemy = (antmask & (~(1 << teamid))) != 0;
    s.isMyHill = teamid == hill;
    s.isEnemyHill = teamid != hill;
    s.isFood = foodamount > 0;
    s.isWall = iswall;
    return s;
}

int antlogic::Cell::foodAmount() const
{
    return foodamount;
}

void antlogic::Cell::foodTaken()
{
    if (foodamount > 0) {
        foodamount--;
    } else {
        throw 1;
    }
}

int antlogic::Cell::getHill() const
{
    return hill;
}

void antlogic::Cell::foodDropped()
{
    foodamount++;
}

void antlogic::Cell::setSmell(int s)
{
    smell = s;
    smellintensity = 100;
}

void antlogic::Cell::cleanAnts()
{
    antmask = 0;
}

void antlogic::Cell::addAnt(int teamid)
{
    antmask |= 1 << teamid;
}

antlogic::Field::Field(int n_, int m_): n(n_), m(m_)
{
    t = new Cell*[n];
    for (int i = 0; i < n; i++) {
        t[i] = new Cell[m];
    }
    t[0][0] = Cell(0);
    t[0][m - 1] = Cell(1);
    t[n - 1][0] = Cell(2);
    t[n - 1][m - 1] = Cell(3);
}

antlogic::Field::Field(const antlogic::Field &f): n(f.n), m(f.m)
{
    t = new Cell*[n];
    for (int i = 0; i < n; i++) {
        t[i] = new Cell[m];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            t[i][j] = f.t[i][j];
        }
    }
}

antlogic::Field& antlogic::Field::operator=(const antlogic::Field &f)
{
    if (this != &f) {
        antlogic::Field tmp(f);
        std::swap(n, tmp.n);
        std::swap(m, tmp.m);
        std::swap(t, tmp.t);
    }
    return *this;
}

antlogic::Field::~Field()
{
    for (int i = 0; i < n; i++) {
        delete []t[i];
    }
    delete []t;
}

void antlogic::Field::fill(const std::vector <std::tuple <int, int, int> > &lst)
{
    for (auto it: lst) {
        int i = std::get<0>(it);
        int j = std::get<1>(it);
        t[i][j] = Cell(t[i][j].getHill(), std::get<2>(it), 0);
    }
}

antlogic::Cell antlogic::Field::at(int i, int j) const
{
    if (i < -1 || j < -1 || i > n || j > m) {
        throw;
    }
    if (i == -1 || j == -1 || i == n || j == m) {
        return Cell(-1, 0, 1);
    }
    return t[i][j];
}

bool antlogic::Field::isCorrectPos(int i, int j) const
{
    return i >= 0 && j >= 0 && i < n && j < m;
}

int antlogic::Field::getN() const
{
    return n;
}

int antlogic::Field::getM() const
{
    return m;
}

void antlogic::Field::cleanAnts()
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            t[i][j].cleanAnts();
        }
    }
}

void antlogic::Field::addAnt(const antlogic::RealAnt &a)
{
    int x = a.getX();
    int y = a.getY();
    if (!isCorrectPos(x, y)) {
        throw "ab";
    }
    t[x][y].addAnt(a.getTeamId());
}

void antlogic::Field::foodTaken(int i, int j)
{
    if (!isCorrectPos(i, j)) {
        throw 'a';
    }
    t[i][j].foodTaken();
}

void antlogic::Field::foodDropped(int i, int j)
{
    if (!isCorrectPos(i, j)) {
        throw this;
    }
    t[i][j].foodDropped();
}
