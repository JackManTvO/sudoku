#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define num0 ((8+8)%9+1)		//左上角第一个数
#define side 9
int sudoku = -1;		//数独终局数量

FILE* fpw, * fpr;
int seed[side] = { 0 };
int map[side][side] = { 0 };
int ans[side][side] = { 0 };
static int trans[side] = { 0,3,6,1,4,7,2,5,8 };
class point {
public:
	int x, y;
}zero[60];


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
		temp[i] = ans[row1][i];
		ans[row1][i] = ans[row2][i];
	}
	for (int i = 0; i < side; i++)
		ans[row2][i] = temp[i];
}

void cpy (int tar[side][side], int src[side][side]) {
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			tar[i][j] = src[i][j];
		}
	}
}

void write (int ans[side][side], FILE* fpw) {
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			fputc (ans[i][j] + '0', fpw);
			if (j < 8)
				fputc (' ', fpw);
		}
		if (i < 8)
			fputc ('\n', fpw);
	}
}

void generate () {		//生成终局
	fopen_s (&fpw,".\\sudoku.txt", "w+");
		/*行变换*/
	for (int i = 0; i < 2 && sudoku>0; i++) {
		for (int j = 0; j < 6 && sudoku>0; j++) {
			for (int k = 0; k < 6 && sudoku>0; k++) {
				cpy (ans, map);
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
				write (ans, fpw);
				sudoku--;
				if (sudoku == 0) {
					if (fpw != NULL)
						fclose (fpw);
					return;
				}
				if (fpw != NULL)
					fprintf (fpw, "\n\n");
			}
		}
	}
}

void permutate (int level) {
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

int dfs (int iz) {
	class point p = zero[iz];
	int pos[10] = { 0 };		//占有
	/*初始化占有标识数组*/
	for (int i = 0; i < 9; i++) {
		if (map[p.x][i] != 0 && pos[map[p.x][i]] == 0)
			pos[map[p.x][i]] = 1;
		if (map[i][p.y] != 0 && pos[map[p.x][i]] == 0)
			pos[map[i][p.y]] = 1;
	}
	for (int i = (p.x / 3) * 3; i < (p.x / 3) * 3 + 3; i++)
		for (int j = (p.y / 3) * 3; j < (p.y / 3) * 3 + 3; j++)
			if (map[i][j] != 0 && pos[map[p.x][i]] == 0)
				pos[map[i][j]] = 1;

	/*寻找可行解*/
	int found = 0;
	for (int i = 1; i < 10; i++) {
		if (pos[i] == 0) {		//找到一可行解
			found = 1;
			map[p.x][p.y] = i;
			pos[i] = 1;
			if (iz == 0) {
				write (map, fpw);
				return 1;
			}
			if (dfs (iz - 1) == 1)		//找下一个空位的可行解
				return 1;
			pos[i] = 0;
		}
	}
	return 0;
}

void solve (FILE* fpr) {
	char buff[255];
	while (1) {
		memset (zero, 0, sizeof (zero));
		int iz = 0;
		for (int i = 0; i < 9; i++) {
			fgets (buff, 255, fpr);
			for (int j = 0; j < 9; j++) {
				if (buff[2 * j] == '0') {
					zero[iz].x = i;
					zero[iz].y = j;
					iz++;
				}
				map[i][j] = buff[2 * j] - '0';
			}
		}
		if (iz == 0)
			write (map, fpw);
		dfs (iz - 1);
		if (!fgets (buff, 255, fpr))
			break;
		fprintf (fpw, "\n\n");
	}
	return;
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
	else if (argc == 3 && strcmp (argv[1], "-s") == 0 && fopen_s (&fpr, argv[2], "r") == 0) {		//求解
		fopen_s (&fpw, ".\\sudoku.txt", "w+");
		solve (fpr);
		if (fpr != NULL)
			fclose (fpr);
		if (fpw != NULL)
			fclose (fpw);
	}
	else
		printf ("参数错误\n");
	return 0;
}