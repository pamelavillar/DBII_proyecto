#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "disco/Disco.h"
#include "modelo/Registro.h"
#include <vector>
#include <string>
#include <map>

class DatabaseManager {
public:
    DatabaseManager(int platos = 2, int superficies = 2, int pistas = 100, 
                   int sectores = 8, int tamanioSector = 100);
    
    void insertRegistro(const Registro& registro);
    void displayAllRecords() const;
    
    const Disco& getDisco() const { return disco; }
    
private:
    Disco disco;
    std::vector<std::pair<DireccionDisco, Registro>> recordLocations;// estructura temporal
    std::vector<string> nombres_atributos;
    void printRecordLocation(const Registro& registro, const DireccionDisco& address) const;
};

#endif