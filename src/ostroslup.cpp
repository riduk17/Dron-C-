#include "ostroslup.hh"
#include <cmath>

using namespace std;
const int promien = 3;

std::vector<drawNS::Point3D> Ostroslup::wez_wierzcholki_3D() const{
    vector<drawNS::Point3D> V;
    drawNS::Point3D O(this->x, this->y, 20);
    for (int i = 0; i < (ilosc_wierzcholkow- 1); i++) {
        double alpha = (((double) i / (ilosc_wierzcholkow - 1)) * (2 * M_PI));
        double alpha2 = (((double) (i+1) / (ilosc_wierzcholkow - 1)) * (2 * M_PI));
        V.push_back(drawNS::Point3D(cos(alpha) * (promien + (ilosc_wierzcholkow - 1)) + this->x,
                                    sin(alpha) * (promien + (ilosc_wierzcholkow - 1)) + this->y, 0 + 0));

        V.push_back(drawNS::Point3D(cos(alpha2) * (promien + (ilosc_wierzcholkow - 1)) + this->x,
                                    sin(alpha2) * (promien + (ilosc_wierzcholkow - 1)) + this->y, 0 + 0));

        V.push_back(drawNS::Point3D(cos(alpha) * (promien + (ilosc_wierzcholkow - 1)) + this->x,
                                    sin(alpha) * (promien + (ilosc_wierzcholkow - 1)) + this->y, 0 + 0));

        V.push_back(O);
    }
    return V;
}

Ostroslup::Ostroslup(int iloscWierzcholkow, double x, double y) : ilosc_wierzcholkow(iloscWierzcholkow), x(x), y(y) {}

 double Ostroslup::zwroc_promien_zewnetrzny() const{

    return (promien + this->ilosc_wierzcholkow - 1);
}

double Ostroslup::zwroc_promien_wewnetrzny() const{

    return 0;
}

double Ostroslup::zwroc_x_srodka() const{
    return this->x;
}

double Ostroslup::zwroc_y_srodka() const {
    return this->y;
}
