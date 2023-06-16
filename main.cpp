#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

struct celda {
    int valor;
    bool fijo;
};

std::vector<std::vector<celda>> tablero(9, std::vector<celda>(9, {0, false}));
int puntaje_jugador1 = 0, puntaje_jugador2 = 0;
bool turno_jugador1 = true, error_jugador1 = false, error_jugador2 = false;

void inicializar_tablero(bool tablero_vacio);

bool es_valido(int fila, int columna, int valor);

void imprimir_tablero();

void guardar_juego(const std::string &ganador, int puntaje);

int getNumero();

int main() {


    std::string jugador1, jugador2;

    std::cout << "Ingrese el nombre del jugador 1: ";
    std::cin >> jugador1;

    std::cout << "Ingrese el nombre del jugador 2: ";
    std::cin >> jugador2;

    char input;
    std::cout << "Quiere un tablero vacio? (s/n): ";
    std::cin >> input;

    inicializar_tablero(input == 's');

    imprimir_tablero();

    while (true) {
        std::cout << "Es el turno de " << (turno_jugador1 ? jugador1 : jugador2) << "\n";
        int fila, columna, valor;
        std::cout << "Ingrese el numero de fila (1-9): ";
        fila = getNumero();
        std::cout << "Ingrese el numero de columna (1-9): ";
        columna = getNumero();
        std::cout << "Ingrese el valor (1-9): ";
        valor = getNumero();

        if (es_valido(fila - 1, columna - 1, valor)) {
            tablero[fila - 1][columna - 1].valor = valor;
            if (turno_jugador1) {
                puntaje_jugador1++;
                error_jugador1 = false;
            } else {
                puntaje_jugador2++;
                error_jugador2 = false;
            }
        } else {
            if (turno_jugador1) {
                error_jugador1 = true;
            } else {
                error_jugador2 = true;
            }
        }

        imprimir_tablero();

        if (puntaje_jugador1 + puntaje_jugador2 == 81) {
            std::string ganador;
            int puntaje;
            if (puntaje_jugador1 > puntaje_jugador2) {
                ganador = jugador1;
                puntaje = puntaje_jugador1;
            } else {
                ganador = jugador2;
                puntaje = puntaje_jugador2;
            }
            guardar_juego(ganador, puntaje);
            break;
        }

        if (error_jugador1 && error_jugador2) {
            std::string ganador;
            int puntaje;
            if (puntaje_jugador1 > puntaje_jugador2) {
                ganador = jugador1;
                puntaje = puntaje_jugador1;
            } else {
                ganador = jugador2;
                puntaje = puntaje_jugador2;
            }
            guardar_juego(ganador, puntaje);
            break;
        }

        if (error_jugador1) {
            turno_jugador1 = false;
        } else if (error_jugador2) {
            turno_jugador1 = true;
        } else {
            turno_jugador1 = !turno_jugador1;
        }
    }


}

int getNumero() {
    int numero;
    while (!(std::cin >> numero) || numero < 1 || numero > 9) {
        std::cout << "Ingrese un numero valido entre 1 y 9: ";
        std::cin.clear();
        std::cin.ignore(100, '\n');

    }
    return numero;
}

bool es_valido(int fila, int columna, int valor) {
    for (int i = 0; i < 9; i++) {
        if (tablero[fila][i].valor == valor) {
            return false;
        }
    }
    for (int i = 0; i < 9; i++) {
        if (tablero[i][columna].valor == valor) {
            return false;
        }
    }
    int fila_inicio = fila / 3 * 3;
    int columna_inicio = columna / 3 * 3;

    for (int i = fila_inicio; i < fila_inicio + 3; i++) {
        for (int j = columna_inicio; j < columna_inicio + 3; j++) {
            if (tablero[i][j].valor == valor) {
                return false;
            }
        }
    }
    return true;
}

void inicializar_tablero(bool tablero_vacio) {
    if (!tablero_vacio) {
        std::vector<std::pair<int, int>> posiciones;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                posiciones.push_back(std::make_pair(i, j));
            }
        }
        std::shuffle(posiciones.begin(), posiciones.end(), std::mt19937(std::random_device()()));

        for (int i = 0; i < 40; i++) {
            int x = posiciones[i].first;
            int y = posiciones[i].second;
            int valor_aleatorio = std::rand() % 9 + 1;
            if (es_valido(x, y, valor_aleatorio)) {
                tablero[x][y].valor = valor_aleatorio;
                tablero[x][y].fijo = true;
            }
        }

    }
}

void imprimir_tablero() {
    std::cout << " " << std::endl;
    for(int i = 0; i < 9; i++) {
        if (i != 0 && i % 3 == 0)
            std::cout << "----------|-----------|-----------|" << std::endl;

        for(int j = 0; j < 9; j++) {
            if (j != 0 && j % 3 == 0)
                std::cout << " | ";

            std::cout << " " << (tablero[i][j].valor == 0 ? '0' : char(tablero[i][j].valor + '0')) << " ";
        }
        std::cout << std::endl;
    }
}




void guardar_juego(const std::string &ganador, int puntaje) {
    std::ofstream archivo_salida;
    archivo_salida.open("juegos.txt", std::ios::app);
    archivo_salida << ganador << " " << puntaje << std::endl;
    archivo_salida.close();
}