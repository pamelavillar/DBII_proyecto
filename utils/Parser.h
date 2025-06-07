#include <iostream>
using namespace std;
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../utils/DataTypeInfo.h"
#include "../modelo/Registro.h"
#include "../DatabaseManager.h"

class DataLoader {
public:
    static std::vector<DataTypeInfo> parseTypes(const std::string& typeLine);
    static std::vector<Registro> loadFromFile(const std::string& filename, DatabaseManager& dbManager);
};

#endif