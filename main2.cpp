#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

// declaramos la estructura que utilizaremos
struct node {
 
	string sym; //caracteres

	float pro; //probabilidad 
	int arr[20];
	int top;
} 
	p[20];

typedef struct node node;

// codificacion 
void shannon(int l, int h, node p[])
{
	float pack1 = 0, pack2 = 0, diff1 = 0, diff2 = 0;
	int i, d, k, j;
	if ((l + 1) == h || l == h || l > h) {
		if (l == h || l > h)
			return;
		p[h].arr[++(p[h].top)] = 0;
		p[l].arr[++(p[l].top)] = 1;
		return;
	}
	else {
		for (i = l; i <= h - 1; i++)
			pack1 = pack1 + p[i].pro;
		pack2 = pack2 + p[h].pro;
		diff1 = pack1 - pack2;
		if (diff1 < 0)
			diff1 = diff1 * -1;
		j = 2;
		while (j != h - l + 1) {
			k = h - j;
			pack1 = pack2 = 0;
			for (i = l; i <= k; i++)
				pack1 = pack1 + p[i].pro;
			for (i = h; i > k; i--)
				pack2 = pack2 + p[i].pro;
			diff2 = pack1 - pack2;
			if (diff2 < 0)
				diff2 = diff2 * -1;
			if (diff2 >= diff1)
				break;
			diff1 = diff2;
			j++;
		}
		k++;
		for (i = l; i <= k; i++)
			p[i].arr[++(p[i].top)] = 1;
		for (i = k + 1; i <= h; i++)
			p[i].arr[++(p[i].top)] = 0;

		// Invoke shannon function
		shannon(l, k, p);
		shannon(k + 1, h, p);
	}
}

// ordenar por frecuencia
void ordenafreq(int n, node p[])
{
	int i, j;
	node temp;
	for (j = 1; j <= n - 1; j++) {
		for (i = 0; i < n - 1; i++) {
			if ((p[i].pro) > (p[i + 1].pro)) {
				temp.pro = p[i].pro;
				temp.sym = p[i].sym;

				p[i].pro = p[i + 1].pro;
				p[i].sym = p[i + 1].sym;

				p[i + 1].pro = temp.pro;
				p[i + 1].sym = temp.sym;
			}
		}
	}
}

// mostrar en terminal (cambiar para guardar en txt)
void display(int n, node p[])
{
	int i, j;
	cout << "\n\n\n\tSymbol\tProbability\tCode";
	for (i = n - 1; i >= 0; i--) {
		cout << "\n\t" << p[i].sym << "\t\t" << p[i].pro << "\t";
		for (j = 0; j <= p[i].top; j++)
			cout << p[i].arr[j];
	}
}

int main()
{
	node temp;
	float total = 0;
	int n,i,j;
	string dir = "data/ejemplo.txt";
	
	map<char, int> freq;
	fstream texto;
	texto.open(dir,ios::in);
	string linea;
	
	if (texto.is_open()){
		while(getline(texto,linea)){
			n=linea.length();
			// crear el diccionario
			for (i = 0; i < n; i++) {
				try{
					freq[linea[i]]=freq[linea[i]]+1;
				}
				catch(exception e){
					freq[linea[i]]=1;
				}
			}
		}
		texto.close();
	}
	// agregar los simbolos al arbol
	n=freq.size();
	i=0;
	for (auto element:freq){
		p[i].sym=element.first;
		p[i].pro=element.second;
		i+=1;
		//printf("Char: %c\t Frec: %i\n",element.first,element.second);
	}
	// cantidad de elementos, se podria reemplazar
	int pesoTotal = 0;
	for(auto element:freq){
		pesoTotal+=element.second;
		
	}
	// para mostrar en pantalla, aunque el porcentaje debemos rescatarlo
	for(auto element: freq){
	printf("Char: %c\tFrec: %i\tPor: %.e\n",element.first,element.second,((double)element.second)/pesoTotal);
	}

	//calculo de probabilidades
	for (i = 0; i < n; i++) {
		if (total > 1) {
			cout << "errores de calculo??";
			total = total - p[i].pro;
			i--;
		}
	}

	p[i].pro = 1 - total;
	// Sorting the symbols based on
	// their probability or frequency
	
	ordenafreq(n, p);
	for (i = 0; i < n; i++)
		p[i].top = -1;
	// Find the shannon code
	shannon(0, n - 1, p);
	// Display the codes
	display(n, p);
	return 0;
}
