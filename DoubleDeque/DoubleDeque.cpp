#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
struct Cvor{
    Tip el;
    Cvor<Tip> *prethodni;
    Cvor<Tip> *naredni;
    Cvor(){}
    Cvor(const Cvor<Tip> &c): el(c->el), naredni(c->naredni), prethodni(c->prethodni){} 
    Cvor(const Tip &el, Cvor<Tip> *naredni, Cvor<Tip> *prethodni): el(el), naredni(naredni), prethodni(prethodni){}
};
template <typename Tip>
class DvostraniRed{
    int brElemenata = 0;
    Cvor<Tip> *pocetak_reda = nullptr, *kraj_reda = nullptr;
    public: 
        DvostraniRed(){}
        DvostraniRed(const DvostraniRed<Tip> &dr){
            brElemenata = dr.brElemenata;
            Cvor<Tip> *temp = dr.pocetak_reda;
            Cvor<Tip> *temp2 = new Cvor<Tip>;
            temp2->el = temp->el;
            pocetak_reda = temp2;
            temp = temp->naredni;
            while(temp!=nullptr){
                temp2->naredni = new Cvor<Tip>;
                temp2->naredni->el = temp->el;
                temp2->naredni->prethodni = temp2;
                temp = temp->naredni;
                temp2 = temp2->naredni;
            }
            kraj_reda = temp2;
        }
        DvostraniRed &operator =(const DvostraniRed &dr){
            if(this==&dr) return *this;
            brisi();
            brElemenata = dr.brElemenata;
            Cvor<Tip> *temp = dr.pocetak_reda;
            Cvor<Tip> *temp2 = new Cvor<Tip>;
            temp2->el = temp->el;
            pocetak_reda = temp2;
            temp = temp->naredni;
            while(temp!=nullptr){
                temp2->naredni = new Cvor<Tip>;
                temp2->naredni->el = temp->el;
                temp2->naredni->prethodni = temp2;
                temp = temp->naredni;
                temp2 = temp2->naredni;
            }
            kraj_reda = temp2;
            return *this;
        }
        ~DvostraniRed(){ brisi(); }
        int brojElemenata(){ return brElemenata; }
        void brisi(){
            while(brElemenata>0){
                Cvor<Tip> *temp = pocetak_reda;
                pocetak_reda = pocetak_reda->naredni;
                delete temp;
                temp = nullptr;
                brElemenata--;
            }
        }
        void staviNaVrh(const Tip &element){
            Cvor<Tip> *novi = new Cvor<Tip>(element, nullptr, nullptr);
            if(brElemenata==0) kraj_reda = pocetak_reda = novi;
            else{
                Cvor<Tip> *predKraj = kraj_reda;
                kraj_reda = novi;
                kraj_reda->prethodni = predKraj;
                predKraj->naredni = kraj_reda;
            }
            brElemenata++;
        }
        void staviNaCelo(const Tip &element){
            Cvor<Tip> *novi = new Cvor<Tip>(element, nullptr, nullptr);
            if(brElemenata==0) pocetak_reda = kraj_reda = novi;
            else{
                Cvor<Tip> * predPocetak = pocetak_reda;
                pocetak_reda = novi;
                pocetak_reda->naredni = predPocetak;
                predPocetak->prethodni = pocetak_reda;
            }
            brElemenata++;
        }
        Tip skiniSaVrha(){
            if(brElemenata==0) throw domain_error("Prazan dvostrani red");
            Tip vrh = kraj_reda->el;
            if(brElemenata==1){
                delete kraj_reda;
                pocetak_reda = nullptr;
                brElemenata = 0;
                return vrh;
            }
            Cvor<Tip> *temp = kraj_reda;
            kraj_reda = kraj_reda->prethodni;
            delete temp;
            brElemenata--;
            return vrh;
        }
        Tip skiniSaCela(){
            if(brElemenata==0) throw domain_error("Prazan dvostrani red");
            Tip celo = pocetak_reda->el;
            if(brElemenata==1){
                delete pocetak_reda;
                kraj_reda = nullptr;
                brElemenata = 0;
                return celo;
            }
            Cvor<Tip> *temp = pocetak_reda;
            pocetak_reda = pocetak_reda->naredni;
            delete temp;
            brElemenata--;
            return celo;
        }
        Tip &vrh(){
            if(brElemenata==0) throw domain_error("Prazan dvostrani red");
            return kraj_reda->el;
        }
        Tip &celo(){
            if(brElemenata==0) throw domain_error("Prazan dvostrani red");
            return pocetak_reda->el;
        }
};

int main(){
    try {
        DvostraniRed<int> red;

        // Dodaj nekoliko elemenata na vrh i celo reda
        red.staviNaVrh(1);
        red.staviNaVrh(2);
        red.staviNaCelo(3);
        red.staviNaCelo(4);

        // Ispisi trenutno stanje reda
        cout << "Broj elemenata u redu: " << red.brojElemenata() << endl;
        cout << "Vrh reda: " << red.vrh() << endl;
        cout << "Celo reda: " << red.celo() << endl;

        // Skini elemente sa vrha i cela reda
        cout << "Skini sa vrha: " << red.skiniSaVrha() << endl;
        cout << "Skini sa cela: " << red.skiniSaCela() << endl;

        // Ispisi ponovno stanje reda
        cout << "Broj elemenata u redu nakon skidanja: " << red.brojElemenata() << endl;
        cout << "Vrh reda nakon skidanja: " << red.vrh() << endl;
        cout << "Celo reda nakon skidanja: " << red.celo() << endl;

    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}