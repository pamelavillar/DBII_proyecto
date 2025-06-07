#ifndef DIRECCION_DISCO_H
#define DIRECCION_DISCO_H

struct DireccionDisco {
    int plato;
    int superficie;
    int pista;
    int sector;
    int byte;

    bool operator==(const DireccionDisco& otra) const {
        return plato == otra.plato &&
               superficie == otra.superficie &&
               pista == otra.pista &&
               sector == otra.sector &&
               byte == otra.byte;
    }
};

#endif 
