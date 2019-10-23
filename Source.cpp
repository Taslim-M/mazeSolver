#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include <thread>
#include <chrono>
#include "cls.h"
using namespace std::this_thread;
using namespace std::chrono;
using std::queue;
using std::stack;
using std::vector;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ostream;
using std::cin;

void printMenu();

class Maze {
private:
	class Cell {
		friend ostream& operator << (ostream&os, const Cell &rhs) {
			os << "(" << rhs.x << " , " << rhs.y << " )";
			return os;
		}
	public:
		Cell() :status(' '), x(-1), y(-1), isVisited(true), prevX(-1), prevY(-1) {}
		int x, y, prevX, prevY;
		bool top, down, right, left;
		bool isVisited;
		char status;
	};

	string fname;
	Maze::Cell** maze;
	int height, width;
	int startX, startY;
	int goalX, goalY;
	vector<string> rawMaze;
	stack<Cell*> s;
	queue<Cell*> q;

	void alloc(int row, int col) {
		maze = new Cell*[col];
		for (int k = 0; k < col; ++k) {
			maze[k] = new Cell[row]{};
		}
	}
	void getFileConfig() {
		ifstream fin(fname);
		if (fin.fail()) {
			cout << "File not found\n";
			return;
		}
		string temp;
		height = 1;
		width = 0;
		while (getline(fin, temp)) {
			if (height == 1)
				width = temp.length();
			++height;
		}
		height = (height - 2) / 2;
		width = (width - 1) / 4;
		fin.close();
	}
	void rePrint(const vector<string> copy); // for animation purpose
public:
	Maze(string filname = "smaze.txt") :fname(filname) {};
	void readMaze();
	void setfname(string f) {
		fname = f;
	}
	void printMaze();
	bool setStart(int atx, int aty) {
		if (atx > width || aty > height || atx < 1 || aty < 1) {
			cout << "Invalid start coordinate";
			return false;
		}
		startX = atx;
		startY = aty;
		maze[atx][aty].status = 'S';
		return true;
	}
	bool setGoal(int atx, int aty) {
		if (atx > width || aty > height || atx < 1 || aty < 1) {
			cout << "Invalid Goal coordinate";
			return false;
		}
		goalX = atx; goalY = aty;
		maze[atx][aty].status = 'G';
		return true;
	}
	void DFS();
	void BFS();
	void setVisitedFalse();
	void printPath();
	void trialPrint(int x);
	~Maze() {
		for (int k = 0; k < height+2; ++k) {
			delete[] maze[k];
		}
		delete[] maze;
		maze = nullptr;
	}
};

void Maze::DFS() {
	setVisitedFalse();
	Cell* currentCell;
	int stX = startX;
	int stY = startY;
	s.push(&maze[startX][startY]);
	while (!s.empty()) {
		trialPrint(50);
		currentCell = s.top();
		currentCell->isVisited = true;
		if (currentCell->status != 'G'&& currentCell->status != 'S')
			currentCell->status = '*';
		if (currentCell->status == 'G') {//success condition
			break;
		}
		if (!currentCell->down && !maze[stX][stY + 1].isVisited) { //if bottom is not wall and not visited
			maze[stX][stY + 1].prevX = stX;
			maze[stX][stY + 1].prevY = stY;
			s.push(&maze[stX][++stY]);
		}
		else if (!currentCell->right && !maze[stX + 1][stY].isVisited) { //if right is not wall and not visited
			maze[stX + 1][stY].prevX = stX;
			maze[stX + 1][stY].prevY = stY;
			s.push(&maze[++stX][stY]);
		}
		else if (!currentCell->top && !maze[stX][stY - 1].isVisited) {//if top is not wall and not visited
			maze[stX][stY - 1].prevX = stX;
			maze[stX][stY - 1].prevY = stY;
			s.push(&maze[stX][--stY]);
		}
		else if (!currentCell->left && !maze[stX - 1][stY].isVisited) { //if left is not wall and not visited
			maze[stX - 1][stY].prevX = stX;
			maze[stX - 1][stY].prevY = stY;
			s.push(&maze[--stX][stY]);
		}
		else {
			stX = currentCell->prevX;
			stY = currentCell->prevY;
			s.pop();
		}
	}
	while (!s.empty()) {
		currentCell = s.top();
		if (currentCell->status != 'G'&& currentCell->status != 'S')
			currentCell->status = '@';
		cout << *currentCell;
		//trialPrint(10);
		s.pop();
	}
}

void Maze::BFS() {
	setVisitedFalse();
	q.push(&maze[startX][startY]);
	Cell* currentCell;
	int stX = startX;
	int stY = startY;
	while (!q.empty()) {
		currentCell = q.front();
		stX = currentCell->x;
		stY = currentCell->y;
		maze[stX][stY].isVisited = true;
		if (currentCell->status != 'G'&& currentCell->status != 'S')
			currentCell->status = '*';
		if (currentCell->status == 'G') {//success condition
			break;
		}
		if (!currentCell->down && !maze[stX][stY + 1].isVisited) {//If bottom is not wall and not visited
			maze[stX][stY + 1].prevX = stX;
			maze[stX][stY + 1].prevY = stY;
			q.push(&maze[stX][stY + 1]);
		}
		if (!currentCell->right && !maze[stX + 1][stY].isVisited) {//If right is not wall and not visited
			maze[stX + 1][stY].prevX = stX;
			maze[stX + 1][stY].prevY = stY;
			q.push(&maze[stX + 1][stY]);
		}
		if (!currentCell->top && !maze[stX][stY - 1].isVisited) {//if top is not wall and not visited
			maze[stX][stY - 1].prevX = stX;
			maze[stX][stY - 1].prevY = stY;
			q.push(&maze[stX][stY - 1]);
		}
		if (!currentCell->left && !maze[stX - 1][stY].isVisited) {//If left is not wall and not visited
			maze[stX - 1][stY].prevX = stX;
			maze[stX - 1][stY].prevY = stY;
			q.push(&maze[stX - 1][stY]);
		}
		q.pop();
	}
	if (!q.empty()) {
		while (true) {
			currentCell = &maze[stX][stY];
			if (currentCell->status != 'G'&& currentCell->status != 'S')
				currentCell->status = '@';
			cout << *currentCell;
			stX = currentCell->prevX;
			stY = currentCell->prevY;
			if (stX == startX && stY == startY) {
				cout << maze[stX][stY];
				break;
			}
		}
	}
}

void Maze::readMaze() {
	getFileConfig();
	alloc(height + 2, width + 2);
	ifstream in(fname.c_str());
	if (in.fail()) {
		cout << "File not found\n";
		return;
	}
	char str1[100], str2[100], str3[100];
	in.getline(str1, 100);
	in.getline(str2, 100);
	in.getline(str3, 100);
	rawMaze.push_back(str1);
	rawMaze.push_back(str2);
	rawMaze.push_back(str3);
	int line = 1, cell = 1;
	while (!in.eof())
	{
		cell = 1;
		//cout << "line " << line << endl;
		int i = 0;
		bool top, down, right, left;
		top = down = right = left = false;
		int j = 0, k = 1;
		//cout << "strlen = " << strlen(str1) << endl;
		Cell currentCell;
		while (i < strlen(str1) - 1)
		{
			top = down = right = left = false;
			if (str1[i] == '+')
				i++; //new cell
			if (str1[i] == '-') {
				top = true;
				currentCell.top = true;
			}
			else if (str1[i] == ' ') {
				top = false;
				currentCell.top = false;
			}
			i = i + 3; //wall above

			if (str2[j] == '|') {
				left = true;
				currentCell.left = true;
			}
			else if (str2[j] == ' ') {
				left = false;
				currentCell.left = false;
			}
			j = j + 4; //left wall

			if (str2[j] == '|') {
				right = true;
				currentCell.right = true;
			}
			else if (str2[j] == ' ') {
				right = false; //right wall
				currentCell.right = false;
			}

			if (str3[k] == ' ') {
				down = false;
				currentCell.down = false;
			}
			else if (str3[k] == '-') {
				down = true;
				currentCell.down = true;
			}
			k = k + 4; //wall below
			currentCell.x = cell;
			currentCell.y = line;
			currentCell.isVisited = false;
			maze[cell][line] = currentCell;
			//cout << "cell = " << cell << endl;
			cell++;
			//cout << "Top :  " << top << "\t";
			//cout << "down :  " << down << endl;
			//cout << "right :  " << right << "\t";
			//cout << "left :  " << left << endl;
			//cout << endl << endl;

		}
		strcpy(str1, str3);
		//cout << str1 << endl;
		in.getline(str2, 100);
		in.getline(str3, 100);
		rawMaze.push_back(str2);
		rawMaze.push_back(str3);
		//cout << str2 << endl;
		//cout << str3 << endl;

		line++;
	}
	cout << "Maze Loaded" << endl;
	in.close();
}

void Maze::setVisitedFalse() {
	for (int h = 1; h <= height; h++) {
		for (int w = 1; w <= width; w++) {
			maze[w][h].isVisited = false;
			if (maze[w][h].status != 'S' && maze[w][h].status != 'G')
				maze[w][h].status = ' ';
		}
	}
}
void Maze::rePrint(const vector<string> copy) { // for animation purpose
	for (int k = 0; k < copy.size(); ++k)
		cout << copy.at(k) << endl;
}
void Maze::printMaze() {
	auto copy = rawMaze;
	if (startX > 0)
		copy.at(2 * startY - 1).replace(2 + 4 * (startX - 1), 1, "S");
	if (goalX > 0)
		copy.at(2 * goalY - 1).replace(2 + 4 * (goalX - 1), 1, "G");
	for (int k = 0; k < copy.size(); k++) {
		cout << copy.at(k) << endl;
	}
}
void Maze::printPath() {
	auto copyMaze = rawMaze;
	string st;
	int h = 1;
	for (int k = 0; k < copyMaze.size() - 2; k++) {
		if (k % 2 == 1) {
			for (int w = 1; w <= width; w++) {
				st = maze[w][h].status;
				copyMaze.at(k).replace(4 * (w - 1) + 2, 1, st);
			}
			sleep_for(milliseconds(200));
			cls();
			//system("CLS");
			rePrint(copyMaze);
			++h;
		}
		//cout << copyMaze.at(k) << endl;
	}
}
void Maze::trialPrint(int x) {
	auto copyMaze = rawMaze;
	string st;
	int h = 1;
	for (int k = 0; k < copyMaze.size() - 2; k++) {
		if (k % 2 == 1) {
			for (int w = 1; w <= width; w++) {
				st = maze[w][h].status;
				copyMaze.at(k).replace(4 * (w - 1) + 2, 1, st);
			}
			
			rePrint(copyMaze);
			sleep_for(milliseconds(x));
			cls();
			++h;
		}
	}
}
//========================================//
int main()
{
	Maze maze{};

	//maze.readMaze();
	//maze.setStart(1, 1);
	//maze.setGoal(5, 5);
	//maze.printMaze();
	//maze.DFS();
	//maze.printPath();
	int x, y;
	string c;
	bool setM = false, setS = false, setG = false;
	while (1) {
		printMenu();
		getline(cin, c);
		if (c == "1") {
			if (setM) {
				cout << "Maze Already loaded" << endl;
			}
			else {
				setM = true;
				cout << "Input 1 to change default filename, anything else to use default ";
				getline(cin, c);
				if (c == "1") {
					cout << "Enter file name: ";
					getline(cin, c);
					maze.setfname(c);
				}
				maze.readMaze();
			}
		}
		else if (c == "2") {
			if (setM)
				maze.printMaze();
			else
				cout << "Load Maze Please" << endl;
		}
		else if (c == "3") {
			if (setM) {
				cout << "Enter x cordinate for start: ";
				cin >> x;
				cout << "Enter y cordinate for start: ";
				cin >> y;
				if (maze.setStart(x, y))
					setS = true;
			}
			else
				cout << "Load Maze Please" << endl;
		}
		else if (c == "4") {
			if (setM) {
				cout << "Enter x cordinate for Goal: ";
				cin >> x;
				cout << "Enter y cordinate for Goal: ";
				cin >> y;
				if (maze.setGoal(x, y))
					setG = true;
			}
			else
				cout << "Load Maze Please" << endl;
		}
		else if (c == "5") {
			if (setM && setG && setS) {
				maze.DFS();
			}
			else
				cout << "Load Configurations Please" << endl;
		}
		else if (c == "6") {
			if (setM && setG && setS) {
				maze.BFS();
			}
			else
				cout << "Load Configurations Please" << endl;
		}
		else if (c == "7") {
			if (setM && setG && setS) {
				maze.printPath();
			}
			else
				cout << "Load Configurations Please" << endl;
		}
		else if (c == "8") {
			break;
		}
	}
	system("CLS");
	cout << "GoodBye" << endl;
	system("pause");
}

void printMenu() {
	cout << "\n1.Load Maze" <<
		"\n2.Display Maze" <<
		"\n3.Set Start" <<
		"\n4.Set Goal" <<
		"\n5.Find Path using DFS" <<
		"\n6.Find Path using BFS" <<
		"\n7.Display Path " <<
		"\n8.Exit" << endl;
}