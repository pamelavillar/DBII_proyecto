#ifndef ATRIBUTO_H
#define ATRIBUTO_H

#include <string>
using namespace std;
#include "../utils/DataTypeInfo.h"  

class Atributo {
public:
    DataTypeInfo tipo; 
    std::string valor;

    Atributo(const DataTypeInfo& typeInfo, const std::string& valor)
    : tipo(typeInfo.type, (typeInfo.type == VARCHAR) ? typeInfo.size : 0), 
      valor(valor) {} 

    //Atributo(const std::string& tipoStr, const std::string& valor);

    int obtenerTamanio() const { return tipo.size; } 
    std::string getTipoString() const { return tipo.toString(); }
    std::string getValor() const { return valor; }
    void setValor(const std::string& nuevoValor) { valor = nuevoValor; }

private:
    DataTypeInfo convertirStringATipo(const string& tipoStr);  
};

#endif
