#ifndef PROSTOKAT_HH
#define PROSTOKAT_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include "Wektor.hh"
#include "Macierz2x2.hh"
#include "Draw3D_api_interface.hh"
#include "Brylka.hh"


template<unsigned int n>
class Prostokat : public Brylka {
private:
    Wektor<n>* wierzcholki;
    double r, R, H;
//    Wektor2D a;
//    Wektor2D b;
//    Wektor2D c;
//    Wektor2D d;
public:
    ///Domyslny konstruktor klasy prostokat
    Prostokat(){
        wierzcholki = new Wektor<n>[(int)pow(2, n)];
    };

    ///konstruktor kopiujacy klasy prostokat
    Prostokat(const Prostokat& stary) {
        wierzcholki = new Wektor<n>[(int)pow(2, n)];

        for (int i = 0; i < (int)pow(2, n); i++) {
            wierzcholki[i] = stary.wierzcholki[i];
        }
    }

    ///konstruktor prostokata z trzema wartosciami width, height i depth
    Prostokat(double w, double h, double d){
        wierzcholki = new Wektor<n>[(int)pow(2, n)];
        if(w<d){this->r=w;}
        else{this->r=d;}
        H = h;
        this->R = sqrt(w*w + d*d)/2;
        Wektor3D  V;


        V.set(0,0 ); // 1
        V.set(1,0 );
        V.set(2,0 );
        this->wierzcholki[0] = V;


        V.set(0,w ); // 2
        V.set(1,0 );
        V.set(2,0 );
        this->wierzcholki[1] = V;


        V.set(0,w ); // 3
        V.set(1,d );
        V.set(2,0 );
        this->wierzcholki[2] = V;
        
        
        V.set(0,0 ); // 4
        V.set(1,d );
        V.set(2,0 );
        this->wierzcholki[3] = V;
        
        
        V.set(0,0 );
        V.set(1,0 );
        V.set(2,h );
        this->wierzcholki[4] = V;

        
        V.set(0,w );
        V.set(1,0 );
        V.set(2,h );
        this->wierzcholki[5] = V;

        
        V.set(0,w );
        V.set(1,d );
        V.set(2,h );
        this->wierzcholki[6] = V;

        
        V.set(0,0 );
        V.set(1,d );
        V.set(2,h );
        this->wierzcholki[7] = V;
        
    };
    ///getter
    Wektor<n> get(int i) const{
        return this->wierzcholki[i];
    }

    ///setter
    void set(int i, Wektor<n> W) {
        for (int s = 0; s < n; s++) {
            this->wierzcholki[i].set(s, W.get(s));
        }
    }
    ///metoda zwraca srodek prostokata
    Wektor3D getSrodek() const{
        Wektor3D X;
        X.set(0, (this->wierzcholki[0].get(0) + this->wierzcholki[2].get(0))/2);
        X.set(1, (this->wierzcholki[0].get(1) + this->wierzcholki[2].get(1))/2);
        X.set(2, this->wierzcholki[0].get(2));

        return X;
    }
    ///metoda ustawiajaca srodek protokata
    void setSrodek(Wektor3D srodek){
        Wektor3D T = srodek - this->getSrodek();

        for(int i=0; i < (int)pow(2, n); i++){
            wierzcholki[i] = wierzcholki[i] + T;
        }
    }

    ///przeciazenie operatora porowania
    Prostokat<n>& operator = (const Prostokat<n>& p) {
        this->wierzcholki = new Wektor<n>[(int)pow(2, n)];
        for (int i = 0; i < pow(2, n); i++){
            this->set(i, p.get(i));
        }
        return *this;
    }

//    const Wektor2D &getD();
//
//    const Wektor2D &getA() const;
//
//    const Wektor2D &getB() const;
//
//    const Wektor2D &getC() const;

    ///pozostalosc po obrotach 2d
    Prostokat<n> obroty_sfer_niebieskich2D(double alpha){
        assert(n==2);
        Macierz2x2 obrotow;
        obrotow.set(0, 0, cos(alpha));
        obrotow.set(0, 1, ((-1)*sin(alpha)));
        obrotow.set(1, 0, sin(alpha));
        obrotow.set(1, 1, cos(alpha));
        Prostokat Noweczka(obrotow*wierzcholki[0], obrotow*wierzcholki[1], obrotow*wierzcholki[2], obrotow*wierzcholki[3]);

        return Noweczka;
    }

    ///pozostalosc po obrotach 3d
    Macierz3x3 wez_macierz_obrotow(double alpha, char O){
        Macierz3x3 obrotowa;
        if(O=='X'){
            obrotowa.set(0,0, 1);
            obrotowa.set(0,1, 0);
            obrotowa.set(0,2, 0);
            obrotowa.set(1,0, 0);
            obrotowa.set(1,1, cos(alpha));
            obrotowa.set(1,2, (-1)*sin(alpha));
            obrotowa.set(2,0, 0);
            obrotowa.set(2,1, sin(alpha));
            obrotowa.set(2,2, cos(alpha));

            /*! |1    0      0   |
                |0    cos   -sin |
                |0    sin    cos | */
        }

        else if(O=='Y'){
            obrotowa.set(0,0, cos(alpha));
            obrotowa.set(0,1, 0);
            obrotowa.set(0,2, sin(alpha));
            obrotowa.set(1,0,0);
            obrotowa.set(1,1,1);
            obrotowa.set(1,2,0);
            obrotowa.set(2,0,(-1)*sin(alpha));
            obrotowa.set(2,1,0);
            obrotowa.set(2,2,cos(alpha));

            /*! |cos    0     sin |
                |0      1     0   |
                |-sin   0     cos | */
        }

        else if(O=='Z'){
            obrotowa.set(0,0, cos(alpha));
            obrotowa.set(0,1,(-1)*sin(alpha));
            obrotowa.set(0,2,0);
            obrotowa.set(1,0,sin(alpha));
            obrotowa.set(1,1, cos(alpha));
            obrotowa.set(1,2, 0);
            obrotowa.set(2,0,0);
            obrotowa.set(2,1,0);
            obrotowa.set(2,2,1);

            /*! |cos    -sin    0 |
                |sin     cos    0 |
                |0       0      1 | */
        }
        return obrotowa;
    }

    ///funkcja obracajaca graniastoslupy o zadany kat
    Prostokat<n> obroty_sfer_niebieskich3D(double alpha, char O){
        assert(n==3);
        Macierz3x3 obrotowa = wez_macierz_obrotow(alpha, O);
        Prostokat<3> nowy;
        for(int i=0; i < pow(2, n); i++){
            nowy.wierzcholki[i] = obrotowa * this->wierzcholki[i];
        }
        return nowy;

    }

    ///metoda sprawdza czy dlugosci bokow sa takie same
    bool sprawdz_boki(Prostokat<n> P){
        if(P.wierzcholki[0] - P.wierzcholki[1] != P.wierzcholki[3] - P.wierzcholki[2]){return false;}
        if(P.wierzcholki[4] - P.wierzcholki[5] != P.wierzcholki[7] - P.wierzcholki[6]){return false;}
        if(P.wierzcholki[0] - P.wierzcholki[4] != P.wierzcholki[1] - P.wierzcholki[5] || P.wierzcholki[0] - P.wierzcholki[1] != P.wierzcholki[3] - P.wierzcholki[7] || P.wierzcholki[0] - P.wierzcholki[1] != P.wierzcholki[2] - P.wierzcholki[6]){return false;}
    }
    ///przesuwanie prostokata o wektor
    Prostokat<n> przesun_wektor(Wektor3D V){
        Prostokat<n> jeszcze_nowsza;
        for (int s = 0; s < pow(2, n); s++) {
            jeszcze_nowsza.wierzcholki[s] = this->wierzcholki[s] + V;
        }
        return jeszcze_nowsza;
    }


    ///metoda zwraca wierzcholki dla metod rysujacych
    std::vector<drawNS::Point3D> wez_wierzcholki_3D() const{
        assert(n==3);
        std::vector<drawNS::Point3D> V;
        int W[] = {
                0, 1, 1, 2, 2, 3, 3, 0, // Dolna podstawa
                4, 5, 5, 6, 6, 7, 7, 4, // Górna podstawa
                0, 4, 1, 5, 2, 6, 3, 7  // Pionowe boki
        };

        for (int i : W) {
            V.push_back(drawNS::Point3D(wierzcholki[i].get(0), wierzcholki[i].get(1), wierzcholki[i].get(2)));
        }

        return V;
    }

    double zwroc_promien_zewnetrzny() const{
        return this->R;
    }

    double zwroc_promien_wewnetrzny() const{
        return this->r;
    }

    double zwroc_x_srodka() const{
        return this->getSrodek().get(0);
    }

    double zwroc_y_srodka() const {
        return this->getSrodek().get(1);
    }
    ///funkcja zwraca wysokosc bryly
    double getH() const {
        return H;
    }

    ///destruktor
    ~Prostokat() {delete[] wierzcholki;}
};

template <unsigned int n>
std::ostream& operator << (std::ostream &Strm, Prostokat<n> P){
    for (int s = 0; s < pow(2, n); s++) {
        Strm << P.get(s) << " ";
    }
    Strm << std::endl;
    return Strm;
}

#endif
