#ifndef Z5_DYSK_HH
#define Z5_DYSK_HH
#include "Wektor.hh"
#include <iostream>
#include <vector>
#include "Draw3D_api_interface.hh"
#include "OpenGL_API.hh"

using namespace std;

class dysk {
private:
    /// klasa dysk opisywana jest srodkiem okregu w dolnej podstawie dysku,
    Wektor3D srodek;
    /// promieniem okregow
    double promien = 2.5;
    /// oraz wysokoscia(gruboscia) dysku
    double wysokosc = 0.5;
public:
    dysk();
    /// standardowy konstruktor klasy dysk
    dysk(const Wektor3D &srodek);

    /// fukcja wierzcholki wyliczajaca i zwracajaca wierzcholki dolnego okregu ktory jest tak na prawde 60 katem foremnym
    std::vector<drawNS::Point3D> wierzcholki_dol();

    /// fukcja wierzcholki wyliczajaca i zwracajaca wierzcholki gornego okregu ktory jest tak na prawde 60 katem foremnym
    std::vector<drawNS::Point3D> wierzcholki_gora();

    void setSrodek(const Wektor3D &s);



};


#endif //Z5_DYSK_HH
