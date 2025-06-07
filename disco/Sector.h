// disco/Sector.h
#ifndef SECTOR_H
#define SECTOR_H

#include <string>
#include <vector>

using namespace std;

class Sector {
private:
    int tamanio;
    vector<char> datos;

public:
    int usado;
    Sector(int tamanio);
    bool escribir(const string& valor, int tamanio);
    string leer() const;
    int espacioDisponible() const;
};

#endif 
