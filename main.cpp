#include<stdio.h>
#include<iostream>
#include<fstream>
#include<map>
#include<string>

using namespace std;

int main()
{
    
    string dir = "data/dna.txt";
    map<char, int> freq;
    fstream texto;
    texto.open(dir,ios::in);
    string linea;
    int n;
    if (texto.is_open()){
      while(getline(texto, linea)){
        n=linea.length();
        for(int i=0;i<n;i++){
            try{
                freq[linea[i]]=freq[linea[i]]+1;
            }
            catch(exception e){
                freq[linea[i]]=0;
            }
        }
      }
      texto.close();
    }


    int pesoTotal=0;
    for (auto element : freq){
       pesoTotal+=element.second;
    }
    for (auto element :freq){
        printf("Caracter: %c\tFrecuencia:%i\tPorcentaje:%.e\n",element.first,element.second,((double)element.second)/pesoTotal);
    }
    
}