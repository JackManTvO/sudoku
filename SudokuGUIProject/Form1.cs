using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;


namespace Game
{
    public partial class Form1 : Form
    {
        public static ArrayList Game_table = new ArrayList();
        public static bool begin = false;
        public static int ipuzzle = 0;

        public Form1()
        {
            InitializeComponent();      //初始化
        }

        private void Form1_Load(object sender, EventArgs e)     //9*9格子
        {
            Game_table.Add(textBox11);
            Game_table.Add(textBox12);
            Game_table.Add(textBox13);
            Game_table.Add(textBox14);
            Game_table.Add(textBox15);
            Game_table.Add(textBox16);
            Game_table.Add(textBox17);
            Game_table.Add(textBox18);
            Game_table.Add(textBox19);
            Game_table.Add(textBox21);
            Game_table.Add(textBox22);
            Game_table.Add(textBox23);
            Game_table.Add(textBox24);
            Game_table.Add(textBox25);
            Game_table.Add(textBox26);
            Game_table.Add(textBox27);
            Game_table.Add(textBox28);
            Game_table.Add(textBox29);
            Game_table.Add(textBox31);
            Game_table.Add(textBox32);
            Game_table.Add(textBox33);
            Game_table.Add(textBox34);
            Game_table.Add(textBox35);
            Game_table.Add(textBox36);
            Game_table.Add(textBox37);
            Game_table.Add(textBox38);
            Game_table.Add(textBox39);
            Game_table.Add(textBox41);
            Game_table.Add(textBox42);
            Game_table.Add(textBox43);
            Game_table.Add(textBox44);
            Game_table.Add(textBox45);
            Game_table.Add(textBox46);
            Game_table.Add(textBox47);
            Game_table.Add(textBox48);
            Game_table.Add(textBox49);
            Game_table.Add(textBox51);
            Game_table.Add(textBox52);
            Game_table.Add(textBox53);
            Game_table.Add(textBox54);
            Game_table.Add(textBox55);
            Game_table.Add(textBox56);
            Game_table.Add(textBox57);
            Game_table.Add(textBox58);
            Game_table.Add(textBox59);
            Game_table.Add(textBox61);
            Game_table.Add(textBox62);
            Game_table.Add(textBox63);
            Game_table.Add(textBox64);
            Game_table.Add(textBox65);
            Game_table.Add(textBox66);
            Game_table.Add(textBox67);
            Game_table.Add(textBox68);
            Game_table.Add(textBox69);
            Game_table.Add(textBox71);
            Game_table.Add(textBox72);
            Game_table.Add(textBox73);
            Game_table.Add(textBox74);
            Game_table.Add(textBox75);
            Game_table.Add(textBox76);
            Game_table.Add(textBox77);
            Game_table.Add(textBox78);
            Game_table.Add(textBox79);
            Game_table.Add(textBox81);
            Game_table.Add(textBox82);
            Game_table.Add(textBox83);
            Game_table.Add(textBox84);
            Game_table.Add(textBox85);
            Game_table.Add(textBox86);
            Game_table.Add(textBox87);
            Game_table.Add(textBox88);
            Game_table.Add(textBox89);
            Game_table.Add(textBox91);
            Game_table.Add(textBox92);
            Game_table.Add(textBox93);
            Game_table.Add(textBox94);
            Game_table.Add(textBox95);
            Game_table.Add(textBox96);
            Game_table.Add(textBox97);
            Game_table.Add(textBox98);
            Game_table.Add(textBox99);
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            Sudoku sudoku = new Sudoku();
            sudoku.nsudoku = 100;
            using (StreamWriter sw = new StreamWriter("checkboard.txt"))
            {
                sw.Write("");
            }
            sudoku.permutate(1);
            MessageBox.Show("重置完成");
        }

        private void button_restart_Click(object sender, EventArgs e)
        {
            StreamReader buf = new StreamReader("checkboard.txt");
            for (int k = 0; k < 10 * (ipuzzle-1); k++)
            {
                string line0 = buf.ReadLine(); 
            }
            for (int i = 0; i < 9; i++)
            { 
                string line = buf.ReadLine();
                for (int j = 0; j < 9; j++)
                {
                    TextBox text_element = Game_table[i * 9 + j] as TextBox;
                    if (line[2 * j] == '0')
                    { 
                        text_element.Text = "";
                        text_element.ReadOnly = false;
                        text_element.ForeColor = Color.Black;
                        text_element.BackColor = Color.Turquoise;
                    }
                    else
                    {
                        text_element.Text = line[2 * j].ToString();
                        text_element.ReadOnly = true;
                        text_element.ForeColor = Color.Black;
                        text_element.BackColor = Color.Pink;
                    }
                }
            }
            begin = true;
        }

        private void button_next_Click(object sender, EventArgs e)
        {
            StreamReader buf = new StreamReader("checkboard.txt");
            for (int k = 0; k < 10 * ipuzzle; k++)
            {
                string linemore = buf.ReadLine();
            }
            for (int i = 0; i < 9; i++)
            {
                string line = buf.ReadLine();
                for (int j = 0; j < 9; j++)
                {
                    TextBox text_element = Game_table[i * 9 + j] as TextBox;
                    if (line[2 * j] == '0')
                    {
                        text_element.Text = "";
                        text_element.ReadOnly = false;
                        text_element.ForeColor = Color.Black;
                        text_element.BackColor = Color.Turquoise;
                    }
                    else
                    {
                        text_element.Text = line[2 * j].ToString();
                        text_element.ReadOnly = true;
                        text_element.ForeColor = Color.Black;
                        text_element.BackColor = Color.Pink;
                    }
                }
            }
            ipuzzle++;
            begin = true;
        }

        private void button_submit_Click(object sender, EventArgs e)
        {
            if (begin == false)
            {
                MessageBox.Show("未开始游戏");
                return;
            }
            int[,] map = new int[9, 9];
    /*检查是否完成*/
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    TextBox text_element = Game_table[i * 9 + j] as TextBox;
                    if (text_element.Text == "")
                    {
                        MessageBox.Show("未完成所有空白");
                        return;
                    }
                    else
                        map[i, j] = (text_element.Text.ToCharArray())[0] - '0';
                }
            }

    /*没有空白*/
            int[] judge = new int[10];
            for (int i = 0; i < 9; i++)     //逐行
            {
                for (int j = 1; j <= 9; j++)
                    judge[i] = 0;
                for (int j = 0; j < 9; j++)
                {
                    int x = map[i, j];
                    judge[x] = 1;
                }
                for (int k = 1; k <= 9; k++)
                {
                    if (judge[k] != 1)
                    {
                        MessageBox.Show("答案错误");
                        return;
                    }
                }
            }
            for (int i = 0; i < 9; i++)     //逐列
            {
                for (int j = 1; j <= 9; j++)
                    judge[i] = 0;
                for (int j = 0; j < 9; j++)
                {
                    int x = map[j, i];
                    judge[x] = 1;
                }
                for (int k = 1; k <= 9; k++)
                {
                    if (judge[k] != 1)
                    {
                        MessageBox.Show("答案错误");
                        return;
                    }
                }
            }
            for (int i = 0; i < 9; i += 3)      //逐宫
                for (int j = 0; j < 9; j += 3)
                {
                    for (int k = 1; k <= 9; k++)
                        judge[k] = 0;
                    for (int a = 0; a < 3; a++)
                        for (int b = 0; b < 3; b++)
                        {
                            int x = map[i + a, j + b];
                            judge[x] = 1;
                        }
                    for (int k = 1; k <= 9; k++)
                    {
                        if (judge[k] != 1)
                        {
                            MessageBox.Show("答案错误");
                            return;
                        }
                    }
                }
            MessageBox.Show("回答正确");
        }

        private void CheckKey(object sender, KeyPressEventArgs e)
        {
            if (begin == false)
                return;
            if (e.KeyChar != '\b' && (e.KeyChar < '1' || e.KeyChar > '9'))
                e.Handled = true;
        }
    }

    class Seed
    {
        private int[] val = new int[9];

        public Seed()
        {
            val[0] = 8;
            for (int i = 1; i < 9; i++)
            {
                if (i >= 8)
                    val[i] = i + 1;
                else
                    val[i] = i;
            }
        }

        public void swapi(int i1, int i2)
        {
            int temp;
            temp = val[i1];
            val[i1] = val[i2];
            val[i2] = temp;
        }

        public int getVal(int i)
        {
            return val[i];
        }
    }

    class Sudoku
    {
        public int nsudoku = 0;
        private Seed seed = new Seed();
        private static int[] trans = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
        private int[,] val = new int[9, 9];

        public void permutate(int level)      //全排列交换数
        {
            if (level == 7)
            {
                mod();
                generate();
                return;
            }
            for (int i = level; i < 9; i++)
            {
                if (i != level)
                    seed.swapi(level, i);
                permutate(level + 1);
                if (nsudoku == 0)
                    return;
                if (i != level)
                    seed.swapi(level, i);
            }
            return;
        }

        public void mod()    //根据seed生成模板
        {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    val[i, j] = seed.getVal((j + trans[i]) % 9);
            return;
        }

        public void puzzle()        //生成谜题
        {
            int max = 60;
            int min = 30;
            int[] rand = new int[9];
            Random rd = new Random();
            for (int i = 0; i < 9; i++)        //逐宫
            {
                int imax = max - (8 - i) * 2;
                int imin = min - (8 - i) * 9;
                if (imin < 2)
                    imin = 2;
                if (imax > 9)
                    imax = 9;
                rand[i] = rd.Next(imin, imax);      //该宫内的数量
                max -= rand[i];
                min -= rand[i];

                int[] arrNum = new int[rand[i]];
                for (int ir = 0; ir < rand[i]; ir++)
                {
                Re:
                    int x = rd.Next(1, 9);
                    for (int ix = 0; ix < ir; ix++)
                    {
                        if (arrNum[ix] == x)
                            goto Re;
                    }
                    arrNum[ir] = x;
                    val[(i / 3) * 3 + (x - 1) / 3, (i % 3) * 3 + (x - 1) % 3] = 0;
                }
            }
        }

        public void generate()        //模板全排列交换行生成终局
        {       //生成终局
            /*行变换*/
            for (int i = 0; i < 2 && nsudoku > 0; i++)
            {
                for (int j = 0; j < 6 && nsudoku > 0; j++)
                {
                    Sudoku map = new Sudoku();
                    map.cpy(this);
                    map.puzzle();
                    for (int k = 0; k < 6 && nsudoku > 0; k++)
                    {
                        Sudoku ans = new Sudoku();
                        ans.cpy(map);
                        switch (i)
                        {
                            case 0:
                                break;
                            case 1:
                                ans.swapRow(1, 2); break;
                        }
                        switch (j)
                        {
                            case 0:
                                break;
                            case 1:
                                ans.swapRow(4, 5); break;
                            case 2:
                                ans.swapRow(3, 4); break;
                            case 3:
                                ans.swapRow(3, 5); ans.swapRow(3, 4); break;
                            case 4:
                                ans.swapRow(3, 4); ans.swapRow(3, 5); break;
                            case 5:
                                ans.swapRow(3, 5); break;
                        }
                        switch (k)
                        {
                            case 0:
                                break;
                            case 1:
                                ans.swapRow(7, 8); break;
                            case 2:
                                ans.swapRow(6, 7); break;
                            case 3:
                                ans.swapRow(6, 8); ans.swapRow(6, 7); break;
                            case 4:
                                ans.swapRow(6, 7); ans.swapRow(6, 8); break;
                            case 5:
                                ans.swapRow(6, 8); break;
                        }

                        ans.write();
                        nsudoku--;
                        if (nsudoku == 0)
                            return;
                        using (StreamWriter sw = new StreamWriter("checkboard.txt", true))
                        {
                            sw.Write("\n\n");
                        }
                    }
                }
            }
            return;
        }

        public void cpy(Sudoku sudoku)
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    val[i, j] = sudoku.val[i, j];
                }
            }
        }

        public void swapRow(int row1, int row2)       //交换行
        {
            int[] temp = new int[9];
            for (int i = 0; i < 9; i++)
            {
                temp[i] = val[row1, i];
                val[row1, i] = val[row2, i];
            }
            for (int i = 0; i < 9; i++)
                val[row2, i] = temp[i];
        }

        public void write()      //写入文件
        {
            string str = "";
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    str += val[i, j].ToString();
                    if (j < 8)
                        str += " ";
                }
                if (i < 8)
                    str += "\n";
            }
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"checkboard.txt", true))
            {
                file.Write(str);//直接追加文件末尾，不换行
            }
        }
    }
}
