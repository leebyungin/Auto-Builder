#include<iostream>
#include<stdlib.h>	//rand()
#include<vector>
#include <windows.h>

void gotoxy(int x, int y) {
	COORD pos{ x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
#define EDGETYPE char
using namespace std;

class wall {
private:
public:
	void operator() (int n) {
		switch (n)
		{
		case 0:
			cout << "��";
			break;
		case 1:
			cout << "��";
			break;
		case 2:
			cout << "��";
			break;
		case 3:
			cout << "��";
			break;
		case 4:
			cout << "��";
			break;
		case 5:
			cout << "��";
			break;
		case 6:
			cout << "  ";
			break;
		default:
			cout << "na ";
			break;
		}
		return;
	}
};

wall build;

class Tile {
private:
	int TileType;
	int EdgeSize;
	EDGETYPE*TileTop;
	EDGETYPE*TileRight;
	EDGETYPE*TileLeft;
	EDGETYPE* TileBottom;
public:
	Tile() {}
	Tile(int id=-1,int edgeSize=0):TileType(id),EdgeSize(edgeSize) {	//Ÿ�� Ÿ�Կ� �ٿ��� id, Ÿ���� �� �𼭸��� �̷�� �ִ� ��� ��
		if (id != -1) {
			if (edgeSize == 0) {
				TileTop = nullptr;
				return;
			}
			TileTop = new EDGETYPE[edgeSize * 4]{ 0 };	//�޸� �������� ����Ű�� ���ؼ�..
			TileRight = TileTop + edgeSize;
			TileBottom = TileRight + edgeSize;
			TileLeft = TileBottom + edgeSize;
		}
	}
	void EdgeDataUpdate(EDGETYPE *arr=nullptr) {	//Ÿ���� �𼭸� ������ �Է�
		int temp;
		if (arr!=nullptr) {
			for (int i = 0; i < EdgeSize*4; i++) {
				*(TileTop + i) = (int)arr[i];
			}
		}
		else {
			cout << "TileTop: ";
			for (int i = 0; i < EdgeSize; i++) {
				cin >> temp;
				*(TileTop + i) = temp;
			}
			cout << "TileRight: ";
			for (int i = 0; i < EdgeSize; i++) {
				cin >> temp;
				*(TileRight + i) = temp;
			}
			cout << "TileBottom: ";
			for (int i = 0; i < EdgeSize; i++) {
				cin >> temp;
				*(TileBottom + i) = temp;
			}
			cout << "TileLeft: ";
			for (int i = 0; i < EdgeSize; i++) {
				cin >> temp;
				*(TileLeft + i) = temp;
			}
		}
	}
	void Print() {
		cout << "\tshape : ";
		build(TileType);
		cout << endl;
		if (TileTop == nullptr) {
			cout << "It's Null" << endl;
			return;
		}
		cout << "\tTOP : ";
		for (int i = 0; i < EdgeSize; i++) {
			cout <<(int) TileTop[i] << " ";
		}
		cout<<endl<< "\tRight : ";
		for (int i = 0; i < EdgeSize; i++) {
			cout << (int)TileRight[i] << " ";
		}
		cout << endl << "\tBottom : ";
		for (int i = 0; i < EdgeSize; i++) {
			cout << (int)TileBottom[i] << " ";
		}
		cout << endl << "\tLeft : ";
		for (int i = 0; i < EdgeSize; i++) {
			cout << (int)TileLeft[i] << " ";
		}
	}
	const EDGETYPE* GetTopEdge() {
		return TileTop;
	}
	const EDGETYPE* GetRightEdge() {
		return TileRight;
	}
	const EDGETYPE* GetBottomEdge() {
		return TileBottom;
	}
	const EDGETYPE* GetLeftEdge() {
		return TileLeft;
	}
	~Tile() {
		delete[] TileTop;
	}
};

class TileManager {
private:
	Tile **arr;
	int TileCount;
	int EdgeSize;
	vector<EDGETYPE> *Top;
	vector<EDGETYPE> *Right;
	vector<EDGETYPE> *Bottom;
	vector<EDGETYPE> *Left;
public:
	TileManager(int c, int TileSize, int autoTile = 0) :TileCount(c), EdgeSize(TileSize) {	//���� �� Ÿ���� �� c, Ÿ���� �𼭸� ũ�� TileSize
		arr = new Tile * [c];
		for (int i = 0; i < c; i++) {
			arr[i] = new Tile(i, TileSize);	//Ÿ�� ����
			cout << endl<< "======Tile : " << i << " (Total: " << c << " Each edge component: " << TileSize << " )" << "======" << endl;

			if(autoTile==0)
				arr[i]->EdgeDataUpdate();
			else {
				EDGETYPE input[28] = {
				0, 2, 0, 2,
				2 ,0 ,2, 0,
				0 ,2 ,2 ,0,
				2 ,2, 0, 0,
				0 ,0 ,2, 2,
				2 ,0, 0, 2,
				0, 0, 0, 0 };
				arr[i]->EdgeDataUpdate(input+i*4);
			}
		}
		Top = new vector<EDGETYPE>[c];	//�̿��ϴ� Ÿ�� ������ ����. [c]�� ������ Ÿ�� ���̵�� ����
		Right= new vector<EDGETYPE>[c];
		Bottom= new vector<EDGETYPE>[c];
		Left= new vector<EDGETYPE>[c];
		
		TileMatch();
	}
	void TileMatch() {
		for (int i = 0; i < TileCount; i++) {
			const EDGETYPE* myTop=arr[i]->GetTopEdge();
			const EDGETYPE* myRight = arr[i]->GetRightEdge();
			const EDGETYPE* myBottom = arr[i]->GetBottomEdge();
			const EDGETYPE* myLeft = arr[i]->GetLeftEdge();

			for (int j = 0; j < TileCount; j++) {
				const EDGETYPE* compareTop = arr[j]->GetTopEdge();
				const EDGETYPE* compareRight = arr[j]->GetRightEdge();
				const EDGETYPE* compareBottom = arr[j]->GetBottomEdge();
				const EDGETYPE* compareLeft = arr[j]->GetLeftEdge();

				for (int k = 0; k < EdgeSize; k++) { //���ʿ� �̿��� Ÿ��
					if (myTop[k] != compareBottom[k])
						break;
					if (k == EdgeSize - 1) {
						Top[i].push_back(j);
					}
				}
				for (int k = 0; k < EdgeSize; k++) { //�����ʿ� �̿��� Ÿ��
					if (myRight[k] != compareLeft[k])
						break;
					if (k == EdgeSize - 1)
						Right[i].push_back(j);
				}
				for (int k = 0; k < EdgeSize; k++) { //�Ʒ��ʿ� �̿��� Ÿ��
					if (myBottom[k] != compareTop[k])
						break;
					if (k == EdgeSize - 1)
						Bottom[i].push_back(j);
				}
				for (int k = 0; k < EdgeSize; k++) { //���ʿ� �̿��� Ÿ��
					if (myLeft[k] != compareRight[k])
						break;
					if (k == EdgeSize - 1)
						Left[i].push_back(j);
				}
			}
		}
	}
	void Print()const {
		for (int i = 0; i < TileCount; i++) {
			cout << endl << "===========TILE ID : " << i << "============" << endl;
			arr[i]->Print();

			cout << "\n\tTop Neighbor : ";
			for (int j = 0; j < Top[i].size(); j++) {
				build((int)Top[i][j]);
				cout<< " ";
			}
			cout << endl<< "\tRight Neighbor : ";;
			for (int j = 0; j < Right[i].size(); j++) {
				build((int)Right[i][j]);
				cout<< " ";
			}
			cout << endl << "\tBottom Neighbor : ";;
			for (int j = 0; j < Bottom[i].size(); j++) {
				build((int)Bottom[i][j]);
				cout<< " ";
			}
			cout << endl << "\tLeft Neighbor : ";;
			for (int j = 0; j < Left[i].size(); j++) {
				build((int)Left[i][j]);
				cout<< " ";
			}
			cout << endl << "=====================================" << endl;
		}
	}
	vector<EDGETYPE> WhatIsNext(int ID,int direction)const {	//direction(0�� 1������ 2�Ʒ� 3����)�� �� �� �ִ� Ÿ�� ����� ��ȯ��
		if (direction > 3 || ID >= TileCount)
			exit(1);
		switch (direction) {
		case 0:
			return Top[ID];
			break;
		case 1:
			return Right[ID];

			break;
		case 2:
			return Bottom[ID];
			break;
		case 3:
			return Left[ID];
			break;
			
		}
	}
	int Variety()const {	//Ÿ�� ���� ��ȯ
		return TileCount;
	}
};

class Map {
private:
	char*map;
	int size;
public:
	Map(int m):size(m) {
		map = new char[m * m]{0};
	}
	char& operator[] (int x) {
		if (x < size * size)
			return *(map + x);
		else
			exit(0);
	}
	void Print(int k=0) {
		if (k == 0) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					gotoxy(j*2, i);	// '��' ���� �ڽ� ��ȣ�� �ùٸ� ������ ����..
					build(*(map + (i * size) + j));
				}
				cout << endl;
			}
		}
		else {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					cout<<(int)*(map + (i * size) + j);
				}
				cout << endl;
			}
		}
	}
	int GetSize() {
		return size;
	}
};

class WaveFunctionCollapse {
private:
	int startTileID;
	const TileManager& tileMan;
public:
	WaveFunctionCollapse(TileManager &Tile):tileMan(Tile) //0~Tile Type
	{
		srand((unsigned int)time(NULL));
		startTileID = rand() % (tileMan.Variety());	//���� Ÿ�� ������ Ÿ�� ���� �� �ϳ� ���� ��
	}
	void Collapse(Map map) {
		map[0] = startTileID;
		vector<EDGETYPE> right = tileMan.WhatIsNext(startTileID, 1);	//�����ʿ� �� �� �ִ� Ÿ�� ����� ������
		vector<EDGETYPE> bottom;
		int i = 0;
		int limit = map.GetSize();
		int setTile;	//� Ÿ���� �� ���ΰ�?

		for (int x = 0; x < limit; x++) {
			setTile = rand() % right.size();	//��Ͽ��� ���°?
			map[i] = right[setTile];	//��Ͽ��� ����
			right = tileMan.WhatIsNext(map[i], 1);	//���� Ÿ�� ��� ������Ʈ
			i++;
		}

		right = tileMan.WhatIsNext(map[0], 2);
		vector <EDGETYPE> same;
		bottom=right;

		for (int y = 1; y < limit; y++) {	// =========== �߰��� �� y>=1���� ���ʹ� �̿��������� ���� Ÿ�ϰ��� Ȯ���ؾ� �� ---------- ���� �߻� ����
			for (int x = 0; x < limit; x++) {

				same.clear();
				bottom = tileMan.WhatIsNext(map[i -limit], 2);

				for(int j=0;j<right.size();j++){
					for (int k = 0; k < bottom.size(); k++) {
						if (right[j] == bottom[k]) {
							same.push_back(right[j]);
							break;
						}
					}
				}
				setTile = rand() % same.size();	//��Ͽ��� ���°?
				map[i] = same[setTile];	//��Ͽ��� ����
				right = tileMan.WhatIsNext(map[i], 1);	//���� Ÿ�� ��� ������Ʈ
				i++;
			}
			right = tileMan.WhatIsNext(map[i-limit], 2);
		}
	}
};

int main(void) {
	TileManager a(7, 1,1);
	a.Print();
	Map map(24);
	WaveFunctionCollapse wave(a);
	wave.Collapse(map);
//	map.Print(1);
	system("cls");
	map.Print();
	for (int t = 0; t < 50; t++) {
		wave.Collapse(map);
		Sleep(2000);
		system("cls");
		map.Print();
	}
	gotoxy(0, 23);
}