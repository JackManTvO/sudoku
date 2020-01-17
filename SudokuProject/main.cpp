#include <cstdio>
#include <cstring>
#include <cstdlib>

#define num0 ((8+8)%9+1)		//左上角第一个数
#define side 9		//边长

static int trans[side] = { 0,3,6,1,4,7,2,5,8 };		//行变换向量
int nsudoku = -1;		//数独终局数量
FILE* fpw, * fpr;		//写文件，读文件


class Seed {		//生成向量，通过行变换生成向量生成模板
	int val[side] = { 0 };
public:
	Seed ();		//初始化
	void swapi (int i1, int i2);		//交换元素
	int getVal (int i) { return val[i]; }		//获取值
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
	
void Seed::swapi(int i1, int i2) {
	int temp;
	temp = val[i1];
	val[i1] = val[i2];
	val[i2] = temp;
}


class Point {
private:
	int x, y;		//行、列坐标
	int pos[10] = { 0 };		//占有
public:
	Point () { x = 0; y = 0; }
	Point (const Point* p) { x = p->x; y = p->y; }
	void set (int x, int y) { this->x = x; this->y = y; }
	int getx () { return x; }
	int gety () { return y; }
	int getpos (int i) { return pos[i]; }
	void setpos (int i, int val) { pos[i] = val; }
};


class Pset {		//空白点集
private:
	Point zero[60];
	int size = 0;
public:
	void add (int x, int y);	//添加空白点
	void del (int iz);		//删除空白点
	int geti ();		//随机取点
	Point getVal (int iz) { return zero[iz]; }
	bool isEmpty () { return size == 0 ? true : false; }
};

void Pset::add (int x,int y) {
	zero[size].set (x, y);
	size++;
}

void Pset::del (int iz) {
	Point p (zero[iz]);
	zero[iz] = zero[size - 1];
	zero[size - 1] = p;
	size--;
}

int Pset::geti () {
	int iz = rand () % size;
	return iz;
}


class Sudoku {
private:
	Seed seed;
	int val[side][side] = { 0 };
	Pset pset;
public:
	void permutate (int level);		//全排列交换数
	void mod ();		//根据seed生成模板
	void generate ();		//模板全排列交换行生成终局
	void cpy (const Sudoku* map);		//复制数独
	void swapRow (int row1, int row2);		//交换行
	void write (FILE* fpw);		//写入文件
	void set (char* buff, int i);		//按行读入
	int dfs ();		//深度优先搜索
	bool isEmpty () { return pset.isEmpty (); }
};

void Sudoku::permutate (int level) {
	if (level == side - 2) {		//全排列结束
		mod ();		//生成模板
		generate ();		//行交换
		return;
	}
	for (int i = level; i < side; i++) {
		if (i != level)
			seed.swapi (level, i);		//交换元素
		permutate (level + 1);		//递归
		if (nsudoku == 0)
			return;
		if (i != level)
			seed.swapi (level, i);
	}
	return;
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
	for (int i = 0; i < 2 && nsudoku>0; i++)		//第一阶段
		for (int j = 0; j < 6 && nsudoku>0; j++)		//第二阶段
			for (int k = 0; k < 6 && nsudoku>0; k++) {		//第三阶段
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
				nsudoku--;		//生成数+1
				if (nsudoku == 0) {
					if (fpw != NULL)
						fclose (fpw);
					return;
				}
				if (fpw != NULL)
					fprintf (fpw, "\n\n");
			}
	return;
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
	char str[256];
	memset (str, 0, sizeof (str));
	int is = 0;
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			str[is++] = val[i][j] + '0';
			if (j < 8)
				str[is++] = ' ';
		}
		if (i < 8)
			str[is++] = '\n';
	}
	fputs (str, fpw);
}

void Sudoku::set (char* buff,int i) {
	for (int j = 0; j < 9; j++) {
		if (buff[2 * j] == '0')
			pset.add (i, j);
		val[i][j] = buff[2 * j] - '0';
	}
}

int Sudoku::dfs () {
	int iz = pset.geti();
	Point p (this->pset.getVal(iz));
	/*初始化占有标识数组*/
	for (int i = 0; i < 9; i++) {
		if (val[p.getx ()][i] != 0 && p.getpos (val[p.getx ()][i]) == 0)
			p.setpos (val[p.getx ()][i], 1);
		if (val[i][p.gety ()] != 0 && p.getpos (val[p.getx ()][i]) == 0)
			p.setpos (val[i][p.gety ()], 1);
	}
	for (int i = (p.getx () / 3) * 3; i < (p.getx () / 3) * 3 + 3; i++)
		for (int j = (p.gety () / 3) * 3; j < (p.gety () / 3) * 3 + 3; j++)
			if (val[i][j] != 0 && p.getpos (val[p.getx ()][i]) == 0)
				p.setpos (val[i][j], 1);
	/*寻找可行解*/
	int found = 0;
	for (int i = 1; i < 10; i++)
		if (p.getpos(i) == 0) {		//找到一可行解
			found = 1;
			val[p.getx ()][p.gety ()] = i;
			p.setpos (i, 1);
			pset.del (iz);
			if (pset.isEmpty()) {
				write (fpw);
				return 1;
			}
			if (dfs ())		//找下一个空位的可行解
				return 1;
			p.setpos (i, 0);
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
		if (puzzle.isEmpty())
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