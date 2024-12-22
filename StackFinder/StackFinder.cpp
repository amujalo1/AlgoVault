#include <iostream>
#include <vector>

using namespace std;

template <typename tip> class Stek {
  struct Cvor {
    tip vrj;
    Cvor *naredni;
    Cvor(const tip &el) : vrj(el), naredni(nullptr) {}
  };
  Cvor *vrhSteka;
  int velicina;

public:
  Stek() : vrhSteka(nullptr), velicina(0) {}

  Stek(const Stek &t) : vrhSteka(nullptr), velicina(0) {
    Cvor *temp = t.vrhSteka;
    Stek<tip> temp_stek;
    while (temp != nullptr) {
      temp_stek.stavi(temp->vrj); // Dodajte element na vrh privremenog steka
      temp = temp->naredni;
    }
    while (!temp_stek.jePrazan()) {
      stavi(temp_stek.vrh()); // Dodajte elemente na vrh steka
      temp_stek.skini();
    }
  }

  Stek &operator=(const Stek &t) {
    if (this == &t)
      return *this;
    brisi();
    Cvor *temp = t.vrhSteka;
    Stek<tip> temp_stek;
    while (temp != nullptr) {
      temp_stek.stavi(temp->vrj); // Dodajte element na vrh privremenog steka
      temp = temp->naredni;
    }
    while (!temp_stek.jePrazan()) {
      stavi(temp_stek.vrh()); // Dodajte elemente na vrh steka
      temp_stek.skini();
    }
    return *this;
  }

  void brisi() {
    while (!jePrazan())
      skini();
  }

  void stavi(const tip &el) {
    Cvor *noviCvor = new Cvor(el);
    noviCvor->naredni = vrhSteka;
    vrhSteka = noviCvor;
    velicina++;
  }

  tip skini() {
    if (jePrazan())
      throw runtime_error("Stek je prazan.");
    Cvor *temp = vrhSteka;
    tip rez = vrhSteka->vrj;
    vrhSteka = vrhSteka->naredni;
    delete temp;
    velicina--;
    return rez;
  }

  tip &vrh() {
    if (jePrazan())
      throw runtime_error("Stek je prazan.");
    return vrhSteka->vrj;
  }

  int brojElemenata() const { return velicina; }

  bool jePrazan() const { return velicina == 0; }

  void obrisi() {
    while (!jePrazan()) {
      skini();
    }
  }
  ~Stek() { obrisi(); }
};

void pretraga(Stek<vector<int>> &s, int trazeni){
    if(s.brojElemenata()==0){
        cout<<"Nema elementa"; return;
    }
    auto p = s.vrh();
    int pocetak = 0, kraj = p.size();
    if(p.size()!=0){
        if(p.at(0)==trazeni){
            cout<<"0"<<" "<<s.brojElemenata()-1; return;
        }
        while(pocetak<kraj){
            int sredina = (pocetak+kraj)/2;
            if(sredina<0 || sredina>p.size()) break;
            if(p.at(sredina)==trazeni){
                cout<<sredina<<" "<<s.brojElemenata()-1; return;
            }else if(p.at(sredina)<trazeni) pocetak = sredina+1; 
            else kraj = sredina-1;
        }
    }
    auto a = s.skini();
    pretraga(s, trazeni);
    s.stavi(a);
}

int main() {
    Stek<vector<int>> stek;

    // Populating the stack with vectors
    stek.stavi({1, 3, 5, 7, 9});
    stek.stavi({2, 4, 6, 8, 10});
    stek.stavi({11, 13, 15, 17, 19});

    // Test case 1: Element is in the stack
    int trazeni1 = 6;
    cout << "Test case 1: Element " << trazeni1 << " found at indices ";
    pretraga(stek, trazeni1);
    cout << endl;

    // Test case 2: Element is not in the stack
    int trazeni2 = 14;
    cout << "Test case 2: Element " << trazeni2 << " ";
    try {
        pretraga(stek, trazeni2);
    } catch (const runtime_error &e) {
        cout << e.what() << endl;
    }

    // Test case 3: Empty stack
    Stek<vector<int>> prazanStek;
    int trazeni3 = 5;
    cout << "Test case 3: Searching in an empty stack - ";
    try {
        pretraga(prazanStek, trazeni3);
    } catch (const runtime_error &e) {
        cout << e.what() << endl;
    }

    return 0;
}
