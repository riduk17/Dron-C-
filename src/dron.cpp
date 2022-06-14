#include "dron.hh"
#include "assert.h"

Droniak::Droniak(double a, double b){
   this->Graniastoslup = Prostokat<3> (10, 2, 7);
   Wektor3D V;
   V.set(0, a);
   V.set(1, b);
    this->x = a;
    this->y = b;
    this->Graniastoslup.setSrodek(V);
   for(int i=0; i<4; i++){
       this->wirniki[i] = Prostokat<3> (1.5,0.5,6);
       this->wirniki[i].setSrodek(Graniastoslup.get((4 + i)));
       //cout << "i: "<< i << "   " << wirniki[i] << endl; //debug
   }
}

//void Droniak::xd(double x, double y, double z){
//    Wektor3D  V;
//
//
//    V.set(0,0 + x); // 1
//    V.set(1,0 + y);
//    V.set(2,0 + z);
//    this->Graniastoslup.set(0, V);
//
//
//    V.set(0,10 + x); // 2
//    V.set(1,0 + y);
//    V.set(2,0 + z);
//    this->Graniastoslup.set(1, V);
//
//
//    V.set(0,10 + x); // 3
//    V.set(1,7 + y);
//    V.set(2,0 + z);
//    this->Graniastoslup.set(2, V);
//
//
//    V.set(0,0 + x); // 4
//    V.set(1,7 + y);
//    V.set(2,0 + z);
//    this->Graniastoslup.set(3, V);
//
//
//    V.set(0,0 + x);
//    V.set(1,0 + y);
//    V.set(2,2 + z);
//    this->Graniastoslup.set(4, V);
//
//
//    wirniki[0].setSrodek(V);
//    V.set(0,10 + x);
//    V.set(1,0 + y);
//    V.set(2,2 + z);
//    this->Graniastoslup.set(5, V);
//
//
//    wirniki[1].setSrodek(V);
//    V.set(0,10 + x);
//    V.set(1,7 + y);
//    V.set(2,2 + z);
//    this->Graniastoslup.set(6, V);
//
//
//    wirniki[2].setSrodek(V);
//    V.set(0,0 + x);
//    V.set(1,7 + y);
//    V.set(2,2 + z);
//    this->Graniastoslup.set(7, V);
//    wirniki[3].setSrodek(V);
//}

void Droniak::startuj(double h){
    Wektor3D X;
    X.set(0, Graniastoslup.getSrodek().get(0));
    X.set(1, Graniastoslup.getSrodek().get(1));
    X.set(2, h);
    Graniastoslup.setSrodek(X);
    for(int i=0; i<4; i++){
        this->wirniki[i].setSrodek(Graniastoslup.get((4 + i)));
    }
}

void Droniak::obroty_wirnika(){
    Wektor3D V;
    for(int i=0; i<4; i++){
        this->wirniki[i].setSrodek(V);
        this->wirniki[i] = this->wirniki[i].obroty_sfer_niebieskich3D((M_PI)/4.0, 'Z');
        this->wirniki[i].setSrodek(Graniastoslup.get((4 + i)));
    }
}

void Droniak::obroty_dronow_niebieskich(double alpha){
    Wektor3D S = Graniastoslup.getSrodek();
    Wektor3D V;
    Graniastoslup.setSrodek(V);
    this->Graniastoslup = this->Graniastoslup.obroty_sfer_niebieskich3D(alpha, 'Z');
    Graniastoslup.setSrodek(S);
    for(int i=0; i<4; i++){
        this->wirniki[i].setSrodek(Graniastoslup.get((4 + i)));
    }
}

void Droniak::lec(double a, double b){
    Wektor3D X;
    X.set(0, a);
    X.set(1, b);
    X.set(2, Graniastoslup.getSrodek().get(2));
    Graniastoslup.setSrodek(X);
    for(int i=0; i<4; i++){
        this->wirniki[i].setSrodek(Graniastoslup.get((4 + i)));
        //cout << "i: "<< i << "   " << wirniki[i] << endl; //debug
    }
}

const Prostokat<3> &Droniak::getGraniastoslup() const {
    return Graniastoslup;
}

 const Prostokat<3> &Droniak::getWirniki(int i) const {
    assert(i<4);
    return wirniki[i];
}


double Droniak::getX() const {
    return x;
}

void Droniak::setX(double x) {
    Droniak::x = x;
}

double Droniak::getY() const {
    return y;
}

void Droniak::setY(double y) {
    Droniak::y = y;
}

double Droniak::getKierunekLotu() const {
    return kierunek_lotu;
}

void Droniak::setKierunekLotu(double kierunekLotu) {
    kierunek_lotu = kierunekLotu;
}

double Droniak::promienDroniaka() {
    return ((sqrt(149)/2) + (sqrt(36 + 1.5*1.5)/2));
}

Droniak::~Droniak() {

}
