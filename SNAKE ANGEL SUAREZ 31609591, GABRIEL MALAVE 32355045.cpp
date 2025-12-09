#include <iostream>
#include <conio.h>    
#include <windows.h>  
#include <cstdlib>
#include <ctime>
#include <string>  
#include <algorithm>

using namespace std;



//variables globales y constantes 

const int MAX_OBSTACULOS = 50; 
int obstaculosx[MAX_OBSTACULOS];
int obstaculosy[MAX_OBSTACULOS];
int t_obstaculos = 0; 
int frutas_comidas = 0; 
int nivel_actual = 1;
int velocidad_juego = 200;


const int MAX_GANADORES = 10;
string ganadores[MAX_GANADORES];
int num_ganadores = 0;


//funciones para el control del juego



void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

void OcultarCursor() {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hcon, &cci);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

const int COLOR_BLANCO = 15; 
const int COLOR_VERDE  = 10; 
const int COLOR_ROJO   = 12; 
const int COLOR_AMARILLO = 14; 


const int max_longitud=100;
const int ancho_tablerox=20; 
const int alto_tableroy=20; 

int cuerpox[max_longitud];
int cuerpoy[max_longitud];
int longitud=1;
int direccion=4; 
int comidax,comiday;

string jugador_actual = ""; 



//mecanicas del juego


void niveles(int nivel); 


void generarComida() {
	
    bool libre;
    do {
        libre = true;
        
        comidax = 1 + rand() % (ancho_tablerox - 2);
        comiday = 1 + rand() % (alto_tableroy - 2);

        for (int i = 0; i < t_obstaculos; i++) {
            if (comidax == obstaculosx[i] && comiday == obstaculosy[i]) {
                libre = false; 
                break;
            }
        }
        
        if (libre) {
            for (int i = 0; i < longitud; i++) {
                if (comidax == cuerpox[i] && comiday == cuerpoy[i]) {
                    libre = false; 
                    break;
                }
            }
        }
    } while (!libre);
}


void dibujartablero() {
    
    for(int y=1; y < alto_tableroy - 1; y++){ 
        for(int x=1; x < ancho_tablerox - 1; x++){
            bool es_serpiente = false;
            int pos_cuerpo = -1;
            
            for(int i = 0; i < longitud; i++){
                if(x == cuerpox[i] && y == cuerpoy[i]){
                    es_serpiente = true;
                    pos_cuerpo = i;
                    break;
                }
            }
            
            bool es_obstaculo = false;
            for(int i = 0; i < t_obstaculos; i++){
                if(x == obstaculosx[i] && y == obstaculosy[i]){
                    es_obstaculo = true;
                    break;
                }
            }

            gotoxy(x, y);
            if (es_serpiente) {
                SetColor(COLOR_VERDE);
                if (pos_cuerpo == 0) {
                    cout << "O"; 
                } else {
                    cout << "+";
                }
            } else if (x == comidax && y == comiday) {
                SetColor(COLOR_AMARILLO);
                cout << "A"; 
            } else if (es_obstaculo) {
                SetColor(COLOR_ROJO);
                cout << "#"; 
            } else {
                SetColor(COLOR_BLANCO);
                cout << " "; 
            }
            SetColor(COLOR_BLANCO);
        }
    }
}


void movimiento(char tecla) {
    
    for (int i = longitud - 1; i > 0; i--) {
        cuerpox[i] = cuerpox[i - 1];
        cuerpoy[i] = cuerpoy[i - 1];
    }

    if (tecla == 'w' && direccion != 2)
        direccion = 1;
    else if (tecla == 's' && direccion != 1)
        direccion = 2;
    else if (tecla == 'a' && direccion != 4)
        direccion = 3;
    else if (tecla == 'd' && direccion != 3)
        direccion = 4;


    if (direccion == 1) 
        cuerpoy[0]--;
    if (direccion == 2) 
        cuerpoy[0]++;
    if (direccion == 3) 
        cuerpox[0]--;
    if (direccion == 4) 
        cuerpox[0]++;
}

bool Comp_comida() {
    if (cuerpox[0] == comidax && cuerpoy[0] == comiday) {
        longitud++;
        frutas_comidas++; 
        generarComida();
        return true;
    }
    return false;
}


bool colisiones() {
    
    if (cuerpox[0] <= 0 || cuerpox[0] >= ancho_tablerox - 1 || cuerpoy[0] <= 0 || cuerpoy[0] >= alto_tableroy - 1) {
        return true;
    }

    for (int i = 1; i < longitud; i++) {
        if (cuerpox[0] == cuerpox[i] && cuerpoy[0] == cuerpoy[i]) {
            return true;
        }
    }
    
    for (int i = 0; i < t_obstaculos; i++) {
        if (cuerpox[0] == obstaculosx[i] && cuerpoy[0] == obstaculosy[i]) {
            return true;
        }
    }
    
    return false;
}

void niveles(int nivel) {

    nivel_actual = nivel;
    t_obstaculos = 0; 
    
    velocidad_juego = 200 - (nivel - 1) * 35;
    if (velocidad_juego < 50) velocidad_juego = 50;

    int obstaculos_a_generar = (nivel - 1) * 4; 

    while (t_obstaculos < obstaculos_a_generar && t_obstaculos < MAX_OBSTACULOS) {
        
        int tempx, tempy;
        bool posicion;
        do {
            posicion= true;
            tempx = 1 + rand() % (ancho_tablerox - 2);
            tempy = 1 + rand() % (alto_tableroy - 2);
            
            if (tempx == cuerpox[0] && tempy == cuerpoy[0]) {
                posicion= false;
                continue;
            }
            
            for (int i = 0; i < t_obstaculos; i++) { 
                if (tempx == obstaculosx[i] && tempy == obstaculosy[i]) {
                    posicion= false;
                    break;
                }
            }

        } while (!posicion); 
        
        obstaculosx[t_obstaculos] = tempx;
        obstaculosy[t_obstaculos] = tempy;
        t_obstaculos++; 
    }
}

//funciones para la lista de ganadores


void RegistrarGanador(string nombre) {
    if (num_ganadores < MAX_GANADORES) {
        ganadores[num_ganadores] = nombre;
        num_ganadores++;
    }
}

void MostrarGanadores() {
    
    for (int i = 0; i < num_ganadores - 1; i++) {
        for (int j = 0; j < num_ganadores - i - 1; j++) {
            
            if (ganadores[j].length() < ganadores[j+1].length()) {
                swap(ganadores[j], ganadores[j+1]); 
            } 
            else if (ganadores[j].length() == ganadores[j+1].length()) {
                if (ganadores[j] > ganadores[j+1]) { 
                    swap(ganadores[j], ganadores[j+1]); 
                }
            }
        }
    }
    
    
    SetColor(COLOR_BLANCO);
    system("cls");
    gotoxy(0, 0);
    cout << " {      GENTE MASOQUISTA (!!GANADORES!!)      } " << endl;
    cout << "===============================================" << endl;
    
    if (num_ganadores == 0) {
        cout << "NADIE HA GANADO AUN, GANE!" << endl;
    } else {
        for (int i = 0; i < num_ganadores; i++) {
            cout << i + 1 << ". " << ganadores[i] << " (Longitud: " << ganadores[i].length() << ")" << endl;
        }
    }
    cout << "================================================" << endl;
    Sleep(3000);
    system("cls");
    
}

void InicializarJuego() {
    frutas_comidas = 0; 
    nivel_actual = 1;
    t_obstaculos = 0;
    longitud = 1;
    direccion = 4;
    
    system("cls");
    SetColor(COLOR_BLANCO);
    for(int y = 0; y < alto_tableroy; y++) {
        for(int x = 0; x < ancho_tablerox; x++) {
            if (y == 0 || y == alto_tableroy - 1 || x == 0 || x == ancho_tablerox - 1) {
                gotoxy(x, y);
                cout << "#";
            }
        }
    }

    int iniciox = ancho_tablerox / 2;
    int inicioy = alto_tableroy / 2;
    cuerpox[0] = iniciox;
    cuerpoy[0] = inicioy;
    
    niveles(1); 
    generarComida(); 
}


int main() {
	
    OcultarCursor();
    
    srand(time(NULL)); 

    string jugar_general;
    

    cout << "Ingrese su nombre: ";
    cin >> jugador_actual;
    cout << "Bienvenido " << jugador_actual << endl;
    Sleep(1000);
    cout << "¿Desea jugar El Culebrero? si/no? ";
    cin >> jugar_general;

    if (jugar_general == "n" || jugar_general == "N" || jugar_general == "no" || jugar_general == "NO") {
        cout << "Que cobarde eres :|" << endl;
        Sleep(1000);
        MostrarGanadores();
        return 0;
    }


    system("cls"); 
    cout << "OOOO :}, me impresiona de ti, y yo pensando que eras un cobarde" << endl;
    Sleep(1000);
    cout << "Reglas: si chocas con algo pierdes.Controles: (w, s, a, d)"<<endl;
    Sleep(1000);
    cout << "......suerte" << endl;
    Sleep(2000);
    
    
    
    //bucle while para el reinicio del juego
    
    
    
    while (jugar_general == "s" || jugar_general == "S" || jugar_general == "si" || jugar_general == "SI") {
        
        InicializarJuego();
        
        bool juego_activo = true;
        char tecla_ultima = 'd';
        
        while (juego_activo) {
            char tecla_input = '\0';
            while (_kbhit()) {
                tecla_input = _getch();
                if (tecla_input == 'w' || tecla_input == 's' || tecla_input == 'a' || tecla_input == 'd') {
                    if ((tecla_input == 'w' && direccion != 2) ||
                        (tecla_input == 's' && direccion != 1) ||
                        (tecla_input == 'a' && direccion != 4) ||
                        (tecla_input == 'd' && direccion != 3)) {
                            tecla_ultima = tecla_input;
                    }
                }
            }
            
            movimiento(tecla_ultima);
            
            if (colisiones()) {
                
                string mensaje = "YOU LOSE!";
                int centro_x = ancho_tablerox / 2;
                int centro_y = alto_tableroy / 2;
                
                for (int i = 0; i < 6; i++) { 
                    
                    if (i % 2 == 0) {
                        SetColor(COLOR_ROJO);
                    } else {
                        SetColor(COLOR_BLANCO);
                    }
                    
                    gotoxy(centro_x - 10, centro_y - 2);
                    cout << "                                   ";
                    gotoxy(centro_x - 10, centro_y);
                    cout << "                                   ";
                    gotoxy(centro_x - 10, centro_y + 2);
                    cout << "                                   ";


                    if (i < 2) { 
                        gotoxy(centro_x - (int)(mensaje.length() / 2), centro_y);
                        cout << mensaje;
                    } else if (i < 4) { 
                        string mensaje_grande = "Y O U  L O S E !";
                        gotoxy(centro_x - (int)(mensaje_grande.length() / 2), centro_y);
                        cout << mensaje_grande;
                    } else { 
                        gotoxy(centro_x - 6, centro_y - 1);
                        cout << "Y O U";
                        gotoxy(centro_x - 8, centro_y + 1);
                        cout << "L O S E !";
                    }
                    
                    Sleep(500); 
                }


                gotoxy(0, alto_tableroy + 1); 
                
                SetColor(COLOR_BLANCO); 
                cout <<"Puntaje: " << longitud - 1 << " | Nivel Maximo Alcanzado: " << nivel_actual << endl;
                Sleep(2000); 
                
                juego_activo = false;
            }

            bool comio = Comp_comida();
            
            int limite_frutas_actual = nivel_actual * 5; 

            if (comio && nivel_actual < 5 && frutas_comidas == limite_frutas_actual) { 
                nivel_actual++;
                
                int ciclos_parpadeo = 6; 
                for (int i = 0; i < ciclos_parpadeo; i++) {
                    if (i % 2 == 0) {
                        SetColor(COLOR_VERDE);
                    } else {
                        SetColor(COLOR_BLANCO);
                    }

                    gotoxy(ancho_tablerox / 2 - 5, alto_tableroy / 2);
                    cout << "NIVEL " << nivel_actual << "!"; 
                    Sleep(300); 
                }
                
                SetColor(COLOR_BLANCO);
                
                system("cls"); 
                for(int y=0; y < alto_tableroy; y++){
                    for(int x=0; x < ancho_tablerox; x++){
                        if(y==0 || y==alto_tableroy-1 || x==0 || x==ancho_tablerox-1){
                            gotoxy(x, y);
                            cout << "#";
                        }
                    }
                }
                
                niveles(nivel_actual); 
                generarComida(); 
                
            } else if (comio && nivel_actual == 5 && frutas_comidas == limite_frutas_actual) {
                
                RegistrarGanador(jugador_actual);
                
                string mensaje_victoria = "¡BINGO!";
                int centro_x = ancho_tablerox / 2;
                int centro_y = alto_tableroy / 2;
                
                system("cls");
                
                for (int i = 0; i < 10; i++) {
                    if (i % 2 == 0) {
                        SetColor(COLOR_AMARILLO);
                    } else {
                        SetColor(COLOR_BLANCO);
                    }
                    gotoxy(centro_x - (int)(mensaje_victoria.length() / 2), centro_y);
                    cout << mensaje_victoria;
                    Sleep(200);
                }
                
                gotoxy(centro_x - 10, centro_y + 2);
                SetColor(COLOR_BLANCO);
                cout << "¡" << jugador_actual << " YOU WIN!";
                Sleep(2000);
                
                juego_activo = false;
            }
            
            
            if(juego_activo){
                dibujartablero();
                
                SetColor(COLOR_BLANCO);
                gotoxy(ancho_tablerox + 2, 5); 
                cout << "Nivel: " << nivel_actual;
                gotoxy(ancho_tablerox + 2, 6);
                cout << "Comidas: " << frutas_comidas;
                gotoxy(ancho_tablerox + 2, 7);
                cout <<"Longitud:"<< longitud;
                Sleep(velocidad_juego); 
            }
        }

        system("cls");
        MostrarGanadores(); 

        SetColor(COLOR_BLANCO);
        gotoxy(0, 0);
        cout << "¿Desea jugar de nuevo? (s/n): ";
        cin >> jugar_general;

    }

    cout << "¡Gracias por jugar, " << jugador_actual << "!" << endl;
    Sleep(2000);

    return 0;
}
