#ifndef REGISTRO_H
#define REGISTRO_H

#include "Atributo.h"
#include "DireccionDisco.h"
#include <vector>
#include <string>

using namespace std;

class Registro {
public:
    vector<Atributo> atributos;
    DireccionDisco direccionEnDisco;

    Registro(const vector<Atributo>& atributos);
    Registro();

    int obtenerTamanioTotal() const;
    const vector<Atributo>& getAtributos() const;
    const Atributo& getAtributoPorIndice(int index) const;

    void setDireccion(DireccionDisco direccion);
    DireccionDisco getDireccion() const;

    string getValorPorIndice(int index) const;
    /// @brief 
    /// @return 
    int getID() const ;
};

#endif // REGISTRO_H
