#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include<ctime>

using namespace std;

/*ESTRUCTURAS*/

//ARBOL
/*
Nodo del arbol 
Contiene los imbolos, su freciencia y los punteros a la izq y derecha 
*/
struct Nodo{
    char simb; //simbolo
    double freq; //frecuencia
    Nodo *izq, *der; //Nodos de izquierda y derecha
};

/*
Toma la izq y la derecha y retorna la frecuencia más baja en la cola de prioridad
*/
struct comp {
    bool operator()(Nodo* i, Nodo* d){
        return i->freq > d->freq;
    };
};





/*Definición de funciones*/
Nodo* getNodo(char simb, double freq, Nodo* izq, Nodo* der);
void encode(Nodo* raiz, string str, unordered_map<char, string> &CodigoHuffman);
void decode(Nodo* raiz, int &index, string str);
void creaArbol(string text,int start, int end);



/*  MAIN  */


int main(){

    string text ="data/dna.txt";
    /*En ambos textos hay 104857601 caracteres*/
    creaArbol(text,1,520);

    return 0;
}





//Asigna nuevos nodos al arbol
Nodo* getNodo(char simb, double freq, Nodo* izq, Nodo* der){       //Recibe símbolo, frecuencia, su nodo izquierdo y derecho
    Nodo* nodo = new Nodo();

    nodo->simb = simb;
    nodo->freq = freq;
    nodo->izq = izq;
    nodo->der = der;
    return nodo;
};

//Asigna 1 y 0 a cada simbolo y los guarda en un map
void encode(Nodo* raiz, string str, unordered_map<char, string> &CodigoHuffman){    
    if(raiz ==nullptr)
        return;
    if(!raiz->izq && !raiz->der){
        CodigoHuffman[raiz->simb] = str;
    }
    encode(raiz->izq, str+"0", CodigoHuffman);
    encode(raiz->der, str+"1", CodigoHuffman);
}



//Revisa el arbol y decodifica los valores
void decode(Nodo* raiz, int &index, string str){
    if(raiz==nullptr){
        return;
    }
    if(!raiz->izq && !raiz->der){
        //cout<< raiz->simb; //Se uso para asegurar que decodificaba bien
        return;
    }
    index++;
    if(str[index]=='0')
        decode(raiz->izq, index, str);
    else
        decode(raiz->der, index, str);

}

void creaArbol(string text,int start, int end){
    unsigned t0, t1;
    cout<<"Creacion del contador (Calcula cuantos digitos hay en el texto) y Calculo de frecuencias de los distintos digitos"<<endl;
    t0=clock();
    long long contador = 0;
    string dir = text; // direccion local de la cual se buscara el texto
    unordered_map<char, int> freqFromText;    // se crea el diccionario de frecuencias clave: char, valor: int
    fstream texto;
    texto.open(dir,ios::in);
    string linea;   //variables para la lectura del archivo
    int n;
    if (texto.is_open()){   // si el archivo se abrio (validacion para no tener errores)
        while(getline(texto, linea)){ //un while que itera por las lineas del texto dejandolas en la variable linea
            n=linea.length();   // en n se almacena el largo de linea
            for(int i=0;i<n;i++){   //se recorre el string de largo n
                try{
                    freqFromText[linea[i]]=freqFromText[linea[i]]+1;    //se le anade +1 al valor del char que se esta leyendo si es que existe clave:valor (try)
                    contador = contador +1;
                }
                catch(exception e){
                    freqFromText[linea[i]]=1;                   //se crea una clave:valor para el char que todavia no tiene (exception)
                }
            }
            freqFromText['\n']=freqFromText['\n']+1; 
            contador = contador +1;

        }
        texto.close();       //se cierra el texto a leer


    }
    t1=clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<"Demoro: "<<time<<" Segundos en Calcular cuantos digitos hay y su frecuencia"<<endl;

    cout << "Hay "<< contador<<" Caracteres en el texto original"<<endl; //Cuenta los carácteres en el array


    t0=clock();
    cout<<"Se crea un arreglo que almacena el caracter y su posicion en el archivo codificado, asi mismo se crea el arbol para el codigo huffman"<<endl;
    int* posiciones = new int[contador]; //Crea array con las posiciones

    priority_queue<Nodo*, vector<Nodo*>, comp> pq;   //Cola de prioridad que guarda los nodos del arbol
    for(auto pair: freqFromText){ //----------------
        pq.push(getNodo(pair.first, pair.second, nullptr, nullptr));   //Pone los caracterer en la cola y dsp su frecuencia
    }

    while(pq.size() != 1){
        Nodo *izq = pq.top(); pq.pop();
        Nodo *der = pq.top(); pq.pop();
        int sum = izq->freq + der->freq;
        pq.push(getNodo('/0',sum,izq,der));

    }

    Nodo* raiz = pq.top();

    unordered_map<char,string> CodigoHuffman;

    encode(raiz, "", CodigoHuffman);
    // cout<<" Los codigos de Huffman son: \n";
    // for(auto pair: CodigoHuffman){
    //     cout<<pair.first << " " << pair.second << " \n";
    // } 
    ofstream file; //Crea archivo codificado
    file.open("data/codificadoHuffman.txt"); //En esta ruta guardara el archivo codificado y con ese nombre

    fstream texto2;  // instrucciones para abrir el archivo en modo lectura
    texto2.open(dir,ios::in);
    string linea1;
    int ntam;   //Al hacer make sale que no se ocupa, pero si, un poco mas abajo pero en otro contexto.  
    int posEnArray = 1;
    int tamanoAcumulado = 0;
    posiciones[0]=0; //Ya que en la pos 0 empieza el primer simbolo
    if(texto2.is_open()){
        while(getline(texto2,linea1)){
            string str="";
            ntam=linea1.length();
            for(char simbolo: linea1){
                str+=CodigoHuffman[simbolo];
                tamanoAcumulado = tamanoAcumulado+CodigoHuffman[simbolo].size();
                posiciones[posEnArray]=tamanoAcumulado;
                posEnArray++;
            }
            str+=CodigoHuffman['\n'];
            tamanoAcumulado = tamanoAcumulado+CodigoHuffman['\n'].size();
            posiciones[posEnArray]=tamanoAcumulado;
            posEnArray++;

            file<<str;
        }
    }
    cout<<"Cantidad de caracteres en el archivo codificado: "<<tamanoAcumulado<<endl;

    file.close(); //Cierra archivo reescrito codificado
    texto.close();       //se cierra el texto a leer

    int inicioSeccion = posiciones[start-1];
    int finalSeccion = posiciones[end+1];
    delete posiciones;  //Elimina el arreglo, ya que al ser tan grande se necesita liberar la memoria usada
    t1=clock();
    time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<"Se demoro: "<<time<<" Segundos en crear un arreglo con las posiciones y calcular huffman"<<endl;

    cout<<"---Empieza DECODE por secciones ----"<<endl;
    cout<<"Desde: "<<start<<" Hasta: "<<end<<" En el archivo original"<<endl;
    cout<<"Donde en el archivo codificado es desde el: "<<inicioSeccion<<" Hasta: "<<finalSeccion<<endl;
    cout<<"------------------------------------"<<endl;
    t0=clock();
   /////////////----------------Asignación de posiciones para la decodificacion por rangos---------////////////////

    string direccion = "data/codificadoHuffman.txt";
    fstream texto3; //Texto codificado a descodificar
    texto3.open(direccion,ios::in);
    string linea3;

    if(texto3.is_open()){
        while(getline(texto3,linea3)){
            string str1 = linea3;
            int index = inicioSeccion-1;
// while(index<(int)str1.size()-2)  Se uso para decodificar tooodo el texto, se deja guardado para posibles experimentos
            while(index<finalSeccion-2){
                decode(raiz,index,str1);
            }
        }
    }
    texto3.close();

    t1=clock();
    time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<"Se demoro: "<<time<<" En decodificar los caracteres desde "<<start<<" hasta "<<end<<" ("<<end-start<<" caracteres en total)."<<endl;
}





