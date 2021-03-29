#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

using std::string;
using std::vector;

void linhas(FILE *p, int **table, int size) {
	int i, j, k;
	
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			fprintf(p, "%d ", table[i][j]);
		}
		fprintf(p, "0\n");
	}
	
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			for(k = j + 1; k < size; k++) {
				fprintf(p, "%d ", -table[i][j]);
				fprintf(p, "%d ", -table[i][k]);
				fprintf(p, "0\n");
			}
		}
	}
}

void colunas(FILE *p, int **table, int size) {
	int i, j, k;
	
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			fprintf(p, "%d ", table[j][i]);
		}
		fprintf(p, "0\n");
	}
	
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			for(k = j + 1; k < size; k++) {
				fprintf(p, "%d ", -table[j][i]);
				fprintf(p, "%d ", -table[k][i]);
				fprintf(p, "0\n");
			}
		}
	}
}

void diagonalPrincipal(FILE *p, int **table, int size) {
	vector<int> aux;
	int i, j, k, l;
	
	for(j = 0; j < size - 1; j++) {
		int a = 0;
		int b = j;
		for(i = j; i < size; i++) {
			aux.push_back(-table[a][b]);
			a++;
			b++;
		}
		for(k = 0; k < (int)aux.size(); k++) {
			for(l = k + 1; l < (int)aux.size(); l++) {
				fprintf(p, "%d ", aux[k]);
				fprintf(p, "%d ", aux[l]);
				fprintf(p, "0\n");
			}
		}
		aux.clear();
	}
	
	aux.clear();
	
	for(j = 1; j < size - 1; j++) {
		int a = 0;
		int b = j;
		for(i = j; i < size; i++) {
			aux.push_back(-table[b][a]);
			a++;
			b++;
		}
		for(k = 0; k < (int)aux.size(); k++) {
			for(l = k + 1; l < (int)aux.size(); l++) {
				fprintf(p, "%d ", aux[k]);
				fprintf(p, "%d ", aux[l]);
				fprintf(p, "0\n");
			}
		}
		aux.clear();
	}
}

void diagonalSecundaria(FILE *p, int **table, int size) {
	vector<int> aux;
	int i, j, k, l;
	
	for(j = 1; j < size; j++) {
		int b = j;
		for(i = 0; i <= j; i++) {
			aux.push_back(-table[i][b]);
			b--;
		}
		for(k = 0; k < (int)aux.size(); k++) {
			for(l = k + 1; l < (int)aux.size(); l++) {
				fprintf(p, "%d ", aux[k]);
				fprintf(p, "%d ", aux[l]);
				fprintf(p, "0\n");
			}
		}
		aux.clear();
	}
	
	aux.clear();
	bool achou;
	
	for(j = 1; j < size; j++) {
		int c = j;
		for(i = size - 1; i >= j; i--) {
			if(c == size - 1 && i == size - 1) {
				achou = true;
				break;
			}
			aux.push_back(-table[c][i]);
			c++;
		}
		for(k = 0; k < (int)aux.size(); k++) {
			for(l = k + 1; l < (int)aux.size(); l++) {
				fprintf(p, "%d ", aux[k]);
				fprintf(p, "%d ", aux[l]);
				if(!achou) {
					fprintf(p, "0\n");
				}
			}
		}
		aux.clear();
	}
}

int clausulas(int n) {
	int m = 0, i = 1, j;
	int temp = n;
	
	m = ((n*n - n)/2) + 1;
	m = n*m;
	m = 2*m;
	
	m = m + ((n*n - n)/2);
	m = m + ((n*n - n)/2);
	
	
	for(j = 0; j < 4; j++) {
		while(temp - i > 1) {
			m += (((temp - i) * (temp - i)) - (temp - i)) / 2;
			i++;
		}
	
		i = 1;
	}
	
	return m;
}

char* toChar(string s) {
	int i, ss = s.size();
	char *str = (char*)malloc(ss * sizeof(char));
	
	for(i = 0; i < ss; i++) {
		str[i] = s[i];
	}
	
	return str;
}

int main(int argc, char *argv[]) {
	int n = atoi(argv[1]);
	int i, j, indicator = 1;
	int **table = new int*[n];
	
	int qtd_atomos = n * n;
	int qtd_clausulas = clausulas(n);
	
	char *txt = toChar("rainha" + (string)argv[1] + ".txt");
	
	FILE *fp = fopen(txt, "a");
	
	for(i = 0; i < n; i++) {
		table[i] = new int[n];
		for(j = 0; j < n; j++) {
			table[i][j] = indicator;
			indicator++;
		}
	}
	
	fprintf(fp, "p cnf %d %d\n", qtd_atomos, qtd_clausulas);
	
	linhas(fp, table, n);
	colunas(fp, table, n);
	diagonalPrincipal(fp, table, n);
	diagonalSecundaria(fp, table, n);
	
	fclose(fp);
	
	for(i = 0; i < n; i++) {
		delete [] table[i];
	}
	delete [] table;
	
	return 0;
}
