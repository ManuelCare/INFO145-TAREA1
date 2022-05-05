#include<stdio.h>
#include<iostream>
#include<fstream>
#include<map>
#include<string>

using namespace std;

int main()
{

    string dir = "data/dna.txt"; // direccion local de la cual se buscara el texto
    map<char, int> freq;    // se crea el diccionario de frecuencias clave: char, valor: int
    fstream texto;  // instrucciones para abrir el archivo en modo lectura
    texto.open(dir,ios::in);
    string linea;   //variables para la lectura del archivo
    int n;
    if (texto.is_open()){   // si el archivo se abrio (validacion para no tener errores)
      while(getline(texto, linea)){ //un while que itera por las lineas del texto dejandolas en la variable linea
        n=linea.length();   // en n se almacena el largo de linea
        for(int i=0;i<n;i++){   //se recorre el string de largo n
            try{
                freq[linea[i]]=freq[linea[i]]+1;    //se le anade +1 al valor del char que se esta leyendo si es que existe clave:valor (try)
            }
            catch(exception e){
                freq[linea[i]]=1;                   //se crea una clave:valor para el char que todavia no tiene (exception)
            }
        }
      }
      texto.close();       //se cierra el texto a leer
    }


    int pesoTotal=0;
    for (auto element : freq){  //se itera por key:value del diccionario freq element.first = key, element.second =value
       pesoTotal+=element.second;   // element.second es la frecuencia (valor) y se suma a pesoTotal para tener el numero total de caracteres
    }
    for (auto element :freq){   //se itera nuevamente ahora para mostrar char, frecuencia, porcentaje
        printf("Caracter: %c\tFrecuencia:%i\tPorcentaje:%.e\n",element.first,element.second,((double)element.second)/pesoTotal);
    }
    
}