#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Parser.h"

std::vector<DataTypeInfo> DataLoader::parseTypes(const std::string& typeLine) {
    std::vector<DataTypeInfo> types;
    std::istringstream iss(typeLine);
    std::string typeStr;
    
    while (iss >> typeStr) {
        if (typeStr == "int") {
            types.emplace_back(INT); //emplace back porque automaticamente genera el objeto de tipo DataTypeInfo
        } 
        else if (typeStr == "float") {
            types.emplace_back(FLOAT);
        }
        else if (typeStr == "double") {
            types.emplace_back(DOUBLE);
        }
        else if (typeStr == "char") {
            types.emplace_back(CHAR);
        }
        else if (typeStr.find("varchar") != std::string::npos) {
            size_t openParen = typeStr.find('(');
            size_t closeParen = typeStr.find(')');
            if (openParen == std::string::npos || closeParen == std::string::npos) {
                throw std::invalid_argument("Formato varchar inválido: " + typeStr);
            }
            int size = std::stoi(typeStr.substr(openParen + 1, closeParen - openParen - 1));
            types.emplace_back(VARCHAR, size);
        }
        else {
            throw std::invalid_argument("Tipo de dato no soportado: " + typeStr);
        }
    }
    return types;
}



std::vector<Registro> DataLoader::loadFromFile(const std::string& filename, DatabaseManager& dbManager) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Registro> registros;
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    // Leer primera línea (tipos de datos)
    if (std::getline(file, line)) {
        auto types = parseTypes(line);
        
        // Leer registros
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Registro registro;
            cout<<"Numero de atributos: "<<types.size()<<endl;            
            for (size_t i = 0; i < types.size(); ++i) {
                std::string value;
                if (!(iss >> value)) {
                    throw std::runtime_error("Formato de registro inválido");
                }
                
                registro.atributos.emplace_back(types[i], value);
            }
            registros.push_back(registro);
            dbManager.insertRegistro(registro);
        }
    }
    
    return registros;
}