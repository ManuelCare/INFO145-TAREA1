#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include <array>
#include<ctime>
#include<map>
#include<math.h>
#include <algorithm> //Se importo para dar vuelta un string
#include <sstream>
using namespace std;

/*ESTRUCTURAS*/

//ARBOL
/*
Nodo del arbol 
Contiene los simbolos, su freciencia y los punteros a la izq y derecha 
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
        return i->freq >= d->freq;
    };
};


/*Definición de funciones*/
Nodo* getNodo(char simb, double freq, Nodo* izq, Nodo* der);
void encode(Nodo* raiz, string str, unordered_map<char, string> &CodigoHuffman);
int toDecimalChar(string n);
void creaArbol(string text,int start, int end);
void decode(string C[], char sigma[], int F[],int posiciones[],int largoCodigoHuffman, int contador,int start, int end, int h);
string toBinary(int n);

/*  

    MAIN  
    string text = nombre y ruta del texto a leer
    creaArbol(text,inicioCodificacion,FinalCodificacion)

*/


int main(){

    string text ="data/english.txt";
    int inicio= 104355;  //I
    int fina = 1106100;   //J
    creaArbol(text,inicio,fina);
    cout<<"total caracteres a decodificar: "<<fina-inicio<<endl;

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

void preorden(Nodo* nodo) {       /*FUE USADO PARA CONFIRMAR QUE FUNCIONA CORRECTAMENTE EL ARBOL*/
    if (nodo != NULL) {
        cout<<"|"<<nodo->simb<<" ";
        preorden(nodo->izq);
        preorden(nodo->der);
    }
}

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


string toBinary(int n){
    string r;
    while (n != 0){
        r += ( n % 2 == 0 ? "0" : "1" );
        n /= 2;
    }
    reverse(r.begin(), r.end());
    return r;
}
int toDecimalChar(string n){
    int x=0;
    int potencia = 1;
    for(int i=n.length()-1;i>=0; i--){
    if(n[i]=='1'){
        x+=1*potencia;
    }
    potencia *=2;
    } 
    return x;
}



void buildHuffmanTree(priority_queue<Nodo*, vector<Nodo*>, comp> &pq){
    while(pq.size()>=2){ //!=1
        Nodo *izq = pq.top(); pq.pop();
        Nodo *der = pq.top(); pq.pop();
        int sum = izq->freq + der->freq;
        pq.push(getNodo('\0',sum,izq,der)); //rellena con caracteres vacios para evitar confusiones a la hora de decodificar

    }
    
}


void creaArbol(string text,int start, int end){
    long long contador = 0;
    string dir = text; // direccion local de la cual se buscara el texto
    unordered_map<char, int> freqFromText;    // se crea el diccionario de frecuencias clave: char, valor: int
    fstream texto;
    texto.open(dir,ios::in);
    string linea;   //variables para la lectura del archivo
    int n;
    int contadorSaltoLinea=0;
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
            if(contadorSaltoLinea==0){
                contadorSaltoLinea++;
            }else{
                freqFromText['\n']=freqFromText['\n']+1;   //SALTOS DE LINEA
                contador = contador +1;
            }
        }
        texto.close();       //se cierra el texto a leer

    }

    cout <<endl<<endl<<endl<< "Caracteres en el texto original: "<< contador<<""<<endl; //Cuenta los carácteres en el array
    int* posiciones = new int[contador]; //Crea array con las posiciones
    priority_queue<Nodo*, vector<Nodo*>, comp> pq;   //Cola de prioridad que guarda los nodos del arbol
    // priority_queue<Nodo*, vector<Nodo*>, comp> pq;   //Cola de prioridad que guarda los nodos del arbol
    for(auto pair: freqFromText){ //----------------
        pq.push(getNodo(pair.first, pair.second, nullptr, nullptr));   //Pone los caracterer en la cola y dsp su frecuencia
        
        
    }

    buildHuffmanTree(pq);

    Nodo* raiz = pq.top();
    // preorden(raiz);    //Se uso para imprimir el arbol y ver si era correcto
    unordered_map<char,string> CodigoHuffman;

    encode(raiz, "", CodigoHuffman);
    cout<<endl<<endl<<endl<<"-----------------------------------------------------------------"<<endl;
    cout<<endl<<"             "<<" Los codigos de Huffman son: \n";
    for(auto pair: CodigoHuffman){
         cout<<"             "<<pair.first << " " << pair.second << " Con una frecuencia de: "<<freqFromText[pair.first]<<"\n";
     } 
    /*
    Empezamos a canonizar el huffman
    */
   int largoCodigoHuffman = CodigoHuffman.size();
   int codigosLargo[largoCodigoHuffman];
   char codigosChar[largoCodigoHuffman];
   int contadorCode = 0;
    for(auto pair: CodigoHuffman){
        codigosChar[contadorCode]=pair.first;
        codigosLargo[contadorCode]=pair.second.size();
        contadorCode++;
    }
    //SE ORDENA EL ARREGLO
    for(int m=0; m<largoCodigoHuffman; m++){
        for(int p=m+1; p<largoCodigoHuffman;p++){
            if(codigosLargo[m]>codigosLargo[p]){
                swap(codigosLargo[m],codigosLargo[p]);
                swap(codigosChar[m],codigosChar[p]);
            }
        }
    }
    /*CREACION DE ARREGLO REPRESENTATIVO DE HUFFMAN CANONICO*/
    string HuffCan[largoCodigoHuffman];
    string codede ="";
    for(int b=0; b<codigosLargo[0];b++){ //Para el primer elemento, cuenta el largo y luego, de ese largo lo llena de 0's (como se debe hacer en canonico)
        codede+="0";
    }
    HuffCan[0]=codede;
    for(int x=1; x<largoCodigoHuffman;x++){
        if(codigosLargo[x]==codigosLargo[x-1]){
            string temp = HuffCan[x-1]; //se cambia a string para ser interpretado por toDecimal
            int decimal = toDecimalChar(temp);
            HuffCan[x]=toBinary(decimal+1);

        }else{
            string temp = HuffCan[x-1];
            int decimal = toDecimalChar(temp);
            int AntesdelBin= 2*(decimal+1);
            HuffCan[x]=toBinary(AntesdelBin);
        }
    }


    unordered_map<char,string> CodigoHuffmanCanonico;
    for(int p = 0; p<largoCodigoHuffman; p++){
        if(HuffCan[p]=="1"){
            HuffCan[p]="01";
        }
        CodigoHuffmanCanonico[codigosChar[p]] = HuffCan[p];
    }

    cout<<endl<<endl<<endl<<"-----------------------------------------------------------------"<<endl;
    cout<<endl<<"             "<<" Los codigos de Huffman Canonicos son: \n";
    for(auto pair: CodigoHuffmanCanonico){
        cout<<"             "<<pair.first << " " << pair.second << " Con una frecuencia de: "<<freqFromText[pair.first]<<"\n";
    } 
    int h = HuffCan[largoCodigoHuffman-1].length();
    // cout<<"El largo del codigo mas grande es y la altura del arbol canonico es: "<<h<<endl;

    



    /*ESCRITURA DEL ARCHIVO (CODIFICANDO DE PASO)*/
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
                str+=CodigoHuffmanCanonico[simbolo];
                tamanoAcumulado = tamanoAcumulado+CodigoHuffmanCanonico[simbolo].size();
                posiciones[posEnArray]=tamanoAcumulado;
                posEnArray++;
            }
            str+=CodigoHuffmanCanonico['\n'];
            tamanoAcumulado = tamanoAcumulado+CodigoHuffmanCanonico['\n'].size();
            posiciones[posEnArray]=tamanoAcumulado;
            posEnArray++;
            file<<str;
        }
    }
    cout<<endl<<endl<<"Cantidad de caracteres en el archivo codificado: "<<tamanoAcumulado<<endl;

    file.close(); //Cierra archivo reescrito codificado
    texto.close();       //se cierra el texto a leer


    /*
    ------------EMPEZAMOS A DECODIFICAR-------------
    */

    /*
    codigosChar = sigma ya que estan ordenados dependiendo de su valor
    h = altura del arbol
    HuffCan = almacena los codigos de huffman en el mismo orden que en que estan en codigosChar
   largoCodigoHuffman = cantidad de codigos huffman canonicos diferentes
   
    */
   //Buscamos la posicion de los primeros codigos de largo l (CREAMOS F)
   int PL = HuffCan[0].length(); //Primer largo
//    cout<<"El codigo de largo mas pequeno es: "<<PL<<endl;
   int F[h+1];   //posiciones del primer codigo huffman de largo l donde F[1]=pos inicio largo 1, F[2]=pos inicio largo 2, etc
   F[0] = -1;
   int contadorTemp;
   int plCamb = PL; //Primer largo cambiable
   for(int k=0; k<h+1; k++){
        contadorTemp = 0;
        while(k!=HuffCan[contadorTemp].length() and contadorTemp<=h+1){
            contadorTemp++;
        }
        if(k==HuffCan[contadorTemp].length()){
            F[k]=contadorTemp;
        }else{
            F[k]=-1;
        }
   }


    //CREAMOS ARREGLO C

    string C[h+1];
    for(int i=0; i<h+1;i++){
        if(F[i]!=-1){
            C[i]=CodigoHuffmanCanonico[codigosChar[F[i]]];
        }else{
            C[i]='-';
        }
    }


    /*
    HACEMOS EL DECODE (PASAR A FUNCION LUEGO)
    entra: codigosChar, C, F
    
    */

    decode(C, codigosChar, F, posiciones, largoCodigoHuffman,contador,start,end,h);


}

void decode(string C[], char sigma[], int F[],int posiciones[],int largoCodigoHuffman, int contador,int start, int end, int h){

    // cout<<"start: "<<start<<" end: "<<end<<endl;
    // cout<<"Largo codeHyff; "<<largoCodigoHuffman<<endl;

    /*EMPEZAMOS*/
    int inicioSeccion = posiciones[start-1];
    int finalSeccion = posiciones[end+1];
    // cout<<"inicio: "<<inicioSeccion<<" final : "<<finalSeccion<<endl;
    delete posiciones;  //Elimina el arreglo, ya que al ser tan grande se necesita liberar la memoria usada

    string direccion = "data/codificadoHuffman.txt";
    fstream texto3; //Texto codificado a descodificar
    texto3.open(direccion,ios::in);
    string linea3;
    string codigos;
    if(texto3.is_open()){
        getline(texto3,linea3);
        cout<<"SE HA LEIDO CORRECTAMENTE EL ARCHIVO CODIFICADO"<<endl;
        codigos = linea3;
    }else{
        // cout<<"ERROR AL LEER EL ARCHIVO CODIFICADO"<<endl;
    }
    texto3.close();
    int index = inicioSeccion-1;
    string Sp = "";
    for(int i = inicioSeccion; i<finalSeccion-2; i++){
        Sp+=codigos[i];
    }
    // cout<<" S prima = "<<Sp << endl;
    texto3.close();
    /*
        Teniendo S empezamos a decodificar esa seccion
    */
    unsigned t0, t1;
 
    t0=clock();

    string S = ""; //Almacenará la palabra decodificada
    string ST = ""; //transforma la palabra y la busca en sigma
    int indx = 0;
    int N;
    while(indx<finalSeccion){
    string NS=""; //N en string
    for(int i=indx; i<indx+h; i++){
        NS+=Sp[i];
    }
    N= toDecimalChar(NS);
    string Np = ""; //N'
    for(int j = 1; j<h; j++){  //j=l
        if((toDecimalChar(C[j])*pow(2,h-j)<=N) and (N<toDecimalChar(C[j+1])*pow(2,h-(j+1))) and (C[j]!="-")){

            for(int f=0; f<j; f++){
                Np+=NS[f];
                indx=indx+j-1;
            }

            ST=sigma[F[j]+(toDecimalChar(Np)-toDecimalChar(C[j]))];
            break;
        }
    }

        S+=ST;      //Almacena la palabra decodificada

   }
    t1 = clock();
    
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Se decodifico en: " << time <<"Segundos"<<endl;
    // cout<<"S es: "<<S<<endl;
}








