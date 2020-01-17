#include <cstdio>
#include <cstring>
#include <cstdlib>

#define num0 ((8+8)%9+1)		//���Ͻǵ�һ����
#define side 9		//�߳�

static int trans[side] = { 0,3,6,1,4,7,2,5,8 };		//�б任����
int nsudoku = -1;		//�����վ�����
FILE* fpw, * fpr;		//д�ļ������ļ�


class Seed {		//����������ͨ���б任������������ģ��
	int val[side] = { 0 };
public:
	Seed ();		//��ʼ��
	void swapi (int i1, int i2);		//����Ԫ��
	int getVal (int i) { return val[i]; }		//��ȡֵ
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
	int x, y;		//�С�������
	int pos[10] = { 0 };		//ռ��
public:
	Point () { x = 0; y = 0; }
	Point (const Point* p) { x = p->x; y = p->y; }
	void set (int x, int y) { this->x = x; this->y = y; }
	int getx () { return x; }
	int gety () { return y; }
	int getpos (int i) { return pos[i]; }
	void setpos (int i, int val) { pos[i] = val; }
};


class Pset {		//�հ׵㼯
private:
	Point zero[60];
	int size = 0;
public:
	void add (int x, int y);	//��ӿհ׵�
	void del (int iz);		//ɾ���հ׵�
	int geti ();		//���ȡ��
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
	void permutate (int level);		//ȫ���н�����
	void mod ();		//����seed����ģ��
	void generate ();		//ģ��ȫ���н����������վ�
	void cpy (const Sudoku* map);		//��������
	void swapRow (int row1, int row2);		//������
	void write (FILE* fpw);		//д���ļ�
	void set (char* buff, int i);		//���ж���
	int dfs ();		//�����������
	bool isEmpty () { return pset.isEmpty (); }
};

void Sudoku::permutate (int level) {
	if (level == side - 2) {		//ȫ���н���
		mod ();		//����ģ��
		generate ();		//�н���
		return;
	}
	for (int i = level; i < side; i++) {
		if (i != level)
			seed.swapi (level, i);		//����Ԫ��
		permutate (level + 1);		//�ݹ�
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

void Sudoku::generate () {		//�����վ�
		/*�б任*/
	Sudoku ans;
	for (int i = 0; i < 2 && nsudoku>0; i++)		//��һ�׶�
		for (int j = 0; j < 6 && nsudoku>0; j++)		//�ڶ��׶�
			for (int k = 0; k < 6 && nsudoku>0; k++) {		//�����׶�
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
				nsudoku--;		//������+1
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
	/*��ʼ��ռ�б�ʶ����*/
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
	/*Ѱ�ҿ��н�*/
	int found = 0;
	for (int i = 1; i < 10; i++)
		if (p.getpos(i) == 0) {		//�ҵ�һ���н�
			found = 1;
			val[p.getx ()][p.gety ()] = i;
			p.setpos (i, 1);
			pset.del (iz);
			if (pset.isEmpty()) {
				write (fpw);
				return 1;
			}
			if (dfs ())		//����һ����λ�Ŀ��н�
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
	if (argc == 3 && strcmp (argv[1], "-c") == 0 && (nsudoku = atoi (argv[2])) >= 1 && nsudoku <= 1000000) {		//�����վ�
		fopen_s (&fpw, ".\\sudoku.txt", "w+");
		Sudoku map;
		map.permutate (1);
	}
	else if (argc == 3 && strcmp (argv[1], "-s") == 0 && fopen_s (&fpr, argv[2], "r") == 0) {		//���
		fopen_s (&fpw, ".\\sudoku.txt", "w+");
		solve (fpr);
		if (fpr != NULL)
			fclose (fpr);
		if (fpw != NULL)
			fclose (fpw);
	}
	else
		printf ("��������\n");
	return 0;
}