#include<iostream>
#include<ctime>
#include<conio.h>
#include<Windows.h>
using namespace std;

// 棋盘大小
const int WIDTH = 9;
const int LENGTH = 9;

// 雷数
const int number_of_mine = 10;

// 颜色常量
const int COLOR_DEFAULT = 7;    // 默认白色
const int COLOR_HIDDEN = 8;     // 灰色（未探测）
const int COLOR_FLAG = 12;      // 亮红色（旗子）
const int COLOR_MINE = 12;      // 亮红色（雷）
const int COLOR_NUM1 = 9;       // 亮蓝色
const int COLOR_NUM2 = 10;      // 亮绿色
const int COLOR_NUM3 = 11;      // 亮青色
const int COLOR_NUM4 = 12;      // 亮红色
const int COLOR_NUM5 = 13;      // 亮紫色
const int COLOR_NUM6 = 14;      // 亮黄色
const int COLOR_NUM7 = 15;      // 亮白色
const int COLOR_NUM8 = 8;       // 灰色
const int COLOR_BORDER = 7;     // 白色（边框）

/*
	点类：表示棋盘中的每个格子
	haveMine: 是否有雷
	temp: 探测状态
		1. 未被探测（显示 #）
		2. 已被插旗（显示 F）
		3. 已被点开（显示周围雷数）
*/
class Point {
public:
	bool haveMine = false;
	int temp = 1;
};

// 棋盘类
class Board {
public:
	Point arr[WIDTH][LENGTH];
};

// 获取控制台句柄
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// 设置文字颜色
void setColor(int color) {
	SetConsoleTextAttribute(hConsole, color);
}

// 计算 (x, y) 周围 8 格中的雷数
int countMineAround(Board& board, int x, int y) {
	int count = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue;
			int nx = x + dx;
			int ny = y + dy;
			if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < LENGTH) {
				if (board.arr[nx][ny].haveMine) {
					count++;
				}
			}
		}
	}
	return count;
}

// 根据数字返回对应颜色
int getNumberColor(int num) {
	switch (num) {
	case 1: return COLOR_NUM1;
	case 2: return COLOR_NUM2;
	case 3: return COLOR_NUM3;
	case 4: return COLOR_NUM4;
	case 5: return COLOR_NUM5;
	case 6: return COLOR_NUM6;
	case 7: return COLOR_NUM7;
	case 8: return COLOR_NUM8;
	default: return COLOR_DEFAULT;
	}
}

// 打印当前棋盘
void printBoard(Board& board) {
	system("cls");

	// 打印列号（0-8）
	setColor(COLOR_BORDER);
	cout << "   ";
	for (int j = 0; j < LENGTH; j++) {
		cout << j << " ";
	}
	cout << endl;

	// 打印上边界
	cout << "  ";
	for (int j = 0; j < LENGTH; j++) {
		cout << "--";
	}
	cout << endl;

	// 打印每一行
	for (int i = 0; i < WIDTH; i++) {
		setColor(COLOR_BORDER);
		cout << i << "|";  // 行号

		for (int j = 0; j < LENGTH; j++) {
			if (board.arr[i][j].temp == 1) {
				// 未探测：灰色 #
				setColor(COLOR_HIDDEN);
				cout << "# ";
			}
			else if (board.arr[i][j].temp == 2) {
				// 插旗：亮红色 F
				setColor(COLOR_FLAG);
				cout << "F ";
			}
			else if (board.arr[i][j].temp == 3) {
				if (board.arr[i][j].haveMine) {
					// 雷：亮红色 *
					setColor(COLOR_MINE);
					cout << "* ";
				}
				else {
					int count = countMineAround(board, i, j);
					if (count == 0) {
						// 周围无雷：显示空格
						setColor(COLOR_DEFAULT);
						cout << "  ";
					}
					else {
						// 显示数字，使用对应颜色
						setColor(getNumberColor(count));
						cout << count << " ";
					}
				}
			}
		}

		setColor(COLOR_BORDER);
		cout << "|" << endl;
	}

	// 打印下边界
	cout << "  ";
	for (int j = 0; j < LENGTH; j++) {
		cout << "--";
	}
	cout << endl;

	// 恢复默认颜色
	setColor(COLOR_DEFAULT);
}

// 递归展开
void expand(Board& board, int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= LENGTH) return;
	if (board.arr[x][y].temp != 1) return;
	if (board.arr[x][y].haveMine) return;

	board.arr[x][y].temp = 3;

	int count = countMineAround(board, x, y);
	if (count == 0) {
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) continue;
				expand(board, x + dx, y + dy);
			}
		}
	}
}

/*
	判断游戏是否结束
		胜利：返回 1
		失败：返回 2
		未结束：返回 0
*/
int isOver(Board& board) {
	// 失败：有雷的格子被点开
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < LENGTH; j++) {
			if (board.arr[i][j].haveMine && board.arr[i][j].temp == 3) {
				return 2;
			}
		}
	}

	// 胜利：所有非雷格子都被点开
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < LENGTH; j++) {
			if (!board.arr[i][j].haveMine && board.arr[i][j].temp != 3) {
				return 0;
			}
		}
	}

	return 1;
}

int main() {
	// 初始化棋盘
	Board board;
	srand((unsigned int)time(NULL));

	// 随机布雷
	int temp_number_of_mine = 0;
	while (temp_number_of_mine < number_of_mine) {
		int x = rand() % WIDTH;
		int y = rand() % LENGTH;

		if (board.arr[x][y].haveMine == 1) {
			continue;
		}
		else {
			board.arr[x][y].haveMine = 1;
			temp_number_of_mine++;
		}
	}

	// 游戏主循环
	while (true) {
		printBoard(board);

		// 检查游戏状态
		int state = isOver(board);
		if (state == 1) {
			setColor(COLOR_NUM2);  // 亮绿色
			cout << "Good Game! You win!" << endl;
			setColor(COLOR_DEFAULT);
			break;
		}
		else if (state == 2) {
			// 显示所有雷
			for (int i = 0; i < WIDTH; i++) {
				for (int j = 0; j < LENGTH; j++) {
					if (board.arr[i][j].haveMine) {
						board.arr[i][j].temp = 3;
					}
				}
			}
			printBoard(board);
			setColor(COLOR_MINE);  // 亮红色
			cout << "Bad Game! You hit a mine!" << endl;
			setColor(COLOR_DEFAULT);
			break;
		}

		// 用户输入
		int input_x, input_y, input_do;
		setColor(COLOR_DEFAULT);
		cout << "输入格式：行 列（中间用空格隔开，范围 0-8）" << endl;
		cout << "请输入你想要进行操作的格子：";
		cin >> input_x >> input_y;

		// 输入合法性检查
		if (input_x < 0 || input_x >= WIDTH || input_y < 0 || input_y >= LENGTH) {
			setColor(COLOR_NUM4);  // 亮红色
			cout << "输入超出范围，请重新输入！" << endl;
			setColor(COLOR_DEFAULT);
			Sleep(1000);
			continue;
		}

		// 已点开的格子不能再操作
		if (board.arr[input_x][input_y].temp == 3) {
			setColor(COLOR_NUM4);
			cout << "该格子已被点开，请重新输入！" << endl;
			setColor(COLOR_DEFAULT);
			Sleep(1000);
			continue;
		}

		cout << "1.查看" << endl << "2.插旗" << endl << "0.重新输入" << endl;
		cout << "请输入你想进行的操作对应的数字：";
		cin >> input_do;

		if (input_do == 1) {
			if (board.arr[input_x][input_y].haveMine) {
				board.arr[input_x][input_y].temp = 3;
			}
			else {
				expand(board, input_x, input_y);
			}
		}
		else if (input_do == 2) {
			if (board.arr[input_x][input_y].temp == 2) {
				board.arr[input_x][input_y].temp = 1;
			}
			else {
				board.arr[input_x][input_y].temp = 2;
			}
		}
		else {
			continue;
		}
	}

	setColor(COLOR_DEFAULT);
	system("pause");
	return 0;
}