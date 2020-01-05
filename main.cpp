#include <cstdio>
#include <cstring>
#include <cstdlib>

#define num0 ((8+8)%9+1)		//左上角第一个数
#define side 9

static int trans[side] = { 0,3,6,1,4,7,2,5,8 };		//行变换向量
int nsudoku = -1;		//数独终局数量

FILE* fpw, * fpr;

class Seed {
	int val[side] = { 0 };	
public:
	Seed ();
	void swapi (int i1, int i2);
	int getVal (int i);
};

Seed::Seed (){
	val[0] = num0;
	for (int i = 1; i < side; i++) {
		if (i >= num0)
			val[i] = i + 1;
		else
			val[i] = i;
	}
}

int Seed::getVal (int i){
	return val[i];
}
	
void Seed::swapi(int i1, int i2) {
	int temp;
	temp = val[i1];
	val[i1] = val[i2];
	val[i2] = temp;
}

class Point {
public:
	int x, y;
	int pos[10] = { 0 };		//占有
	Point () { x = 0; y = 0; }
	Point (Point* p);
};

Point::Point (Point* p) {
	x = p->x;
	y = p->y;
}

class Sudoku {
private:
	Seed seed;
	int val[side][side] = { 0 };
	Point zero[60];
public:
	int iz = 0;
	void permutate (int level);		//全排列交换数
	void mod ();		//根据seed生成模板
	void generate ();		//模板全排列交换行生成终局
	void cpy (const Sudoku* map);		//复制数独
	void swapRow (int row1, int row2);		//交换行
	void write (FILE* fpw);		//写入文件
	void set (char* buff,int i);		//按行读入
	int dfs ();		//深度优先搜索
};

void Sudoku::permutate (int level) {
	if (level == side - 2) {
		mod ();
		generate ();
		return;
	}
	for (int i = level; i < side; i++) {
		if (i != level)
			seed.swapi (level, i);
		permutate (level + 1);
		if (nsudoku == 0)
			return;
		if (i != level)
			seed.swapi (level, i);
	}
}

void Sudoku::mod () {
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			val[i][j] = seed.getVal ((j + trans[i]) % 9);
	return;
}

void Sudoku::generate () {		//生成终局
		/*行变换*/
	Sudoku ans;
	for (int i = 0; i < 2 && nsudoku>0; i++) {
		for (int j = 0; j < 6 && nsudoku>0; j++) {
			for (int k = 0; k < 6 && nsudoku>0; k++) {
				ans.cpy(this);
				switch (i) {
				case 0:
					break;
				case 1:
					ans.swapRow (1, 2); break;
				}
				switch (j) {
				case 0:
					break;
				case 1:
					ans.swapRow (4, 5); break;
				case 2:
					ans.swapRow (3, 4); break;
				case 3:
					ans.swapRow (3, 5); ans.swapRow (3, 4); break;
				case 4:
					ans.swapRow (3, 4); ans.swapRow (3, 5); break;
				case 5:
					ans.swapRow (3, 5); break;
				}
				switch (k) {
				case 0:
					break;
				case 1:
					ans.swapRow (7, 8); break;
				case 2:
					ans.swapRow (6, 7); break;
				case 3:
					ans.swapRow (6, 8); ans.swapRow (6, 7); break;
				case 4:
					ans.swapRow (6, 7); ans.swapRow (6, 8); break;
				case 5:
					ans.swapRow (6, 8); break;
				}
				ans.write (fpw);
				nsudoku--;
				if (nsudoku == 0) {
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

void Sudoku::cpy (const Sudoku* map) {
	for (int i = 0; i < side; i++)
		for (int j = 0; j < side; j++)
			val[i][j] = map->val[i][j];
}

void Sudoku::swapRow (int row1, int row2) {
	int temp[side];
	for (int i = 0; i < side; i++) {
		temp[i] = val[row1][i];
		val[row1][i] = val[row2][i];
	}
	for (int i = 0; i < side; i++)
		val[row2][i] = temp[i];
}

void Sudoku::write (FILE* fpw) {
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			fputc (val[i][j] + '0', fpw);
			if (j < 8)
				fputc (' ', fpw);
		}
		if (i < 8)
			fputc ('\n', fpw);
	}
}

void Sudoku::set (char* buff,int i) {
	for (int j = 0; j < 9; j++) {
		if (buff[2 * j] == '0') {
			zero[iz].x = i;
			zero[iz].y = j;
			iz++;
		}
		val[i][j] = buff[2 * j] - '0';
	}
}

int Sudoku::dfs () {
	Point p (zero[iz - 1]);
	/*初始化占有标识数组*/
	for (int i = 0; i < 9; i++) {
		if (val[p.x][i] != 0 && p.pos[val[p.x][i]] == 0)
			p.pos[val[p.x][i]] = 1;
		if (val[i][p.y] != 0 && p.pos[val[p.x][i]] == 0)
			p.pos[val[i][p.y]] = 1;
	}
	for (int i = (p.x / 3) * 3; i < (p.x / 3) * 3 + 3; i++)
		for (int j = (p.y / 3) * 3; j < (p.y / 3) * 3 + 3; j++)
			if (val[i][j] != 0 && p.pos[val[p.x][i]] == 0)
				p.pos[val[i][j]] = 1;

	/*寻找可行解*/
	int found = 0;
	for (int i = 1; i < 10; i++) {
		if (p.pos[i] == 0) {		//找到一可行解
			found = 1;
			val[p.x][p.y] = i;
			p.pos[i] = 1;
			iz--;
			if (iz == 0) {
				write (fpw);
				return 1;
			}
			if (dfs ())		//找下一个空位的可行解
				return 1;
			p.pos[i] = 0;
		}
	}
	return 0;
}



void solve (FILE* fpr) {
	char buff[255];
	while (1) {
		Sudoku puzzle;
		for (int i = 0; i < 9; i++) {
			fgets (buff, 255, fpr);
			puzzle.set (buff,i);
		}
		if (puzzle.iz == 0)
			puzzle.write (fpw);
		puzzle.dfs ();
		if (!fgets (buff, 255, fpr))
			break;
		fprintf (fpw, "\n\n");
	}
	return;
}

int main (int argc, char* argv[]) {
	if (argc == 3 && strcmp (argv[1], "-c") == 0 && (nsudoku = atoi (argv[2])) >= 1 && nsudoku <= 1000000) {		//生成终局
		fopen_s (&fpw, ".\\sudoku.txt", "w+");
		Sudoku map;
		map.permutate (1);
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