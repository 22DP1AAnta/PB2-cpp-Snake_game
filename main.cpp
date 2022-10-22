#include <iostream>
#include <chrono>  // includoju chrono, lai varetu uzstadit fps limiteri
#include <conio.h> // includoju conio, lai varetu izvadit console
#include <fstream>  // includoju file streamu, lai ierakstitu personal best

using namespace std;

bool speles_beigas;
const int platums = 20; // laucina platums
const int garums = 17; // laucina garums
int x, y, fruits_x, fruits_y, punktu_skaits, high_score;
int aste_X[100], aste_Y[100]; //Cuskas koordinates by default
int astes_garums;
enum virziens {STOP = 0, LEFT,RIGHT, UP, DOWN}; // Kontrole
virziens dir;


void uzstadisana() {
    ifstream fails("high_score.txt", ios::binary);
    if (fails.is_open())
        fails.read((char*)&high_score, sizeof(int));
    else
        high_score = 0;
    speles_beigas = false;

    dir = STOP;

    x = platums / 2;
    y = garums / 2;

    fruits_x = rand() % platums; //Ar randint palīdzību randomā iespawneju fruitu
    fruits_y = rand() % garums; punktu_skaits = 0;

}


void zimesana() {
    system("cls");
    for(int i = 0; i < platums + 2; i++)
        cout << char(219); // Sienas (horizontalas)
    cout << endl ;

    for (int i = 0; i < garums ; i++) {
        for (int j = 0; j < platums; j++) {
            if (j == 0)
                cout << char(219); //Sienas (vertikalas)
            if (i == y && j == x)
                cout << "o"; // Cuskas galva
            else if (i == fruits_y && j == fruits_x )
                cout << "*"; // Fruits
            else {
                bool print = false;
                for (int k = 0; k < astes_garums ; k++) {
                    if (aste_X [k] == j && aste_Y [k] == i) {
                        cout << "0"; print = true; // Aste (turpinājums)
                    }
                }
                if (!print) cout << " ";
            }
            if (j == platums - 1)
                cout << char(219);
        }
        cout << endl;
    }

    for (int i = 0; i < platums + 2; i++)
        cout << char(219);

    cout << endl;
    cout << "Punktu skaits: " << punktu_skaits << endl << "Labakais rezultats: " << high_score << endl ;
}


void ievade ()
{
    if (_kbhit ()) { // detecto vai uzspiests taustins
        switch (_getch ()) { // uzzin characteru no taustina
            case 'a':
                if (dir != RIGHT)
                    dir = LEFT;
                break;

            case 'd':
                if (dir != LEFT)
                    dir = RIGHT;
                break;

            case 'w':
                if (dir != DOWN)
                    dir = UP;
                break;

            case 's':
                if (dir != UP)
                    dir = DOWN ;
                break;

            case 'x':
                speles_beigas = true;
                break;

        }

    }

}


void algoritms_kustiba()
{
    int ieprieksejais_X = aste_X [0];
    int ieprieksejais_Y = aste_Y [0];
    int ieprieksejais_2X, ieprieksejais_2Y;

    aste_X[0] = x;
    aste_Y[0] = y;

    for(int i = 1; i < astes_garums ; i++) {
        ieprieksejais_2X = aste_X[i];
        ieprieksejais_2Y = aste_Y[i];
        aste_X[i] = ieprieksejais_X;
        aste_Y[i] = ieprieksejais_Y;
        ieprieksejais_X = ieprieksejais_2X;
        ieprieksejais_Y = ieprieksejais_2Y ;
    }
    switch (dir) {
        case LEFT:
            x--;
            break;

        case RIGHT:
            x++;
            break;

        case UP:
            y--;
            break;

        case DOWN:
            y++;
            break;

        default:
            break;
    }
    if (x >= platums) x =0;else if (x < 0) x = platums - 1;
    if (y >= garums) y = 0; else if (y < 0) y = garums - 1;
    for (int i =0; i < astes_garums ; i++)
        if (aste_X[i] == x && aste_Y[i] == y)
            speles_beigas = true;


    if (x == fruits_x && y == fruits_y) {
        punktu_skaits +=1;
        fruits_x = rand() % platums;
        fruits_y = rand() % garums;
        astes_garums ++;
    }
}


int main()
{
    int duration_time = 0;
    uzstadisana();
    while (!speles_beigas) {
        auto sakums = chrono::steady_clock::now();
        zimesana();
        ievade();
        algoritms_kustiba();
        do { // do while, kas nosaka frekvenci (frequency limiter)
            auto beigas = chrono::steady_clock::now();
            duration_time = chrono::duration_cast<chrono::nanoseconds>(beigas - sakums).count();
        } while (duration_time < 1.0/7.0*1e9 ); //1.0/fps.0*1e9
    }
    ofstream fails("high_score.txt", ios::binary);
    fails.write((char*)&punktu_skaits, sizeof(int));
    return 0;
}