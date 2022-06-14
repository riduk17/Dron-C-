#ifndef Z5_DRON_HH
#define Z5_DRON_HH
#include "Wektor.hh"
#include "Prostokat.hh"
#include "dysk.hh"
#include <vector>


///klasa definiujaca drona
class Droniak{
private:
    ///"cialo" drona ktore jest graniastoslupem
    Prostokat<3> Graniastoslup;
    ///imitacja wirnikow drona ktore sa dyskami a w zasadzie 60-katami foremnymi
    Prostokat<3> wirniki[4];
    ///parametry drona
    double x, y, kierunek_lotu = 0;


public:
    ///domyslny konstruktor ustawiajacy drona w punkcie startowym
    Droniak(double a, double b);

    ///zwraca graniastoslup - cialo drona
    const Prostokat<3> &getGraniastoslup() const;

    ///zwraca wirniki drona
    const Prostokat<3> &getWirniki(int i) const;

    ///funkcja realizujaca przelot, przyjmuje x oraz y punktu docelowego
    void lec(double a, double b);


    //void xd(double x, double y, double z);

    ///obraca wirniki
    void obroty_wirnika();

    ///start drona
    void startuj(double h);

    ///destruktor
    ~Droniak();

    ///obraca drona
    void obroty_dronow_niebieskich(double alpha);

    ///zwraca x drona
    double getX() const;

    ///ustawia x drona
    void setX(double x);

    ///zwraca y drona
    double getY() const;

    ///ustawia y drona
    void setY(double y);

    ///zwraca kierunek lotu drona
    double getKierunekLotu() const;

    ///ustawia kierunek lotu drona
    void setKierunekLotu(double kierunekLotu);

    ///zwraca promien drona
    double promienDroniaka();
};

#endif //Z5_DRON_HH
