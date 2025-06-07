#include <vector>
#include <string>
#include "Sector.h"
#include "../modelo/DireccionDisco.h"
#include "../modelo/Registro.h"
using namespace std;

class Disco {
private:
    int platos;
    int superficiesPorPlato;
    int pistasPorSuperficie;
    int sectoresPorPista;
    int tamanioSector;

    //disco[plato][superficie][pista][sector]
    vector<vector<vector<vector<Sector>>>> disco;

    DireccionDisco ultimaPosicion; // Donde termina el último atributo insertado

public:
    Disco(int platos, int superficies, int pistas, int sectores, int tamanioSector);

    DireccionDisco insertarAtributo(const string& valor, int tamanio);
    void avanzarSector();
    
    DireccionDisco storeRecord(const Registro& registro) {
        DireccionDisco inicio;
        bool first = true;
        for (const auto& attr : registro.atributos) {
            DireccionDisco dir = insertarAtributo(attr.valor, attr.obtenerTamanio());
            if (first) {
                inicio = dir;
                first = false;
            }
        }
        return inicio;
    }



    // Reinicia la posición de escritura
    void reset();
};

