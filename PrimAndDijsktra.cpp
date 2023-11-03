#include <iostream>
#include <stack>
using namespace std;


struct PoslednjiNeizgradjeni {
	int cvor=-1;
	int brNeizgradjenih=0;
};
struct Edge {
	int e;
	int w;
	bool izgradjen = false;
};

struct Tezina {
	int w= 1000000;
	bool izgradjen=false;
};

struct Graf {
	int n;
	int e;
	int* indices;
	Edge* edges;
};

Graf* kreirajGraf(int n) {
	Graf* g = new Graf;
	
	g->n = n;
	g->e = g->n;

	g->indices = new int[g->n + 1];
	g->edges = new Edge[g->e];
	int i;
	for (i = 0; i < g->n; i++) {
		g->indices[i] = i;
		g->edges[i].e = -1;
		g->edges[i].w = 0;
	}
	g->indices[i] = i;
	return g;
}

void dodajCvor(Graf& g) {
	int* temp = new int[++g.n + 1];
	g.e++;
	int i;
	for (i = 0; i < g.n; i++) {
		temp[i] = g.indices[i];
	}
	temp[i] = g.e;
	delete[] g.indices;
	g.indices = temp;
	Edge* temp2 = new Edge[g.e];
	for (i = 0; i < g.e - 1; i++) {
		temp2[i] = g.edges[i];
	}
	temp2[i].e = -1;
	temp2[i].w = 0;
	delete[] g.edges;
	g.edges = temp2;
	temp2 = nullptr;
}

void brisiCvor(Graf& g,int br) {
	int graneBrisanje = 0;
	for (int i = g.indices[br]; i < g.indices[br + 1]; i++) {
		g.edges[i].e = -1;
		graneBrisanje++;
	}
	for (int i = 0; i < g.e - graneBrisanje; i++) {
		if (i >= g.indices[br] && br != g.n - 1) {
			g.edges[i] = g.edges[i + graneBrisanje];
		}
		if (g.edges[i].e == br) {
			g.edges[i].e = -1;
		}
		if (g.edges[i].e > br) {
			g.edges[i].e--;
		}
	}
	//
	int* temp = new int[g.n];
	for (int i = 0; i < g.n; i++) {
		if (i < br) {
			temp[i] = g.indices[i];
		}
		else {
			temp[i] = g.indices[i + 1] - graneBrisanje;
		}

	}
	g.n--;
	delete[] g.indices;
	g.indices = temp;
	temp = nullptr;
	for (int i = 0, j = 0; i < g.e - graneBrisanje; i++) {
		if (i == g.indices[j]) {
			if (g.edges[i].e == -1 && g.indices[j + 1] == i + 1) {
				j++;
				continue;
			}
			j++;
		}
		if (g.edges[i].e == -1) {
			for (int k = i, l = j; k <= g.e - graneBrisanje; k++) {
				if (k == g.indices[l]) {
					g.indices[l++]--;
				}
				g.edges[k] = g.edges[k + 1];
			}
			graneBrisanje++;
		}
	}
	g.e -= graneBrisanje;
	Edge* temp1 = new Edge[g.e];
	for (int i = 0; i < g.e; i++) {
		temp1[i].e = g.edges[i].e;
		temp1[i].w = g.edges[i].w;
	}
	delete[] g.edges;
	g.edges = temp1;
	temp1 = nullptr;
	g.indices[g.n] = g.e;

}

void dodajGranu(Graf& g,int poz1,int poz2,int w,bool izgr=0) {
	if (poz1 >= g.n || poz2 >= g.n || poz1 < 0 || poz2 < 0) {
		cout << "Unesite validne indekse incidentnih cvorova grana" << endl;
		return;
	}
	int min = poz1 > poz2 ? poz2 : poz1;
	bool granaPostoji = false;
	for (int i = g.indices[min]; i < g.indices[min + 1]; i++) {
		if (g.edges[i].e == poz1 || g.edges[i].e == poz2) {
			cout << "Grana vec postoji" << endl;
			granaPostoji = true;
		}
	}
	if (!granaPostoji) {
		for (int i = 0; i < g.n + 1; i++) {
			if (i == poz1 || i == poz2) {
				if (g.edges[g.indices[i]].e == -1) {
					g.edges[g.indices[i]].e = i == poz1 ? poz2 : poz1;
					g.edges[g.indices[i]].w = w;
					g.edges[g.indices[i]].izgradjen = izgr;
				}
				else {
					g.e++;
					Edge* temp = new Edge[g.e];
					for (int i = 0; i < g.e - 1; i++) {
						temp[i].e = g.edges[i].e;
						temp[i].w = g.edges[i].w;
						temp[i].izgradjen = g.edges[i].izgradjen;
					}
					temp[g.e - 1].e = -1;
					temp[g.e - 1].w = 0;
					delete[] g.edges;
					g.edges = temp;
					temp = nullptr;
					int j;
					for (j = g.e - 1; j > g.indices[i]; j--) {
						g.edges[j] = g.edges[j - 1];
					}
					g.edges[j].e = i == poz1 ? poz2 : poz1;
					g.edges[j].w = w;
					g.edges[j].izgradjen = izgr;
					for (int k = i + 1; k < g.n + 1; k++) {
						g.indices[k]++;
					}
				}
			}
		}
	}
}

void brisiGranu(Graf& g) {
	cout << "Unesite postojecu granu za brisanje: " << endl;
	int poz1, poz2;
	cin >> poz1 >> poz2;
	int min = poz1 < poz2 ? poz1 : poz2;
	int max = poz1 > poz2 ? poz1 : poz2;
	int promena = 0;
	for (int i = 0; i < g.n; i++) {
		bool nadjen = false;
		if (i == min) {
			for (int j = g.indices[i]; j < g.e - 1; j++) {
				if (g.edges[j].e == max && (i < g.n - 1 ? j < g.indices[i + 1] : true)) {
					if (j + 1 == g.indices[i + 1] && j == g.indices[i]) {
						g.edges[j].e = -1;
						g.edges[j].w = 0;
					}
					else {
						nadjen = true;
						promena++;
						g.indices[g.n]--;
					}
				}
				if (nadjen) {
					g.edges[j] = g.edges[j + 1];
				}
			}
		}
		if (i == max) {
			for (int j = g.indices[i]; j < g.e - 1; j++) {
				if (g.edges[j].e == min && (i < g.n - 1 ? j < g.indices[i + 1] : true)) {
					if (j + 1 == g.indices[i + 1] && j == g.indices[i]) {
						g.edges[j].e = -1;
						g.edges[j].w = 0;
					}
					else {
						nadjen = true;
						promena++;
						g.indices[g.n]--;
					}
				}
				if (nadjen) {
					g.edges[j] = g.edges[j + 1];
				}
			}
		}
		if (nadjen) {
			for (int k = i; k < g.n - 1; k++)
				g.indices[k + 1] --;
		}
	}
	Edge* temp = new Edge[g.e -= promena];
	for (int i = 0; i < g.e; i++) {
		temp[i] = g.edges[i];
	}
	delete[] g.edges;
	g.edges = temp;
	temp = nullptr;

}

void ispis(const Graf& g) {
	cout << "Broj cvorova u grafu: " << g.n << endl
		<< "Broj grana u grafu: " << g.e / 2 << endl
		<< "Niz indeksa cvorova u jednom redu, a u narednom pocetni indeks niza edges koji sadrzi susede cvora:" << endl;
	for (int i = 0; i < g.n; i++) {
		cout << i << " ; ";

	}
	cout << endl;
	for (int i = 0; i < g.n; i++) {
		cout << g.indices[i] << " ; ";
	}
	if (g.edges) {
		cout << endl << "Indeksi niza edges u jednom, a zatim vredniosti u drugom redu i tezine u trecem redu" << endl;
		for (int i = 0; i < g.e; i++) {
			cout << i << " ; ";
		}
		cout << endl;
		for (int i = 0; i < g.e; i++) {
			cout << g.edges[i].e << " ; ";
		}
		cout << endl;
		for (int i = 0; i < g.e; i++) {
			cout << g.edges[i].w << " ; ";
		}
		cout << endl;
	}
}

void brisiGraf(Graf& g) {
	delete[] g.indices;
	delete[] g.edges;
	g.e = 0;
	g.n = 0;
}

int prim(Graf& g, int s) {
	Graf u;
	u.e = g.e;
	u.n = g.n;
	for (int i = 0; i < g.e; i++) {
		if (g.edges[i].e == -1) {
			return -1;
		}
	}
	int* temp = new int[g.n + 1];
	for (int i = 0; i < g.n + 1; i++) {
		temp[i] = g.indices[i];
	}
	u.indices = temp;
	temp = nullptr;
	Edge* temp1 = new Edge[g.e];
	for (int i = 0; i < g.e; i++) {
		temp1[i] = g.edges[i];
	}
	u.edges = temp1;
	temp1 = nullptr;
	int i = 0;
	int* niz = new int[u.n];
	niz[i++] = s;
	int minnod = s;
	int minukupnacena = 0;
	while (i < u.n) {
		int mincena = 100000;
		for (int l = 0; l < u.e; l++) {
			if (u.edges[l].e == minnod) {
				u.edges[l].e = -1;
				u.edges[l].w = 0;
			}
		}
		for (int j = 0; j < i; j++) {
			for (int k = u.indices[niz[j]]; k < u.indices[niz[j] + 1]; k++) {
				if (u.edges[k].w < mincena && u.edges[k].w>0) {
					mincena = u.edges[k].w;
					minnod = u.edges[k].e;
				}
			}
		}
		
		minukupnacena += mincena;
		niz[i++] = minnod;
	}
	return minukupnacena;
}

void dijkstra(Graf& g,int v, Tezina** w, int* d, PoslednjiNeizgradjeni** t) {
	int* niz2 = new int[g.n];
	int* brojIstih = new int[g.n ];
	for (int i = 0; i < g.n; i++) {
		if (i != v) {
			d[i] = w[v][i].w;
			if (w[v][i].w != 1000000) {
				t[0][i].cvor = v;
				t[0][i].brNeizgradjenih += (!w[v][i].izgradjen);
			}
			else {
				t[0][i].cvor = 0;
			}
			niz2[i] = i;
			
		}
		else {
			niz2[i] = -1;
		}
		brojIstih[i] = 0;
	}
	int* s = new int[g.n];
	int iters = 0;
	s[iters++] = v;
	int minind = 0;
	for (int i = 0; i < g.n - 1; i++) {
		int minduz = 10000000;
		int nePostoji = 0;
		for (int k = 0; k < g.n; k++) {
			if (niz2[k] != -1) {
				if (d[k] < minduz) {
					minduz = d[k];
					minind = k;
					nePostoji = !w[0][k].izgradjen;
				}
			}
		}
		s[iters++] = minind;
		niz2[minind] = -1;
		for (int j = 0,l=0; j < g.n; j++) {
			if (niz2[j] != -1) {
					if (minduz + w[minind][j].w <= d[j]) {
						if (minduz + w[minind][j].w == d[j]) {
							brojIstih[j]++;
						}
						d[j] = minduz + w[minind][j].w;
						t[brojIstih[j]][j].cvor = minind;
						t[brojIstih[j]][j].brNeizgradjenih = (t[brojIstih[minind]][minind].brNeizgradjenih + nePostoji);
					}
				
			}
		}
	}
}

int main() {
	cout << "Unesi broj kljucnih lokacija u gradu:" << endl;
	int n;
	cin >> n;
	
	Graf* metro = kreirajGraf(n);
	while (1) {
		cout << "Unesite izmedju koje dve lokacije od 0 do " << metro->n-1 << " zelite da izgradite metro kao i cenu izgradnje (-1 za kraj unosa): ";
		cout << "Prva lokacija: ";
		int poz1;
		cin >> poz1;
		if (poz1 == -1) {
			break;
		}
		else if (poz1 >= metro->n) {
			continue;
		}
		cout << "Druga lokacija: ";
		int poz2;
		cin >> poz2;
		if (poz2 >= metro->n) {
			continue;
		}
		cout << "Cena izgradnje: ";
		int w;
		cin >> w;
		if (w < 0) {
			continue;
		}
		dodajGranu(*metro, poz1, poz2, w);
	}
	ispis(*metro);
	int najmanjaCena = prim(*metro, 2);
	cout <<"Najmanja cena za metro je: "<< najmanjaCena<<endl;
	brisiGraf(*metro);
	Graf* najkraciPut = kreirajGraf(n);
	int cvorV;
	cout << "Unesite cvor za parking od 0 do "<<najkraciPut->n-1<<" : ";
	cin >> cvorV;
	if (cvorV >= najkraciPut->n) {
		cvorV = najkraciPut->n - 1;
	}
	while (1) {
		cout << "Unesite izmedju koje dve lokacije od 0 do " << najkraciPut->n-1 << " postoji put kao i duzinu (-1 za kraj unosa): ";
		cout << "Prva lokacija: ";
		int poz1;
		cin >> poz1;
		if (poz1 == -1) {
			break;
		}
		else if (poz1 >= najkraciPut->n) {
			continue;
		}
		cout << "Druga lokacija: ";
		int poz2;
		cin >> poz2;
		if (poz2 >= najkraciPut->n) {
			continue;
		}
		cout << "Duzina: ";
		int w;
		cin >> w;
		if (w < 0) {
			continue;
		}
		dodajGranu(*najkraciPut, poz1, poz2, w,true);
	}
	while (1) {
		cout << "Unesite izmedju koje dve lokacije od 0 do " << najkraciPut->n-1 << " moze da se izgradi put kao i duzinu (-1 za kraj unosa):  ";
		cout << "Prva lokacija: ";
		int poz1;
		cin >> poz1;
		if (poz1 == -1) {
			break;
		}
		else if (poz1 >= najkraciPut->n) {
			continue;
		}
		cout << "Druga lokacija: ";
		int poz2;
		cin >> poz2;
		if (poz2 >= najkraciPut->n) {
			continue;
		}
		cout << "Duzina: ";
		int w;
		cin >> w;
		if (w < 0) {
			continue;
		}
		dodajGranu(*najkraciPut, poz1, poz2,w, false);
	}
	Tezina** w = new Tezina* [najkraciPut->n];
	for (int i = 0; i < najkraciPut->n; i++) {
		w[i] = new Tezina[najkraciPut->n];
	}
	for (int i = 0; i < najkraciPut->n; i++) {
		for (int j = najkraciPut->indices[i]; j < najkraciPut->indices[i + 1]; j++) {
			w[i][najkraciPut->edges[j].e].izgradjen = najkraciPut->edges[j].izgradjen;
			w[i][najkraciPut->edges[j].e].w = najkraciPut->edges[j].w;
		}
	}
	int* d = new int[najkraciPut->n];
	PoslednjiNeizgradjeni** t = new PoslednjiNeizgradjeni*[najkraciPut->n];
	for (int i = 0; i < najkraciPut->n; i++) {
		t[i] = new PoslednjiNeizgradjeni[najkraciPut->n];
	}
	dijkstra(*najkraciPut,cvorV ,w, d, t);
	for (int i = 0; i < najkraciPut->n; i++) {
		if (i != cvorV) {
			cout << "Najkraci put od lokacije " << i << ": ";
			int l = 0;
			while (t[l+1][i].cvor != -1) {
				if (t[l][i].brNeizgradjenih > t[l + 1][i].brNeizgradjenih) {
					int pom = t[l][i].brNeizgradjenih;
					t[l][i].brNeizgradjenih = t[l + 1][i].brNeizgradjenih;
					t[l + 1][i].brNeizgradjenih = pom;
				}
				l++;
			}
			l = 0;
			while (t[l][i].cvor != -1) {
				stack<int>* s = new stack<int>[najkraciPut->n];
				int pom = t[l][i].cvor;
				for (int k = 0; k < najkraciPut->n; k++) {
						while (pom != cvorV) {
							if (t[k][pom].cvor != -1) {
								s[k].push(pom);
								pom = t[k][pom].cvor;
							}
							else {
								break;
							}
						}
						int pom = t[l][i].cvor;
				}
				cout << "V - ";
				bool nijejedan = true;
				for (int k = 0; k < najkraciPut->n; k++) {

					int brojNeizgr=0;
					bool ima = false;
					while (!s[k].empty()) {
						nijejedan = false;
						ima = true;
						int temp = s[k].top();
						cout << temp << " - ";
						s[k].pop();
						if (!s[k].empty()) {
							if (!w[temp][s[k].top()].izgradjen)
								brojNeizgr++;
						}
						else {
							brojNeizgr += w[l][i].izgradjen;
						}
						
					}
					if (ima) {
						cout << i << "; Duzina je: " << d[i] << " , broj neizgradjenih puteva: " << (brojNeizgr += w[l][i].izgradjen) << endl;
					}
				}
				if (nijejedan) {
					cout << i << "; Duzina je: " << d[i] << " , broj neizgradjenih puteva: " << ( w[l][i].izgradjen) << endl;
				}
				
				l++;
			}
		}
	}
	return 0;
}