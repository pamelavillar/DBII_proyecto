# Makefile para proyecto C++ con varias carpetas

CXX = clang++
CXXFLAGS = -std=c++14 -g -Wall -fcolor-diagnostics -fansi-escape-codes

# Archivos fuente: todos los .cpp en las carpetas indicadas + main.cpp en la raíz
SRC = main.cpp \
      DatabaseManager.cpp \
      disco/*.cpp \
      modelo/*.cpp \
      utils/*.cpp\
      #estructura/*.cpp

# Nombre del ejecutable
TARGET = main

# Regla para compilar todo
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
