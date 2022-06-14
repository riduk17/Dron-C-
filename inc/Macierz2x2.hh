#ifndef MACIERZ2X2_HH
#define MACIERZ2X2_HH


#include <iostream>
#include "Wektor.hh"


template<unsigned int n>
class Macierz {
private:
    double macierz[n][n];

public:
    double get(int w, int k) const {
        return macierz(w, k);
    }
    void set(int w, int k, double x) {
        this->macierz[w][k] = x;
    }

    Macierz() : macierz{0}{}

    Wektor<n> operator * (Wektor<n> W) const{
        Wektor<n> v;
        for (int i=0;i<n;i++){
            for (int j=0;j<n;j++){
                v.set(i, (v.get(i) + this->macierz[i][j]*W.get(j)));
            }
        }
        return v;
    }

    Macierz<n> operator * (Macierz<n> M) const{
        Macierz c;
        for (int w = 0; w < n; w++) {
            for (int k = 0; k < n; k++) {
                double suma = 0;

                for(int i = 0; i < n; i++) {
                    suma += this->macierz[w][i] * M.macierz[i][k];
                }

                c[w][k] = suma;
            }
        }
        return c;
    }

    double operator () (int w, int k) const{
        return this->macierz[w][k];
    }
};
template <unsigned int n>
std::ostream& operator << (std::ostream &Strm, const Macierz<n> &Mac){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            Strm << Mac(j, i) << "  ";
        }
        Strm << std::endl;
    }
    return Strm;
}


typedef Macierz<2> Macierz2x2;
typedef Macierz<3> Macierz3x3;
/*
 * To przeciazenie trzeba opisac. Co ono robi. Jaki format
 * danych akceptuje. Jakie jest znaczenie parametrow itd.
 * Szczegoly dotyczace zalecen realizacji opisow mozna
 * znalezc w pliku:
 *    ~bk/edu/kpo/zalecenia.txt 
 *
 * Przeciążenie to może być użyteczne w trakcie debugowania programu.
 */


#endif
