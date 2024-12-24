#include<iostream>
#include <utility>
template<typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti& operator [] (const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti operator [] (const TipKljuca& kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0; 
    virtual void obrisi(const TipKljuca& kljuc) = 0;
};

template<typename TipKljuca,typename TipVrijednosti>
class AVLCvor {
    public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    int balans = 0;
    AVLCvor<TipKljuca,TipVrijednosti> *lijevo, *desno, *roditelj;
    AVLCvor() {
        lijevo=nullptr;
        desno=nullptr;
        roditelj=nullptr;
    }
    AVLCvor(const TipKljuca& k, const TipVrijednosti& v, AVLCvor<TipKljuca,TipVrijednosti>* l=nullptr, AVLCvor<TipKljuca,TipVrijednosti> *d=nullptr, AVLCvor<TipKljuca,TipVrijednosti>*r=nullptr) {
        kljuc=k;
        vrijednost=v;
        lijevo=l;
        desno=d;
        roditelj=r;
    }
};

template<typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca,TipVrijednosti> {
    AVLCvor<TipKljuca,TipVrijednosti> *korijen=nullptr;
    int brojEl=0;
    void brisi(AVLCvor<TipKljuca,TipVrijednosti> *r) {
        if(r){
            brisi(r->lijevo);
            brisi(r->desno);
            delete r;
        }
        korijen=nullptr;
    }
    public:
    AVLStabloMapa() {}
    
    void jednostrukaRotacijaUlijevo(AVLCvor<TipKljuca,TipVrijednosti> *&cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo==cvor) cvor->roditelj->lijevo=cvor->desno;
        else if(cvor->roditelj) cvor->roditelj->desno=cvor->desno;
        AVLCvor<TipKljuca,TipVrijednosti> *tmp = cvor->desno;
        bool desno=false;
        if(cvor->desno->lijevo) {
            cvor->desno=tmp->lijevo;
            tmp->lijevo->roditelj=cvor;
            tmp->lijevo=nullptr;
            desno=true;
        }
        tmp->roditelj=cvor->roditelj;
        if(cvor->roditelj) cvor->roditelj=tmp;
        else {
            cvor->roditelj=tmp;
            korijen=tmp;
        }
        tmp->lijevo=cvor;
        cvor->balans=0;
        tmp->balans=0;
        if(!desno) cvor->desno=nullptr;
    }
    
     void jednostrukaRotacijaUdesno(AVLCvor<TipKljuca,TipVrijednosti> *&cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo==cvor) cvor->roditelj->lijevo=cvor->lijevo;
        else if(cvor->roditelj) cvor->roditelj->desno=cvor->lijevo;
        AVLCvor<TipKljuca,TipVrijednosti> *tmp = cvor->lijevo;
        bool lijevo=false;
        if(cvor->lijevo->desno) {
            cvor->lijevo=tmp->desno;
            tmp->desno->roditelj=cvor;
            tmp->desno=nullptr;
            lijevo=true;
        }
        tmp->roditelj=cvor->roditelj;
        if(cvor->roditelj) cvor->roditelj=tmp;
        else {
            cvor->roditelj=tmp;
            korijen=tmp;
        }
        tmp->desno=cvor;
        cvor->balans=0;
        tmp->balans=0;
        if(!lijevo) cvor->lijevo=nullptr;
    }
    
    
    void azurirajBalans(AVLCvor<TipKljuca,TipVrijednosti> *&m) {
        if(m->roditelj == nullptr) return;
        if(!(m->lijevo && m->desno && m->balans==0)) {
            if(m->roditelj->lijevo == m) m->roditelj->balans++;
            else m->roditelj->balans--;
        }
        bool nastavi=true;
        AVLCvor<TipKljuca,TipVrijednosti> *r=m->roditelj;
        if(m->roditelj->balans< -1) {
            jednostrukaRotacijaUlijevo(r);
            nastavi=false;
        } else if(m->roditelj->balans > 1) {
            jednostrukaRotacijaUdesno(r);
            nastavi=false;
        }
        if(nastavi) azurirajBalans(m->roditelj);
    }
    void Umetni(AVLCvor<TipKljuca,TipVrijednosti> *&k) {
        AVLCvor<TipKljuca,TipVrijednosti> *y=nullptr;
        AVLCvor<TipKljuca,TipVrijednosti> *x=korijen;
        while(x!=nullptr) {
            y=x;
            if(k->kljuc < x->kljuc) x=x->lijevo;
            else x=x->desno;
        }
        bool ima=false;
        if(y==nullptr) {
            korijen=k;
            ima=true;
        } else {
            if(k->kljuc < y->kljuc) {
                y->lijevo=k;
                k->roditelj=y;
                if(y->desno) {
                    ima=true;
                    y->balans=0;
                }
            } else {
                y->desno=k;
                k->roditelj=y;
                if(y->lijevo) {
                    ima=true;
                    y->balans=0;
                }
            }
        }
        if(!ima) azurirajBalans(k);
        brojEl++;
    }
    TipVrijednosti& dodaj(TipKljuca &k) {
        AVLCvor<TipKljuca,TipVrijednosti> *novi=new AVLCvor<TipKljuca,TipVrijednosti>(k,TipVrijednosti{});
        Umetni(novi);
        return novi->vrijednost;
    }
    void Preorder(AVLCvor<TipKljuca,TipVrijednosti>* &r) {
        if(r!=nullptr) {
            dodaj(r->kljuc)=r->vrijednost;
            Preorder(r->lijevo);
            Preorder(r->desno);
        }
    }
    void Preorder() {
        Preorder(korijen);
    }
    AVLStabloMapa(const AVLStabloMapa<TipKljuca,TipVrijednosti> &m) {
        AVLCvor<TipKljuca,TipVrijednosti> *tmp=m.korijen;
        Preorder(tmp);
    }
    
    AVLStabloMapa& operator=(const AVLStabloMapa<TipKljuca,TipVrijednosti> &m) {
        if(this==&m) return *this;
        obrisi();
        korijen=nullptr;
        AVLCvor<TipKljuca,TipVrijednosti> *novi=m.korijen;
        Preorder(novi);
        return *this;
    }
    
    ~AVLStabloMapa() {
        brisi(korijen);
    }
    
    
    TipVrijednosti& operator [] (const TipKljuca& kljuc) {
        AVLCvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr && kljuc!=p->kljuc){
            if(kljuc< p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p!=nullptr) return p->vrijednost;
        AVLCvor<TipKljuca,TipVrijednosti> *novi= new AVLCvor<TipKljuca,TipVrijednosti>{kljuc,TipVrijednosti{}};
        Umetni(novi);
        return novi->vrijednost;
    }
    TipVrijednosti operator [] (const TipKljuca& kljuc) const  {
         AVLCvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr && kljuc!=p->kljuc){
            if(kljuc< p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p!=nullptr) return p->vrijednost;
        return TipVrijednosti{};
    }
    int brojElemenata() const {
        return brojEl;
    };
    void obrisi() {
        brisi(korijen);
        brojEl=0;
    }
    void obrisi(const TipKljuca& kljuc) {
        AVLCvor<TipKljuca,TipVrijednosti> *p=korijen;
        AVLCvor<TipKljuca,TipVrijednosti> *roditelj=nullptr,*m=nullptr,*pm=nullptr,*tmp=nullptr;
        while(p!=nullptr && kljuc!=p->kljuc) {
            roditelj=p;
            if(kljuc<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p==nullptr) return;
        if(p->lijevo==nullptr) m=p->desno;
        else if(p->desno==nullptr) m=p->lijevo;
        else {
            pm=p;
            m=p->lijevo;
            tmp=m->desno;
            while(tmp!=nullptr) {
                pm=m;
                m=tmp;
                tmp=m->desno;
            }
            if(pm!=p) {
                pm->desno=m->lijevo;
                m->lijevo=p->lijevo;
                pm->roditelj=m;
                m->roditelj=roditelj;
                if(pm->desno) pm->desno->roditelj=pm;
            }
            m->desno=p->desno;
            p->desno->roditelj=m;
        }
        if(roditelj==nullptr) {
            korijen=m;
            if(korijen) korijen->roditelj=nullptr;
        } else if(p==roditelj->lijevo){
            roditelj->lijevo=m;
            if(m) m->roditelj=roditelj;
        } else {
            roditelj->desno=m;
            if(m) m->roditelj=roditelj;
        }
        if(p->roditelj && p->roditelj->lijevo==p) p->roditelj->balans--;
        else if(p->roditelj) p->roditelj->balans++;
        delete p;
        brojEl--;
    }
};

using namespace std;


template<typename TipKljuca,typename TipVrijednosti>
class BSCvor {
  public:
  TipKljuca kljuc;
  TipVrijednosti vrijednost;
  BSCvor *lijevo,*desno,*roditelj;
  BSCvor() {
      lijevo=nullptr;
      desno=nullptr;
      roditelj=nullptr;
  }
  BSCvor(const TipKljuca &k, const TipVrijednosti &v, BSCvor* l=nullptr, BSCvor* d=nullptr, BSCvor* r=nullptr) {
      kljuc=k;
      vrijednost=v;
      lijevo=l;
      desno=d;
      roditelj=r;
  }
};


template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca,TipVrijednosti> {
    BSCvor<TipKljuca,TipVrijednosti>* korijen;
    int brojEl=0;
    void brisi(BSCvor<TipKljuca,TipVrijednosti> *r) {
     if (r) {
         brisi(r->lijevo);
         brisi(r->desno);
         delete r;
     }
     korijen=nullptr;
    }
    public:
    BinStabloMapa() {
        korijen=nullptr;
    }
    
    BinStabloMapa(const BinStabloMapa<TipKljuca,TipVrijednosti> &bsm) {
        korijen=nullptr;
        BSCvor<TipKljuca,TipVrijednosti>* cvor=bsm.korijen;
        Preorder(cvor);
    }
    
    TipVrijednosti &dodaj(TipKljuca &k) {
        BSCvor<TipKljuca,TipVrijednosti> *novi=new BSCvor<TipKljuca,TipVrijednosti>(k,TipVrijednosti{});
        Umetni(novi);
        return novi->vrijednost;
    }
    
    void Preorder(BSCvor<TipKljuca,TipVrijednosti> *r) {
        if(r!=nullptr) {
            dodaj(r->kljuc)=r->vrijednost;
            Preorder(r->lijevo);
            Preorder(r->desno);
        }
    }
    
    
    BinStabloMapa& operator=(const BinStabloMapa<TipKljuca,TipVrijednosti> &bsm) {
        if(this==&bsm) return *this;
        obrisi();
        korijen=nullptr;
        BSCvor<TipKljuca,TipVrijednosti> *cvor=bsm.korijen;
        Preorder(cvor);
        return *this;
    }
    
    ~BinStabloMapa() {
        brisi(korijen);
    }
    
    void Umetni(BSCvor<TipKljuca,TipVrijednosti>* &n) {
        BSCvor<TipKljuca,TipVrijednosti> *y=nullptr, *x=korijen;
        while(x!=nullptr) {
            y=x;
            if(n->kljuc<x->kljuc) x=x->lijevo;
            else x=x->desno;
        }
        if(y==0) {
            korijen=n;
            brojEl++;
        } else {
            if(n->kljuc < y->kljuc) {
                y->lijevo=n;
                n->roditelj=y;
                brojEl++;
            } else {
                y->desno=n;
                n->roditelj=y;
                brojEl++;
            }
        }
    }
    
    TipVrijednosti& operator [] (const TipKljuca& kljuc) {
        BSCvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr && kljuc!=p->kljuc) {
            if(kljuc<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p!=nullptr) return p->vrijednost;
        BSCvor<TipKljuca,TipVrijednosti> * novi=new BSCvor<TipKljuca,TipVrijednosti>{kljuc, TipVrijednosti{}};
        Umetni(novi);
        return novi->vrijednost;
    }
    TipVrijednosti operator [] (const TipKljuca& kljuc) const {
        BSCvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr && kljuc!=p->kljuc) {
            if(kljuc<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p!=nullptr) return p->vrijednost;
        TipVrijednosti v{};
        return v;
    }
    int brojElemenata() const {
        return brojEl;
    }
    void obrisi() {
        brisi(korijen);
        brojEl=0;
    }
    void obrisi(const TipKljuca& kljuc) {
        BSCvor<TipKljuca,TipVrijednosti> *p=korijen,*rod=nullptr;
        BSCvor<TipKljuca,TipVrijednosti> *tmp=nullptr,*pm=nullptr,*m=nullptr;
        while(p!=nullptr && kljuc!=p->kljuc) {
            rod=p;
            if(kljuc< p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p==nullptr) throw "Greska: ne postoji kljuc u BinStabloMapa";
        if(p->lijevo==nullptr) m=p->desno;
        else if(p->desno==nullptr) m=p->lijevo;
        else {
            pm=p;
            m=p->lijevo;
            tmp=m->desno;
            while(tmp!=nullptr) {
                pm=m;
                m=tmp;
                tmp=m->desno;
            }
            if(pm!=p) {
                pm->desno=m->lijevo;
                m->lijevo=p->lijevo;
                pm->roditelj=m;
                m->roditelj=rod;
                if(pm->desno) pm->desno->roditelj=pm;
            }
            m->desno=p->desno;
        }
        if(rod==nullptr) {
            korijen=m;
        }
        else {
            if(p==rod->lijevo) rod->lijevo=m;
            else rod->desno=m;
        }
        delete p;
        brojEl--;
    }  
};


int dodajElementeUBinStabloMapa(BinStabloMapa<int,int> &mapa) {
    int br=0;
    int n=10000;
    for(int i=0;i<n;i++) {
        int v=rand() % 100000;
        mapa[v]=48;
        if(i==n/2) br=v;
    }
    return br;
}


int dodajElementeUAVLStabloMapa(AVLStabloMapa<int,int> &mapa) {
    int br=0;
    int n=10000;
    for(int i=0;i<n;i++) {
        int v=rand() % 100000;
        mapa[v]=48;
        if(i==n/2) br=v;
    }
    return br;
}

int main() {
    // Test BinStabloMapa
    BinStabloMapa<int, int> binStabloMapa;
    int kljucBinStabloMapa = dodajElementeUBinStabloMapa(binStabloMapa);

    cout << "BinStabloMapa - Broj elemenata: " << binStabloMapa.brojElemenata() << endl;
    cout << "Vrijednost kljuca " << kljucBinStabloMapa << ": " << binStabloMapa[kljucBinStabloMapa] << endl;

    // Dodaj još jedan element
    int noviKljucBinStabloMapa = 99999;
    binStabloMapa[noviKljucBinStabloMapa] = 99;

    cout << "BinStabloMapa nakon dodavanja novog elementa - Broj elemenata: " << binStabloMapa.brojElemenata() << endl;
    cout << "Vrijednost novog kljuca " << noviKljucBinStabloMapa << ": " << binStabloMapa[noviKljucBinStabloMapa] << endl;

    // Test AVLStabloMapa
    AVLStabloMapa<int, int> avlStabloMapa;
    int kljucAVLStabloMapa = dodajElementeUAVLStabloMapa(avlStabloMapa);

    cout << "\nAVLStabloMapa - Broj elemenata: " << avlStabloMapa.brojElemenata() << endl;
    cout << "Vrijednost kljuca " << kljucAVLStabloMapa << ": " << avlStabloMapa[kljucAVLStabloMapa] << endl;

    // Dodaj još jedan element
    int noviKljucAVLStabloMapa = 77777;
    avlStabloMapa[noviKljucAVLStabloMapa] = 77;

    cout << "AVLStabloMapa nakon dodavanja novog elementa - Broj elemenata: " << avlStabloMapa.brojElemenata() << endl;
    cout << "Vrijednost novog kljuca " << noviKljucAVLStabloMapa << ": " << avlStabloMapa[noviKljucAVLStabloMapa] << endl;

    return 0;
}
