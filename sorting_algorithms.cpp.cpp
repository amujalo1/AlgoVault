#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iterator>

using namespace std;


template <typename Tip>
void selection_sort(Tip *niz, int vel){
    Tip min;
    int pmin;
    for(int i=0; i<vel-1;i++){
        min=niz[i];
        pmin=i;
        for(int j=i+1; j<vel; j++){
            if(niz[j]<min){
                min=niz[j];
                pmin=j;
            }
        }
        niz[pmin]=niz[i];
        niz[i]=min;
    }
}


template <typename Tip>
void bubble_sort(Tip *niz, int vel){
    
    for(int i=vel; i>1; i--){
        for(int j=1; j<i; j++){
          if(niz[j-1]>niz[j]){
              Tip temp=niz[j-1];
              niz[j-1]=niz[j];
              niz[j]=temp;
          }  
        }
    }
    
}


template <typename Tip>
int particija (Tip *niz, int prvi, int zadnji){
    Tip pivot(niz[prvi]);
    int p(prvi+1);
    while (p<=zadnji && niz[p]<=pivot){
        p=p+1;
    }
    
    for(int i=p+1; i<=zadnji; i++){
        if(niz[i]<pivot){
            Tip temp(niz[p]);
            niz[p]=niz[i];
            niz[i]=temp;
            p++;
        }
    }
    
    Tip temp(niz[prvi]);
    niz[prvi]=niz[p-1];
    niz[p-1]=temp;
    return p-1;
}

template <typename Tip>
void Pomocna(Tip *niz, int prvi, int zadnji){   
    if(prvi<zadnji){
        int j(particija(niz,prvi,zadnji));
        Pomocna(niz,prvi,j-1);
        Pomocna(niz,j+1,zadnji);
    }
    
} 


template <typename Tip>
void quick_sort(Tip *niz, int vel){
    Pomocna(niz,0,vel-1);
}

template <typename Tip>
void merge(Tip *niz, int l, int p, int q, int u){
    int i(0), j(q-l), k(l);
    Tip *nizB = new Tip[u-l+1];
    for(int m=0; m<=u-l; m++){
        nizB[m]=niz[l+m];
    }
    
    while(i<=p-l && j<=u-l){
        if(nizB[i]<nizB[j]){
            niz[k]=nizB[i];
            i++;
        }else{
            niz[k]=nizB[j];
            j++;
        }
        k++;
    }
    
    while(i<=p-l){
        niz[k]=nizB[i];
        k++;
        i++;
    }
    
    while(j<=u-l){
        niz[k]=nizB[j];
        k++;
        j++;
    }
    delete[] nizB;
    
    
}


template <typename Tip>
void PomocnaMS(Tip *niz, int l, int u){
    if(l<u){
        int p((l+u-1)/2);
        int q(p+1);
        PomocnaMS(niz,l,p);
        PomocnaMS(niz,q,u);
        merge(niz,l,p,q,u);
        
    }
}

template <typename Tip>
void merge_sort(Tip *niz, int vel){
    PomocnaMS(niz, 0, vel-1);
}

int prodjiKrozDatoteku(string filename){
vector<int>veca;
int broj;
ifstream datoteka;
datoteka.open(filename.c_str());
while (!datoteka.eof()) {
    datoteka>>broj;
    veca.push_back(broj);
    
}
datoteka.close();
return veca.size();
}

void ucitaj(string filename, int *&niz, int &vel){
    ifstream datoteka;
    niz=new int[prodjiKrozDatoteku(filename)];
    datoteka.open(filename.c_str());
    vel=0;
    if(datoteka.is_open()){
        while (!datoteka.eof()) 
            datoteka>>niz[vel];
            vel++;
    }
        vel--;
        datoteka.close();
    }
    
    void generisi(string filename, int vel){
        ofstream datoteka;
        srand(time(NULL));
        datoteka.open(filename.c_str());
        for(int i=0; i<vel; i++){
            datoteka<<rand() % 1000<<" ";
        }
        datoteka.close();
    }
    void zapisi(string filename, int *niz, int vel){
        ofstream datoteka;
        datoteka.open(filename.c_str());
        for(int i=0; i<vel; i++){
            datoteka<<niz[i]<<" ";
        }
        datoteka.close();
    }
    



int main() {
    bool sortiran=true;
    int *niz;
    int vel=10000;
    clock_t t1,t2;
    generisi("datoteka.txt",vel);
    ucitaj("datoteka.txt",niz,vel);
    int izbor;
    std::cout<<"Izaberite algoritam sortiranja";
    for(;;){
        std::cin>>izbor;
        if(izbor==1){
            t1=clock();
            bubble_sort(niz,vel);
            t2=clock();
            break;
        }
        else if(izbor==2){
            t1=clock();
            selection_sort(niz,vel);
            t2=clock();
            break;
        }
        else if(izbor==3){
            t1=clock();
            quick_sort(niz,vel);
            t2=clock();
            break;
        }
        else if(izbor==4){
            t1=clock();
            merge_sort(niz,vel);
            t2=clock();
            break;
        }
        else{
            cout<<"Pogresan izbor, pokusajte ponovo";
            continue;
        }
        
        
    }
    
    for(int i=0; i<vel-1;i++)
        if(niz[i]>niz[i+1]) sortiran=false;
    
    if(sortiran) cout<<"Niz je sortiran.";
    else cout<<"Niz nije sortiran.";
    
    cout<<"Vrijeme sortiranja: "<<((float)t2-(float)t1);
    zapisi("ime datoteke",niz,vel);
    delete[] niz;
    
    return 0;
}