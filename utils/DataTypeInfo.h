#ifndef DATA_TYPE_INFO_H
#define DATA_TYPE_INFO_H

#include <string>
#include <stdexcept>

enum DataType {
    INT,
    VARCHAR,
    FLOAT,
    DOUBLE,
    CHAR
};

struct DataTypeInfo {
    DataType type;
    int size;
    
    DataTypeInfo(DataType t, int varcharSize = 0) : type(t) {
        switch(t) {
            case INT:    size = 4; break;
            case FLOAT:  size = 4; break;
            case DOUBLE: size = 8; break;
            case CHAR:   size = 1; break;
            case VARCHAR: 
                if(varcharSize <= 0) 
                    throw std::invalid_argument("Tamaño de VARCHAR debe ser positivo");
                size = varcharSize; 
                break;
            default: 
                throw std::invalid_argument("Tipo de dato no soportado");
        }
    }
    
    std::string toString() const {
        switch(type) {
            case INT: return "int";
            case FLOAT: return "float";
            case DOUBLE: return "double";
            case CHAR: return "char";
            case VARCHAR: return "varchar(" + std::to_string(size) + ")";
            default: return "unknown";
        }
    }
};

#endif