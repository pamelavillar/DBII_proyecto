#include "DatabaseManager.h"
#include <iostream>
#include <iomanip>

DatabaseManager::DatabaseManager(int platos, int superficies, int pistas, 
                               int sectores, int tamanioSector)
    : disco(platos, superficies, pistas, sectores, tamanioSector) {}

void DatabaseManager::insertRegistro(const Registro& registro) {
    // Almacenar en disco y obtener la dirección
    DireccionDisco address = disco.storeRecord(registro);
    
    // Registrar la ubicación
    recordLocations.push_back({address, registro});

    // Mostrar información de almacenamiento
    //printRecordLocation(registro, address);
}

void DatabaseManager::displayAllRecords() const {
    std::cout << "\n=== REGISTROS ALMACENADOS ===" << std::endl;
    for (const auto& par : recordLocations) {
        const auto& address = par.first;
        const auto& registro = par.second;
        printRecordLocation(registro, address);
    }


}

void DatabaseManager::printRecordLocation(const Registro& registro, const DireccionDisco& address) const {
    std::cout << "Registro almacenado en:\n"
              << " - Plato: " << address.plato << "\n"
              << " - Superficie: " << address.superficie << "\n"
              << " - Pista: " << address.pista << "\n"
              << " - Sector: " << address.sector << "\n"
              << " - Byte inicial: " << address.byte << "\n"
              << "Contenido: ";
    
    for (const auto& atributo : registro.atributos) {
        std::cout << atributo.valor << " ";
    }
    std::cout << "\n---------------------------" << std::endl;
}