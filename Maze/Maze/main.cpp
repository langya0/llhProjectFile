#include<iostream>
#include<assert.h>
#include<stack>
using namespace std;
////////////////////////////////////////////////////////////
//题目:迷宫求解问题。
//1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1
//2 2 2 1 1 1 1 1 1 1
//1 1 2 1 1 1 1 1 1 1
//1 1 3 2 2 2 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 3 2 2 2 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 2 1 1 1 1 1 1 1
//入口，出口判断:坐标为边界。
//走过的路进行标记并且入栈。
//回退时，修改为-1；表示此路不通。
//4个方向进行遍历，顺时针进行。
//
//
////////////////////////////////////////////////////////////


/////////////////////////
//本程序解决了迷宫求解问题。
//基本思路，靠右行，或者靠左行。--找到解
//优化点1走过的路程并非固定标记。而是+=2。这样的做法可以得到具体路过该位置的次数

//优化点2通过判断下一步的值，确定之前是否路过，如果路过，那么一直回退到该点。排除了死胡同和环的问题。


//缺点，没有找到所有的可通行路径，只是找到两种走法的最优路线。


//最优解求法思路：1递归实现，对每个位置进行四个方向分别求解，而非靠左或者右行走。
//2.遇到十字路口，在递归中对每个路线求解，
//3.最终路线为最短的几个路线

//if(方向3个
//stack+=min（）；
//
void GetMazeMap(int *a,int row,int col)
{
	FILE *FOut;
	fopen_s(&FOut,"MazeMap.txt", "r");
	
	assert(FOut);

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col;)
		{
			char ch = fgetc(FOut);
			if (ch == '0' || ch == '1')
			{
				a[i*col + j] = ch - '0';
				++j;
			}
		}
	}
}

void PrintMazeMap(int *a, int row, int col)
{
	cout << "MazeMap:(row,col):(" << row <<","<< col <<")."<< endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << a[i*col + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
struct Step
{
	int row;// 行
	int col;//列
	bool operator==(Step &s)
	{
		return row == s.row&&col == s.col;
	}
};
struct man
{
	man(Step s, int d)
	:_cur(s)
	, _dir(d)
	{}
	Step _cur;
	int _dir;// 0-3 表明4个方向

	man nextPos(int dir)
	{
		Step cur = _cur;
		dir = (dir+4) % 4;
		switch (dir)
		{
		case 0:
			cur.row--;
			break;
		case 1:
			cur.col++;
			break;
		case 2:
			cur.row++;
			break;
		case 3:
			cur.col--;
			break;
		}
		return man(cur, dir);
	}
};

stack<man> paths;


void PrintPathStep()
{
	while (!paths.empty())
	{
		man tmp = paths.top();
		cout << "[" << tmp._cur.row << "," << tmp._cur.col << "]:" << tmp._dir << "" << "-->\t";

		paths.pop();
	}
	cout << "Over!" << endl;
}

//给定map和入口点坐标，求迷宫解
void GetMazePaths(int *map, int row, int col, Step& entry)
{
	//当前位置.
	man m(entry, 0);
	map[m._cur.row*col + m._cur.col] = 2;
	paths.push(m);
	while (1)
	{
		man top = paths.top();
		man cur = top.nextPos(top._dir - 1);
		//man cur = top.nextPos(top._dir + 1);

		if (cur._cur.col<0 || cur._cur.row<0 || cur._cur.col>=col || cur._cur.row>=row)
		{
			cout << "越界" << endl;
			top._dir++;
			
			//top._dir--;
			paths.pop();
			paths.push(top);
			continue;
		}
		//边界，也就是出入口
		if ((cur._cur.col == 0 || cur._cur.row == 0 || cur._cur.col == col-1 || cur._cur.row == row-1)
			&&map[cur._cur.row*col + cur._cur.col] == 0)
		{
			cout << "这里是出入口" << endl;
			if (!(cur._cur == entry))
			{

				map[cur._cur.row*col + cur._cur.col] = 2;
				paths.push(cur);
				cout << "得到出口" <<cur._cur.row<<" "<<cur._cur.col<< endl;
				break;
			}
		}

		//遍历：
		//下一个位置为当前方向的下一个位置
		if (map[cur._cur.row*col + cur._cur.col] != 1)
		{
			map[cur._cur.row*col + cur._cur.col] += 2;

			if (map[cur._cur.row*col + cur._cur.col] == 4)
			{
				Step tmp;
				tmp.row = cur._cur.row;
				tmp.col = cur._cur.col;
				//////////////////////////////////////////////////////////////////////////////
				//回退过程。
				while (paths.top()._cur.row != tmp.row || paths.top()._cur.col != tmp.col)
				{
					map[paths.top()._cur.row*col + paths.top()._cur.col] = 1;
					paths.pop();
				}
			}
			map[cur._cur.row*col + cur._cur.col] = 2;
			paths.push(cur);
		}
		else
		{
			top._dir++;
			
			//top._dir--;
			paths.pop();
			paths.push(top);
		}
	}
}

void GetNextAccessPath(int *map, int row, int col, man& entry)
{
	man tmp(entry);
	if (
		(
		entry._cur.row == row - 1 
		|| entry._cur.col == col - 1
		||entry._cur.row == 0 
	//	|| entry._cur.col == 0
	)
		&&	map[entry._cur.row*col + entry	._cur.col] != 1
		)
	{
		paths.push(entry);
		return;
	}
	else
	{
		paths.push(entry);
		tmp = entry.nextPos(entry._dir-1);
		while (map[tmp._cur.row*col + tmp._cur.col] == 1)
		{
			tmp = entry.nextPos(tmp._dir+1);
		}
		entry = tmp;
		GetNextAccessPath(map, row, col, entry);
	}
}

void GetMazePathsMin(int *map, int row, int col, man& entry)
{
	man tmp(entry);
	if (
		(
		entry._cur.row == row - 1
		|| entry._cur.col == col - 1
		|| entry._cur.row == 0
		//	|| entry._cur.col == 0
		)
		&& map[entry._cur.row*col + entry._cur.col] != 1
		)
	{
		paths.push(entry);
		return;
	}
	else
	{
		man m(entry);
		map[m._cur.row*col + m._cur.col] = 2;
		paths.push(m);

		man tmp1 = m.nextPos(m._dir - 1);
		man tmp2 = m.nextPos(m._dir);
		man tmp3 = m.nextPos(m._dir + 1);

		if (map[tmp1._cur.row*col + tmp1._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp1);
		}
		if (map[tmp2._cur.row*col + tmp2._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp2);
		}
		if (map[tmp3._cur.row*col + tmp3._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp3);
		}
			//向左走，向右走

			//if(左右走，位置不同)

			//分支确定，可能为3个方向。
			//求该点到末端的最短路径。
	}

}

void main()
{
	int a[20][20] = {};   
	::GetMazeMap((int*)a, 20, 20);
	Step ent = { 2, 0 };
	man entm = { { 2, 0 }, 1 };
	GetMazePaths((int*)a, 20, 20, ent);
	//GetMazePathsMin((int*)a, 20, 20, entm);
	//GetNextAccessPath((int*)a, 10, 10, man(ent, 0));

	::PrintMazeMap((int*)a, 20, 20);
	::PrintPathStep();
	system("pause");
}
