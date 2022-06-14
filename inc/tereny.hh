#ifndef Z5_TERENY_H
#define Z5_TERENY_H

#include "ostroslup.hh"
#include <random>
#include <vector>
#include "Prostokat.hh"
#include "Draw3D_api_interface.hh"
#include "dron.hh"
#include "Brylka.hh"

using namespace std;

class Teren{
private:

//vector<Ostroslup> Ostroslupy;
//vector<Prostokat<3>> Graniastoslupy;
vector<Droniak> Droniarze;
vector<Brylka*> Bryly;

public:
    ///zwraca vector z dronami
    vector<Droniak> &getDroniarze();

    ///zwraca wiercholki wszystkich obiektow terenu dla metod rysujacych
    vector<drawNS::Point3D> zwroc_wszystkie_wierzcholki();

    ///dodaje el terenu
    void dodaj_smaczek();

    ///dodaje byka droniarza
    void dodaj_byka_droniarza();

    ///niszczy gory bo jest potezna
    void wysadz_gore(int i);

    ///zniszcz drona
    void dokonaj_aborcji_drona(int i);

    ///zwraca ilosc obiektow terenu
    int ile_jest_gor();

    ///funkcja sprawdza czy dron moze ladowac w pozycji drona
    bool czy_droniak_moze_ladowac(Droniak Pan_Dron);

    ///funkcja sprawdza czy dron moze ladowac w zadanym x, y
    bool czy_droniak_moze_ladowac(double x, double y, Droniak Pan_Dron);

    ///funkcja zwraca wysokosc teren pod dronem w x i y
    double jak_duzy_jestes(double x, double y);
};

#endif //Z5_TERENY_H
