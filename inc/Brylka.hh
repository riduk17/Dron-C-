#ifndef Z5_BRYLKA_H
#define Z5_BRYLKA_H
#include "Draw3D_api_interface.hh"
#include "OpenGL_API.hh"

///klasa brylka po ktorej dziedzicza klasy ostroslup i prostokat
class Brylka{
private:


public:
    ///funkcja zwraca wierzcholki bryly
    virtual std::vector<drawNS::Point3D> wez_wierzcholki_3D() const = 0;
    ///funkcja zwraca promien zewnetrzny bryly
    virtual double zwroc_promien_zewnetrzny() const = 0;
    ///funkcja zwraca promien wewnetrzny bryly
    virtual double zwroc_promien_wewnetrzny() const = 0;
    ///funkcja zwraca parametr x srodka bryly
    virtual double zwroc_x_srodka() const = 0;
    ///funkcja zwraca parametr y srodka bryly
    virtual double zwroc_y_srodka() const = 0;
};

#endif
