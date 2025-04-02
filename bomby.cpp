#include <cstdlib>
#include <ctime>
#include "bomby.h"

/*
funkcja bomby - genrator tabel bomb i ich okolic

sposob uzycia:

int ** nazwa_tablicy_dwuwymiarowej=bomby(poziom_trudnosci)

@param n - (int) poziom trudnosci (1-3)
@return - wskaznik do dynamicznej tablicy dwuwymiarowej zawierajacej mapêe bomb
*/
int** bomby(int n) {
	int x;
	int y = 10;
	int bombs;
	if (n == 1) {
		x = 10;
		bombs = 20;
	}
	else if (n == 2) {
		x = 20;
		bombs = 60;
	}
	else if (n == 3) {
		x = 20;
		bombs = 80;
	}
	//tworzenie tablicy
	int** tab = new int* [x];
	for (int i = 0; i < x; i++) {
		tab[i] = new int[y];
	}
	for (int i = 0; i < x; i++) {
		for (int t = 0; t < y; t++) {
			tab[i][t] = 0;
		}
	}
	//losowanie bomb i okolic
	srand(time(0));
	for (int i = 0; i < bombs + 1; i++) {
		int tx = rand() % x;
		int ty = rand() % y;
		if (tab[tx][ty] != -1 && tx != 0) {
			tab[tx][ty] = -1;
			if (tx + 1 <= x - 1) { if (tab[tx + 1][ty] != -1) { tab[tx + 1][ty]++; } }
			if (tx - 1 >= 0) { if (tab[tx - 1][ty] != -1) { tab[tx - 1][ty]++; } }
			if (ty - 1 >= 0) { if (tab[tx][ty - 1] != -1) { tab[tx][ty - 1]++; } }
			if (ty + 1 <= y - 1) { if (tab[tx][ty + 1] != -1) { tab[tx][ty + 1]++; } }
			if ((tx + 1 <= x - 1) && (ty + 1 <= y - 1)) { if (tab[tx + 1][ty + 1] != -1) { tab[tx + 1][ty + 1]++; } }
			if ((tx - 1 >= 0) && (ty - 1 >= 0)) { if (tab[tx - 1][ty - 1] != -1) { tab[tx - 1][ty - 1]++; } }
			if ((tx + 1 <= x - 1) && (ty - 1 >= 0)) { if (tab[tx + 1][ty - 1] != -1) { tab[tx + 1][ty - 1]++; } }
			if ((tx - 1 >= 0) && (ty + 1 <= y - 1)) { if (tab[tx - 1][ty + 1] != -1) { tab[tx - 1][ty + 1]++; } }
		}
		else { i--; }
	}
	return tab;
}