#ifndef Z5_OSTROSLUP_H
#define Z5_OSTROSLUP_H

#include "Wektor.hh"
#include "Draw3D_api_interface.hh"
#include <vector>
#include "Brylka.hh"

using namespace std;

class Ostroslup : public Brylka{
private:
int ilosc_wierzcholkow;
double x,y;

public:

    ///domyslny konstruktor ostroslupa
    Ostroslup(int iloscWierzcholkow, double x, double y);

    ///zwraca wierzcholki dla metod rysujacych
    vector<drawNS::Point3D> wez_wierzcholki_3D() const;

    double zwroc_promien_zewnetrzny() const;

    double zwroc_promien_wewnetrzny() const;

    double zwroc_x_srodka() const;

    double zwroc_y_srodka() const;
};

#endif //Z5_OSTROSLUP_H
