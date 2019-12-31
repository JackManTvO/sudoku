#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define num0 ((8+8)%9+1)		//左上角第一个数
#define side 9
int sudoku = -1;		//数独终局数量

FILE* fp, * fp0;
int seed[side] = { 0 };
int map[side][side] = { 0 };
int end[side][side] = { 0 };
static int trans[side] = { 0,3,6,1,4,7,2,5,8 };


void swapi (int i1, int i2) {
	int temp;
	temp = seed[i1];
	seed[i1] = seed[i2];
	seed[i2] = temp;
}

void mod () {
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			map[i][j] = seed[(j + trans[i]) % 9];
	return;
}

void swaprow (int row1, int row2) {
	int temp[side];
	for (int i = 0; i < side; i++) {
		temp[i] = end[row1][i];
		end[row1][i] = end[row2][i];
	}
	for (int i = 0; i < side; i++)
		end[row2][i] = temp[i];
}

void cpy (int tar[side][side], int src[side][side]) {
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			tar[i][j] = src[i][j];
		}
	}
}

void write (int end[side][side],FILE *fp) {
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			fputc (end[i][j] + '0', fp);
			if (j < 8)
				fputc (' ', fp);
		}
		if (i < 8)
			fputc ('\n', fp);
	}
}

void generate () {		//生成终局
	fp = fopen (".\\sudoku.txt", "w+");
		/*行变换*/
	for (int i = 0; i < 2 && sudoku>0; i++) {
		for (int j = 0; j < 6 && sudoku>0; j++) {
			for (int k = 0; k < 6 && sudoku>0; k++) {
				cpy (end, map);
				switch (i) {
				case 0:
					break;
				case 1:
					swaprow (1, 2);
					break;
				}
				switch (j) {
				case 0:
					break;
				case 1:
					swaprow (4, 5);
					break;
				case 2:
					swaprow (3, 4);
					break;
				case 3:
					swaprow (3, 5);
					swaprow (3, 4);
					break;
				case 4:
					swaprow (3, 4);
					swaprow (3, 5);
					break;
				case 5:
					swaprow (3, 5);
					break;
				}
				switch (k) {
				case 0:
					break;
				case 1:
					swaprow (7, 8);
					break;
				case 2:
					swaprow (6, 7);
					break;
				case 3:
					swaprow (6, 8);
					swaprow (6, 7);
					break;
				case 4:
					swaprow (6, 7);
					swaprow (6, 8);
					break;
				case 5:
					swaprow (6, 8);
					break;
				}
				write (end, fp);
				sudoku--;
				if (sudoku == 0) {
					fclose (fp);
					return;
				}
				fprintf (fp, "\n\n");
			}
		}
	}
}

void permutate (int level) {
	int i;
	if (level == side - 2) {
		mod ();
		generate ();
		return;
	}
	for (int i = level + 1; i < side; i++) {
		swapi (level, i);
		permutate (level + 1);
		if (sudoku == 0)
			return;
		swapi (level, i);
	}
}


int main (int argc, char* argv[]) {
	if (argc == 3 && strcmp (argv[1], "-c") == 0 && (sudoku = atoi (argv[2])) >= 1 && sudoku <= 1000000) {		//生成终局
		seed[0] = num0;
		for (int i = 1; i < side; i++) {
			if (i >= num0)
				seed[i] = i + 1;
			else
				seed[i] = i;
		}
		permutate (1);
	}
	else if (argc == 3 && strcmp (argv[1], "-s") == 0 && (fp0 = fopen (argv[2], "r")) != NULL) {		//求解

	}
	else
		printf ("参数错误\n");
	return 0;
}