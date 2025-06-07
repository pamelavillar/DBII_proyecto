// disco/Sector.cpp
#include "Sector.h"

Sector::Sector(int tamanio) : tamanio(tamanio), usado(0) {
    datos.resize(tamanio, '\0');
}

bool Sector::escribir(const string& valor, int tamanioAtributo) {
    if (tamanioAtributo > espacioDisponible()) return false;

    int bytesEscribir = min(static_cast<int>(valor.size()), tamanioAtributo);
    for (int i = 0; i < bytesEscribir; ++i) {
        datos[usado++] = valor[i];
    }

    for (int i = bytesEscribir; i < tamanioAtributo; ++i) {
        datos[usado++] = ' '; 
    }

    return true;
}
string Sector::leer() const {
    return string(datos.begin(), datos.begin() + usado);
}

int Sector::espacioDisponible() const {
    return tamanio - usado;
}
