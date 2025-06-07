#include "Registro.h"

Registro::Registro() {
}
Registro::Registro(const vector<Atributo>& atributos)
    : atributos(atributos) {}

int Registro::obtenerTamanioTotal() const {
    int total = 0;
    for (const auto& attr : atributos) {
        total += attr.obtenerTamanio();
    }
    return total;
}

const vector<Atributo>& Registro::getAtributos() const {
    return atributos;
}

const Atributo& Registro::getAtributoPorIndice(int index) const {
    return atributos.at(index);
}

void Registro::setDireccion(DireccionDisco direccion) {
    direccionEnDisco = direccion;
}

DireccionDisco Registro::getDireccion() const {
    return direccionEnDisco;
}

string Registro::getValorPorIndice(int index) const {
    return atributos.at(index).getValor();
}

int Registro::getID() const {
    return std::stoi(atributos[0].getValor());  
}

