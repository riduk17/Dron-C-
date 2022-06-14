#include "tereny.hh"
#include <cstdlib>
#include "Prostokat.hh"

double losowy_numer(double min, double max){
    return ((max - min) * ((double)rand() / RAND_MAX)) + min;
}

vector<drawNS::Point3D> Teren::zwroc_wszystkie_wierzcholki(){
    vector<drawNS::Point3D> V;

    for(int i=0; i<this->Bryly.size(); i++){
        const vector<drawNS::Point3D> &d = this->Bryly[i]->wez_wierzcholki_3D();
        V.insert(V.end(), d.begin(), d.end());
    }

//    for(int i=0; i<this->Bryly.size(); i++){
//        const vector<drawNS::Point3D> &wierzcholki = this->Bryly[i]->wez_wierzcholki_3D();
//        V.insert(V.end(), wierzcholki.begin(), wierzcholki.end());
//    }

    return V;
}

void Teren::dodaj_smaczek(){
   int ksztalt = rand() % 2; // 0 = graniastoslup   1 = ostroslup
   int x = losowy_numer(-90, 90);
   int y = losowy_numer(-90, 90);
   Wektor3D W;
   W.set(0, x);
   W.set(1, y);
   if(ksztalt==0){
       int w = losowy_numer(5, 20);
       int h = losowy_numer(1, 10);
       int d = losowy_numer(5, 25);
       Prostokat<3> *P = new Prostokat<3>(w, h, d);
       P->setSrodek(W);
       Bryly.push_back(P);
   }
   else{
       int ilosc_w = losowy_numer(4, 20);
       Ostroslup *O = new Ostroslup(ilosc_w, x, y);
       Bryly.push_back(O);
   }
}

void Teren::dodaj_byka_droniarza(){
    int a = losowy_numer(-90, 90);
    int b = losowy_numer(-90, 90);
    Droniak D(a, b);
    Droniarze.push_back(D);
}

vector<Droniak> &Teren::getDroniarze() {
    return Droniarze;
}

void Teren::wysadz_gore(int i){
    this->Bryly.erase(Bryly.begin() + i);
}

void Teren::dokonaj_aborcji_drona(int i){
    this->Droniarze.erase(Droniarze.begin() + i);
}

int Teren::ile_jest_gor(){
    int i = Bryly.size();
    return i;
}

bool Teren::czy_droniak_moze_ladowac(Droniak Pan_Dron){
    double odl;
    for(int i=0; i<this->Bryly.size(); i++){
        odl = sqrt(pow((Bryly[i]->zwroc_x_srodka() - Pan_Dron.getX()),2) + pow((Bryly[i]->zwroc_y_srodka() - Pan_Dron.getY()),2));
        if(odl + Pan_Dron.promienDroniaka() > Bryly[i]->zwroc_promien_wewnetrzny() && odl < Pan_Dron.promienDroniaka() + Bryly[i]->zwroc_promien_zewnetrzny()){return false;}
    }
    return true;
}

bool Teren::czy_droniak_moze_ladowac(double x, double y, Droniak Pan_Dron){
    double odl;
    for(int i=0; i<this->Bryly.size(); i++){
        odl = sqrt(pow((Bryly[i]->zwroc_x_srodka() - x),2) + pow((Bryly[i]->zwroc_y_srodka() - y),2));
        if(odl + Pan_Dron.promienDroniaka() > Bryly[i]->zwroc_promien_wewnetrzny() && odl < Pan_Dron.promienDroniaka() + Bryly[i]->zwroc_promien_zewnetrzny()){return false;}
    }
    return true;
}

double Teren::jak_duzy_jestes(double x, double y) {
    double odl;
    for(int i=0; i<this->Bryly.size(); i++){
        odl = sqrt(pow((Bryly[i]->zwroc_x_srodka() - x),2) + pow((Bryly[i]->zwroc_y_srodka() - y),2));
        if(odl <= Bryly[i]->zwroc_promien_zewnetrzny()){
            if(Prostokat<3>* v = dynamic_cast<Prostokat<3>*>(Bryly[i])){
                return v->getH();
            }
        }
    }
    return 0;
}
