#include <iostream>
#include <cmath>
#include "Wektor.hh"
#include "Prostokat.hh"
#include "OpenGL_API.hh"
#include "dron.hh"
#include <vector>
#include "ostroslup.hh"
#include "tereny.hh"


using namespace std;
using namespace drawNS;



void wait4key() {
    do {
        std::cout << "\n Nacisnij klawisz by kontynuowac..." << std::endl;
    } while(std::cin.get() != '\n');
    do {
    } while(std::cin.get() != '\n');
}

void menu() {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~ MENU ~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "1) przemiesc drona" << endl;
    cout << "2) wypisz ilosc obecnie uzywanych wektorow oraz ilosc stworzonych podczas pracy programu" << endl;
    cout << "3) dodaj obiekt terenowy" << endl;
    cout << "4) wybierz drona" << endl;
    cout << "5) dodaj drona" << endl;
    cout << "6) usun gore bo masz buldozer na tytanie" << endl;
    cout << "7) usun drona "<< endl;
    cout << "8) Zakoncz program :(" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

//void rysuj_drona(drawNS::Draw3DAPI * api, Droniak dron, vector<uint> V){
//    while(!V.empty()){
//        api -> erase_shape(V.back());
//        V.pop_back();
//    }
//    for(int i=0; i < 24; i+=2) {
//        V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i], dron.getGraniastoslup().wez_wierzcholki_3D()[i+1], "purple")); //rysuje fioletowy prostokat
//    }
//    for(int i=0; i<4;i++) {
//        V.push_back(api->draw_polygonal_chain(dron.getWirniki(i).wierzcholki_dol(), "red"));
//        //api->draw_polygonal_chain(dron.getWirniki(i).wierzcholki_gora(), "red");
//    }
//}



int main( int argc, char * argv[] ) {
    srand( time( NULL ) );
    double wysokosc = 20;
    vector<uint> V;
    Teren teren;
    teren.dodaj_smaczek(); ///smaczek to gora lub plaskowyz
    teren.dodaj_smaczek();
    teren.dodaj_smaczek();
    int z = 100, id=0;
    drawNS::Draw3DAPI * api(new APIopenGL3D(-z, z, -z, z, -z, z, 0, &argc, argv));
    teren.dodaj_byka_droniarza();
    teren.dodaj_byka_droniarza();
    teren.dodaj_byka_droniarza();
    ///pierwsze dwa for'y odpowiadaja ze rysowanie podloza
    for(int k = -z; k < z; k+=5){
        api->draw_line(Point3D(k, -z, 0), Point3D(k, z, 0), "light-blue");
    }
    for(int k = -z; k < z; k+=5){
        api->draw_line(Point3D(-z, k, 0), Point3D(z, k, 0), "light-blue");
    }

    ///tutaj rysujemy obiekty terenu
    for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
        V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
    }

    ///kolejne dwa rysuja "cialo" oraz wirniki drona, nie moglem wrzucic tego do funkcji poniewaz api z jakiegos powodu zwracalo bledy
    for(int k=0; k < teren.getDroniarze().size(); k++) {
        Droniak dron = teren.getDroniarze()[k];
        for (int i = 0; i < 24; i += 2) {
            V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                       dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                       "purple")); //rysuje fioletowy prostokat
        }
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 24; i += 2) {
                V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                           dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                           "red")); //rysuje czerwone wirniki a one to prostokat
            }
        }
    }
    int a;
    double v = 20; ///predkosc w m/s
    int fps = 23;
    bool p = false;
    menu();
    while (true) {
        cin >> a;
        switch (a) {
            ///takiego case'a jeszcze Pan nie widzial, jednak api nie daje sie podac do funkcji xD
            case 1:{
                double nowy_x, nowy_y;
                cout << "Podaj wspolrzene(x a potem y): " << endl;
                cin >> nowy_x >> nowy_y;
                Droniak &moj_ulubiony_dron = teren.getDroniarze()[id];
                double nowy_kierunek = atan2(nowy_y - moj_ulubiony_dron.getY(), nowy_x - moj_ulubiony_dron.getX());
                while(!(teren.czy_droniak_moze_ladowac(nowy_x,nowy_y, moj_ulubiony_dron))){
                    nowy_x += cos(nowy_kierunek);
                    nowy_y += sin(nowy_kierunek);
                }



                ///wznoszenie
                double t_c_h = 2.5;
                double t_xD = 0;
                int wznoszenie_klatki = fps * t_c_h;
                for(int h = 0; h < wznoszenie_klatki; h++){
                    moj_ulubiony_dron.startuj((teren.jak_duzy_jestes(moj_ulubiony_dron.getX(), moj_ulubiony_dron.getY()) + wysokosc * (t_xD / t_c_h)));
                    moj_ulubiony_dron.obroty_wirnika();
                    while(!V.empty()){
                        api -> erase_shape(V.back());
                        V.pop_back();
                    }
                    ///rysowanie nowego drona
                    for(int k=0; k < teren.getDroniarze().size(); k++) {
                        Droniak dron = teren.getDroniarze()[k];
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                       dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                       "purple")); //rysuje fioletowy prostokat
                        }
                        for (int j = 0; j < 4; j++) {
                            for (int i = 0; i < 24; i += 2) {
                                V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                           dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                           "red")); //rysuje czerwone wirniki a one to prostokat
                            }
                        }
                    }
                    for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                        V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                    }
                    std::this_thread::sleep_for (std::chrono::milliseconds(1000/fps));
                    t_xD += (1.0/fps);
                }


                ///obrot

                double t_o = 2;
                int obrot_klatki = fps * t_o;
                for(int h = 0; h < obrot_klatki; h++){
                    moj_ulubiony_dron.obroty_dronow_niebieskich((nowy_kierunek - moj_ulubiony_dron.getKierunekLotu()) / obrot_klatki);
                    moj_ulubiony_dron.obroty_wirnika();
                    while(!V.empty()){
                        api -> erase_shape(V.back());
                        V.pop_back();
                    }
                    ///rysowanie nowego drona
                    for(int k=0; k < teren.getDroniarze().size(); k++) {
                        Droniak dron = teren.getDroniarze()[k];
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                       dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                       "purple")); //rysuje fioletowy prostokat
                        }
                        for (int j = 0; j < 4; j++) {
                            for (int i = 0; i < 24; i += 2) {
                                V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                           dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                           "red")); //rysuje czerwone wirniki a one to prostokat
                            }
                        }
                    }
                    for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                        V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                    }
                    std::this_thread::sleep_for (std::chrono::milliseconds(1000/fps));
                }
                moj_ulubiony_dron.setKierunekLotu(nowy_kierunek);


                ///lot

                double s = sqrt(pow((nowy_x - moj_ulubiony_dron.getX()),2) + pow((nowy_y - moj_ulubiony_dron.getY()),2));
                double t_c = s/v;
                int klatki = fps * t_c;
                double t=0;
                for(int h = 0; h < klatki; h++){
                    moj_ulubiony_dron.lec((moj_ulubiony_dron.getX() + (nowy_x - moj_ulubiony_dron.getX()) * (t / t_c) ), (moj_ulubiony_dron.getY() + (nowy_y - moj_ulubiony_dron.getY()) * (t / t_c) ) );
                    moj_ulubiony_dron.obroty_wirnika();
                    while(!V.empty()){
                        api -> erase_shape(V.back());
                        V.pop_back();
                    }
                    ///rysowanie nowego drona
                    for(int k=0; k < teren.getDroniarze().size(); k++) {
                        Droniak dron = teren.getDroniarze()[k];
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                       dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                       "purple")); //rysuje fioletowy prostokat
                        }
                        for (int j = 0; j < 4; j++) {
                            for (int i = 0; i < 24; i += 2) {
                                V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                           dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                           "red")); //rysuje czerwone wirniki a one to prostokat
                            }
                        }
                    }
                    for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                        V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                    }
                    std::this_thread::sleep_for (std::chrono::milliseconds(1000/fps));
                    t += (1.0/fps);
                }


                ///poprawka jakby dolecial na wspolrzedne z bledem pomiarowym

                moj_ulubiony_dron.lec(nowy_x, nowy_y);
                moj_ulubiony_dron.setX(nowy_x);
                moj_ulubiony_dron.setY(nowy_y);
                ///usuwanie poprzedniego obrazu
                while(!V.empty()){
                    api -> erase_shape(V.back());
                    V.pop_back();
                }
                ///rysowanie nowego drona
                for(int k=0; k < teren.getDroniarze().size(); k++) {
                    Droniak dron = teren.getDroniarze()[k];
                    for (int i = 0; i < 24; i += 2) {
                        V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                   dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                   "purple")); //rysuje fioletowy prostokat
                    }
                    for (int j = 0; j < 4; j++) {
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                       dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                       "red")); //rysuje czerwone wirniki a one to prostokat
                        }
                    }
                }
                for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                    V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                }


                ///ladowanie

                //cout <<"mozna? jakby nie mozna bylo...." << czy_mozna_ladowac << endl;
                double t_c_l = 2.5;
                double t_xDD = 0;
                int ladowanie_klatki = fps * t_c_l;
                for(int h = 0; h < ladowanie_klatki; h++){
                    moj_ulubiony_dron.startuj((wysokosc + (teren.jak_duzy_jestes(nowy_x, nowy_y)-wysokosc) * (t_xDD / t_c_l) ));
                    moj_ulubiony_dron.obroty_wirnika();
                    while(!V.empty()){
                        api -> erase_shape(V.back());
                        V.pop_back();
                    }
                    ///rysowanie nowego drona
                    for(int k=0; k < teren.getDroniarze().size(); k++) {
                        Droniak dron = teren.getDroniarze()[k];
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                       dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                       "purple")); //rysuje fioletowy prostokat
                        }
                        for (int j = 0; j < 4; j++) {
                            for (int i = 0; i < 24; i += 2) {
                                V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                           dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                           "red")); //rysuje czerwone wirniki a one to prostokat
                            }
                        }
                    }
                    for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                        V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                    }
                    std::this_thread::sleep_for (std::chrono::milliseconds(1000/fps));
                    t_xDD += (1.0/fps);
                }




                wait4key();
                menu();
                break;
            }

            case 2:{
                cout << "obecnie jest " << Wektor3D::getIleJest() << " wektorow" << endl;
                cout << "stworzono w sumie " << Wektor3D::getIleStworzono() << " wektorow" << endl;
                wait4key();
                menu();
                break;
            }

            case 3:{
                teren.dodaj_smaczek();
                for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                    V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                }
                wait4key();
                menu();
                break;
            }

            case 4:{
                cout << "Podaj numer drona wariacie" << endl;
                cout << "wybrane aktualnie id: " << id << endl;
                cin >> id;
                if(id >= teren.getDroniarze().size() || id < 0){
                    cout << "No takiego drona to nie ma mordeczko" << endl;
                    id = 0;
                }
                wait4key();
                menu();
                break;
            }

            case 5:{
                teren.dodaj_byka_droniarza();
                cout << "dodano drona, id nowego drona: " << teren.getDroniarze().size()-1 << endl;
                for(int k=0; k < teren.getDroniarze().size(); k++) {
                    Droniak dron = teren.getDroniarze()[k];
                    for (int i = 0; i < 24; i += 2) {
                        V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                   dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                   "purple")); //rysuje fioletowy prostokat
                    }
                    for (int j = 0; j < 4; j++) {
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                       dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                       "red")); //rysuje czerwone wirniki a one to prostokat
                        }
                    }
                }
                wait4key();
                menu();
                break;
            }

            case 6:{
                int id_g;
                cout << "Podaj id gory ktorej chcesz sie pozbyc: " ;
                cin >> id_g;
                cout << endl;
                while(id_g < 0 || id_g >= teren.ile_jest_gor()){
                    cout << "nie ma takiej gory, sprobuj ponownie" << endl;
                    cin >> id_g;
                }
                teren.wysadz_gore(id_g);
                while(!V.empty()){
                    api -> erase_shape(V.back());
                    V.pop_back();
                }
                ///tutaj rysujemy obiekty terenu
                for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                    V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                }

                ///kolejne dwa rysuja "cialo" oraz wirniki drona, nie moglem wrzucic tego do funkcji poniewaz api z jakiegos powodu zwracalo bledy
                for(int k=0; k < teren.getDroniarze().size(); k++) {
                    Droniak dron = teren.getDroniarze()[k];
                    for (int i = 0; i < 24; i += 2) {
                        V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                   dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                   "purple")); //rysuje fioletowy prostokat
                    }
                    for (int j = 0; j < 4; j++) {
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                       dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                       "red")); //rysuje czerwone wirniki a one to prostokat
                        }
                    }
                }
                wait4key();
                menu();
                break;
            }

            case 7:{
                int id_d;
                cout << "Podaj id drona ktorego chcesz wyrzucic: " ;
                cin >> id_d;
                cout << endl;
                while(id_d < 0 || id_d >= teren.getDroniarze().size()){
                    cout << "nie ma takiego drona, sprobuj ponownie" << endl;
                    cin >> id_d;
                }
                teren.dokonaj_aborcji_drona(id_d);
                while(!V.empty()){
                    api -> erase_shape(V.back());
                    V.pop_back();
                }
                ///tutaj rysujemy obiekty terenu
                for(int i=0; i < teren.zwroc_wszystkie_wierzcholki().size(); i+=2) {
                    V.push_back(api->draw_line(teren.zwroc_wszystkie_wierzcholki()[i], teren.zwroc_wszystkie_wierzcholki()[i+1], "green"));
                }

                ///kolejne dwa rysuja "cialo" oraz wirniki drona, nie moglem wrzucic tego do funkcji poniewaz api z jakiegos powodu zwracalo bledy
                for(int k=0; k < teren.getDroniarze().size(); k++) {
                    Droniak dron = teren.getDroniarze()[k];
                    for (int i = 0; i < 24; i += 2) {
                        V.push_back(api->draw_line(dron.getGraniastoslup().wez_wierzcholki_3D()[i],
                                                   dron.getGraniastoslup().wez_wierzcholki_3D()[i + 1],
                                                   "purple")); //rysuje fioletowy prostokat
                    }
                    for (int j = 0; j < 4; j++) {
                        for (int i = 0; i < 24; i += 2) {
                            V.push_back(api->draw_line(dron.getWirniki(j).wez_wierzcholki_3D()[i],
                                                       dron.getWirniki(j).wez_wierzcholki_3D()[i + 1],
                                                       "red")); //rysuje czerwone wirniki a one to prostokat
                        }
                    }
                }
                wait4key();
                menu();
                break;
            }

            case 8:{
                p = true;
                break;
            }
            default:{
                menu();
                break;
            }

        }
        if(p == 1){break;}
        }

    delete api;
    return 0;
}