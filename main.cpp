#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#define side 30
#define num0 ((8+8)%9+1)		//左上角第一个数

void generate (int times) {		//生成终局

}

int main (int argc, char* argv[]) {
	int sudoku = -1;		//数独终局数量
	if (argc == 3 && strcmp (argv[1], "-c") == 0 && (sudoku = atoi (argv[2])) > 0) {		//生成终局
		generate (sudoku);
	}
	else if (argc == 3 && strcmp (argv[1], "-s") == 0 /*&& (fps = fopen (argv[2], "r")) != NULL*/) {		//求解

	}
	else
		printf ("参数错误\n");
	return 0;
}