#include <assert.h>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Tip> class Lista {
public:
  Lista() {}
  virtual int brojElemenata() const = 0;
  virtual Tip &trenutni() = 0;
  virtual Tip &trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &operator[](int i) = 0;
  virtual const Tip &operator[](int i) const = 0;
  virtual ~Lista() {}
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip element;
    Cvor *prethodni;
    Cvor *sljedeci;
    Cvor(const Tip &el, Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr)
        : element(el), prethodni(prethodni), sljedeci(sljedeci) {}
  };
  Cvor *pocetak_liste;
  Cvor *kraj_liste;
  Cvor *trenutni_cvor;
  int broj_elemenata;

public:
  DvostrukaLista()
      : pocetak_liste(nullptr), kraj_liste(nullptr), trenutni_cvor(nullptr),
        broj_elemenata(0) {}
  DvostrukaLista(const DvostrukaLista<Tip> &original) {
    pocetak_liste = nullptr;
    kraj_liste = nullptr;
    trenutni_cvor = nullptr;
    broj_elemenata = 0;

    Cvor *original_cvor = original.pocetak_liste;
    while (original_cvor != nullptr) {
      dodajIza(original_cvor->element);
      original_cvor = original_cvor->sljedeci;
    }
  }

  // Kopirajući operator dodjele
  DvostrukaLista<Tip> &operator=(const DvostrukaLista<Tip> &original) {
    if (this == &original) {
      return *this; // Samododjela, ništa se ne mijenja
    }

    // Oslobađanje resursa trenutne liste
    while (pocetak_liste != nullptr) {
      Cvor *za_brisanje = pocetak_liste;
      pocetak_liste = pocetak_liste->sljedeci;
      delete za_brisanje;
    }

    pocetak_liste = nullptr;
    kraj_liste = nullptr;
    trenutni_cvor = nullptr;
    broj_elemenata = 0;

    Cvor *original_cvor = original.pocetak_liste;
    while (original_cvor != nullptr) {
      dodajIza(original_cvor->element);
      original_cvor = original_cvor->sljedeci;
    }

    // Postavljanje trenutnog čvora nove liste na početak
    trenutni_cvor = pocetak_liste;

    return *this;
  }

  int brojElemenata() const override { return broj_elemenata; }

  Tip &trenutni() override {
    if (broj_elemenata == 0 || trenutni_cvor == nullptr) {
      throw std::out_of_range("Trenutni element nije validan.");
    }
    return trenutni_cvor->element;
  }

  Tip &trenutni() const override {
    if (broj_elemenata == 0 || trenutni_cvor == nullptr) {
      throw std::out_of_range("Trenutni element nije validan.");
    }
    return trenutni_cvor->element;
  }

  bool prethodni() override {
    if (broj_elemenata <= 1 || trenutni_cvor == pocetak_liste) {
      return false;
    }
    trenutni_cvor = trenutni_cvor->prethodni;
    return true;
  }

  bool sljedeci() override {
    if (broj_elemenata == 0 || trenutni_cvor == nullptr ||
        trenutni_cvor->sljedeci == nullptr) {
      return false;
    }
    trenutni_cvor = trenutni_cvor->sljedeci;
    return true;
  }

  void pocetak() override { trenutni_cvor = pocetak_liste; }

  void kraj() override { trenutni_cvor = kraj_liste; }

  void obrisi() override {
    if (broj_elemenata == 0 || trenutni_cvor == nullptr) {
        throw std::out_of_range("Trenutni element nije validan.");
    }

    Cvor* sljedeci_cvor = trenutni_cvor->sljedeci;
    Cvor* prethodni_cvor = trenutni_cvor->prethodni;

    if (prethodni_cvor != nullptr) {
        prethodni_cvor->sljedeci = sljedeci_cvor;
    } else {
        pocetak_liste = sljedeci_cvor;
    }

    if (sljedeci_cvor != nullptr) {
        sljedeci_cvor->prethodni = prethodni_cvor;
    } else {
        kraj_liste = prethodni_cvor;
    }

    delete trenutni_cvor;
    broj_elemenata--;

    // Postavljanje trenutnog čvora na sledeći čvor ili na početak ako je lista prazna
    trenutni_cvor = (sljedeci_cvor != nullptr) ? sljedeci_cvor : pocetak_liste;
}


  void dodajIspred(const Tip &el) override {
    Cvor *novi_cvor = new Cvor(el);
    if (broj_elemenata == 0) {
      pocetak_liste = kraj_liste = trenutni_cvor = novi_cvor;
    } else if (trenutni_cvor->prethodni == nullptr) {
      novi_cvor->sljedeci = trenutni_cvor;
      trenutni_cvor->prethodni = novi_cvor;
      pocetak_liste = novi_cvor;
    } else {
      novi_cvor->sljedeci = trenutni_cvor;
      novi_cvor->prethodni = trenutni_cvor->prethodni;
      trenutni_cvor->prethodni->sljedeci = novi_cvor;
      trenutni_cvor->prethodni = novi_cvor;
    }
    broj_elemenata++;
  }

  void dodajIza(const Tip &el) override {
    Cvor *novi_cvor = new Cvor(el);
    if (broj_elemenata == 0) {
      pocetak_liste = kraj_liste = trenutni_cvor = novi_cvor;
    } else if (trenutni_cvor->sljedeci == nullptr) {
      novi_cvor->prethodni = trenutni_cvor;
      trenutni_cvor->sljedeci = novi_cvor;
      kraj_liste = novi_cvor;
    } else {
      novi_cvor->prethodni = trenutni_cvor;
      novi_cvor->sljedeci = trenutni_cvor->sljedeci;
      trenutni_cvor->sljedeci->prethodni = novi_cvor;
      trenutni_cvor->sljedeci = novi_cvor;
    }
    broj_elemenata++;
  }

  // Funkcije za dokazivanje ispravnosti
  void ispisiListu() const {
    Cvor *tekuci = pocetak_liste;
    while (tekuci != nullptr) {
      cout << tekuci->element << " ";
      tekuci = tekuci->sljedeci;
    }
    cout << endl;
  }

  void provjeriPoredak() const {
    Cvor *tekuci = pocetak_liste;
    while (tekuci != nullptr && tekuci->sljedeci != nullptr) {
      if (tekuci->sljedeci->prethodni != tekuci) {
        throw logic_error("Pogrešan poredak elemenata.");
      }
      tekuci = tekuci->sljedeci;
    }
  }

  ~DvostrukaLista() {
    while (pocetak_liste != nullptr) {
      Cvor *za_brisanje = pocetak_liste;
      pocetak_liste = pocetak_liste->sljedeci;
      delete za_brisanje;
    }
  }
  Tip &operator[](int i) override {
    if (i < 0 || i >= broj_elemenata) {
      throw std::out_of_range("Indeks van opsega.");
    }

    Cvor *tekuci_cvor = pocetak_liste;
    for (int j = 0; j < i; j++) {
      tekuci_cvor = tekuci_cvor->sljedeci;
    }

    return tekuci_cvor->element;
  }

  const Tip &operator[](int i) const override {
    if (i < 0 || i >= broj_elemenata) {
      throw std::out_of_range("Indeks van opsega.");
    }

    Cvor *tekuci_cvor = pocetak_liste;
    for (int j = 0; j < i; j++) {
      tekuci_cvor = tekuci_cvor->sljedeci;
    }

    return tekuci_cvor->element;
  }
};
template <typename Tip> Tip dajMaksimum(const Lista<Tip> &n) {
  if (n.brojElemenata() == 0) {
    throw logic_error("Lista je prazna.");
  }

  Tip maksimum = n[0];

  for (int i = 1; i < n.brojElemenata(); i++) {
    if (n[i] > maksimum) {
      maksimum = n[i];
    }
  }

  return maksimum;
}

template <typename Tip>
void testDvostrukaLista() {
    DvostrukaLista<Tip> lista;

    std::cout << "Dodavanje elemenata: ";
    for (int i = 1; i <= 5; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    lista.pocetak();
    lista.sljedeci();
    lista.sljedeci();
    std::cout << "Trenutni element nakon pomicanja: " << lista.trenutni() << std::endl;

    std::cout << "Brisanje trenutnog elementa: ";
    lista.obrisi();
    for (int i = 0; i < lista.brojElemenata(); i++)
        std::cout << lista[i] << " ";
    std::cout << std::endl;

    std::cout << "Broj elemenata nakon brisanja: " << lista.brojElemenata() << std::endl;
}

template <typename Tip>
void testDajMaksimum() {
    DvostrukaLista<Tip> lista;

    std::cout << "Dodavanje elemenata: ";
    for (int i = 1; i <= 10; i++) {
        lista.dodajIza(i);
    }

    try {
        std::cout << "Maksimum u listi: " << dajMaksimum(lista) << std::endl;
    } catch (const std::logic_error& e) {
        std::cerr << "Izuzetak uhvaćen: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "Testiranje klase DvostrukaLista:" << std::endl;
    testDvostrukaLista<int>();

    std::cout << "\nTestiranje funkcije dajMaksimum:" << std::endl;
    testDajMaksimum<int>();

    return 0;
}
