#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

void posicionarCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void mostrarLaberinto(int **matriz, int filas, int columnas, pair<int, int> jugador, pair<int, int> meta, int **monedas, int monedasRecogidas, int segundosTranscurridos) {
    posicionarCursor(0, 0);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Mostrar laberinto
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == jugador.first && j == jugador.second) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << char(219) << char(219);
                SetConsoleTextAttribute(hConsole, 7);
            } else if (i == meta.first && j == meta.second) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << char(219) << char(219);
                SetConsoleTextAttribute(hConsole, 7);
            } else if (matriz[i][j] == 1) {
                cout << char(178) << char(178);
            } else if (monedas[i][j] == 1) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "$$";
                SetConsoleTextAttribute(hConsole, 7);
            } else {
                cout << "  ";
            }
        }
        cout << "\n";
    }

    // Mostrar información adicional
    posicionarCursor(0, filas);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Monedas: " << monedasRecogidas;
    SetConsoleTextAttribute(hConsole, 7);

    // Mostrar el cronómetro (tiempo transcurrido)
    int minutos = segundosTranscurridos / 60;
    int segundos = segundosTranscurridos % 60;
    posicionarCursor(columnas - 15, filas);  // Ajustar la posición a la derecha
    cout << "Tiempo: " << minutos << "m " << segundos << "s";
}

bool esAccesible(int **matriz, int filas, int columnas, pair<int, int> pos) {
    return pos.first >= 0 && pos.first < filas && pos.second >= 0 && pos.second < columnas && matriz[pos.first][pos.second] == 0;
}

void generarLaberinto(int filas, int columnas, float densidad, int **&matriz, pair<int, int> &meta, int **&monedas, int cantidadMonedas) {
    matriz = new int *[filas];
    monedas = new int *[filas];
    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
        monedas[i] = new int[columnas];
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1) ? 1 : 0;
            monedas[i][j] = 0;
        }
    }

    srand(time(NULL));
    for (int i = 1; i < filas - 1; i++) {
        for (int j = 1; j < columnas - 1; j++) {
            if (rand() / float(RAND_MAX) < densidad) {
                matriz[i][j] = 1;
            }
        }
    }

    meta.first = rand() % (filas - 2) + 1;
    meta.second = rand() % (columnas - 2) + 1;
    while (matriz[meta.first][meta.second] == 1) {
        meta.first = rand() % (filas - 2) + 1;
        meta.second = rand() % (columnas - 2) + 1;
    }

    for (int i = 0; i < cantidadMonedas; i++) {
        int x = rand() % (filas - 2) + 1;
        int y = rand() % (columnas - 2) + 1;
        while (matriz[x][y] == 1 || (x == meta.first && y == meta.second)) {
            x = rand() % (filas - 2) + 1;
            y = rand() % (columnas - 2) + 1;
        }
        monedas[x][y] = 1;
    }
}

int main() {
    int filas = 15, columnas = 30;
    int **matriz, **monedas;
    pair<int, int> jugador = {1, 1};
    pair<int, int> meta;
    int monedasRecogidas = 0;
    int segundosTranscurridos = 0;
    time_t tiempoInicio = time(NULL);

    generarLaberinto(filas, columnas, 0.2, matriz, meta, monedas, 10);
    mostrarLaberinto(matriz, filas, columnas, jugador, meta, monedas, monedasRecogidas, segundosTranscurridos);

    while (true) {
        if (_kbhit()) {
            char tecla = _getch();
            pair<int, int> nuevaPos = jugador;

            if (tecla == 'w') nuevaPos.first--;
            else if (tecla == 's') nuevaPos.first++;
            else if (tecla == 'a') nuevaPos.second--;
            else if (tecla == 'd') nuevaPos.second++;

            if (esAccesible(matriz, filas, columnas, nuevaPos)) {
                jugador = nuevaPos;
                if (jugador == meta) {
                    monedasRecogidas = 0;
                    generarLaberinto(filas, columnas, 0.2, matriz, meta, monedas, 10);
                    mostrarLaberinto(matriz, filas, columnas, jugador, meta, monedas, monedasRecogidas, segundosTranscurridos);
                    cout << "\n¡Has llegado a la meta! Presiona 'C' para continuar jugando o 'Q' para salir.\n";

                    while (true) {
                        if (_kbhit()) {
                            char opcion = _getch();
                            if (opcion == 'c' || opcion == 'C') {
                                break;
                            } else if (opcion == 'q' || opcion == 'Q') {
                                return 0;
                            }
                        }
                    }
                }
                if (monedas[jugador.first][jugador.second] == 1) {
                    monedas[jugador.first][jugador.second] = 0;
                    monedasRecogidas++;
                }
            }
            mostrarLaberinto(matriz, filas, columnas, jugador, meta, monedas, monedasRecogidas, segundosTranscurridos);
        }

        // Actualizar cronómetro
        segundosTranscurridos = difftime(time(NULL), tiempoInicio);
        Sleep(100);
    }

    return 0;
}
