#include <iostream>
#include <stdexcept>
#include <vector>


using namespace std; // zbog testova


template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;



template <typename TipOznake>
class UsmjereniGraf {

public:
    UsmjereniGraf(int) {}
    virtual ~UsmjereniGraf() {}
    
    virtual void postaviBrojCvorova(int) = 0;
    virtual void postaviTezinuGrane(int, int, double) = 0;
    virtual void postaviOznakuCvora(int, TipOznake) = 0;
    virtual void postaviOznakuGrane(int, int, TipOznake) = 0;

    virtual void dodajGranu(int, int, double) = 0;
    virtual void obrisiGranu(int, int) = 0;

    virtual int dajBrojCvorova() const = 0;
    virtual double dajTezinuGrane(int, int)  = 0;
    virtual TipOznake dajOznakuCvora(int)  = 0;
    virtual TipOznake dajOznakuGrane(int, int)  = 0;
    
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int p, int k) = 0;
    
    virtual bool postojiGrana(int, int) = 0;

    Grana<TipOznake> dajGranu(int p, int k) {
        return Grana<TipOznake>(this, p, k);
    }
    Cvor<TipOznake> dajCvor(int c) {
        return Cvor<TipOznake>(this, c);
    }
};


template <typename TipOznake>
class Grana {
    int pocetak, kraj;
    bool postojiGrana;
    UsmjereniGraf<TipOznake> *usmjereniGraf;

public:
    Grana(UsmjereniGraf<TipOznake> *it, int p, int k) : pocetak(p), kraj(k), usmjereniGraf(it) {}

    TipOznake dajOznaku() const {
        return usmjereniGraf -> dajOznakuGrane(pocetak, kraj);
    }
    double dajTezinu() const {
        return usmjereniGraf -> dajTezinuGrane(pocetak, kraj);
    }

    Cvor<TipOznake> dajPolazniCvor() {
       return usmjereniGraf -> dajCvor(pocetak);
    }
    Cvor<TipOznake> dajDolazniCvor() {
        return usmjereniGraf -> dajCvor(kraj);
    }

    void postaviOznaku(TipOznake o) {
        usmjereniGraf -> postaviOznakuGrane(pocetak, kraj, o);
    }
    void postaviTezinu(double t) {
        usmjereniGraf -> postaviTezinuGrane(pocetak, kraj, t);
    } 
};


template <typename TipOznake>
class Cvor {
    int redniBroj;
    UsmjereniGraf<TipOznake> *usmjereniGraf;

public:
    Cvor(UsmjereniGraf<TipOznake> *ug, int rb) : usmjereniGraf(ug), redniBroj(rb) {}

    int dajRedniBroj() const {
        return redniBroj;
    }
    
    TipOznake dajOznaku() const {
        return usmjereniGraf -> dajOznakuCvora(redniBroj);
    }
    void postaviOznaku(TipOznake o) {
        usmjereniGraf -> postaviOznakuCvora(redniBroj, o);
    }
};

template <typename TipOznake>
class GranaIterator {

    int dolazniCvor, polazniCvor;
    UsmjereniGraf<TipOznake> *usmjereniGraf;

public:
    GranaIterator(UsmjereniGraf<TipOznake> *ug, int p, int k) {
        usmjereniGraf = ug;
        polazniCvor = p;
        dolazniCvor = k;
    }
    Grana<TipOznake> operator *() {
        return Grana<TipOznake>(usmjereniGraf, polazniCvor, dolazniCvor);
    }
    bool operator ==(const GranaIterator &it) const {
        return (usmjereniGraf == it.usmjereniGraf && polazniCvor == it.polazniCvor && dolazniCvor == it.dolazniCvor);
    }
    bool operator !=(GranaIterator it) {
        return !(it == *this);
    }
    bool operator !=(const GranaIterator &it) const {
        return !(it == *this);
    }
    GranaIterator &operator ++() {
        if(polazniCvor == -1 && dolazniCvor == -1)
            throw std::logic_error("Izasli ste izvan opsega!");
        GranaIterator<TipOznake> it = usmjereniGraf -> dajSljedecuGranu(polazniCvor, dolazniCvor);
        polazniCvor = it.polazniCvor;
        dolazniCvor = it.dolazniCvor;
        return *this;
    }
    GranaIterator operator ++(int) {
        GranaIterator temp = *this;
        ++*this;
        return temp;
    }
};


template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
    struct infoAboutBranch {
        TipOznake oznakaCvora;
        double tezinaGrane;
        bool postojiGrana;
    };

    std::vector<std::vector<infoAboutBranch>> informacije;
    std::vector<TipOznake> oznakeCvorova;

public:
    ListaGraf(int brojCvorova) : UsmjereniGraf<TipOznake>(brojCvorova) {
        postaviBrojCvorova(brojCvorova);
    }

    void postaviOznakuCvora(int cvor, TipOznake o) override {
        oznakeCvorova[cvor] = o;
    }
    void postaviOznakuGrane(int p, int k, TipOznake o) override {
        informacije[p][k].oznakaCvora = o;
    }

    TipOznake dajOznakuCvora(int cvor) override {
        return oznakeCvorova[cvor];
    }
    TipOznake dajOznakuGrane(int p, int k) override  {
        return informacije[p][k].oznakaCvora;
    }
    int dajBrojCvorova() const override {
        return informacije.size();
    }


    void postaviBrojCvorova(int brojCvorova) override  {
        infoAboutBranch info;
        info.postojiGrana = false;

        for(int i = 0; i < informacije.size(); i++) {
            informacije[i].resize(brojCvorova, info);
        }

        std::vector<infoAboutBranch> noviRed(brojCvorova, info);
        informacije.resize(brojCvorova, noviRed);
        oznakeCvorova.resize(brojCvorova);
    }

    void dodajGranu(int p, int k, double t) override {
        infoAboutBranch info;
        info.tezinaGrane = t;
        info.postojiGrana = true;
        informacije[p][k] = info;
    }

    void obrisiGranu(int p, int k) override {
        informacije[p][k].postojiGrana = false;
    }

    void postaviTezinuGrane(int p, int k, double t) {
        informacije[p][k].tezinaGrane = t;
    }

    bool postojiGrana(int p, int k) {
        return informacije[p][k].postojiGrana;
    }

    double dajTezinuGrane(int p, int k) override {
        return informacije[p][k].tezinaGrane;
    }

    GranaIterator<TipOznake> dajGranePocetak() override {
        GranaIterator<TipOznake> it(this, 0, -1);
        ++it;
        return it;
    }
    GranaIterator<TipOznake> dajGraneKraj() override {
        GranaIterator<TipOznake> it(this, -1, -1);
        return it;
    }
    GranaIterator<TipOznake> dajSljedecuGranu(int p, int k) override {
        for(int i = p; i < informacije.size(); i++)  {
            for(int j = 0; j < informacije.size(); j++) {
                if(i == p && j <= k) continue;
                if(informacije[i][j].postojiGrana)
                    return GranaIterator<TipOznake>(this, i, j);
            }
        }
        return GranaIterator<TipOznake>(this, -1, -1);
    }

};


template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *usmjereniGraf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> c) {
    bool posjecenCvor = false; // 0 pa onda susjedi 1 pa susjedi susjeda 2 i tako dok se ne obidju svi

    for(int i = 0; i < obilazak.size(); i++) {
        if(c.dajRedniBroj() == obilazak[i].dajRedniBroj()) {
            posjecenCvor = true;
            break;
        }
    }

    if(!posjecenCvor)
        obilazak.push_back(c);

    int brojCvorova = usmjereniGraf -> dajBrojCvorova();

    for(int i = 0; i < brojCvorova; i++) {
        if(!posjecenCvor)
            bfs(usmjereniGraf, obilazak, usmjereniGraf -> dajCvor(i));
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *usmjereniGraf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> c) {
    bool posjecenCvor = false; // krenemo od nekog cvora i obilazimo sve dok postoji slobodna grana tj. idemo duboko u graf

    for(int i = 0; i < obilazak.size(); i++) {
        if(c.dajRedniBroj() == obilazak[i].dajRedniBroj()) {
            posjecenCvor = true;
            break;
        }
    }

    if(!posjecenCvor)
        obilazak.push_back(c);

    int brojCvorova = usmjereniGraf -> dajBrojCvorova();

    for(int i = 0; i < brojCvorova; i++) {
        if(usmjereniGraf -> postojiGrana(c.dajRedniBroj(), i) && !posjecenCvor) 
            if(!posjecenCvor)
                dfs(usmjereniGraf, obilazak, usmjereniGraf -> dajCvor(i));
    }
}


template <typename TipKljuca, typename TipVrijednosti>
class Mapa {

public:
    Mapa(Mapa<TipKljuca, TipVrijednosti> &&);
    Mapa() {};
    Mapa &operator=(Mapa<TipKljuca, TipVrijednosti> &&);
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti &operator[](TipKljuca) = 0;
    virtual TipVrijednosti operator[](TipKljuca) const = 0;
    virtual ~Mapa() {} 
};


template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {

    std::pair<TipKljuca, TipVrijednosti> *elementi;
    int ukupniKapacitet;
    int velicina;
    unsigned int (*hashFunkcija)(TipKljuca, unsigned int) = nullptr;

    void kopirajMapu(const HashMapaLan<TipKljuca, TipVrijednosti> &m) {
        elementi = new std::pair<TipKljuca, TipVrijednosti>[m.ukupniKapacitet];

        for(int i = 0; i < m.ukupniKapacitet; i++) {
            elementi[i] = m.elementi[i];
        }

        ukupniKapacitet = m.ukupniKapacitet; velicina = m.velicina; hashFunkcija = m.hashFunkcija;
    }


public:
    HashMapaLan() : elementi(nullptr), ukupniKapacitet(1000), velicina(0), hashFunkcija(nullptr) {
        
    }
    ~HashMapaLan() {
        delete[] elementi;
        velicina = 0;
        ukupniKapacitet = 1000;
    }
    HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
        kopirajMapu(mapa);
    }
    HashMapaLan &operator =(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
        if(&mapa != this) {
            delete[] elementi;
            elementi = nullptr;
            kopirajMapu(mapa);
        }
        return *this;
    }

    int brojElemenata() const override {
        return velicina;
    }

    void definisiHashFunkciju(unsigned int(*hashFja)(TipKljuca kljuca, unsigned int max)) {
        hashFunkcija = hashFja;
    }

    void obrisi() override {
        velicina = 0;
        ukupniKapacitet = 1000;
        delete[] elementi;
    }
    void obrisi(const TipKljuca &kljuc) override {

        if(velicina == 0)
            return;

        unsigned int hash = hashFunkcija(kljuc, ukupniKapacitet);
        int pozicija = velicina;

        if(elementi[hash].first == kljuc)
            pozicija = hash;
        else {
            for(int i = 0; i < ukupniKapacitet; i++) {
                if(i != hash && elementi[i].first == kljuc) {
                    pozicija = i;
                    break;
                }
            }
        }

        if(pozicija == velicina)
            return; // nema kljuca u mapi;

        elementi[pozicija].first = TipKljuca();
        elementi[pozicija].second = TipVrijednosti();
        velicina--;
    }

    TipVrijednosti operator [](TipKljuca kljuc) const override {
        unsigned int hash = hashFunkcija(kljuc, ukupniKapacitet);
        
        if(elementi[hash].first == kljuc)
            return elementi[hash].second;
        
        for(int i = hash + 1; i < velicina; i++) {
            if(elementi[i].first == kljuc)
                return elementi[i].second;
        }

        for(int i = 0; i < hash; i++) {
            if(elementi[i].first == kljuc)
                return elementi[i].second;
        }

        return TipVrijednosti();
    }
    TipVrijednosti &operator [](TipKljuca kljuc) override {
        if(hashFunkcija == nullptr)
            throw std::logic_error("Hash funkcija nije definisana!");
        
        unsigned int hash = hashFunkcija(kljuc, ukupniKapacitet);

        if(velicina == 0) {
            elementi = new std::pair<TipKljuca, TipVrijednosti>[ukupniKapacitet];
            elementi[hash].first = kljuc;
            elementi[hash].second = TipVrijednosti();
            velicina++;
            for(int i = 0; i < ukupniKapacitet; i++) {
                if(i != hash)
                    elementi[i].first = TipKljuca();
            }
            return elementi[hash].second;
        }
        else {
            if(elementi[hash].first == kljuc)
                return elementi[hash].second;

            for(int i = hash + 1; i < ukupniKapacitet; i++) {
                if(elementi[i].first == kljuc)
                    return elementi[i].second;
            }

            for(int i = 0; i < hash; i++) {
                if(elementi[i].first == kljuc)
                    return elementi[i].second;
            }

            for(int i = 0; i < ukupniKapacitet; i++) {
                if(i != hash && elementi[i].first == TipKljuca()) {
                    elementi[i].first = kljuc;
                    elementi[i].second = TipVrijednosti();
                    velicina++;
                    return elementi[i].second;
                }
            }

            if(velicina == ukupniKapacitet) {
                ukupniKapacitet *= 2;
                std::pair<TipKljuca, TipVrijednosti> *privremeniElementi = new std::pair<TipKljuca, TipVrijednosti>[ukupniKapacitet];

                for(int i = 0; i < velicina; i++) {
                    privremeniElementi[i].first = elementi[i].first;
                    privremeniElementi[i].second = elementi[i].second;
                }

                delete[] elementi;
                elementi = privremeniElementi;
                privremeniElementi = nullptr;
            }

            elementi[velicina].first = kljuc;
            elementi[velicina].second = TipVrijednosti();
            velicina++;
            return elementi[velicina - 1].second;
        }
    }
};



int main() {
    ListaGraf<int> graf(5);

    graf.postaviOznakuCvora(0, 10);
    graf.postaviOznakuCvora(1, 20);
    graf.postaviOznakuCvora(2, 30);
    graf.postaviOznakuCvora(3, 40);
    graf.postaviOznakuCvora(4, 50);

    graf.dodajGranu(0, 1, 1.5);
    graf.dodajGranu(1, 2, 2.5);
    graf.dodajGranu(2, 3, 3.5);
    graf.dodajGranu(3, 4, 4.5);

    GranaIterator<int> it = graf.dajGranePocetak();
    GranaIterator<int> itEnd = graf.dajGraneKraj();

    cout << "Edges: ";
    while (it != itEnd) {
        Grana<int> grana = *it;
        cout << "(" << grana.dajPolazniCvor().dajRedniBroj() << " -> " << grana.dajDolazniCvor().dajRedniBroj() << ") ";
        ++it;
    }
    cout << endl;

    // Testing BFS
    vector<Cvor<int>> bfsObilazak;
    bfs(&graf, bfsObilazak, graf.dajCvor(0));
    cout << "BFS traversal: ";
    for (const auto &cvor : bfsObilazak) {
        cout << cvor.dajRedniBroj() << " ";
    }
    cout << endl;

    // Testing DFS
    vector<Cvor<int>> dfsObilazak;
    dfs(&graf, dfsObilazak, graf.dajCvor(0));
    cout << "DFS traversal: ";
    for (const auto &cvor : dfsObilazak) {
        cout << cvor.dajRedniBroj() << " ";
    }
    cout << endl;

    return 0;
}
