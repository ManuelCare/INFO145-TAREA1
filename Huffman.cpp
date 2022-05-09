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

/*
Nodo del arbol 
Contiene los imbolos, su freciencia y los punteros a la izq y derecha 
*/
struct Nodo{
    char simb; //simbolo
    double freq; //frecuencia
    Nodo *izq, *der; //Nodos de izquierda y derecha
};



//Asigna nuevos nodos al arbol
Nodo* getNodo(char simb, double freq, Nodo* izq, Nodo* der){       //Recibe símbolo, frecuencia, su nodo izquierdo y derecho
    Nodo* nodo = new Nodo();

    nodo->simb = simb;
    nodo->freq = freq;
    nodo->izq = izq;
    nodo->der = der;
    return nodo;
};
/*
Toma la izq y la derecha y retorna la frecuencia más baja en la cola de prioridad
*/
struct comp {
    bool operator()(Nodo* i, Nodo* d){
        return i->freq > d->freq;
    };
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
        cout<< raiz->simb;
        return;
    }
    index++;
    if(str[index]=='0')
        decode(raiz->izq, index, str);
    else
        decode(raiz->der, index, str);

}


void creaArbol(string text, int s, int e){
    // se recibe s (start), que es el valor del caracter dentro del texto donde se empieza a leer y e(end) que es el final, se define hasta que caracter se lee dentro del texto (útil para los experimentos)
    int contCaract = 0; //Cuenta los carácteres ya leidos
    string dir = text; // direccion local de la cual se buscara el texto
    unordered_map<char, int> freqFromText;    // se crea el diccionario de frecuencias clave: char, valor: int
    fstream texto;
    texto.open(dir,ios::in);
    string linea;   //variables para la lectura del archivo
    int n;
    if (texto.is_open()){   // si el archivo se abrio (validacion para no tener errores)
        while(getline(texto, linea)){ //un while que itera por las lineas del texto dejandolas en la variable linea
            n=linea.length(); // en n se almacena el largo de linea
            for(int i=0;i<n;i++){   //se recorre el string de largo n
                contCaract++;  //Con cada iteración (lectura de 1 carácter) se suma 1 al contador de caracteres
                try{
                    if(contCaract>=s and contCaract<=e){  //Si esta dentro del rango para leer
                        freqFromText[linea[i]]=freqFromText[linea[i]]+1;    //se le anade +1 al valor del char que se esta leyendo si es que existe clave:valor (try)
                    }
                }
                catch(exception e){
                    freqFromText[linea[i]]=1;                   //se crea una clave:valor para el char que todavia no tiene (exception)
                }
            }

        if(contCaract>=e){   //Se comprueba que leyó la linea completa, y hace un break al while ya que cumple con la condición de lectura hasta el carácter e y además marca un hito para no repetir la suma a los saltos de linea
            break;
        }
        freqFromText['\n']=freqFromText['\n']+1;  //Añade un salto de linea al arbol cada vez que salte linea el getline (Ya que por propiedades de esta función, no los agrega solo)
        contCaract++;
        }
        texto.close();       //se cierra el texto a leer

    }

    priority_queue<Nodo*, vector<Nodo*>, comp> pq;   //Cola de prioridad que guarda los nodos del arbol ordenados 
    for(auto pair: freqFromText){ 
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

    
    encode(raiz, "", CodigoHuffman); //Codifica el codigo huffman

    //Imprime los códigos de huffman en consola, para dejarlos a la vista del usuario, se ouede descomentar si se desea
    /*
    cout<<" Los codigos de Huffman son: \n";
    for(auto pair: CodigoHuffman){
        if(pair.first=='\n'){
            cout<<"Salto de linea:" << " " << pair.second << ", Y se repite : " << freqFromText[pair.first]<<" \n";
        }else{
            cout<<pair.first << ": " << pair.second << ", Y se repite : " << freqFromText[pair.first]<<" \n";
        }
    }
    cout<<"\n Entre los caracteres "<<s<<" y "<<e<<"\n";
    */
} 

    /*****************************************************************************************/
    /*                  Los siguientes códigos comentados fueron hechos para                 */
    /*         Comprobar que se hicieron bien las funciones, se dejarán comentados           */
    /*    junto a una breve explicación de su funcionamiento, para efectos prácticos en los  */
    /*         experimentos, no vale la pena dejarlos ya que relentizarían el proceso        */
    /*****************************************************************************************/

    //------- CREA ARCHIVO CON LOS DÍGITOS CODIFICADOS -------- //
    /*
    ofstream file; //Crea archivo codificado
    file.open("data/codificadoHuffman.txt"); //En esta ruta guardara el archivo codificado y con ese nombre

    fstream texto2;  // instrucciones para abrir el archivo en modo lectura
    texto2.open(dir,ios::in);
    string linea1;
    int ntam;
    if(texto2.is_open()){
        while(getline(texto2,linea1)){     //Consigue la linea en un string
            string str="";
            ntam=linea1.length();
            for(char simbolo: linea1){
                str+=CodigoHuffman[simbolo];   
            }
            str+=CodigoHuffman['\n']; //Agrega el salto de linea aparte, ya que el getline, al detectar el salto de linea corta el string justo antes

            file<<str;
        }
    }


    file.close(); //Cierra archivo reescrito codificado
    */



/* //---------- Lee el archivo con los digitos codificados y luego decodifica, imprimiendolo en pantalla (Fue usado en pruebas para comprobar que funcionaba correctamente el código) -------------//
    string direccion = "data/codificadoHuffman.txt";
    fstream texto3; //Texto codificado a descodificar
    texto3.open(direccion,ios::in);
    string linea3;
    int n3;
    if(texto3.is_open()){
        while(getline(texto3,linea3)){
            string str1 = linea3;
            int index = -1;
            while(index<(int)str1.size()-2){
                decode(raiz,index,str1);
            }
        }
    }
    texto3.close();
    string str = "";
    for(char simbolo: text){
        str += CodigoHuffman[simbolo];
    }
    cout << " El texto codificado es: "<< str << " \n";
    int index = -1;
    cout << "El texto decodificado es: \n";
    while (index < (int)str.size()-2){
        decode(raiz, index, str);
    }

}

*/

int main(int argc, char *argv[]){
    if(argc!=4) {
        cout<<"Debe ingresar como: ./HuffmanC archivo i j \n Donde: \n archivo: dna o english \n i el indice inicial del trozo de texto a comprimir y j el indice final. \n Si quiere probar con un i=0 hasta un k, debe ingresar: ./Tarea archivo 0 k";
        exit(1);
    }

    int i = atoi(argv[2]);
    int j = atoi(argv[3]);
    if(i>j){
        cout<<"Debe ingresar como: ./HuffmanC archivo i j \n Donde: i debe ser menor a j \n";
        exit(1);
    }
    string text = "data/";
    text+=argv[1];
    text+=".txt";
    unsigned t0, t1;
    t0 = clock();
    creaArbol(text,i,j);
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion con i = " << i <<" j = " << j << " es: " <<time << endl;
    return 0;
}
