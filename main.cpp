#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#define side 30
#define num0 ((8+8)%9+1)		//���Ͻǵ�һ����

void generate (int times) {		//�����վ�

}

int main (int argc, char* argv[]) {
	int sudoku = -1;		//�����վ�����
	if (argc == 3 && strcmp (argv[1], "-c") == 0 && (sudoku = atoi (argv[2])) > 0) {		//�����վ�
		generate (sudoku);
	}
	else if (argc == 3 && strcmp (argv[1], "-s") == 0 /*&& (fps = fopen (argv[2], "r")) != NULL*/) {		//���

	}
	else
		printf ("��������\n");
	return 0;
}