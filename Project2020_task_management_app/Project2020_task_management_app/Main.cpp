#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <vector>
using namespace std;

// 전처리 -----------------------------------------------------------------
#define MAX_GROUP_SIZE 100
#define MAX_TASK_SIZE_PER_GROUP 100
#define MAX_TASK_SIZE 1000

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 100
#define BACK_SPACE -10

// 구조체 및 클래스 -------------------------------------------------------

typedef struct date {
	int month;
	int day;
}date;

class SettingProperty {
private:
	char color[2];
public:
	SettingProperty(char _BGcolor, char _textColor) {
		color[0] = _BGcolor;
		color[1] = _textColor;
		char command[9] = "COLOR ";
		command[6] = color[0];
		command[7] = color[1];
		system(command);
	}
	SettingProperty() {
		SettingProperty(0, 15);
	}

	void refresh_color() {
		char command[9] = "COLOR ";
		command[6] = color[0] < 10 ? color[0] + '0' : color[0] + 'A' - 10;
		command[7] = color[1] < 10 ? color[1] + '0' : color[1] + 'A' - 10;
		system(command);
	}

	bool set_BGcolor(char _BGcolor) {
		if (color[1] == _BGcolor) {
			return false;
		}
		else {
			color[0] = _BGcolor;
			refresh_color();
			return true;
		}
	}
	bool set_textColor(char _textColor) {
		if (color[0] == _textColor) {
			return false;
		}
		else {
			color[1] = _textColor;
			refresh_color();
			return true;
		}
	}

	char get_BGcolor(void) {
		return color[0];
	}

	char get_textColor(void) {
		return color[1];
	}
};

class Task {
private:
	string name;
	date deadline;
public:
	Task(string _name, date _deadline) {
		set_name(_name);
		set_deadline(_deadline);
	}

	bool set_name(string _name) {
		this->name = _name;
		return true;
	}

	string get_name(void) {
		return this->name;
	}

	bool set_deadline(date _deadline) {
		this->deadline = _deadline;
		return true;
	}
};

class Group {
private:
	static int group_size;
	static Group* curPos;
	enum VisualMode {Vector, Queue, Stack, Heap};

	int index;
	string name;
	char color;
	Task* rootTask;
	VisualMode visualMode;

	bool reduplication_test(string str) {
		Group* iterPos = curPos;
		do {
			if (iterPos->name == str) {
				return true;
			}
			iterPos = iterPos->next;
		} while (iterPos != curPos);
		return false;
	}

public:
	Group* prev;
	Group* next;

	Group() {
		set_color(7);
		set_name("");
		index = group_size;
		group_size++;
		if (group_size == 1) {
			curPos = this;
			this->next = this;
			this->prev = this;
		}
		else {
			this->next;
		}
		rootTask = NULL;
	}

	bool set_color(int index) {
		this->color = index % 16;
		return true;
	}

	int get_color() {
		return this->color;
	}

	bool set_name(string _name) {
		bool isReduplicated = reduplication_test(_name);
		if (isReduplicated) {
			// 이름 중복 처리
			return false;
		}
		// 이름 적용 처리
		this->name = _name;
		return true;
	}

	void set_visual_mode(int index) {
		this->visualMode = (VisualMode)index;
		return;
	}

	void add(string data, date deadline) {
		Task* newTask = (Task*)malloc(sizeof(Task));
		*newTask = Task(data, deadline);
	}
};

class GroupProperty {
private:
	int size;
	Group* head;
	Group* tail;
public:

	GroupProperty() {
		size = 0;
		head = NULL;
		tail = NULL;
	}

	void add(string name, int visualizer, int color) {
		Group* newGroup = (Group*)malloc(sizeof(Group));
		*newGroup = Group();
		newGroup->set_name(name);
		newGroup->set_visual_mode(visualizer);
		newGroup->set_color(color);

		if (tail != NULL) {
			tail->next = newGroup;
			newGroup->prev = tail;
		}
		tail = newGroup;
		if (size == 0) {
			head = tail;
		}
		size++;
	}

	Group* get_group_by_index(int index) {
		Group* iterPos = head;
		for (int i = 0; i < index; i++) {
			if (iterPos == tail->next) {
				return NULL;
			}
			iterPos = iterPos->next;
		}
		return iterPos;
	}

	void insert(int departure, int destination) {
		if (departure == destination || departure >= size || destination >= size) {
			return;
		}
		Group* before = get_group_by_index(departure);
		Group* after = get_group_by_index(destination);
		before->prev->next = before->next;
		before->next->prev = before->prev;

		before->next = after->next;
		before->prev = after;

		after->next = before;
		return;
	}

	void remove(int index) {
		Group* tmp = get_group_by_index(index);
		if (tmp == NULL) {
			return;
		}
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		free(tmp);
	}

	void tour() {
		Group* iterPos = head;
		while (iterPos != NULL) {


			iterPos = iterPos->next;
		}
		return;
	}
};

class User {
private:
	string id;
	string pw;
public:
	SettingProperty mySetting;
	GroupProperty myGroups;

	User(string _id, string _pw) {
		id = _id;
		pw = _pw;
		mySetting = SettingProperty(0, 15);
		myGroups = GroupProperty();
	}

	User() {
		User("", "");
	}

	bool certify(string _id, string _pw) {
		return (id == _id) && (pw == _pw);
	}
};

class Stack {
private:
	static const int MAX_STACK_SIZE = 100;
	int top;
	int container[MAX_STACK_SIZE];
public:
	Stack(void) {
		top = -1;
		container[0] = 1;
	}

	bool isEmpty(void) {
		return top == -1;
	}

	bool isFull(void) {
		return (top == MAX_STACK_SIZE - 1);
	}

	void push(int item) {
		if (this->isFull()) {
			for (int i = 40; i < MAX_STACK_SIZE; i++) {
				container[i - 40] = container[i];
			}
		}
		else {
			container[++top] = item;
		}
		return;
	}

	int pop(void) {
		if (this->isEmpty()) {
			fprintf(stderr, "Stack Excess Error!\n");
			return 1;
		}
		else {
			return container[top--];
		}
	}

	int peek(void) {
		if (this->isEmpty()) {
			fprintf(stderr, "Stack Excess Error!\n");
			return 1;
		}
		else {
			return container[top];
		}
	}
};

// 여기부터 윤재형

typedef struct Node
{
	string Name;
	struct Node* next;
	struct Todo* Thead; //Todo 헤드 
	struct Todo* Ttail; //Todo 테일 
	int numOfTodo; //Todo 개수 
}Node;

typedef struct Todo {
	struct Todo* next;
	string Date;
	string Data;
}Todo;

class LinkedList // 더미데이터 사용 연결리스트 
{
private:
	Node* head;
	Node* tail;
	Node* cur;
	Node* before;
	int numOfList;//static으로 바꾸면 에러남 
	int selected;

public:
	LinkedList();
	void Insert(string name);
	void deleteList();
	void print();
	void editN(string name);
	void select(int n);
	void addTodo(string date, string data);
	void deleteTodo(int n);
	void deleteAllinList(Node* ln);
	void printTodo();
	void mergeList(int n);
	void sortTodo(Node* nd);
};


LinkedList::LinkedList() {
	tail = new Node;
	head = tail;
	tail->next = NULL;
	cur = NULL;
	before = NULL;
	numOfList = 0;
	selected = 0;
}

void LinkedList::Insert(string name) {
	Node* newNode = new Node;
	newNode->Name = name;
	newNode->next = NULL;

	newNode->Ttail = new Todo; //할 일 초기화 
	newNode->Thead = newNode->Ttail;
	newNode->Ttail->next = NULL;
	newNode->numOfTodo = 0;

	tail->next = newNode;
	tail = newNode;
	numOfList++;
}

void LinkedList::deleteList() {//n번째 리스트를 지우기 
	if (selected < 1 || selected > numOfList) {//리스트 선택안됨
		std::cout << "리스트를 선택해주십시오.\n";
		return;
	}
	Node* tmp = cur;
	before->next = before->next->next;
	if (selected == numOfList) tail = before; //맨 끝 노드 삭제라면 tail 갱신 
	deleteAllinList(tmp);
	delete(tmp);
	--numOfList;
	cur = NULL;
	before = NULL;
	selected = 0;
}

void LinkedList::editN(string name)//n번째 리스트의 이름 수정 
{
	if (selected < 1 || selected > numOfList) {//리스트 범위 밖
		std::cout << "리스트를 선택해주십시오.\n";
		return;
	}
	cur->Name = name;
}

void LinkedList::print()
{
	Node* tmp = head->next;
	std::cout << "리스트 목록: \n";
	if (tmp != NULL) { //해당 노드가 NULL이 아닐때 
		for (;;) {
			std::cout << tmp->Name;
			if (tmp->next == NULL) {
				printf("\n");
				break;
			}
			else cout << " -> ";
			tmp = tmp->next;
		}
	}
	else cout << "NO DATA";//NULL일 때 
}

void LinkedList::select(int n)
{
	if (n < 1 || n > numOfList) //리스트 범위 밖 
		std::cout << "해당 위치(" << n << ")에 리스트가 존재하지 않습니다.\n";
	else {// 범위 안 
		before = head;
		int i = n;
		while (--i) before = before->next; //선택할 노드 전까지 찾아감
		cur = before->next;
		selected = n;
	}
}

void LinkedList::addTodo(string date, string data)
{
	if (selected < 1 || selected > numOfList) {//리스트 범위 밖
		std::cout << "리스트를 선택해주십시오.\n";
		return;
	}
	Todo* newTodo = new Todo;
	newTodo->Date = date;
	newTodo->Data = data;
	newTodo->next = NULL;

	cur->Ttail->next = newTodo;
	cur->Ttail = newTodo;
	cur->numOfTodo++;
	sortTodo(cur);
}

void LinkedList::deleteAllinList(Node* ln)
{
	Todo* tmp = ln->Thead->next;
	Todo* s;
	if (tmp == NULL) return;
	while (tmp != ln->Ttail) {
		s = tmp;
		tmp = tmp->next;
		delete(s);
	}
	ln->Thead = ln->Ttail;
}

void LinkedList::deleteTodo(int n)
{
	if (n < 1 || n > cur->numOfTodo)//Todo 범위 밖
		std::cout << "존재하지 않는 Todo.\n";
	else {
		Todo* t;
		Todo* c = cur->Thead;
		int i = n;
		while (--i) c = c->next;

		t = c->next;
		c->next = c->next->next;
		if (n == cur->numOfTodo) cur->Ttail = c;

		free(t);
		--cur->numOfTodo;
	}
}

void LinkedList::printTodo()
{
	Todo* tmp = cur->Thead->next;
	cout << "그룹 " << cur->Name << "의 Todo:\n";
	if (tmp != NULL) {
		for (;;) {
			cout << tmp->Date << ": " << tmp->Data << endl;
			if (tmp->next == NULL) break;
			tmp = tmp->next;
		}
	}
	else cout << "할 일이 없습니다.\n";
}

void LinkedList::mergeList(int n)//선택된 리스트를 n번째 리스트에 합침
{
	if (selected < 1 || selected > numOfList) {//리스트 범위 밖
		cout << "리스트를 선택해주십시오.\n";
		return;
	}
	Node* tmp = head;
	Node* s = cur;
	int i = n;
	while (i--) tmp = tmp->next;
	if (s == tmp) {
		cout << "동일한 리스트 선택\n";
		return;
	}
	else {
		tmp->Ttail->next = s->Thead->next;
		tmp->Ttail = s->Ttail;
		tmp->numOfTodo += s->numOfTodo;
		before->next = before->next->next;
		if (cur->next == NULL) tail = before;
		delete(s);
		cur = NULL;
		before = NULL;
		selected = 0;
		numOfList--;
	}
	sortTodo(tmp);
}

void LinkedList::sortTodo(Node* nd) //선택 정렬 사용
{
	if (nd->numOfTodo == 1 || nd->numOfTodo == 0) return;//Todo가 없거나 하나밖에 없다면
	Todo* min_Todo = nd->Thead->next, * bf_min_Todo, * save_Todo = NULL, * count_Todo = nd->Thead->next, * bf_count_Todo = nd->Thead->next, * compare_Todo = NULL;
	compare_Todo = nd->Thead->next;
	for (int i = 0; i < nd->numOfTodo; i++) {
		for (int j = 0; j < nd->numOfTodo; j++) {
			if (compare_Todo->next == NULL) break;
			compare_Todo = compare_Todo->next;
			if (min_Todo->Date.compare(compare_Todo->Date) > 0) min_Todo = compare_Todo;
		}
		compare_Todo = min_Todo;
		if (min_Todo != count_Todo) {
			bf_min_Todo = nd->Thead->next;
			while (1) {
				if (bf_min_Todo->next == min_Todo) break;
				else bf_min_Todo = bf_min_Todo->next;
			}
			if (count_Todo == bf_min_Todo) {
				save_Todo = min_Todo->next;
				bf_count_Todo = nd->Thead->next;
				for (int k = 0; k < i - 1; k++) bf_count_Todo = bf_count_Todo->next;
				bf_count_Todo->next = min_Todo;
				min_Todo->next = count_Todo;
				count_Todo->next = save_Todo;
			}
			else {
				save_Todo = count_Todo->next;
				bf_min_Todo->next = count_Todo;
				min_Todo->next = save_Todo;
				if (count_Todo == nd->Thead->next) nd->Thead->next = min_Todo;
				else {
					bf_count_Todo = nd->Thead->next;
					for (int k = 0; k < i - 1; k++) bf_count_Todo = bf_count_Todo->next;
					bf_count_Todo->next = min_Todo;
				}
			}
			count_Todo = min_Todo->next;
			compare_Todo = count_Todo;
			min_Todo = count_Todo;
		}
	}

	int i = nd->numOfTodo;
	Todo* tmp = nd->Thead->next;
	while (--i)
		tmp = tmp->next;
	nd->Ttail = tmp;
	nd->Ttail->next = NULL;//Todo tail바꾸기

	min_Todo = nd->Thead->next;
	bf_count_Todo = nd->Thead->next;
	count_Todo = nd->Thead->next;
	bf_min_Todo = nd->Thead->next;
	compare_Todo = NULL;
	save_Todo = NULL;
}

/*
int main(int argc, char** argv) {

	LinkedList list;
	int a, b;
	string ww, ee;

	list.Insert("자료구조");
	list.Insert("겜시구");
	list.Insert("선대수");
	list.print();

	while (1) {
		cout << "리스트 선택: ";
		cin >> a;
		list.select(a);
		cout << "1. 할 일 추가 2. 선택한 리스트 합병 3. 선택한 리스트 삭제 4. 이름바꾸기 5. 할 일 보기 -1. 종료\n";
		cin >> b;
		switch (b)
		{
		case 1:
			cout << "날짜 입력(YYYY-MM-DD): ";
			cin >> ww;
			cout << "할 일 입력: ";
			cin >> ee;
			list.addTodo(ww, ee);
			break;
		case 2:
			cout << "합병할 리스트 입력: ";
			cin >> a;
			list.mergeList(a);
			list.print();
			break;
		case 3:
			list.deleteList();
			list.print();
			break;
		case 4:
			cout << "바꿀 이름 입력: ";
			cin >> ww;
			list.editN(ww);
			break;
		case 5:
			list.printTodo();
			break;
		case -1:
			return 0;
		}
	}

	return 0;
}
*/

// 전역 변수 --------------------------------------------------------------
typedef pair<int, int> pii;
pii curSelect = { 0, 0 };
time_t trashTime;
bool isEnd = false;
User user = User();


// 함수 선언 --------------------------------------------------------------
void cursor_Draw(int x, int y, int sceneIndex, bool mode);
int input(int row, int col, int type);
void timePrint(void);
void sys_init(void);
void textOperator(int len, char text[], pii pos, bool hidden);

// Scene Func
int log_in(void);
int home(void);
int calender(void);
int group_task(void);
int setting(void);
int merge_user(void);
int FTM(void);
int group_management(void);
int current_group_task(void);
int set_group_property(void);
void try_initialize();

int(*scene[10])(void) = { log_in, home, calender, group_task, group_management, FTM, merge_user, setting, current_group_task, set_group_property };

// Design Func
void design_Default(void);

void design_log_in(void);
void design_home(void);
void design_calender(void);
void design_group_task(void);
void design_setting(void);
void additional_design_setting(void);
void design_merge_user(void);
void design_fast_task_manager(void);
void design_group_manager(void);
void design_current_group_task(void);
void design_set_group_property(void);

// 함수 정의 --------------------------------------------------------------

void hideCursor(bool show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//x값을 2x로 변경, 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입
void pgotoxy(int x, int y, char* s) { 
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}

void pgotoxy(int x, int y, string s) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}

int main(void) {
	Stack sceneHistory = Stack();
	sceneHistory.push(0);
	int nextSceneIndex = 0;
	bool isLogIn = false;
	sys_init();
	sceneHistory.push(log_in());
	do {
		gotoxy(0, 0);
		nextSceneIndex = scene[sceneHistory.peek()]();
		if (nextSceneIndex == BACK_SPACE) {
			sceneHistory.pop();
			nextSceneIndex = sceneHistory.peek();
		}
		else {
			sceneHistory.push(nextSceneIndex);
		}
	} while (nextSceneIndex != -1);
	return 0;
}

void sys_init(void) {
	system("Title Task Manager");
	system("color 0f");
	hideCursor(true);
	system("cls");
}

int log_in(void) {
	enum Status { ID, PW };
	Status status = ID;
	design_log_in();
	char id[13] = "            ";
	char pw[13] = "            ";
	switch (status) {
	case ID:
		textOperator(12, id, { 56, 16 }, false);
	case PW:
		textOperator(12, pw, { 56, 18 }, true);
		break;
	}
	user = User(id, pw);
	system("cls");
	return 1;
}


int home(void) {
	design_home();
	isEnd = false;
	thread paintT(timePrint);
	int direction = 0;
	do {
		cursor_Draw(curSelect.first, 0, 0, 1);
	    direction = input(5, 1, 0);
		cursor_Draw(curSelect.first, 0, 0, 0);
		Sleep(5);
		
		switch (direction) {
		case 'T': case 't':
			isEnd = true;
			paintT.join();
			system("cls");
			return 2;
		case 'G': case 'g':
			isEnd = true;
			paintT.join();
			system("cls");
			return 3;
		case 1: case -1:
			curSelect.first += direction;
			break;
		case ENTER:
			isEnd = true;
			paintT.join();
			system("cls");
			if (curSelect.first == 4) {
				return -1;
			}
			return 4 + curSelect.first;
		case BACK_SPACE:
			isEnd = true;
			paintT.join();
			system("cls");
			return BACK_SPACE;
		}
	} while (true);
}

void timePrint(void) {
	struct tm* curTime;
	while (!isEnd) {
		time_t curT = time(&trashTime);
		curTime = localtime(&curT);
		gotoxy(12, 2);
		printf("%02d:%02d:%02d", curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
		Sleep(500);
	}
}

int calender(void) {
	int term;
	do {
		gotoxy(0, 0);
		design_calender();
		term = _getch();
	} while (term != 8);
	system("cls");
	return 1;
}

int group_task(void) {
	int term;
	do {
		gotoxy(0, 0);
		design_group_task();
		term = _getch();
	} while (term != 8);
	system("cls");
	return 1;
}

int setting(void) {
	design_setting();
	additional_design_setting();

	int direction = 0;
	int selected = -1;
	do {
		switch (selected) {
		case 0:
			user.mySetting.set_BGcolor(curSelect.first + (curSelect.second * 2));
			break;
		case 1:
			user.mySetting.set_textColor(curSelect.first + (curSelect.second * 2));
			break;
		}

		if (-1 < selected && selected < 3) {
			cursor_Draw(curSelect.first + (curSelect.second * 2), selected, 1, 1);
		}
		direction = input(2, 8, 0);
		if (-1 < selected && selected < 3) {
			cursor_Draw(curSelect.first + (curSelect.second * 2), selected, 1, 0);
		}
		Sleep(5);
		switch (direction) {
		case 'G': case 'g':
			curSelect = { 0, 0 };
			selected = 0;
			break;
		case 'T': case 't':
			curSelect = { 0, 0 };
			selected = 1;
			break;
		case 'I': case 'i':
			if (selected == 2) {
				user.mySetting.set_BGcolor(0);
				user.mySetting.set_textColor(15);
			}
			curSelect = { 0, 0 };
			selected = 2;
			break;
		case 'D': case 'd':
			try_initialize();
			gotoxy(0, 0);
			design_setting();
			additional_design_setting();
			curSelect = { 0, 0 };
			selected = 3;
			break;
		case 'B': case 'b':
			curSelect = { 0, 0 };
			system("cls");
			return 1;
		case 1: case -1:
			if (-1 < selected && selected < 2) {
				curSelect.first += direction;
			}
			break;
		case 2: case -2:
			if (-1 < selected && selected < 2) {
				curSelect.second += direction / 2;
			}
			break;
		}
	} while (true);
}

void additional_design_setting(void) {
	gotoxy(17, 6);
	printf("●");
	gotoxy(25, 6);
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			gotoxy(25, 8);
		}
		printf("●  ");
	}
	gotoxy(17, 11);
	printf("●");
	gotoxy(25, 11);
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			gotoxy(25, 13);
		}
		printf("●  ");
	}
}

void try_initialize() {
	enum Status { ID, PW };
	Status status = ID;
	char id[13] = "            ";
	char pw[13] = "            ";
	switch (status) {
	case ID:
		textOperator(12, id, { 27, 23 }, false);
	case PW:
		textOperator(12, pw, { 27, 25 }, true);
		break;
	}
	if (user.certify(id, pw)) {
		// 초기화
		user.mySetting.set_BGcolor(0);
		user.mySetting.set_textColor(15);
	}
	return;
}

int merge_user(void) {
	int a;
	design_merge_user();
	scanf("%d", &a);
	system("cls");
	return 1;
}

int FTM(void) {
	design_fast_task_manager();

	int direction = 0;
	int selected = -1;
	do {

		if (-1 < selected && selected < 3) {
			cursor_Draw(curSelect.first + (curSelect.second * 3), 0, 2, 1);
		}
		direction = input(3, 8, 0);
		if (-1 < selected && selected < 3) {
			cursor_Draw(curSelect.first + (curSelect.second * 3), 0, 2, 0);
		}
		Sleep(5);
		switch (direction) {
		case 'A': case 'a':
			curSelect = { 0, 0 };
			selected = 0;
			break;
		case 'D': case 'd':
			curSelect = { 0, 0 };
			selected = 1;
			break;
		case 'M': case 'm':
			curSelect = { 0, 0 };
			selected = 2;
			break;
		case 'B': case 'b':
			curSelect = { 0, 0 };
			system("cls");
			return 1;
		// 상하 이동
		case 1: case -1:
			if (-1 < selected && selected < 2) {
				curSelect.first += direction;
			}
			break;
		// 페이지 이동
		case 2: case -2:
			if (-1 < selected && selected < 2) {
				curSelect.second += direction / 2;
			}
			break;
		}
	} while (true);
}

int group_management(void) {
	int a;
	design_group_manager();

	cin.ignore();
	scanf("%d", &a);
	system("cls");
	return 1;
}

int current_group_task(void) {
	int a;
	design_current_group_task();
	scanf("%d", &a);
	system("cls");
	return 1;
}

int set_group_property(void) {
	int a;
	design_set_group_property();
	scanf("%d", &a);
	system("cls");
	return 1;
}

void cursor_Draw(int x, int y, int sceneIndex, bool mode) {
	int row, col;
	int rowStart;
	int rowGap;
	int colStart;
	int colGap;
	switch (sceneIndex) {
	case 0:
		rowStart = 3;
		rowGap = 5;
		colStart = 87;
		colGap = 29;
		row = rowStart + x * 5;
		col = colStart;
		gotoxy(col, row);
		if (mode) printf("┏"); else printf(" ");
		gotoxy(col, row + rowGap);
		if (mode) printf("└"); else printf(" ");
		gotoxy(col + colGap, row);
		if (mode) printf("┐"); else printf(" ");
		gotoxy(col + colGap, row + rowGap);
		if (mode) printf("┘"); else printf(" ");
		break;
	case 1:
		rowStart = 6;
		colStart = 23;
		colGap = 4;
		row = rowStart + (y * 5) + ((x % 2) * 2);
		col = colStart + ((x / 2) * 4);
		gotoxy(col, row);
		if (mode) printf("▶"); else printf(" ");
		gotoxy(col + colGap, row);
		if (mode) printf("◀"); else printf(" ");
		break;
	}
}

void textOperator(int len, char text[], pii pos, bool hidden) {
	int curLen = 0;
	char term = 65;
	while (term != 13) {
		gotoxy(pos.first, pos.second);
		for (int i = 0; i < len; i++) {
			printf("%c", (i < curLen ? (hidden ? '*' : text[i]) : '_'));
		}
		term = _getch();
		if (term == 8 && curLen > 0) {
			text[curLen--] = ' ';
		}
		else if (curLen != len && 32 <= term && term <= 126) {
			text[curLen++] = term;
		}
	}
	return;
}

int input(int row, int column, int sceneNumber) // 조작기능을 가진 함수. 건들지 마시오.
{
	while (true)
	{
		int userSelect = _getch();
		if (userSelect == 0xE0 || userSelect == 0)
		{
			userSelect = _getch();
			if (userSelect == UP && curSelect.first > 0) // 위
			{
				return -1;
			}
			else if (userSelect == DOWN && curSelect.first < row - 1) // 아래
			{
				return 1;
			}
			else if (userSelect == LEFT && column >= 2 && curSelect.second > 0) // 왼쪽
			{
				return -row;
			}
			else if (userSelect == RIGHT && column >= 2 && curSelect.second < 7) // 오른쪽
			{
				return row;
			}
		}
		else if (userSelect == 13 && sceneNumber != -1) // 엔터키
		{
			return ENTER;
		}
		else if (userSelect == 8 && (sceneNumber >= 1 || sceneNumber == -1)) // 백스페이스
		{
			return BACK_SPACE;
		}
		else if (userSelect == 9) // 탭 -> 타이틀
		{
			return 1234;
		}
		else {
			return userSelect;
		}
	}
}

void design_Default(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃   Default                                                                                                           ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

void design_log_in(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃   Log In                                                                                                            ┃\n");
	printf("┃ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                       Task Manager                                                  ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                       ____________                                                  ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                       ____________                                                  ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

void design_home(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃ Calender          ( C )┃┃ Today                                              ( T )┃┃             MENU             ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃                              ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┃         그룹 관리        ┃ ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃                              ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                        ┃┃                                                         ┃┃ ┃   Fast Task Management   ┃ ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃ ┃                          ┃ ┃┃\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃ Group Task                                                                   ( G )┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃         유저 병합        ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃          setting         ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃          log out         ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

void design_setting(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃   설정                                                                            ┃┃             MENU             ┃┃\n");
	printf("┃┃ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃     ( G ) 배경 컬러      ┃ ┃┃\n");
	printf("┃┃  배경 컬러   ㅁ   ▶    ㅁ ㅁ                                                     ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃   ( T ) 텍스트 컬러      ┃ ┃┃\n");
	printf("┃┃  텍스트 컬러 ㅁ   ▶    ㅁ ㅁ                                                     ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃   ( I ) 설정 초기화      ┃ ┃┃\n");
	printf("┃┃  설정 초기화      ▶  ii                                                          ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃   ( D ) 데이터 초기화    ┃ ┃┃\n");
	printf("┃┃  데이터 초기화   ┏━━━━━━━━━━━━━━━━━━━━━━━━┓                                       ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                  ┃                        ┃                                       ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                  ┃  ID  ㅁ                ┃                                       ┃┃                              ┃┃\n");
	printf("┃┃                  ┃                        ┃                                       ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                  ┃  PW  ㅁ                ┃                                       ┃┃ ┃      ( G ) 뒤로 가기     ┃ ┃┃\n");
	printf("┃┃                  ┃                        ┃                                       ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                  ┗━━━━━━━━━━━━━━━━━━━━━━━━┛                                       ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}
// 13 6 / 13 11 / 13 16 / 13 21 -> 배경 컬러 텍스트 컬러 설정 초기화 데이터 초기화
// 22 7 -> 24 7 ... 2씩 늘어남 좌표 (배경컬러 1)
// 5칸씩 내려갑니다..
// ID PW 부분은 23 / 11 , 25 / 11
// ㅁ는 써논 좌표 위치 표시한거임 지우고 하셈

void design_calender(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃   Calender                                                                                                          ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┃                                                                                                                     ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

void design_group_task(void) {
printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
printf("┃   Group Task                                                                                                        ┃\n");
printf("┃ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ┃\n");
printf("┃                                                                                                                     ┃\n");
printf("┃                                                                                                                     ┃\n");
printf("┃                                                                                                                     ┃\n");
printf("┃            그룹 이름                                                                                                ┃\n");
printf("┃      ━━━━━━━━━━━━━━━━━━━━━                                                                                          ┃\n");
printf("┃                                                                                                                     ┃\n");
printf("┃    ┏━━━━━━━━━━━━━━━━━━━━━━━┓                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃	                                                                                      ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                               		                  ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┃                       ┃                                                                                        ┃\n");
printf("┃    ┗━━━━━━━━━━━━━━━━━━━━━━━┛                                                                                        ┃\n");
printf("┃                                                  ◁ (1 / 4) ▷                                                        ┃\n");
printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}
// 14, 6 그룹이름
// 52, 29 / 62, 29 화살표
// 26 차이남 그룹이름

void design_merge_user(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃                                         ┃                                         ┃┃             MENU             ┃┃\n");
	printf("┃┃                   ID                    ┃                   PW                    ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃        ( J ) 병합        ┃ ┃┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃  ┃ ㅁ                                ┃  ┃  ┃ ㅁ                                ┃  ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃  ┃                                   ┃  ┃  ┃                                   ┃  ┃┃                              ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( B ) 뒤로 가기      ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

// ID -> 7 ,7
// PW -> 48, 7


void design_fast_task_manager(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃                                         ┃                Result      <(1 / 2)>    ┃┃             MENU             ┃┃\n");
	printf("┃┃              Search / Add               ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃  ┃ ㅁ                                ┃  ┃┃ ┃        ( A ) 추가        ┃ ┃┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃  ┃ ㅁ                                ┃  ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃  ┃                                   ┃  ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃                              ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃  ┃ ㅁ                                ┃  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┃        ( D ) 삭제        ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃  ┃ ㅁ                                ┃  ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃┃ ┃        ( M ) 수정        ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃       ( B ) 뒤로 가기    ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

// search -> 7, 7
// result -> 49 ,5 에서 4칸씩 내려감
// 화살표 -> 75, 3 이거 전체가 120이면 85

void design_group_manager(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃             MENU             ┃┃\n");
	printf("┃┃  ┃  1. ㅁ                         ㅁ ┃  ┃  ┃  9. ㅁ                         ㅁ ┃  ┃┃                              ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃                                         ┃┃ ┃ ( D ) 현재그룹 위치변경  ┃ ┃┃\n");
	printf("┃┃  ┃  2. ㅁ                         ㅁ ┃  ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃  ( T ) 현재그룹 태스크   ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( M ) 그룹 병합      ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃   ( P ) 그룹 프로퍼티    ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( B ) 뒤로 가기      ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

// 1 -> 11, 3, 37, 3
// 2 -> 11, 7, 37, 7
// 9 -> 51, 3, 77, 3

void design_current_group_task(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃┃             MENU             ┃┃\n");
	printf("┃┃  ┃  1. ㅁ                         ㅁ ┃  ┃  ┃  9. ㅁ                         ㅁ ┃  ┃┃                              ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( A ) 태스크 추가    ┃ ┃┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃  ┃  2. ㅁ                         ㅁ ┃  ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( M ) 태스크 수정    ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( D ) 태스크 삭제    ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃       ( I ) 초기화       ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃     ( B ) 뒤로 가기      ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                         ┃                                         ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}
// 그룹관리랑 좌표 동일
void design_set_group_property(void) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃┏━━━━━━━━━━━━━━━━━━━━━━━━--━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓┃\n");
	printf("┃┃  그룹명                                                                           ┃┃             MENU             ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃    ( V ) 비주얼라이저    ┃ ┃┃\n");
	printf("┃┃  ┏━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━━━━━━━━━━━━━━━━━┓  ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃  ┃        스 택         ┃    ┃          큐          ┃    ┃          힙         ┃  ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃  ┗━━━━━━━━━━━━━━━━━━━━━━┛    ┗━━━━━━━━━━━━━━━━━━━━━━┛    ┗━━━━━━━━━━━━━━━━━━━━━┛  ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃        ( C ) 컬러        ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃            ㅁ       ◀   ㅁ   ㅁ                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃      ( B ) 뒤로 가기     ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┃                          ┃ ┃┃\n");
	printf("┃┃                                                                                   ┃┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┃                                                                                   ┃┃                              ┃┃\n");
	printf("┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

// 스택 가운데 위치 -> 15, 7
// 큐 위치 -> 42, 7
// 힙 위치 -> 69, 7
// 맨 왼쪽 동그라미 15 ,13
// 27,13 부터 4 차이 씩 남
