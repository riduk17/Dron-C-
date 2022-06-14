#ifndef WEKTOR_HH
#define WEKTOR_HH

#include <iostream>

template <unsigned int n>
class Wektor{
private:
    double tab[n];
   inline static uint ile_stworzono;
   inline static uint ile_jest;
public:
    double get(int i) const {
        return tab[i];
    }
    void set(int i, double x) {
        tab[i] = x;
    }
    double operator [] (int x) const{
        if(x>=n){
            std::cout << "wykraczasz poza wektor byq" << std::endl;
        }
        return this->tab[x];
    }
    Wektor() : tab{0} {ile_stworzono++; ile_jest++;}

    Wektor(const Wektor<n> & stary){
        for (int i = 0; i < n; i++) {
            this->tab[i] = stary.tab[i];
        }
        ile_stworzono++;
        ile_jest++;
    }

    static uint getIleStworzono() {
        return ile_stworzono;
    }

    static uint getIleJest() {
        return ile_jest;
    }

    Wektor<n> operator + (Wektor<n> V) const {
        Wektor<n> suma;
        for (int i = 0; i < n; i++) {
            suma.set(i, this->get(i) + V.get(i));
        }
        return suma;
    }


    Wektor<n> operator - (Wektor<n> V) const{
        Wektor<n> T{};
        for(int i=0; i<n; i++){
            T.set(i, (*this)[i] - V[i]);
        }
        return T;
    }


    double operator * (Wektor<n> V) const{
        double wynik = 0;
        for (int i=0; i<n; i++){
            wynik += (*this)[i] * V[i];
        }
        return wynik;
    }

    ~Wektor(){ile_jest--;}
};

template <unsigned int n>
std::istream& operator >> (std::istream &Strm, Wektor<n> &W){
    double x;
    for(int i=0; i<n; i++){
        Strm >> x;
        W.set(i, x);
    }
    return Strm;
}

template <unsigned int n>
std::ostream& operator << (std::ostream &Strm, Wektor<n> W){
    Strm << "[";
    for(int i=0; i<n; i++){
        if(i<(n-1)) {
            Strm << W.get(i) << ", ";
        }
        else{
            Strm << W.get(i);
        }
    }
    Strm << "]";
    return Strm;
}

typedef Wektor<2> Wektor2D;
typedef Wektor<3> Wektor3D;

/*class Wektor2D {
private:
    double x;
    double y;

public:
    double getX() const {
        return x;
    }

    void setX(double _x) {
        Wektor2D::x = _x;
    }

    double getY() const {
        return y;
    }

    void setY(double _y) {
        Wektor2D::y = _y;
    }
    double operator [] (int n) const;*/

    //Wektor2D(double _x, double _y);
    /*Wektor2D(): x(0),y(0){

    }*/
    /*Wektor2D operator + (Wektor2D V) const;
    Wektor2D operator - (Wektor2D V) const;
    double operator * (Wektor2D V) const;
};*/



#endif
