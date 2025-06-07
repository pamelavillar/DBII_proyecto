#include "DatabaseManager.h"
#include "utils/Parser.h"
#include <iostream>

int main() {
    try {
        DatabaseManager dbManager(2, 2, 100, 8, 100); // 2 platos, 2 superficies, 100 pistas, 8 sectores, 100 bytes por sector
        
        std::cout << "Cargando datos desde archivo..." << std::endl;
        DataLoader::loadFromFile("datos.txt", dbManager);
        
        std::cout << "\n=== RESUMEN DE ALMACENAMIENTO ===" << std::endl;
        dbManager.displayAllRecords();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}