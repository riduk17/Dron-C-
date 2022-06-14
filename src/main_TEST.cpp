#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "Wektor.hh"
#include "Prostokat.hh"
#include "Dr3D_gnuplot_api.hh"


using namespace std;
using namespace drawNS;

void wait4key() {
    do {
        std::cout << "\n Nacisnij klawisz by kontynuowac..." << std::endl;
    } while(std::cin.get() != '\n');
    do {
    } while(std::cin.get() != '\n');
}

void menu() {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~ MENU ~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "1) Wyswietl bryle(fioletowy)" << endl;
    cout << "2) Obroc bryle(czerwony)" << endl;
    cout << "3) Przesun bryle o wektor(niebieski)" << endl;
    cout << "4) Wypisz wierzcholki naszej bryly" << endl;
    cout << "5) Powtorz poprzedni obrot" << endl;
    cout << "6) Wypisz macierz obrotow" << endl;
    cout << "7) Zakoncz program :(" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}


int main() {
    Wektor3D a; // inicjalizacja zerami
    std::shared_ptr<drawNS::Draw3DAPI> api(new drawNS::APIGnuPlot3D(-10,10,-10,10,-10,10,1000));
    api->change_ref_time_ms(
            0); //odświeżanie sceny zmienione na opcję "z każdym pojawieniem się lub zniknięciem kształtu"
    cout << endl << "Podaj wierzcholki bryly zaczynajac od dolnej podstawy, lewego dolnego wierzcholka i idac przeciwnie do kierynku wskazowek zegara" << endl;
    Prostokat<3> Pr;
    for(int i=0; i<8; i++){
        cin >> a;
        Pr.set(i, a);
    }
    double alpha; /*! kat w stopniach */
    char Os;
    int x;
    bool p = false;
    menu();
    while (true){
        cin >> x;
        switch (x) {
            case 1:{
                for(int i=0; i < 24; i+=2) {
                    api->draw_line(Pr.wez_wierzcholki_3D()[i], Pr.wez_wierzcholki_3D()[i+1], "purple"); //rysuje fioletowy prostokat
                }
                wait4key();
                menu();
                break;
            }
            case 2:{
                cout << "Obrocmy ten prostokat! By to zrobic niezbedny jest nam kat w stopniach oraz os obrotu(do wyboru X, Y, Z), prosze podaj mi je:" << endl;
                cin >> alpha;
                cin >> Os;
                alpha = (alpha/180) * 3.14;
                for(int i=0; i < 24; i+=2) {
                    api->draw_line(Pr.obroty_sfer_niebieskich3D(alpha, Os).wez_wierzcholki_3D()[i], Pr.obroty_sfer_niebieskich3D(alpha, Os).wez_wierzcholki_3D()[i+1] , "red");
                }
                Pr = Pr.obroty_sfer_niebieskich3D(alpha, Os);
                wait4key();
                menu();
                break;
            }
            case 3:{
                cout << "Teraz przesuniemy sobie o wektor bo czemu nie? Podaj prosze wspolrzedne: " << endl;
                Wektor3D W;
                cin >> W;
                for(int i=0; i < 24; i+=2) {
                    api->draw_line(Pr.przesun_wektor(W).wez_wierzcholki_3D()[i], Pr.przesun_wektor(W).wez_wierzcholki_3D()[i+1], "blue");
                }
                Pr = Pr.przesun_wektor(W);
                wait4key();
                menu();
                break;
            }
            case 4:{
                cout << "wiercholki prostokata: " << endl;
                cout << Pr << endl;
                menu();
                break;
            }
            case 5:{
                for(int i=0; i < 24; i+=2) {
                    api->draw_line(Pr.obroty_sfer_niebieskich3D(alpha, Os).wez_wierzcholki_3D()[i], Pr.obroty_sfer_niebieskich3D(alpha, Os).wez_wierzcholki_3D()[i+1] , "red");
                }
                Pr = Pr.obroty_sfer_niebieskich3D(alpha, Os);
                wait4key();
                menu();
                break;
            }
            case 6:{
                Macierz3x3 M= Pr.wez_macierz_obrotow(alpha, Os);
                cout << M << endl;
                wait4key();
                menu();
                break;
            }
            case 7:{
                p = true;
                break;
            }
            default:{
                menu();
                break;
            }

        }
        if(p == 1){break;}
    }
    return 0;
}
