
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct node {
 
	char sym; //caracteres
	float pro; //probabilidad 
  string codigo="";
};

int comparaFreq(const void *simbolo1,const void *simbolo2);
void printSimbolos(node *p, int n);
void Codificar(string DataNOM);
void shannonF(int l, int h, node *p);

int main(int argc, char **argv){
	if(argc != 2){
		cout << "Error. Debe ejecutarse como ./Tarea archivoNombre.txt " << endl;
		exit(EXIT_FAILURE);
	}
	string DataNOM = argv[1];

	Codificar(DataNOM);
}

void Codificar(string DataNOM){
	string dir = "data/" + DataNOM; 	// direccion local de la cual se buscara el texto
  map<char, int> freq;  			  // se crea el diccionario de frecuencias clave: char, valor: int
  fstream texto;  				// instrucciones para abrir el archivo en modo lectura
  texto.open(dir,ios::in);
  string linea;  					 //variables para la lectura del archivo
  int i,n,z;

    if (texto.is_open()){ 		 	 // si el archivo se abrio (validacion para no tener errores)
      while(getline(texto, linea)){ //un while que itera por las lineas del texto dejandolas en la variable linea
        n=linea.length();
        for(int i=0;i<n;i++){  		 //se recorre el string de largo n
            try{
                freq[linea[i]]=freq[linea[i]]+1;    //se le anade +1 al valor del char que se esta leyendo si es que existe clave:valor (try)
            }
            catch(exception e){
                freq[linea[i]]=1;                   //se crea una clave:valor para el char que todavia no tiene (exception)
            }
        }
      }
      texto.close();    						   //se cierra el texto a leer
    }

  int pesoTotal=0;
  for (auto element :freq){ 					 //se itera por key:value del diccionario freq element.first = key, element.second =value
    pesoTotal+=element.second;   			// element.second es la frecuencia (valor) y se suma a pesoTotal para tener el numero total de caracteres
  }
  
  n=freq.size();
  node *p = new node[n];
  i=0;
  for (auto element:freq){ 			
    p[i].sym=element.first;
		p[i].pro=((float)element.second)/pesoTotal;
		i++; 		
  }

  qsort(p, n, sizeof(node), comparaFreq);
  shannonF(0, n-1, p);
  printSimbolos(p, n);
  /////////////////////////////////////////////
  ofstream outfile ("codificado.txt");

  texto.open(dir,ios::in);
  if (texto.is_open()){ 		 	 // si el archivo se abrio (validacion para no tener errores)
      while(getline(texto, linea)){ //un while que itera por las lineas del texto dejandolas en la variable linea
        n=linea.length();  			 // en n se almacena el largo de linea
        for(int i=0;i<n;i++){  		 //se recorre el string de largo n
            for(z=0; linea[i] != p[z].sym; z++){}
            outfile << p[z].codigo;
        }
      }
      outfile << endl;
      texto.close();    						   //se cierra el texto a leer
  }
  outfile.close();
};

int comparaFreq(const void *simbolo1,const void *simbolo2){
  float prob1 = (*(node*)simbolo1).pro;
  float prob2 = (*(node*)simbolo2).pro;

  if (prob1 < prob2) return 1;
  else if(prob1 > prob2) return -1;
  return 0;
};

void printSimbolos(node *p, int n){
  for (int i=0; i<n; i++){ 			
    cout << "Símbolo: " << p[i].sym 
    << " Porcentaje: " << p[i].pro
    << " Codigo: " << p[i].codigo
    << endl;
  }
};

void shannonF(int l, int r, node *p){ 
  if(l==r) { return; }
  else if(r-l == 1){
    p[l].codigo= p[l].codigo + "1";
    p[r].codigo= p[r].codigo + "0";
  }
  else {
  float sumaT=0, sumaP=p[l].pro;
  int i,z;

  for (i=l; i<=r; i++) {    sumaT += p[i].pro;  }

  for (i=l; sumaP<=(sumaT/2);) {
    p[i].codigo= p[i].codigo + "1";
    i++;
    sumaP += p[i].pro;
  }

  for (z=i; z<=r ;z++) {
    p[z].codigo= p[z].codigo + "0";
  }

  shannonF(l, i-1, p);
  shannonF(i, r, p);
  }
};
















