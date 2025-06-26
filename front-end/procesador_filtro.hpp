
#pragma once
#include "../DatabaseManager.h"
#include "json.hpp"
#include <vector>
#include <string>
using json = nlohmann::json;

/*
json procesarPeticionCompleta(const json& payload, DatabaseManager* dbmanager) {
    // Extraer los filtros
    std::vector<std::string> campos = payload["campos_bus"].get<std::vector<std::string>>();
    std::vector<std::string> valores = payload["valores_bus"].get<std::vector<std::string>>();
    std::string operador = payload["operador"].get<std::string>();
    std::vector<std::string> columnas = payload["columnas"].get<std::vector<std::string>>();

    std::string atributo_orden = payload["ordenar"]["atributo"];
    std::string direccion_orden = payload["ordenar"]["direccion"];

    // Llamar a la búsqueda
    std::vector<std::vector<std::string>> resultado = dbmanager->buscar(campos, valores, operador, columnas, atributo_orden, direccion_orden);

    return resultado; // json automáticamente lo convierte si es vector<vector<string>>
}
*/


json procesarPeticionCompleta1(const json& payload) {

    std::vector<std::vector<std::string>> resultado ={
        { "Carlos", "20", "12345678", "1.75", "15.5"},
        {"Juana", "22", "87654321", "1.62", "18.3"},
        {"Carlos", "21", "11112222", "1.80", "17.0"},
        { "Maria", "18", "45651354", "1.60", "14.9"},
        {"Ivan", "20", "78945612", "1.72", "16.4"},
        {"Carlos", "20", "32165498", "1.75", "15.5"},
        { "Juana", "19", "99999999", "1.62", "17.8"},
        { "Pedro", "23", "65498712", "1.82", "19.1"},
        { "Maria", "21", "14725836", "1.60", "16.0"},
        {"Lucia", "22", "85236974", "1.70", "18.0"},
        { "Andres", "24", "35795148", "1.78", "19.5"}
    };

    return resultado; 
}

