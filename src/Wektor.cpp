#include "Wektor.hh"

/*template <unsigned int n>
double Wektor<n>::operator [] (int a) const{
    if(a>=n){
        std::cout << "Normalny jestes?" << std::endl;
    }
    return this->tab[a];
}*/

/*template <unsigned int n>
Wektor<n> Wektor<n>::operator + (Wektor<n> V) const{
    Wektor T{};
    for(int i=0; i<n; i++){
        T.set(i, (*this)[i] + V[i]);
    }
    return T;
}*/
/*template <unsigned int n>
Wektor<n> Wektor<n>::operator - (Wektor V) const{
    Wektor T{};
    for(int i=0; i<n; i++){
        T.set(i, (*this)[i] - V[i]);
    }
    return T;
}*/

/*template <unsigned int n>
double Wektor<n>::operator * (Wektor V) const{
    double wynik = 0;
    for (int i=0; i<n; i++){
        wynik += (*this)[i] * V[i];
    }
    return wynik;
}*/



/*template <unsigned int n>
std::ostream& operator << (std::ostream &Strm, Wektor<n> W){
    Strm << "[";
    for(int i=0; i<n; i++){
        Strm << W.get(i) << ", ";
    }
    Strm << "]";
    return Strm;
}*/

/*
template <unsigned int n>
std::istream& operator >> (std::istream &Strm, Wektor<n> &W){
    double x;
    for(int i=0; i<n; i++){
        Strm >> x;
        W.set(i, x);
    }
    return Strm;
}*/
