#include "dysk.hh"
#include <iostream>
#include <cmath>

using namespace std;

#define katy 60

std::vector<drawNS::Point3D> dysk::wierzcholki_dol() {
    vector<drawNS::Point3D> V;
    for(int i=0; i <= katy; i++){
        double alpha = (((double)i/katy)*(2*M_PI));
        V.push_back(drawNS::Point3D(cos(alpha) * this->promien + this->srodek[0], sin(alpha) * this->promien + this->srodek[1], 0 + this->srodek[2]));
    }
    return V;
}

std::vector<drawNS::Point3D> dysk::wierzcholki_gora() {
    vector<drawNS::Point3D> V;
    for(int i=0; i <= katy; i++){
        double alpha = (((double)i/katy)*(2*M_PI));
        V.push_back(drawNS::Point3D(cos(alpha) * this->promien + this->srodek[0], sin(alpha) * this->promien + this->srodek[1], 0 + this->srodek[2] + this->wysokosc));
    }
    return V;
}


dysk::dysk(const Wektor3D &srodek) : srodek(srodek) {}

void dysk::setSrodek(const Wektor3D &s) {
    dysk::srodek = s;
}

dysk::dysk() {}
