#include "Macierz2x2.hh"
#include "Wektor.hh"

/*double* Macierz2x2::operator [] (size_t i) const {
    return (double*)macierz[i];
}*/

/*Macierz2x2::Macierz2x2(double a, double b, double c, double d) :macierz { {a, b}, {c, d} } {

}*/


/*Wektor2D Macierz2x2::operator * (Wektor2D W) const{
    Wektor2D Z{};
    Z.set(0, (*this)[0][0]*W[0]+(*this)[0][1]*W[1]);
    Z.set(1, (*this)[1][0]*W[0]+(*this)[1][1]*W[1]);
    return Z;
}*/

/*Macierz2x2 Macierz2x2::operator * (Macierz2x2 M) const{
    Macierz2x2 c(0, 0, 0, 0);
    for (int w = 0; w < 2; w++) {
        for (int k = 0; k < 2; k++) {
            double suma = 0;

            for(int i = 0; i < 2; i++) {
                suma += this->macierz[w][i] * M.macierz[i][k];
            }

            c[w][k] = suma;
        }
    }
    return c;
}*/

