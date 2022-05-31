#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include<ctime>
using namespace std;

#define DIVISION_AUXILIAR 12 // 2^division_auxiliar

struct node
{

  char sym;  // caracteres
  float pro; // probabilidad
  string codigo = "";
};

int comparaFreq(const void *simbolo1, const void *simbolo2);
void printSimbolos(node *p, int n);
void Codificar(string DataNOM);
void Decodificar();
void Decodificar(int u, int k);
void shannonF(int l, int h, node *p);
void exportarCodigos(node *p, int n);
int potencia(int base, int exponente);
long int *importarAuxiliar(int &nAux, long int &dist);

int main(int argc, char **argv)
{
  if (argc != 2 && argc!=4)
  {
    cout << "Error. Debe ejecutarse como ./ShannonFano archivoNombre.txt " << endl;
    cout << "Error. Debe ejecutarse como ./ShannonFano archivoNombre.txt inicio fin" << endl;
    exit(EXIT_FAILURE);
  }
  string DataNOM = argv[1];
  unsigned t0,t1,t2,t3;
  if(argc == 4){
    int l = atoi(argv[2]), r=atoi(argv[3]);
    
    t0=clock();
    Codificar(DataNOM);
    t1=clock();
    double timeCod = (double(t1-t0)/CLOCKS_PER_SEC);
    printf("tiempo de codificacion: %f[Segundos]\n",timeCod);
    

    t2=clock();
    Decodificar(l,r);
    t3=clock();
    double timeDec = (double(t3-t2)/CLOCKS_PER_SEC);
    printf("tiempo de decodificacion: %f[Segundos]\n",timeDec);
  }
  if(argc==2){
    
    t0=clock();
    Codificar(DataNOM);
    t1=clock();
    double timeCod = (double(t1-t0)/CLOCKS_PER_SEC);
    printf("tiempo de codificacion: %f[Segundos]\n",timeCod);
    
    t2=clock();
    Decodificar();
    t3=clock();
    double timeDec = (double(t3-t2)/CLOCKS_PER_SEC);
    printf("tiempo de decodificacion: %f[Segundos]\n",timeDec);
    
  }
}
void Codificar(string DataNOM)
{
  unsigned t4,t5;
  t4=clock();
  string dir = "data/" + DataNOM, linea; // direccion local de la cual se buscara el texto
  map<char, int> freq;                   // se crea el diccionario de frecuencias clave: char, valor: int
  fstream texto;                         // instrucciones para abrir el archivo en modo lectura
  texto.open(dir, ios::in);
  int i, n, z;
  if (texto.is_open())
  { // si el archivo se abrio (validacion para no tener errores)
    while (getline(texto, linea))
    { // un while que itera por las lineas del texto dejandolas en la variable linea
      n = linea.length();
      for (i = 0; i < n; i++)
      { // se recorre el string de largo n
        try
        {
          freq[linea[i]] = freq[linea[i]] + 1; // se le anade +1 al valor del char que se esta leyendo si es que existe clave:valor (try)
        }
        catch (exception e)
        {
          freq[linea[i]] = 1; // se crea una clave:valor para el char que todavia no tiene (exception)
        }
      }
    }
    texto.close(); // se cierra el texto a leer
  }
  t5=clock();
  double timeFreq = (double(t5-t4)/CLOCKS_PER_SEC);
  printf("tiempo de conteo: %f[Segundos]\n",timeFreq);
  int pesoTotal = 0;
  for (auto element : freq)
  {                              // se itera por key:value del diccionario freq element.first = key, element.second =value
    pesoTotal += element.second; // element.second es la frecuencia (valor) y se suma a pesoTotal para tener el numero total de caracteres
  }
  n = freq.size();
  node *p = new node[n]; // crear el arreglo p con char y freq
  i = 0;
  for (auto element : freq)
  {
    p[i].sym = element.first;
    p[i].pro = ((float)element.second) / pesoTotal;
    i++;
  }

  // qsort sizeof(node) => tamano de bits que ocupa la estructura node
  qsort(p, n, sizeof(node), comparaFreq); // (1) queda p ordenado descendentemente en base a freq(p) de cada char
  shannonF(0, n - 1, p);
  printSimbolos(p, n);

  ofstream TextoOutput("codificado.txt");
  exportarCodigos(p, n);

  texto.open(dir, ios::in);

  FILE *archivoAuxiliar;
  archivoAuxiliar = fopen("auxiliar.txt", "wb");
  long cont = 0;
  int particiones = potencia(2, DIVISION_AUXILIAR);
  long contMax = (long)(pesoTotal / particiones) + 1;
  long contBit = 0;
  fprintf(archivoAuxiliar, "%li %i", contMax, particiones - 1);

  if (texto.is_open())
  { // si el archivo se abrio (validacion para no tener errores)
    while (getline(texto, linea))
    {                     // un while que itera por las lineas del texto dejandolas en la variable linea
      n = linea.length(); // en n se almacena el largo de linea
      for (i = 0; i < n; i++)
      { // se recorre el string de largo n
        for (z = 0; linea[i] != p[z].sym; z++)
          ;
        cont += 1;
        if (cont == contMax)
        {
          fprintf(archivoAuxiliar, "\n%li", contBit + 1);
          cont = 0;
        }
        TextoOutput << p[z].codigo;
        contBit += p[z].codigo.length();
      }
      TextoOutput << endl;
    }
    texto.close(); // se cierra el texto a leer
  }

  TextoOutput.close();
  fclose(archivoAuxiliar);
  /////////////////////////////////////////////
};

void Decodificar()
{
  int i, n, j, z;
  FILE *CodigosImport;
  string dir = "codificado.txt", linea, CodigoSUM, codificado; //
  fstream texto;

  CodigosImport = fopen("Codigos.txt", "r");
  fscanf(CodigosImport, "%i", &n);
  fgetc(CodigosImport);
  node *p = new node[n];

  for (i = 0; i < n; i++)
  {
    fscanf(CodigosImport, "%c %f %s", &(p[i].sym), &(p[i].pro), (codificado).c_str());
    p[i].codigo = (codificado).c_str();
    fgetc(CodigosImport);
  }
  // printSimbolos(p, n);
  /////////////////////////////////////////////
  ofstream TextoOutput("decodificado.txt");
  texto.open(dir, ios::in);
  if (texto.is_open())
  { // si el archivo se abrio (validacion para no tener errores)
    while (getline(texto, linea))
    {                     // un while que itera por las lineas del texto dejandolas en la variable linea
      j = linea.length(); // en n se almacena el largo de linea
      CodigoSUM = "";
      for (i = 0; i < j; i++)
      { // se recorre el string de largo n
        CodigoSUM += linea[i];
        for (z = 0; z < n; z++)
        {
          if (CodigoSUM == p[z].codigo)
          {
            TextoOutput << p[z].sym;
            CodigoSUM = "";
            break;
          }
        }
      }
      TextoOutput << endl;
    }
    texto.close(); // se cierra el texto a leer
  }
  TextoOutput.close();
  /////////////////////////////////////////////
};

int comparaFreq(const void *simbolo1, const void *simbolo2)
{
  float prob1 = (*(node *)simbolo1).pro; // probabilidad simbolo 1
  float prob2 = (*(node *)simbolo2).pro; // probabilidad simbolo 2

  // si son iguales queda igual
  // devuelve un bool si p1 < p2 (positivo)
  // devuelve un bool si p1 > p2 (negativo)
  if (prob1 < prob2)
    return 1;
  else if (prob1 > prob2)
    return -1;
  return 0;
};

void printSimbolos(node *p, int n)
{
  for (int i = 0; i < n; i++)
  {
    printf("Símbolo: %c\tPorcentaje: %f\tCodigo: %s\n", p[i].sym, p[i].pro, (p[i].codigo).c_str());
  }
};

void shannonF(int l, int r, node *p)
{
  float sumaT = 0, sumaP = p[l].pro, sumaMitad;
  int i, z;
  for (i = l; i <= r; i++)
  {
    sumaT += p[i].pro;
  }
  sumaMitad = sumaT * 0.5f;
  if (l == r)
  {
    return;
  } // l y r son la posicion de intervalo izq y intervalo der
  // si son iguales retorna
  else if (r - l == 1)
  {
    p[l].codigo += "1"; // si hay dos elementos en el intervalo [l,r]
    p[r].codigo += "0"; // le asigna un 1 al izq y un 0 al derecho
  }

  else
  {
    for (i = l; i <= r;)
    {
      p[i].codigo = p[i].codigo + "0";
      i++;
      sumaP += p[i].pro;
      // cout<< p[i].codigo <<" p[i]"<< endl;
      if (sumaP >= sumaMitad)
        break;
    }
    for (z = i; z <= r; z++)
    {
      p[z].codigo = p[z].codigo + "1";
      // cout<< p[z].codigo << "p[z] "<< endl;
    }
    shannonF(l, i - 1, p);
    shannonF(i, r, p);
  }
};

void exportarCodigos(node *p, int n)
{
  FILE *TablaCodigos;
  TablaCodigos = fopen("Codigos.txt", "wb");

  fprintf(TablaCodigos, "%i\n", n);
  for (int i = 0; i < n; i++)
  {
    fprintf(TablaCodigos, "%c\t%f\t%s\n", p[i].sym, p[i].pro, (p[i].codigo).c_str());
  }
  fclose(TablaCodigos);
};

int potencia(int base, int exponente)
{
  int res = base;
  for (int i = 2; i <= exponente; i++)
  {
    res *= base;
  }
  return res;
}
void Decodificar(int u, int k)
{
  if (k < u)
  {
    printf("Error de entrada fin<inicio");
    return;
  }
  int i, n, j, z;
  FILE *CodigosImport;
  string dir = "codificado.txt", linea, CodigoSUM, codificado; //
  fstream texto;

  CodigosImport = fopen("Codigos.txt", "r");
  fscanf(CodigosImport, "%i", &n);
  fgetc(CodigosImport);

  int nAux;
  long dist;
  long int *arregloAuxiliar = importarAuxiliar(nAux, dist);

  long int indiceAux = ((int)(u / dist) > 1) ? arregloAuxiliar[(int)(u / dist) - 1] : 0;
  int symFaltantes = (int)(u - 1 - (int)(u / dist) * dist)+1;
  int symALeer = k - u + 1;
  node *p = new node[n];

  for (i = 0; i < n; i++)
  {
    fscanf(CodigosImport, "%c %f %s", &(p[i].sym), &(p[i].pro), (codificado).c_str());
    p[i].codigo = (codificado).c_str();
    fgetc(CodigosImport);
  }
  // printSimbolos(p, n);
  /////////////////////////////////////////////

  ofstream TextoOutput("decodificado.txt");

  // comentado 1

  texto.open(dir, ios::in);
  long int index;

  getline(texto, linea);
  j = linea.length();
  while (texto.is_open() && indiceAux - j > 0)
  {
    indiceAux = indiceAux - j;
    getline(texto, linea);
    j = linea.length();
  }

  index = indiceAux;

  CodigoSUM = "";
  while (texto.is_open() && symFaltantes > 0)
  {
    CodigoSUM += linea[index];
    for (z = 0; z < n; z++)
    {
      if (CodigoSUM == p[z].codigo)
      {
        symFaltantes = symFaltantes - 1;
        CodigoSUM = "";
      }
    }
    index = index + 1;
    if (index == j)
    {
      index = 0;
      getline(texto, linea);
      j = linea.length();
    }
  }

  CodigoSUM = "";
  while (texto.is_open() && symALeer > 0)
  {
    CodigoSUM += linea[index];
    for (z = 0; z < n; z++)
    {
      if (CodigoSUM == p[z].codigo)
      {
        TextoOutput << p[z].sym;
        symALeer = symALeer - 1;
        CodigoSUM = "";
      }
    }
    index = index + 1;
    if (index == j)
    {
      index = 0;
      getline(texto, linea);
      j = linea.length();
    }
  }
  texto.close();
  TextoOutput.close();
  /////////////////////////////////////////////
}
long int *importarAuxiliar(int &nAux, long int &dist)
{
  FILE *auxiliar;
  auxiliar = fopen("auxiliar.txt", "r");
  long temp;
  fscanf(auxiliar, "%li %i", &dist, &nAux);
  fgetc(auxiliar);

  long int *V = new long[nAux];
  for (int i = 0; i < nAux; i++)
  {
    fscanf(auxiliar, "%li", &temp);
    fgetc(auxiliar);
    V[i] = temp;
  }
  fclose(auxiliar);
  return V;
}