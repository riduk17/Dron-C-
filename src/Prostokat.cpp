//#include "Prostokat.hh"
//#include "Wektor.hh"
//#include "Macierz2x2.hh"
//#include <cmath>


//using namespace std;


//const Wektor2D &Prostokat::getA() const {
//    return a;
//}
//
//const Wektor2D &Prostokat::getB() const {
//    return b;
//}
//
//const Wektor2D &Prostokat::getC() const {
//    return c;
//}
//
//const Wektor2D &Prostokat::getD(){
//    this->d = (a+c-b);
//    return d;
//}

//Prostokat::Prostokat(const Wektor2D &a, const Wektor2D &b, const Wektor2D &c, const Wektor2D &d) : a(a), b(b), c(c),
//                                                                                                   d(d) {}

//Prostokat Prostokat::obroty_sfer_niebieskich(double alpha){
//
//    Macierz2x2 obrotow;
//    obrotow.set(0, 0, cos(alpha));
//    obrotow.set(0, 1, ((-1)*sin(alpha)));
//    obrotow.set(1, 0, sin(alpha));
//    obrotow.set(1, 1, cos(alpha));
//    Prostokat Noweczka(obrotow*a, obrotow*b, obrotow*c, obrotow*d);
//
//    return Noweczka;
//}

//Prostokat Prostokat::przesun_wektor (Wektor2D V){
//    Prostokat jeszcze_nowsza(a+V,b+V,c+V, d+V);
//    return jeszcze_nowsza;
//}

//std::ostream& operator << (std::ostream &Strm, Prostokat P){
//    Strm << P.getA() << endl << P.getB() << endl << P.getC() << endl << P.getD() << endl;
//    return Strm;
//}

//std::vector<drawNS::Point2D> Prostokat::wez_wierzcholki_2D() const{
//    std::vector<drawNS::Point2D> V;
//    V.push_back(drawNS::Point2D(a.get(0), a.get(1)));
//    V.push_back(drawNS::Point2D(b.get(0), b.get(1)));
//    V.push_back(drawNS::Point2D(c.get(0), c.get(1)));
//    V.push_back(drawNS::Point2D(d.get(0), d.get(1)));
//    V.push_back(drawNS::Point2D(a.get(0), a.get(1)));
//    return V;
//}