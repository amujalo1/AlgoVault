#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
void radixSort(vector<int> &a) {
  int max = *max_element(a.begin(), a.end());
  int maxBrCifara = 0;
  while (max != 0) {
    max /= 10;
    maxBrCifara++;
  }
  vector<vector<int>> Q(10);
  for (int i = 0; i < maxBrCifara; i++) {
    int stepen = pow(10, i);
    for (int m = 0; m < Q.size(); m++)
        Q.at(m).clear();
    for (int j = 0; j < a.size(); j++)
      Q[(a[j] % (10 * stepen)) / stepen].push_back(a[j]);
    a.clear();
    for (int k = 0; k < Q.size(); k++) {
      for (int l = 0; l < Q[k].size(); l++)
        a.push_back(Q[k][l]);
    }
  }
}
void popraviDolje(vector<int> &a, int i, int vel);
void stvoriGomilu(vector<int> &a) {
    int vel = a.size();
    for(int i = (vel/2); i >= 0; i--)
        popraviDolje(a,i,a.size());
}
void popraviDolje(vector<int> &a, int i, int vel){
    while(!(i>=(vel/2) && i<vel)){
        int veci = 2*i+1;
        int dd = 2*i+2;
        if(dd<vel && a[dd]>a[veci])
            veci = dd;
        if(a[i] > a[veci])
            return;
        swap(a[i], a[veci]);
        i=veci;
    }
}
void popraviGore(vector<int> &a, int i);
void umetniUGomilu(vector<int> &a,int umetnuti, int&vel){
    if(vel > a.size())
        throw range_error("Gomila je puna");
    a.push_back(umetnuti);
    vel++;
    popraviGore(a,vel-1);
} 
void popraviGore(vector<int> &a, int i){
    while(i!=0 && a[i]>a[(i-1)/2]){
        swap(a[i], a[(i-1)/2]);
        i=(i-1)/2;
    }
}
int izbaciPrvi(vector<int> &a, int &vel){
    if(a.size() == 0)
        throw range_error("Gomila je puna");
    vel--;
    swap(a[0], a[vel]);
    if(vel != 0)
        popraviDolje(a, 0, vel);
    return a[vel];
}
void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int vel = a.size();
    for(int i = a.size()-1; i > 0; i--) {
        swap(a[0], a[i]);
        vel--;
        popraviDolje(a, 0, vel);
    } 
}
int main() {
  vector<int> data(
      {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625});
  radixSort(data);
  for (int n : data)
    cout << n << " ";
    cout << endl;
    vector<int> v;
    int vel = 0;
    umetniUGomilu(v, 123, vel);
    umetniUGomilu(v, 456, vel);
    umetniUGomilu(v, 789, vel);
    umetniUGomilu(v, 415, vel);
    umetniUGomilu(v, 742, vel);
    umetniUGomilu(v, 252, vel);
    izbaciPrvi(v, vel);
    for(auto element: v){
        cout<<element<<" ";
    }
}