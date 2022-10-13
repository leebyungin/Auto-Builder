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
			cout << "─";
			break;
		case 1:
			cout << "ㅣ";
			break;
		case 2:
			cout << "┌";
			break;
		case 3:
			cout << "└";
			break;
		case 4:
			cout << "┐";
			break;
		case 5:
			cout << "┘";
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
	Tile(int id=-1,int edgeSize=0):TileType(id),EdgeSize(edgeSize) {	//타일 타입에 붙여줄 id, 타일의 한 모서리를 이루고 있는 요소 수
		if (id != -1) {
			if (edgeSize == 0) {
				TileTop = nullptr;
				return;
			}
			TileTop = new EDGETYPE[edgeSize * 4]{ 0 };	//메모리 지역성을 향상시키기 위해서..
			TileRight = TileTop + edgeSize;
			TileBottom = TileRight + edgeSize;
			TileLeft = TileBottom + edgeSize;
		}
	}
	void EdgeDataUpdate(EDGETYPE *arr=nullptr) {	//타일의 모서리 데이터 입력
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
	TileManager(int c, int TileSize, int autoTile = 0) :TileCount(c), EdgeSize(TileSize) {	//관리 할 타일의 수 c, 타일의 모서리 크기 TileSize
		arr = new Tile * [c];
		for (int i = 0; i < c; i++) {
			arr[i] = new Tile(i, TileSize);	//타일 생성
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
		Top = new vector<EDGETYPE>[c];	//이웃하는 타일 저장할 벡터. [c]는 본인의 타일 아이디와 같음
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

				for (int k = 0; k < EdgeSize; k++) { //위쪽에 이웃한 타일
					if (myTop[k] != compareBottom[k])
						break;
					if (k == EdgeSize - 1) {
						Top[i].push_back(j);
					}
				}
				for (int k = 0; k < EdgeSize; k++) { //오른쪽에 이웃한 타일
					if (myRight[k] != compareLeft[k])
						break;
					if (k == EdgeSize - 1)
						Right[i].push_back(j);
				}
				for (int k = 0; k < EdgeSize; k++) { //아래쪽에 이웃한 타일
					if (myBottom[k] != compareTop[k])
						break;
					if (k == EdgeSize - 1)
						Bottom[i].push_back(j);
				}
				for (int k = 0; k < EdgeSize; k++) { //왼쪽에 이웃한 타일
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
	vector<EDGETYPE> WhatIsNext(int ID,int direction)const {	//direction(0위 1오른쪽 2아래 3왼쪽)에 올 수 있는 타일 목록을 반환함
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
	int Variety()const {	//타일 종류 반환
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
					gotoxy(j*2, i);	// '┌' 같은 박스 기호의 올바른 정렬을 위해..
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
		startTileID = rand() % (tileMan.Variety());	//시작 타일 선택은 타일 종류 중 하나 고르면 됨
	}
	void Collapse(Map map) {
		map[0] = startTileID;
		vector<EDGETYPE> right = tileMan.WhatIsNext(startTileID, 1);	//오른쪽에 올 수 있는 타일 목록을 저장함
		vector<EDGETYPE> bottom;
		int i = 0;
		int limit = map.GetSize();
		int setTile;	//어떤 타일을 고를 것인가?

		for (int x = 0; x < limit; x++) {
			setTile = rand() % right.size();	//목록에서 몇번째?
			map[i] = right[setTile];	//목록에서 선택
			right = tileMan.WhatIsNext(map[i], 1);	//다음 타일 목록 업데이트
			i++;
		}

		right = tileMan.WhatIsNext(map[0], 2);
		vector <EDGETYPE> same;
		bottom=right;

		for (int y = 1; y < limit; y++) {	// =========== 추가할 것 y>=1에서 부터는 이웃가능한지 위의 타일과도 확인해야 함 ---------- 에러 발생 지역
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
				setTile = rand() % same.size();	//목록에서 몇번째?
				map[i] = same[setTile];	//목록에서 선택
				right = tileMan.WhatIsNext(map[i], 1);	//다음 타일 목록 업데이트
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