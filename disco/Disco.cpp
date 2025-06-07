#include "Disco.h"
#include <stdexcept>

Disco::Disco(int platos, int superficies, int pistas, int sectores, int tamanioSector)
    : platos(platos), superficiesPorPlato(superficies), pistasPorSuperficie(pistas), sectoresPorPista(sectores), tamanioSector(tamanioSector) {

    disco.resize(platos);
    for (int p = 0; p < platos; ++p) {
        disco[p].resize(superficiesPorPlato);
        for (int s = 0; s < superficiesPorPlato; ++s) {
            disco[p][s].resize(pistasPorSuperficie);
            for (int pi = 0; pi < pistasPorSuperficie; ++pi) {
                disco[p][s][pi].resize(sectoresPorPista, Sector(tamanioSector));
            }
        }
    }

    ultimaPosicion = {0, 0, 0, 0, 0};
}

DireccionDisco Disco::insertarAtributo(const string& valor, int tamanio) {
    if (tamanio > tamanioSector) {
        throw runtime_error("El atributo excede el tamaño de un sector.");
    }


    while (true) {
        Sector& sector = disco
            [ultimaPosicion.plato]
            [ultimaPosicion.superficie]
            [ultimaPosicion.pista]
            [ultimaPosicion.sector];


        if (sector.espacioDisponible() >= tamanio) {
            DireccionDisco inicio = ultimaPosicion;
            sector.escribir(valor.substr(0, tamanio), tamanio);
            ultimaPosicion.byte = sector.usado;
            return inicio;
        } else {
            // Avanzar al siguiente sector
            ++ultimaPosicion.sector;
            ultimaPosicion.byte = 0;
            if (ultimaPosicion.sector >= sectoresPorPista) {
                ultimaPosicion.sector = 0;
                ++ultimaPosicion.pista;
                if (ultimaPosicion.pista >= pistasPorSuperficie) {
                    ultimaPosicion.pista = 0;
                    ++ultimaPosicion.superficie;
                    if (ultimaPosicion.superficie >= superficiesPorPlato) {
                        ultimaPosicion.superficie = 0;
                        ++ultimaPosicion.plato;
                        if (ultimaPosicion.plato >= platos) {
                            throw runtime_error("Disco lleno.");
                        }
                    }
                }
            }
        }
    }
}

void Disco::reset() {
    ultimaPosicion = {0, 0, 0, 0, 0};
}
