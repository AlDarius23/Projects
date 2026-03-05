#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;


int matrice[8][8] = {0};
int vculori[16] = {0};
int activeCerc = 0;
int culoareCercActiv = 15;


void deseneazaButonMeniu() {
    int butonLatime = 100;
    int butonInaltime = 40;
    int butonX = 20;
    int butonY = 20;


    setfillstyle(SOLID_FILL, BLUE);
    bar(butonX, butonY, butonX + butonLatime, butonY + butonInaltime);
    rectangle(butonX, butonY, butonX + butonLatime, butonY + butonInaltime);

    // Setare culori pentru textul din buton
    setbkcolor(BLUE);
    setcolor(WHITE);


    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(butonX + 20, butonY + 10, "Meniu");

    // Revino la culoarea implicită a fundalului aplicației
    setbkcolor(BLACK);
}

// Verifică dacă un click a fost pe butonul din meniu
bool verificaClickMeniu(int x, int y) {
    int butonLatime = 100;
    int butonInaltime = 40;
    int butonX = 20;
    int butonY = 20;

    // Verificăm dacă coordonatele x, y ale click-ului sunt în interiorul butonului
    return (x >= butonX && x <= butonX + butonLatime &&
            y >= butonY && y <= butonY + butonInaltime);
}

// Resetează starea jocului
void reseteazaJoc() {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrice[i][j] = 0;
        }
    }


    for (int i = 0; i < 16; i++) {
        vculori[i] = 0;
    }


    activeCerc = 0;
    culoareCercActiv = 15;


    cleardevice();
}

// Desenează matricea și indicii linie-coloană
void deseneazaMatrice(int startX, int startY, int dimensiuneCelula) {
    for (int i = 0; i <= 8; i++) {
        // Desenează liniile orizontale
        line(startX, startY + i * dimensiuneCelula,
             startX + 8 * dimensiuneCelula, startY + i * dimensiuneCelula);

        if (i < 8) { // Adaugă indicii liniilor (numerotarea 1-8)
            char indiceLinie[2];
            indiceLinie[0] = '0' + i + 1;
            indiceLinie[1] = '\0';
            outtextxy(startX - 20, startY + i * dimensiuneCelula + dimensiuneCelula / 2 - 5, indiceLinie);
        }
    }

    for (int j = 0; j <= 8; j++) {
        // Desenează liniile verticale
        line(startX + j * dimensiuneCelula, startY,
             startX + j * dimensiuneCelula, startY + 8 * dimensiuneCelula);

        if (j < 8) { // Adaugă indicii coloanelor
            char indiceColoana[2];
            indiceColoana[0] = '0' + j + 1;
            indiceColoana[1] = '\0';
            outtextxy(startX + j * dimensiuneCelula + dimensiuneCelula / 2 - 5, startY - 30, indiceColoana);
        }
    }
}

// Desenează un cerc la poziția specificată cu o anumită culoare
void deseneazaCerc(int x, int y, int dimensiuneCelula, int culoare) {
    setfillstyle(SOLID_FILL, culoare);
    int centruX = x + dimensiuneCelula / 2;
    int centruY = y + dimensiuneCelula / 2;
    int raza = dimensiuneCelula / 4;
    fillellipse(centruX, centruY, raza, raza); // Desenează cercul plin
}

// Șterge cercul de la poziția specificată
void stergeCerc(int x, int y, int dimensiuneCelula) {
    setfillstyle(SOLID_FILL, BLACK);
    bar(x, y, x + dimensiuneCelula, y + dimensiuneCelula); // Șterge zona
    setcolor(WHITE); // Contur alb
    rectangle(x, y, x + dimensiuneCelula, y + dimensiuneCelula); // Redesenarea
}

// Verifică dacă se poate activa un cerc la o anumită poziție
bool pot(int linie, int coloana) {
    if (matrice[linie][coloana] != 0) return false; // Poziția este deja ocupată

    int directii[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; i++) {
        int linieVecin = linie + directii[i][0];
        int coloanaVecin = coloana + directii[i][1];

        // Verifică dacă vecinul este valid și are aceeași culoare activă
        if (linieVecin >= 0 && linieVecin < 8 && coloanaVecin >= 0 && coloanaVecin < 8) {
            if (matrice[linieVecin][coloanaVecin] == culoareCercActiv) {
                return true;
            }
        }
    }
    return false;
}


void deseneazaPatrate(int startX, int startY, int dimensiuneCelula, int culoare) {
    int margin = 1; // Marginea între pătrat și liniile grilei

    while (activeCerc) {
        if (ismouseclick(WM_RBUTTONDOWN)) { // Detectare click dreapta
            int x, y;
            getmouseclick(WM_RBUTTONDOWN, x, y);
            if (verificaClickMeniu(x, y)) { // Verificare click pe butonul de meniu
                reseteazaJoc();
                return; // Întoarce-te la meniul principal
            }
            clearmouseclick(WM_RBUTTONDOWN);
        }

        if (ismouseclick(WM_LBUTTONDOWN)) { // Detectare click stânga
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            int linie = (y - startY) / dimensiuneCelula;
            int coloana = (x - startX) / dimensiuneCelula;

            if (linie >= 0 && linie < 8 && coloana >= 0 && coloana < 8 && pot(linie, coloana)) {
                int patratX = startX + coloana * dimensiuneCelula + margin;
                int patratY = startY + linie * dimensiuneCelula + margin;

                setfillstyle(SOLID_FILL, culoare); // Umple pătratul cu culoarea activă
                bar(patratX, patratY, patratX + dimensiuneCelula - 2, patratY + dimensiuneCelula - 2);

                matrice[linie][coloana] = culoare; // Actualizează matricea
                vculori[culoare]++;

                if (vculori[culoare] == 7) { // Dacă se ating 7 celule, dezactivează cercul
                    activeCerc = 0;
                }
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}

// Verifică dacă tabla este completă
bool estePlina() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (matrice[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

// Afișează mesajul de câștig
void afiseazaMesajCastig() {
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(300, 50, "AI CASTIGAT! Vei ajunge in meniu in 3 secunde!");
}

// Funcție pentru jucarea unui nivel cu interacțiuni din partea utilizatorului
void joacanivele(int startX, int startY, int dimensiuneCelula) {
    deseneazaButonMeniu(); // Desenează butonul de meniu pentru acest nivel

    while (true) { // Bucla principală a nivelului
        if (ismouseclick(WM_RBUTTONDOWN)) { // Dacă se detectează un click dreapta
            int x, y;
            getmouseclick(WM_RBUTTONDOWN, x, y);
            if (verificaClickMeniu(x, y)) { // Verificăm dacă s-a apăsat pe butonul de meniu
                reseteazaJoc(); // Resetează starea jocului
                cleardevice(); // Curăță ecranul
                return; // Ieșim din funcție
            }

            // Calculăm poziția pe matrice unde s-a efectuat click
            int linie = (y - startY) / dimensiuneCelula;
            int coloana = (x - startX) / dimensiuneCelula;

            if (linie >= 0 && linie < 8 && coloana >= 0 && coloana < 8) {
                int patratX = startX + coloana * dimensiuneCelula;
                int patratY = startY + linie * dimensiuneCelula;

                if (matrice[linie][coloana] == -1) {
                    int culoare = rand() % 14 + 1;
                    while (vculori[culoare] > 0) {
                        culoare = rand() % 14 + 1;
                    }

                    deseneazaCerc(patratX, patratY, dimensiuneCelula, culoare);
                    matrice[linie][coloana] = culoare;
                    culoareCercActiv = culoare;
                    activeCerc = 1;
                    deseneazaPatrate(startX, startY, dimensiuneCelula, culoare);

                    if (vculori[culoare] == 0) {
                        return;
                    }
                }

                if (estePlina()) {
                    afiseazaMesajCastig();
                    delay(3000);
                    reseteazaJoc();
                    cleardevice();
                    return;
                }
            }
            clearmouseclick(WM_RBUTTONDOWN);
        }
    }
}

// Funcție pentru interacțiuni custom
void click(int startX, int startY, int dimensiuneCelula) {
    int cnt = 0; // Numărăm cercurile adăugate manual
    deseneazaButonMeniu();

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (verificaClickMeniu(x, y)) {
                reseteazaJoc();
                return;
            }

            int linie = (y - startY) / dimensiuneCelula;
            int coloana = (x - startX) / dimensiuneCelula;

            if (linie >= 0 && linie < 8 && coloana >= 0 && coloana < 8) {
                int patratX = startX + coloana * dimensiuneCelula;
                int patratY = startY + linie * dimensiuneCelula;

                if (matrice[linie][coloana] == -1) {
                    stergeCerc(patratX, patratY, dimensiuneCelula);
                    matrice[linie][coloana] = 0;
                    cnt--;
                } else if (matrice[linie][coloana] == 0 && cnt < 8) {
                    deseneazaCerc(patratX, patratY, dimensiuneCelula, 15);
                    matrice[linie][coloana] = -1;
                    cnt++;
                }
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }

        if (ismouseclick(WM_RBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_RBUTTONDOWN, x, y);

            int linie = (y - startY) / dimensiuneCelula;
            int coloana = (x - startX) / dimensiuneCelula;

            if (linie >= 0 && linie < 8 && coloana >= 0 && coloana < 8) {
                int patratX = startX + coloana * dimensiuneCelula;
                int patratY = startY + linie * dimensiuneCelula;

                if (matrice[linie][coloana] == -1) {
                    int culoare = rand() % 14 + 1;
                    while (vculori[culoare] > 0) {
                        culoare = rand() % 14 + 1;
                    }

                    deseneazaCerc(patratX, patratY, dimensiuneCelula, culoare);
                    matrice[linie][coloana] = culoare;
                    culoareCercActiv = culoare;
                    activeCerc = 1;
                    deseneazaPatrate(startX, startY, dimensiuneCelula, culoare);
                }

                if (estePlina()) {
                    afiseazaMesajCastig();
                    delay(3000);
                    reseteazaJoc();
                    cleardevice();
                    return;
                }
            }
            clearmouseclick(WM_RBUTTONDOWN);
        }
    }
}

// Generează cercuri aleatorii pe tablă
void genereazaRandom(int dimensiuneCelula, int startX, int startY) {
    srand(time(0)); // Seed pentru generare aleatoare
    deseneazaButonMeniu();
    int cercuriGenerate = 0;

    while (cercuriGenerate < 8) {
        int linie = rand() % 6 + 1;
        int coloana = rand() % 6 + 1;


        if (matrice[linie][coloana] == 0) {
            int x = startX + coloana * dimensiuneCelula;
            int y = startY + linie * dimensiuneCelula;

            deseneazaCerc(x, y, dimensiuneCelula, culoareCercActiv);
            matrice[linie][coloana] = -1;
            cercuriGenerate++;
        }
    }
    joacanivele(startX, startY, dimensiuneCelula);
}



void nivel1(int startX, int startY, int dimensiuneCelula) {
            deseneazaMatrice(startX, startY, dimensiuneCelula);
            deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            matrice[0][0] = -1;
            matrice[1][1] = -1;
            matrice[2][2] = -1;
            matrice[3][3] = -1;
            matrice[4][4] = -1;
            matrice[5][5] = -1;
            matrice[6][6] = -1;
            matrice[7][7] = -1;
            joacanivele(startX, startY, dimensiuneCelula);
}
void nivel2(int startX, int startY, int dimensiuneCelula) {
            deseneazaMatrice(startX, startY, dimensiuneCelula);
            deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            matrice[0][0] = -1;
            matrice[6][0] = -1;
            matrice[3][2] = -1;
            matrice[0][4] = -1;
            matrice[4][4] = -1;
            matrice[6][5] = -1;
            matrice[2][6] = -1;
            matrice[7][7] = -1;
            joacanivele(startX, startY, dimensiuneCelula);



}
void nivel3(int startX, int startY, int dimensiuneCelula) {
            deseneazaMatrice(startX, startY, dimensiuneCelula);
            deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            matrice[1][1] = -1;
            matrice[4][1] = -1;
            matrice[6][1] = -1;
            matrice[7][3] = -1;
            matrice[0][4] = -1;
            matrice[2][5] = -1;
            matrice[4][5] = -1;
            matrice[6][5] = -1;
            joacanivele(startX, startY, dimensiuneCelula);
}
void nivel4(int startX, int startY, int dimensiuneCelula) {
            deseneazaMatrice(startX, startY, dimensiuneCelula);
            deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
            matrice[0][0] = -1;
            matrice[2][1] = -1;
            matrice[1][2] = -1;
            matrice[6][2] = -1;
            matrice[4][4] = -1;
            matrice[1][5] = -1;
            matrice[6][5] = -1;
            matrice[1][7] = -1;
            joacanivele(startX, startY, dimensiuneCelula);
}
void nivel5(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[1][0] = -1; matrice[3][1] = -1; matrice[6][2] = -1; matrice[2][3] = -1;
    matrice[5][4] = -1; matrice[0][5] = -1; matrice[4][6] = -1; matrice[7][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
void nivel6(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[0][0] = -1; matrice[7][1] = -1; matrice[4][2] = -1; matrice[1][3] = -1;
    matrice[6][4] = -1; matrice[2][5] = -1; matrice[3][6] = -1; matrice[5][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
void nivel7(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[2][0] = -1; matrice[6][1] = -1; matrice[3][2] = -1; matrice[7][3] = -1;
    matrice[4][4] = -1; matrice[1][5] = -1; matrice[5][6] = -1; matrice[0][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
void nivel8(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[7][0] = -1; matrice[5][1] = -1; matrice[3][2] = -1; matrice[1][3] = -1;
    matrice[6][4] = -1; matrice[4][4] = -1; matrice[2][2] = -1; matrice[0][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
void nivel9(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[6][0] = -1; matrice[4][1] = -1; matrice[2][1] = -1; matrice[0][3] = -1;
    matrice[7][4] = -1; matrice[5][5] = -1; matrice[7][6] = -1; matrice[1][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
void nivel10(int startX, int startY, int dimensiuneCelula) {
    deseneazaMatrice(startX, startY, dimensiuneCelula);
    deseneazaCerc(startX + 0 * dimensiuneCelula, startY + 3 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 1 * dimensiuneCelula, startY + 5 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 2 * dimensiuneCelula, startY + 7 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 3 * dimensiuneCelula, startY + 2 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 4 * dimensiuneCelula, startY + 0 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 5 * dimensiuneCelula, startY + 4 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 6 * dimensiuneCelula, startY + 6 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);
    deseneazaCerc(startX + 7 * dimensiuneCelula, startY + 1 * dimensiuneCelula, dimensiuneCelula, culoareCercActiv);

    matrice[3][0] = -1; matrice[5][1] = -1; matrice[7][2] = -1; matrice[2][3] = -1;
    matrice[0][4] = -1; matrice[4][5] = -1; matrice[6][6] = -1; matrice[1][7] = -1;

    joacanivele(startX, startY, dimensiuneCelula);
}
// Funcție pentru selectarea unui nivel de joc
void selecteazaNivel(int latime, int inaltime, int startX, int startY, int dimensiuneCelula) {
    int butonLatime = 100;
    int butonInaltime = 50;
    int spatiuIntreButoane = 20;


    int startXNiveluri = (latime - 5 * butonLatime - 4 * spatiuIntreButoane) / 2;
    int startYNiveluri = (inaltime - 2 * butonInaltime - spatiuIntreButoane) / 2;


    void (*niveluri[10])(int, int, int) = {nivel1, nivel2, nivel3, nivel4, nivel5,
                                           nivel6, nivel7, nivel8, nivel9, nivel10};

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    for (int i = 0; i < 10; i++) {
        int linie = i / 5; // Calculăm linia în care se află butonul
        int coloana = i % 5; // Calculăm coloana în care se află butonul


        int x1 = startXNiveluri + coloana * (butonLatime + spatiuIntreButoane);
        int y1 = startYNiveluri + linie * (butonInaltime + spatiuIntreButoane);
        int x2 = x1 + butonLatime;
        int y2 = y1 + butonInaltime;


        setfillstyle(SOLID_FILL, BLUE);
        bar(x1, y1, x2, y2);
        setcolor(WHITE);
        rectangle(x1, y1, x2, y2); // Contur alb al butonului


        if (i < 9) {
            char textNivel[2] = {'1' + i, '\0'};
            outtextxy(x1 + butonLatime / 3, y1 + butonInaltime / 4, textNivel);
        } else {
            outtextxy(x1 + butonLatime / 3, y1 + butonInaltime / 4, "10");
        }
    }

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX, mouseY;
            getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

            for (int i = 0; i < 10; i++) {
                int linie = i / 5;
                int coloana = i % 5;

                int x1 = startXNiveluri + coloana * (butonLatime + spatiuIntreButoane);
                int y1 = startYNiveluri + linie * (butonInaltime + spatiuIntreButoane);
                int x2 = x1 + butonLatime;
                int y2 = y1 + butonInaltime;

                if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2) {
                    clearmouseclick(WM_LBUTTONDOWN);
                    cleardevice();
                    niveluri[i](startX, startY, dimensiuneCelula);
                    return;
                }
            }
        }
    }
}

// Funcție pentru desenarea meniului principal
void deseneazaMeniuPrincipal(int latime, int inaltime, int butonLatime, int butonInaltime,
                             int butonStartX, int butonStartY, int butonCumX, int butonCumY) {

    setfillstyle(SOLID_FILL, BLUE);
    bar(butonStartX, butonStartY, butonStartX + butonLatime, butonStartY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonStartX, butonStartY, butonStartX + butonLatime, butonStartY + butonInaltime);
    setbkcolor(BLUE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(butonStartX + butonLatime / 4, butonStartY + butonInaltime / 4, "Start");
    setbkcolor(BLACK);


    setfillstyle(SOLID_FILL, BLUE);
    bar(butonCumX, butonCumY, butonCumX + butonLatime, butonCumY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonCumX, butonCumY, butonCumX + butonLatime, butonCumY + butonInaltime);
    setbkcolor(BLUE);
    outtextxy(butonCumX + butonLatime / 8, butonCumY + butonInaltime / 4, "Reguli");
    setbkcolor(BLACK);
}

// Funcție pentru desenarea submeniului "reguli"
void deseneazaSubmeniuCumSeJoaca(int latime, int inaltime, int butonLatime, int butonInaltime) {
    cleardevice();
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(100, 100, "Cum se joaca:");
    outtextxy(100, 140, "1. Alege un mod de joc.");
    outtextxy(100, 180, "2. Activezi culorile cercurilor cu click dreapta.");
    outtextxy(100, 220, "3. Desenezi cercuri cu click stanga.");
    outtextxy(100, 260, "4. Poti desena cu click stanga celulele adiacente cu cercul activat sau celule deja desenate de aceeasi culoare.");
    outtextxy(100, 300, "5. Obiectivul este sa completezi matricea.");


    int butonInapoiX = (latime - butonLatime) / 2;
    int butonInapoiY = (inaltime - butonInaltime) / 2 + 120;

    setfillstyle(SOLID_FILL, BLUE);
    bar(butonInapoiX, butonInapoiY, butonInapoiX + butonLatime, butonInapoiY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonInapoiX, butonInapoiY, butonInapoiX + butonLatime, butonInapoiY + butonInaltime);
    setbkcolor(BLUE);
    outtextxy(butonInapoiX + butonLatime / 4, butonInapoiY + butonInaltime / 4, "Inapoi");
    setbkcolor(BLACK);
}



int afiseazaMeniu(int latime, int inaltime) {
    int butonLatime = 200;
    int butonInaltime = 100;


    int butonStartX = (latime - butonLatime) / 2;
    int butonStartY = (inaltime - butonInaltime) / 2 - 60;


    int butonCumX = (latime - butonLatime) / 2;
    int butonCumY = (inaltime - butonInaltime) / 2 + 60;


    deseneazaMeniuPrincipal(latime, inaltime, butonLatime, butonInaltime, butonStartX, butonStartY, butonCumX, butonCumY);

    while (true) { // Bucla principală pentru interacțiunea cu meniul
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX, mouseY;
            getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);


            if (mouseX >= butonStartX && mouseX <= butonStartX + butonLatime &&
                mouseY >= butonStartY && mouseY <= butonStartY + butonInaltime) {
                clearmouseclick(WM_LBUTTONDOWN);
                cleardevice();
                    setfillstyle(SOLID_FILL, BLUE);
                bar(butonStartX, butonStartY, butonStartX + butonLatime, butonStartY + butonInaltime);
                setcolor(WHITE);
                rectangle(butonStartX, butonStartY, butonStartX + butonLatime, butonStartY + butonInaltime);
                setbkcolor(BLUE);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
                outtextxy(butonStartX + butonLatime / 4+25, butonStartY + butonInaltime / 4+10, "Start");
                setbkcolor(BLACK);


                setfillstyle(SOLID_FILL, BLUE);
                bar(butonCumX, butonCumY, butonCumX + butonLatime, butonCumY + butonInaltime);
                setcolor(WHITE);
                rectangle(butonCumX, butonCumY, butonCumX + butonLatime, butonCumY + butonInaltime);
                setbkcolor(BLUE);
                outtextxy(butonCumX + butonLatime / 8+45, butonCumY + butonInaltime / 4+10, "Reguli");
                setbkcolor(BLACK);
                delay(3000);
                cleardevice();
                return 1;
            }

            // Verificăm dacă utilizatorul a apăsat butonul reguli
            if (mouseX >= butonCumX && mouseX <= butonCumX + butonLatime &&
                mouseY >= butonCumY && mouseY <= butonCumY + butonInaltime) {
                clearmouseclick(WM_LBUTTONDOWN);
                deseneazaSubmeniuCumSeJoaca(latime, inaltime, butonLatime, butonInaltime);

                while (true) { // Bucla submeniului
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        int backX, backY;
                        getmouseclick(WM_LBUTTONDOWN, backX, backY);


                        int butonInapoiX = (latime - butonLatime) / 2;
                        int butonInapoiY = (inaltime - butonInaltime) / 2 + 120;


                        if (backX >= butonInapoiX && backX <= butonInapoiX + butonLatime &&
                            backY >= butonInapoiY && backY <= butonInapoiY + butonInaltime) {
                            clearmouseclick(WM_LBUTTONDOWN);
                            cleardevice();
                            deseneazaMeniuPrincipal(latime, inaltime, butonLatime, butonInaltime, butonStartX, butonStartY, butonCumX, butonCumY);
                            break; // Revenim la meniul principal
                        }
                    }
                }
            }

            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}

// Funcție pentru selecția modului de joc
int selecteazaModJoc(int latime, int inaltime, int dimensiuneCelula) {
    int butonLatime = 200;
    int butonInaltime = 100;

    // Calculăm dimensiunile și pozițiile necesare
    int dimensiuneMatrice = dimensiuneCelula * 8;
    int startX = (latime - dimensiuneMatrice) / 2;
    int startY = (inaltime - dimensiuneMatrice) / 2;

    int butonRandomX = (latime - butonLatime) / 2;
    int butonRandomY = (inaltime - butonInaltime) / 2 - 120;

    int butonCustomX = (latime - butonLatime) / 2;
    int butonCustomY = (inaltime - butonInaltime) / 2;

    int butonNiveleX = (latime - butonLatime) / 2;
    int butonNiveleY = (inaltime - butonInaltime) / 2 + 120;

    // Desenăm butoanele pentru modurile de joc
    setfillstyle(SOLID_FILL, BLUE);
    bar(butonRandomX, butonRandomY, butonRandomX + butonLatime, butonRandomY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonRandomX, butonRandomY, butonRandomX + butonLatime, butonRandomY + butonInaltime);
    outtextxy(butonRandomX + 40, butonRandomY + 30, "Random");

    setfillstyle(SOLID_FILL, BLUE);
    bar(butonCustomX, butonCustomY, butonCustomX + butonLatime, butonCustomY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonCustomX, butonCustomY, butonCustomX + butonLatime, butonCustomY + butonInaltime);
    outtextxy(butonCustomX + 40, butonCustomY + 30, "Custom");

    setfillstyle(SOLID_FILL, BLUE);
    bar(butonNiveleX, butonNiveleY, butonNiveleX + butonLatime, butonNiveleY + butonInaltime);
    setcolor(WHITE);
    rectangle(butonNiveleX, butonNiveleY, butonNiveleX + butonLatime, butonNiveleY + butonInaltime);
    outtextxy(butonNiveleX + 40, butonNiveleY + 30, "Nivele");

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= butonRandomX && x <= butonRandomX + butonLatime &&
                y >= butonRandomY && y <= butonRandomY + butonInaltime) {
                clearmouseclick(WM_LBUTTONDOWN);
                cleardevice();
                return 1; // Modul Random
            }

            if (x >= butonCustomX && x <= butonCustomX + butonLatime &&
                y >= butonCustomY && y <= butonCustomY + butonInaltime) {
                clearmouseclick(WM_LBUTTONDOWN);
                cleardevice();
                return 2; // Modul Custom
            }

            if (x >= butonNiveleX && x <= butonNiveleX + butonLatime &&
                y >= butonNiveleY && y <= butonNiveleY + butonInaltime) {
                clearmouseclick(WM_LBUTTONDOWN);
                cleardevice();
                selecteazaNivel(latime, inaltime, startX, startY, dimensiuneCelula);
                cleardevice();
                return 0; // Înapoi la meniul principal
            }

            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}


int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TDM-GCC-32");


    int latime = GetSystemMetrics(SM_CXSCREEN);
    int inaltime = GetSystemMetrics(SM_CYSCREEN);
    int dimensiuneCelula = 60;
    int dimensiuneMatrice = dimensiuneCelula * 8;
    int startX = (latime - dimensiuneMatrice) / 2;
    int startY = (inaltime - dimensiuneMatrice) / 2;

    initwindow(latime, inaltime, "Fullscreen");

    while (true) {
        int optiuneMeniu = afiseazaMeniu(latime, inaltime);

        if (optiuneMeniu == 1) {
            int modJoc = selecteazaModJoc(latime, inaltime, dimensiuneCelula);

            if (modJoc == 1) { // Modul Random
                deseneazaMatrice(startX, startY, dimensiuneCelula);
                genereazaRandom(dimensiuneCelula, startX, startY);
            } else if (modJoc == 2) { // Modul Custom
                deseneazaMatrice(startX, startY, dimensiuneCelula);
                click(startX, startY, dimensiuneCelula);
            }
        }
        cleardevice();
    }

    closegraph();
    return 0;
}
