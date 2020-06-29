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

// 瞪籀葬 -----------------------------------------------------------------
#define MAX_GROUP_SIZE 100
#define MAX_TASK_SIZE_PER_GROUP 100
#define MAX_TASK_SIZE 1000

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 1000
#define BACK_SPACE -10

// 掘褻羹 塽 贗楚蝶 -------------------------------------------------------

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
	Task* next;
	Task* prev;

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

	string get_deadline_string(void) {
		return to_string(this->deadline.month) + "/" + to_string(this->deadline.day);
	}

	int get_deadline_weight() {
		return deadline.month * 100 + deadline.day;
	}

	bool set_deadline(date _deadline) {
		this->deadline = _deadline;
		return true;
	}
};

class Group {
private:
	enum VisualMode {Vector, Queue, Stack, Heap};

	int taskSize;
	int index;
	string name;
	Task* rootTask;
	VisualMode visualMode;

	bool compare_task(Task* a, Task* b) {
		return a->get_deadline_weight() < b->get_deadline_weight();
	}

	Task* change_task(Task* a, Task* b) {
		if(a->prev != NULL) {
			a->prev->next = b;
		}
		if (b->next != NULL) {
			b->next->prev = a;
		}
		
		b->prev == a->prev;
		a->next = b->next;
		
		b->next = a;
		a->prev = b;

		return a;
	}
public:
	Group* prev;
	Group* next;

	Group(int _index) {
		set_name("            ");
		index = _index;
		taskSize = 0;
		rootTask = NULL;
	}

	void set_taskSize(int _taskSize) {
		taskSize = _taskSize;
	}

	int get_taskSize() {
		return taskSize;
	}

	bool set_name(string _name) {
		name.resize(_name.size());
		this->name = _name;
		return true;
	}

	string get_name() {
		return name;
	}

	Task* get_rootTask() {
		return rootTask;
	}

	void set_rootTask(Task* root) {
		rootTask = root;
	}

	void set_visual_mode(int index) {
		this->visualMode = (VisualMode)index;
		return;
	}

	int get_visual_mode() {
		return (int)visualMode;
	}

	void set_index(int _index) {
		index = _index;
	}

	int get_index() {
		return index;
	}

	void add(string data, date deadline) {
		Task* newTask = new Task(data, deadline);
		Task* iterPos = rootTask;
		taskSize++;
		if (iterPos == NULL) {
			rootTask = newTask;
		}
		else  {
			while (iterPos->next != NULL) {
				iterPos = iterPos->next;
			}
			iterPos->next = newTask;
			newTask->prev = iterPos;
		}
		return;
	}

	void remove(Task* tmp) {
		if (tmp == NULL) {
			return;
		}

		if (tmp->prev != NULL) {
			tmp->prev->next = tmp->next;
		}
		else {
			rootTask = tmp->next;
		}

		if (tmp->next != NULL) {
			tmp->next->prev = tmp->prev;
		}

		taskSize--;
		delete(tmp);
	}

	Task* get_task_by_index(int index) {
		Task* iterPos = rootTask;
		for (int i = 0; i < index; i++) {
			if (iterPos == NULL) {
				return rootTask;
			}
			iterPos = iterPos->next;
		}
		return iterPos;
	}

	void get_task_text(vector<string>* taskTexts) {
		taskTexts->clear();
		taskTexts->reserve(taskSize);
		Task* iterPos = rootTask;
		while (iterPos != NULL) {
			taskTexts->push_back(iterPos->get_deadline_string() + " : " + iterPos->get_name());
			iterPos = iterPos->next;
		}
		return;
	}

	void sort_task() {
		for (int i = taskSize - 1; i > 0; i--) {
			int taskIndex = 0;
			for (Task* iterPos = rootTask; taskIndex < i; taskIndex++, iterPos = iterPos->next) {
				if (!compare_task(iterPos, iterPos->next)) {
					if (rootTask == iterPos) {
						rootTask = iterPos->next;
					}
					change_task(iterPos, iterPos->next);
				}
			}
		}
	}
};

class GroupProperty {
private:
	int size;
	Group* head;
	Group* tail;

	bool reduplication_test(string str) {
		if (size == 0) {
			return true;
		}
		Group* iterPos = head;
		do {
			if (iterPos->get_name() == str) {
				return false;
			}
			iterPos = iterPos->next;
		} while (iterPos != NULL);
		return true;
	}
public:

	GroupProperty() {
		size = 0;
		head = NULL;
		tail = NULL;
	}

	int get_size() {
		return size;
	}

	void get_group_text(vector<string> * groupTexts) {
		groupTexts->clear();
		groupTexts->reserve(size);
		Group* iterPos = head;
		while (iterPos != NULL) {
			groupTexts->push_back(iterPos->get_name());
			iterPos = iterPos->next;
		}
		return;
	}

	void add(string name, int visualizer) {
		Group* newGroup = new Group(size);
		if (reduplication_test(name)) {
			newGroup->set_name(name);
		}
		else {
			newGroup->set_name("group " + to_string(size + 1));
		}
		newGroup->set_visual_mode(visualizer);

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
		Group* a = get_group_by_index(departure);
		Group* b = get_group_by_index(destination);
		Group* tmp1;
		Group* tmp2;

		int tmpInt = a->get_index();
		a->set_index(b->get_index());
		b->set_index(tmpInt);

		tmpInt = a->get_visual_mode();
		a->set_visual_mode(b->get_visual_mode());
		b->set_visual_mode(tmpInt);

		tmpInt = a->get_taskSize();
		a->set_taskSize(b->get_taskSize());
		b->set_taskSize(tmpInt);

		Task* tmpTask = a->get_rootTask();
		a->set_rootTask(b->get_rootTask());
		b->set_rootTask(tmpTask);

		string tmpStr = a->get_name();
		a->set_name(b->get_name());
		b->set_name(tmpStr);

		return;
	}

	void remove(int index) {
		Group* tmp = get_group_by_index(index);
		if (tmp == NULL) {
			return;
		}

		if (tmp->prev != NULL) {
			tmp->prev->next = tmp->next;
		}
		else {
			head = tmp->next;
		}

		if (tmp->next != NULL) {
			tmp->next->prev = tmp->prev;
		}
		else {
			tail = tmp->prev;
		}

		index--;
		size--;
		Group* iterPos = tmp->next;
		delete(tmp);
		while (iterPos != NULL) {
			iterPos->set_index(index++);
			iterPos = iterPos->next;
		}
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

	string get_id() {
		return id;
	}

	string get_pw() {
		return pw;
	}
};

class MonthTable {
protected:
	int day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int nday;//⑷營 橾
	int nmonth;//⑷營 錯
	int nyear;//⑷營 喇紫
	int total_day;//1喇睡攪 ⑷營 喇紫梱雖曖 識 橾 熱
	int first_date;//羅廓簞 輿曖 奢寥 偃熱

	bool is_leap_year(int year) { //彌喇檣雖 �挫�
		if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
			day[1] = 29;
			return true;
		}
		day[1] = 28;
		return false;
	}
public:
	time_t curTime = time(NULL);
	struct tm* pLocal = localtime(&curTime);
	int date[5][7];//殖溘 寡翮
	MonthTable()
	{
		nday = pLocal->tm_mday;
		nmonth = pLocal->tm_mon + 1;
		nyear = pLocal->tm_year + 1900;
		total_day = 0;
		first_date = 0;
	}
	void calc_day()//total_day, first_date 啗骯
	{
		total_day = (nyear - 1) * 365 + (nyear - 1) / 4 - (nyear - 1) / 100 + (nyear - 1) / 400;
		for (int i = 0; i < nmonth - 1; i++) {
			if (i == 1) is_leap_year(nyear);
			total_day += day[i];
		}
		first_date = total_day % 7;
	}
	void make_Cal()
	{
		int count = 0;
		int c = 0;

		calc_day();

		for (int i = 0; i <= first_date; i++)
		{
			date[c][count] = 0;
			count++;
		}
		for (int i = 1; i <= day[nmonth - 1]; i++)
		{
			if (count >= 7)
			{
				cout << endl;
				count = 0;
				c++;
			}
			date[c][count] = i;
			count++;
		}
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

// 罹晦睡攪 彌營⑽

typedef struct Node
{
	string Name;
	struct Node* next;
	struct Todo* Thead; //Todo ④萄 
	struct Todo* Ttail; //Todo 纔橾 
	int numOfTodo; //Todo 偃熱 
}Node;

typedef struct Todo {
	struct Todo* next;
	string Date;
	string Data;
}Todo;

class LinkedList // 渦嘐等檜攪 餌辨 翱唸葬蝶お 
{
private:
	Node* head;
	Node* tail;
	Node* cur;
	Node* before;
	int numOfList;//static戲煎 夥紱賊 縑楝陴 
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

	newNode->Ttail = new Todo; //й 橾 蟾晦�� 
	newNode->Thead = newNode->Ttail;
	newNode->Ttail->next = NULL;
	newNode->numOfTodo = 0;

	tail->next = newNode;
	tail = newNode;
	numOfList++;
}

void LinkedList::deleteList() {//n廓簞 葬蝶お蒂 雖辦晦 
	if (selected < 1 || selected > numOfList) {//葬蝶お 摹鷗寰脾
		std::cout << "葬蝶お蒂 摹鷗п輿褊衛螃.\n";
		return;
	}
	Node* tmp = cur;
	before->next = before->next->next;
	if (selected == numOfList) tail = before; //裔 部 喻萄 餉薯塭賊 tail 偵褐 
	deleteAllinList(tmp);
	delete(tmp);
	--numOfList;
	cur = NULL;
	before = NULL;
	selected = 0;
}

void LinkedList::editN(string name)//n廓簞 葬蝶お曖 檜葷 熱薑 
{
	if (selected < 1 || selected > numOfList) {//葬蝶お 彰嬪 夤
		std::cout << "葬蝶お蒂 摹鷗п輿褊衛螃.\n";
		return;
	}
	cur->Name = name;
}

void LinkedList::print()
{
	Node* tmp = head->next;
	std::cout << "葬蝶お 跡煙: \n";
	if (tmp != NULL) { //п渡 喻萄陛 NULL檜 嬴棍陽 
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
	else cout << "NO DATA";//NULL橾 陽 
}

void LinkedList::select(int n)
{
	if (n < 1 || n > numOfList) //葬蝶お 彰嬪 夤 
		std::cout << "п渡 嬪纂(" << n << ")縑 葬蝶お陛 襄營ж雖 彊蝗棲棻.\n";
	else {// 彰嬪 寰 
		before = head;
		int i = n;
		while (--i) before = before->next; //摹鷗й 喻萄 瞪梱雖 瓊嬴馬
		cur = before->next;
		selected = n;
	}
}

void LinkedList::addTodo(string date, string data)
{
	if (selected < 1 || selected > numOfList) {//葬蝶お 彰嬪 夤
		std::cout << "葬蝶お蒂 摹鷗п輿褊衛螃.\n";
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
	if (n < 1 || n > cur->numOfTodo)//Todo 彰嬪 夤
		std::cout << "襄營ж雖 彊朝 Todo.\n";
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
	cout << "斜瑜 " << cur->Name << "曖 Todo:\n";
	if (tmp != NULL) {
		for (;;) {
			cout << tmp->Date << ": " << tmp->Data << endl;
			if (tmp->next == NULL) break;
			tmp = tmp->next;
		}
	}
	else cout << "й 橾檜 橈蝗棲棻.\n";
}

void LinkedList::mergeList(int n)//摹鷗脹 葬蝶お蒂 n廓簞 葬蝶お縑 м藹
{
	if (selected < 1 || selected > numOfList) {//葬蝶お 彰嬪 夤
		cout << "葬蝶お蒂 摹鷗п輿褊衛螃.\n";
		return;
	}
	Node* tmp = head;
	Node* s = cur;
	int i = n;
	while (i--) tmp = tmp->next;
	if (s == tmp) {
		cout << "翕橾и 葬蝶お 摹鷗\n";
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

void LinkedList::sortTodo(Node* nd) //摹鷗 薑溺 餌辨
{
	if (nd->numOfTodo == 1 || nd->numOfTodo == 0) return;//Todo陛 橈剪釭 ж釭夤縑 橈棻賊
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
	nd->Ttail->next = NULL;//Todo tail夥紱晦

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

	list.Insert("濠猿掘褻");
	list.Insert("啞衛掘");
	list.Insert("摹渠熱");
	list.print();

	while (1) {
		cout << "葬蝶お 摹鷗: ";
		cin >> a;
		list.select(a);
		cout << "1. й 橾 蹺陛 2. 摹鷗и 葬蝶お м煽 3. 摹鷗и 葬蝶お 餉薯 4. 檜葷夥紱晦 5. й 橾 爾晦 -1. 謙猿\n";
		cin >> b;
		switch (b)
		{
		case 1:
			cout << "陳瞼 殮溘(YYYY-MM-DD): ";
			cin >> ww;
			cout << "й 橾 殮溘: ";
			cin >> ee;
			list.addTodo(ww, ee);
			break;
		case 2:
			cout << "м煽й 葬蝶お 殮溘: ";
			cin >> a;
			list.mergeList(a);
			list.print();
			break;
		case 3:
			list.deleteList();
			list.print();
			break;
		case 4:
			cout << "夥羚 檜葷 殮溘: ";
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

// 瞪羲 滲熱 --------------------------------------------------------------
typedef pair<int, int> pii;
pii curSelect = { 0, 0 };
time_t trashTime;
bool isEnd = false;
User user = User();
bool isThreadPrinting = false;


// л熱 摹樹 --------------------------------------------------------------
void cursor_Draw(int x, int y, int sceneIndex, bool mode);
int input(int row, int col, int type);
void timePrint(void);
void sys_init(void);
void textOperator(int len, char text[], pii pos, bool hidden);

// Scene Func
int log_in(void);
int home(void);
int calendar(void);
int setting(void);
int FTM(void);
int group_management(void);
void current_group_task(int idenfier);
void set_group_property(int idenfier);
void FTM_paint(string texts[3]);

void try_initialize();

void management_paint(pii start, pii volume, pii gap, int type, vector<string> info);
void management_paint_dynamic_list(pii start, pii volume, string text);

void home_paint(pii start, pii volume, pii gap, vector<pair<string, vector<string>>> info);
void home_paint_dynamic_list(pii start, pii volume, string text);
void home_task_paint(pii start, pii gap, vector<string> texts);

int(*scene[8])(void) = { log_in, home, calendar, calendar, group_management, FTM, calendar, setting};

// Design Func
void design_Default(void);
void design_log_in(void);
void design_home(void);
void design_calendar(void);
void design_setting(void);
void additional_design_setting(void);
void design_fast_task_manager(void);
void design_group_manager(void);
void design_current_group_task(void);
void design_set_group_property(void);

// FILE IO Func
void save();
void load(string id);

// л熱 薑曖 --------------------------------------------------------------

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
		save();
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
	load(id);
	system("cls");
	return 1;
}


// だ諒 僥濠 : \n
void save() {
	string fileName = user.get_id() + ".txt";
	FILE* fp = fopen(fileName.c_str(), "wt");
	fprintf(fp, "%d %d \n%d\n", user.mySetting.get_BGcolor(), user.mySetting.get_textColor(), user.myGroups.get_size());
	for (int i = 0; i < user.myGroups.get_size(); i++) {
		// 斜瑜 薑爾 盪濰
		Group* group = user.myGroups.get_group_by_index(i);
		fprintf(fp, "%s\n%d\n%d\n", group->get_name().c_str(), group->get_visual_mode(), group->get_taskSize());
		for (int j = 0; j < group->get_taskSize(); j++) {
			// 鷓蝶觼 薑爾 盪濰
			Task* task = group->get_task_by_index(j);
			fprintf(fp, "%s\n%d\n", task->get_name().c_str(), task->get_deadline_weight());
		}
	}
	fflush(fp);
	fclose(fp);
	return;
}

void load(string id) {
	string fileName = user.get_id() + ".txt";
	FILE* fp = fopen(fileName.c_str(), "rt");
	int settingProp[2] = { 0, 0 };
	int groupSize = 0;
	// 嶸盪 薑爾 煎萄
	if (fp != NULL) {
		fscanf(fp, "%d %d %d\n", &settingProp[0], &settingProp[1], &groupSize);
		for (int i = 0; i < groupSize; i++) {
			// 斜瑜 薑爾 煎萄
			char pname[20];
			
			int groupVisualMode = 0;
			int taskSize = 0;
			vector<pair<string, int>> taskContainer = vector<pair<string, int>>();
			fflush(fp);
			fgets(pname, 20, fp);
			string groupName(pname);
			fflush(fp);
			fscanf(fp, "%d %d\n", &groupVisualMode, &taskSize);
			taskContainer.reserve(taskSize);
			for (int j = 0; j < taskSize; j++) {
				// 鷓蝶觼 薑爾 煎萄
				pair<string, int> taskInfo = pair<string, int>();
				fflush(fp);

				char pname1[20];
				fgets(pname1, 20, fp);
				
				int k = 0;
				while (pname1[k] != '\n' && k < 19) {
					taskInfo.first += pname1[k++];
				}

				fscanf(fp, "%d\n", &taskInfo.second);
				taskContainer.push_back(taskInfo);
			}
			user.mySetting.set_BGcolor(settingProp[0]);
			user.mySetting.set_textColor(settingProp[1]);
			user.myGroups.add(groupName, groupVisualMode);
			Group* curGroup = user.myGroups.get_group_by_index(i);
			for (int i = 0; i < taskSize; i++) {
				date taskDate = { taskContainer[i].second / 100, taskContainer[i].second % 100 };
				curGroup->add(taskContainer[i].first, taskDate);
			}
		}
	}
	return;
}

int home(void) {
	vector<string> groupTexts = vector<string>();
	vector<pair<string, vector<string>>> groupInfos = vector<pair<string, vector<string>>>();
	user.myGroups.get_group_text(&groupTexts);
	groupInfos.reserve(groupTexts.size());
	for (int i = 0; i < groupTexts.size(); i++) {
		vector<string> taskInfos = vector<string>();
		Task* iterPos = user.myGroups.get_group_by_index(i)->get_rootTask();
		for (; iterPos != NULL; iterPos = iterPos->next) {
			taskInfos.push_back(iterPos->get_deadline_string() + " : " + iterPos->get_name());
		}
		groupInfos.push_back({ groupTexts[i], taskInfos });
	}

	int limit = groupTexts.size();
	design_home();
	if (limit > 0) {
		home_paint({ 5, 14 }, { 50, 15 }, { 27, 0 }, groupInfos);
	}

	isEnd = false;
	thread paintT(timePrint);
	int direction = 0;
	do {
		cursor_Draw(curSelect.first, 0, 0, 1);
	    direction = input(5, 1, 0);
		cursor_Draw(curSelect.first, 0, 0, 0);
		Sleep(5);
		
		switch (direction) {
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
	isThreadPrinting = true;
	Sleep(10);
	struct tm* curTime;
	MonthTable mt;
	mt.make_Cal();
	gotoxy(4, 4);
	printf("橾 錯 �� 熱 跡 旎 饜");
	for (int i = 0; i < 5; i++) {
		gotoxy(4, 5 + i);
		for (int j = 0; j < 7; j++) {
			if (mt.date[i][j] != 0) printf("%2d ", mt.date[i][j]);
			else printf("   ");
		}
	}
	while (!isEnd) {
		time_t curT = time(&trashTime);
		curTime = localtime(&curT);
		gotoxy(12, 2);
		printf("%02d:%02d:%02d", curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
		isThreadPrinting = false;
		Sleep(500);
	}

}

int calendar(void) {
	int term = 0;
	MonthTable mt;
	mt.make_Cal();
	do {
		int c = 0;
		gotoxy(0, 0);
		design_calendar();
		gotoxy(1, 1);
		printf(" %2d喇 / %2d錯 / %2d橾", mt.pLocal->tm_year + 1900, mt.pLocal->tm_mon + 1, mt.pLocal->tm_mday);
		gotoxy(1, 7);
		printf("\t橾");
		printf("\t\t錯\t\t�苒t\t熱\t\t跡\t\t旎");
		printf("\t\t饜");
		for (int i = 0; i < 5; i++) {
			gotoxy(3, 12 + c);
			for (int j = 0; j < 7; j++) {
				if (mt.date[i][j]) {
					//if (mt.date[i][j] == mt.pLocal->tm_mday)
					printf("\t%2d\t", mt.date[i][j]);
				}
				else printf("\t\t");
			}
			c += 3;
		}
		do {
			int tsec = time(0);
			int csec = tsec % 60;
			int tmin = tsec / 60;
			int cmin = tmin % 60;
			int tho = tmin / 60 + 9;
			int cho = tho % 24;
			gotoxy(2, 3);
			printf("%2d:%02d:%02d", cho, cmin, csec);
			Sleep(500);
		} while (_kbhit() == 0);
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
	printf("≒");
	gotoxy(25, 6);
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			gotoxy(25, 8);
		}
		printf("≒  ");
	}
	gotoxy(17, 11);
	printf("≒");
	gotoxy(25, 11);
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			gotoxy(25, 13);
		}
		printf("≒  ");
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
		// 蟾晦��
		user.mySetting.set_BGcolor(0);
		user.mySetting.set_textColor(15);
	}
	return;
}

// 澀 陛奢ж晦
int FTM(void) {
	design_fast_task_manager();
	vector<Group*> groups = vector<Group*>();
	for (int i = 0; i < user.myGroups.get_size(); i++) {
		groups.push_back(user.myGroups.get_group_by_index(i));
	}
 
	vector<Task*> tasks = vector<Task*>();
	char keyword[20] = "                   ";
	int direction = 0;
	int selected = 0;
	int limit = 0;
	int groups_size = groups.size();
	
	do {
		// 鷓蝶觼 碳楝螃晦
		selected = 0;
		tasks.clear();
		for (int i = 0; i < groups_size; i++) {
			int tasks_size = groups[i]->get_taskSize();
			for (int j = 0; j < tasks_size; j++) {
				Task* iterPos = groups[i]->get_rootTask();
				if (iterPos == NULL) {
					break;
				}
				tasks.push_back(iterPos);
				iterPos = iterPos->next;
			}
		}
		// 醴憮 轎溘 -> 匐儀ж朝 睡碟縑虜 
		if (selected) {
			cursor_Draw(curSelect.first + (curSelect.second * 3), 0, 2, 1);
		}
		direction = input(3, 8, 0);
		if (selected) {
			cursor_Draw(curSelect.first + (curSelect.second * 3), 0, 2, 0);
		}
		Sleep(5);
		switch (direction) {
		case 'A': case 'a':
			curSelect = { 0, 0 };
			selected = 1;
			break;
		case 'D': case 'd':
			curSelect = { 0, 0 };
			selected = 2;
			break;
		case 'M': case 'm':
			curSelect = { 0, 0 };
			selected = 3;
			break;
		case 'B': case 'b':
			curSelect = { 0, 0 };
			system("cls");
			return 1;
		// 鼻ж 檜翕
		case 1: case -1:
			if (selected) {
				curSelect.first += direction;
			}
			break;
		// む檜雖 檜翕
		case 2: case -2:
			if (selected) {
				curSelect.second += direction / 3;
			}
			break;
		case ENTER:
			switch (selected) {
			case 1:
				
				break;
			case 2:
				break;
			case 3:
				break;
			}
			selected = 0;
			break;
		}

		// 陳瞼, 鷓蝶觼 殮溘
		/*
		char newDateString[2][3] = { "00", "00" };
		char keyword[23] = "                      ";
		
		gotoxy(14, 7);
		printf("                           ");
		textOperator(2, newDateString[0], { 14, 7 }, false);
		textOperator(2, newDateString[1], { 17, 7 }, false);
		date newDeadline = { atoi(newDateString[0]), atoi(newDateString[1]) };
		textOperator(23, keyword, { 20, 7 }, false);
		
		if (selected == 1) {
			string paramArr[3] = { groups[curSelect.second * 3]->get_name(), groups[curSelect.second * 3 + 1]->get_name(),groups[curSelect.second * 3 + 2]->get_name() };
			FTM_paint(paramArr);

			groups[curSelect.first + curSelect.second * 3]->add(keyword, newDeadline);
			groups[curSelect.first + curSelect.second * 3]->sort_task();
		}
		*/
	} while (true);
}

void FTM_paint(string texts[3]) {
	int j = 0;
	for (int i = 5; i < 3; i+= 4) {
		gotoxy(i, 57);
		printf("%s", texts[j++].c_str());
	}
	return;
}

int group_management(void) {
	design_group_manager();
	vector<string> groupTexts;
	int limit;
	int bufferIndex = -1;
	int direction = 0;
	int selected = -1;
	curSelect = { 0,0 };
	do {
		user.myGroups.get_group_text(&groupTexts);
		limit = groupTexts.size();
		management_paint({ 4, 2 }, { 74, 3 }, { 42, 3 }, 0, groupTexts);
		cursor_Draw(curSelect.first + (curSelect.second * 9), 0, 2, 1);
		direction = input(10, 2, 0);
		cursor_Draw(curSelect.first + (curSelect.second * 9), (curSelect.first + (curSelect.second * 9) + 1) < limit ? '@' : (curSelect.first + (curSelect.second * 9) + 49), 2, 0);
		Sleep(5);

	switch (direction) {
		case 'D': case 'd':
			selected = 1;
			bufferIndex = curSelect.first + curSelect.second * 9;
			break;
		case 'T': case 't':
			if (curSelect.first + (curSelect.second * 9) == limit) {
				if (limit < 18) {
					char newGroupName[23] = "                      ";
					textOperator(23, newGroupName, { 10 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);
					user.myGroups.add(newGroupName, 0);
				}
			}
			else {
				system("cls");
				current_group_task(curSelect.first + curSelect.second * 9);
				gotoxy(0, 0);
				design_group_manager();
			}
			break;
		case 'M': case 'm':
			selected = 2;
			bufferIndex = curSelect.first + (curSelect.second * 9);
			// 斜瑜 煽м
			break;
		case 'P': case 'p':
			if (curSelect.first + (curSelect.second * 9) != limit) {
				cin.ignore();
				system("cls");
				set_group_property(curSelect.first + curSelect.second * 9);
				gotoxy(0, 0);
				design_group_manager();
			}
			break;
		case 'B': case 'b':
			curSelect = { 0, 0 };
			system("cls");
			return 1;
		case 1: case -1:
			if (curSelect.first + direction + curSelect.second * 9 <= limit) {
				curSelect.first += direction;
				if (curSelect.first == 9 && direction == 1) {
					curSelect.second = 1;
					curSelect.first = 0;
				}
				if (curSelect.first == -1 && curSelect.second == 1 && direction == -1) {
					curSelect.second = 0;
					curSelect.first = 8;
				}
			}
			break;
		case 9: case -9:
			if (curSelect.first + ( (direction /9) + curSelect.second ) * 9 <= limit) {
				curSelect.second += direction / 9;
			}
			break;
		case ENTER:
			if (selected != -1 && bufferIndex != curSelect.first + curSelect.second * 8) {
				switch (selected) {
				case 1: // 嬪纂滲唳
					user.myGroups.insert(bufferIndex, curSelect.first + (curSelect.second * 9));
					break;
				case 2: // 煽м籀葬
					break;
				}
			}
			bufferIndex = -1;
			selected = -1;
			break;
		}
	} while (true);
	system("cls");
	return 1;
}

void current_group_task(int identifier) {
	gotoxy(0, 0);
	design_current_group_task();

	Group* group = user.myGroups.get_group_by_index(identifier);
	vector<string> taskTexts;
	Task* curTask;
	int limit;
	int direction = 0;
	curSelect = { 0,0 };
	do {
		// 鷓蝶觼 賅舒 餉薯腆 陽 ��轎腎朝 睡碟 -> 縑楝曖 錳檣
		group->get_task_text(&taskTexts);
		limit = taskTexts.size();
		management_paint({ 4, 2 }, { 74, 3 }, { 42, 3 }, 1, taskTexts);
		cursor_Draw(curSelect.first + (curSelect.second * 9), 0, 2, 1);
		direction = input(10, 2, 0);
		cursor_Draw(curSelect.first + (curSelect.second * 9), 0, 2, 0);
		Sleep(5);

		curTask = group->get_task_by_index(curSelect.first + (curSelect.second * 9));

		switch (direction) {
		case 'A': case 'a': // add
			if (limit < 18) {
				char newDateString[2][3] = { "00", "00" };
				char newTaskName[23] = "                      ";

				gotoxy(10 + (limit / 9) * 42, 3 + (limit % 9) * 3);
				printf("                           ");

				textOperator(2, newDateString[0], { 10 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);
				textOperator(2, newDateString[1], { 13 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);
				date newDeadline = { atoi(newDateString[0]), atoi(newDateString[1]) };
				textOperator(23, newTaskName, { 16 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);

				group->add(newTaskName, newDeadline);
				group->sort_task();
			}
			break;
		case 'M': case 'm': // modify
			if (limit < 18) {
				char newDateString[2][3] = { "00", "00" };

				gotoxy(10 + (limit / 9) * 42, 3 + (limit % 9) * 3);
				printf("                           ");

				textOperator(2, newDateString[0], { 10 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);
				textOperator(2, newDateString[1], { 13 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);
				date newDeadline = { atoi(newDateString[0]), atoi(newDateString[1]) };
				char newTaskName[23] = "                      ";
				textOperator(23, newTaskName, { 16 + (limit / 9) * 42  , 3 + (limit % 9) * 3 }, false);

				curTask->set_deadline(newDeadline);
				curTask->set_name(newTaskName);
				group->sort_task();
			}
			break;
		case 'D': case 'd': //delete
			group->remove(curTask);
			gotoxy(0, 0);
			design_current_group_task();

			if (curSelect.first + (curSelect.second * 9)) {
				if (curSelect.first == 0 && curSelect.second == 1) {
					curSelect.second--;
					curSelect.first = 8;
				}
				else {
					curSelect.first--;
				}
			}
			break;
		case 'I': case 'i':
			user.myGroups.remove(identifier);
		case 'B': case 'b':
			curSelect = { 0, 0 };
			system("cls");
			return;
		case 1: case -1:
			if (curSelect.first + direction + curSelect.second * 9 <= limit) {
				curSelect.first += direction;
				if (curSelect.first == 9 && direction == 1) {
					curSelect.second = 1;
					curSelect.first = 0;
				}
				if (curSelect.first == -1 && curSelect.second == 1 && direction == -1) {
					curSelect.second = 0;
					curSelect.first = 8;
				}
			}
			break;
		case 9: case -9:
			if (curSelect.first + ((direction / 9) + curSelect.second) * 9 <= limit) {
				curSelect.second += direction / 9;
			}
			break;
		}
	} while (true);

	system("cls");
	return;
}

void set_group_property(int identifier) {
	gotoxy(0, 0);
	design_set_group_property();
	Group* group = user.myGroups.get_group_by_index(identifier);
	gotoxy(4, 2);
	printf("%s", (group->get_name()).c_str());
	int direction = 0;
	curSelect = { 0,0 };
	do {
		cursor_Draw(curSelect.second, 0, 3, 1);
		direction = input(1, 3, 0);
		cursor_Draw(curSelect.second, 0, 3, 0);
		Sleep(5);

		switch (direction) {
		case 1: case -1:
			curSelect.second += direction;
			group->set_visual_mode(curSelect.second);
			break;
		case 'B': case 'b': case ENTER:
			curSelect = { 0,0 };
			system("cls");
			return;
		}
	} while (true);
}

void cursor_Draw(int x, int y, int sceneIndex, bool mode) {
	int row, col;
	int rowStart;
	int rowGap;
	int colStart;
	int colGap;
	switch (sceneIndex) {
	case 0: // Menu
		rowStart = 3;
		rowGap = 5;
		colStart = 87;
		colGap = 29;
		row = rowStart + x * 5;
		col = colStart;
		gotoxy(col, row);
		if (mode) printf("旨"); else printf(" ");
		gotoxy(col, row + rowGap);
		if (mode) printf("戌"); else printf(" ");
		gotoxy(col + colGap, row);
		if (mode) printf("忖"); else printf(" ");
		gotoxy(col + colGap, row + rowGap);
		if (mode) printf("戎"); else printf(" ");
		break;
	case 1: // Setting
		rowStart = 6;
		colStart = 23;
		colGap = 4;
		row = rowStart + (y * 5) + ((x % 2) * 2);
		col = colStart + ((x / 2) * 4);
		gotoxy(col, row);
		if (mode) printf("Ⅱ"); else printf(" ");
		gotoxy(col + colGap, row);
		if (mode) printf("９"); else printf(" ");
		break;
	case 2: // List
		rowStart = 3;
		colStart = 7;
		row = rowStart + (x % 9) * 3;
		col = colStart + (x / 9) * 42;
		gotoxy(col, row);
		if (mode) printf("Ⅱ"); else printf("  ");
		break;
	case 3: // property
		rowStart = 5;
		colStart = 3;
		rowGap = 4;
		colGap = 25;
		row = rowStart;
		col = colStart + x * 28;
		gotoxy(col, row);
		if (mode) printf("旨"); else printf(" ");
		gotoxy(col, row + rowGap);
		if (mode) printf("戌"); else printf(" ");
		gotoxy(col + colGap, row);
		if (mode) printf("忖"); else printf(" ");
		gotoxy(col + colGap, row + rowGap);
		if (mode) printf("戎"); else printf(" ");
		break;
	}

}

void management_paint(pii start, pii volume, pii gap, int type, vector<string> info) {
	int size = info.size();
	int x, y;
	for (int i = 0; i <= size; i++) {
		x = start.first + (gap.first * (i / 9));
		y = start.second + (gap.second * (i % 9));
		if (i == size) {
			switch (type) {
			case 0:
				management_paint_dynamic_list({ x, y }, volume, "@ Press T -> new group");
				break;
			case 1:
				management_paint_dynamic_list({ x, y }, volume, "@ Press A -> new task");
				break;
			}
		}
		else {
			management_paint_dynamic_list({ x, y }, volume, info[i]);
		}
	}
	return;
}

void management_paint_dynamic_list(pii start, pii volume, string text) {
	gotoxy(start.first, start.second);
	printf("旨");
	for (int i = 2; i < volume.first - 2; i += 2) {
		printf("收");
	}
	printf("旬");

	for (int i = 1; i < volume.second - 1; i++) {
		gotoxy(start.first, start.second + i);
		printf("早");
		for (int j = 2; j < volume.first - 2; j += 2) {
			printf(" ");
		}
		printf("早");
	}

	gotoxy(start.first, start.second + volume.second - 1);
	printf("曲");
	for (int i = 2; i < volume.first - 2; i += 2) {
		printf("收");
	}
	printf("旭");

	int length = text.size();
	gotoxy(start.first + 7, start.second + 1);
	for (int i = 0; i < length; i++) {
		printf("%c", text[i]);
	}
	return;
}

void home_paint(pii start, pii volume, pii gap, vector<pair<string, vector<string>>> info) {
	int size = info.size();
	int x, y;
	for (int i = 0; i <= size; i++) {
		x = start.first + (gap.first * (i % 9));
		y = start.second + (gap.second * (i % 9));
		if (i == 3)
		{
			break;
		}
		if (i == size) {
			printf("");
		}
		else {
			home_paint_dynamic_list({ x, y }, volume, info[i].first);
			home_task_paint({ x, y + 2 }, { 0, 5 }, info[i].second);
		}
	}
	return;
}

void home_paint_dynamic_list(pii start, pii volume, string text) {
	gotoxy(start.first, start.second);
	printf("旨");
	for (int i = 2; i < volume.first - 2; i += 2) {
		printf("收");
	}
	printf("旬");

	for (int i = 1; i < volume.second - 1; i++) {
		gotoxy(start.first, start.second + i);
		printf("早");
		for (int j = 2; j < volume.first - 2; j += 2) {
			printf(" ");
		}
		printf("早");
	}

	gotoxy(start.first, start.second + volume.second - 1);
	printf("曲");
	for (int i = 2; i < volume.first - 2; i += 2) {
		printf("收");
	}
	printf("旭");

	int length = text.size();
	gotoxy(start.first + 2, start.second + 1);
	for (int i = 0; i < length; i++) {
		printf("%c", text[i]);
	}
	return;
}

void home_task_paint(pii start, pii gap, vector<string> texts)
{
	int length = texts.size();
	
	for (int i = 0; i < length; i++) {
		gotoxy(start.first + 2, start.second + 1 + i);
		printf("%s", texts[i].c_str());
		gotoxy(start.first + 24, start.second + 1 + i);
		printf("早");
	}
	
	return;
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
		else if (term == 0xE0 || term == 0) {
			term = _getch();
			if (term == UP) {
				return;
			}
			else if (term == DOWN)
			{
				return;
			}
		}
	}
	return;
}

int input(int row, int column, int sceneNumber) // 褻濛晦棟擊 陛霞 л熱. 勒菟雖 葆衛螃.
{
	while (true)
	{
		int userSelect = _getch();
		if (userSelect == 0xE0 || userSelect == 0)
		{
			userSelect = _getch();
			if (userSelect == UP && curSelect.first > 0) // 嬪
			{
				return -1;
			}
			else if (userSelect == DOWN && curSelect.first < row - 1) // 嬴楚
			{
				return 1;
			}
			else if (userSelect == LEFT && column >= 2 && curSelect.second > 0) // 豭薹
			{
				return -row;
			}
			else if (userSelect == RIGHT && column >= 2 && curSelect.second < 7) // 螃艇薹
			{
				return row;
			}
		}
		else if (userSelect == 13 && sceneNumber != -1) // 縛攪酈
		{
			return ENTER;
		}
		else if (userSelect == 8 && (sceneNumber >= 1 || sceneNumber == -1)) // 寥蝶む檜蝶
		{
			return BACK_SPACE;
		}
		else if (userSelect == 9) // 龔 -> 顫檜ぎ
		{
			return 1234;
		}
		else {
			return userSelect;
		}
	}
}

void design_Default(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早   Default                                                                                                           早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_log_in(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早   Log In                                                                                                            早\n");
	printf("早 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                       Task Manager                                                  早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                       ____________                                                  早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                       ____________                                                  早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_home(void) {
	while (isThreadPrinting) {
		Sleep(50);
	}
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早 Calendar               早早 Guide                                                   早早             MENU             早早\n");
	printf("早早                        早早                                                         早早                              早早\n");
	printf("早早                        早早  嬪 嬴楚 酈煎 詭景蒂 褻濛м棲棻.                        早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                        早早                                                         早早 早         斜瑜 婦葬        早 早早\n");
	printf("早早                        早早  縛攪酈煎 檜翕м棲棻.                                   早早 早                          早 早早\n");
	printf("早早                        早早                                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                        早早  寥蝶む檜蝶煎 檜瞪 寞僥 �飛橉虞� 給嬴骨棲棻.            早早                              早早\n");
	printf("早早                        早早                                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                        早早  欽蹴酈陛 噙罹 氈棻賊 欽蹴酈煎 檜翕м棲棻.              早早 早        偃嫦 霤罹濠       早 早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早 早                          早 早早\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早 Group Task                                                                        早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早          Calendar        早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早          setting         早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早          log out         早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_setting(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早   撲薑                                                                            早早             MENU             早早\n");
	printf("早早 收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早     ( G ) 寡唳 鏽楝      早 早早\n");
	printf("早早  寡唳 鏽楝   仃   Ⅱ    仃 仃                                                     早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早   ( T ) 臢蝶お 鏽楝      早 早早\n");
	printf("早早  臢蝶お 鏽楝 仃   Ⅱ    仃 仃                                                     早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早   ( I ) 撲薑 蟾晦��      早 早早\n");
	printf("早早  撲薑 蟾晦��      Ⅱ  ii                                                          早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早   ( D ) 等檜攪 蟾晦��    早 早早\n");
	printf("早早  等檜攪 蟾晦��   旨收收收收收收收收收收收收收收收收收收收收收收收收旬                                       早早 早                          早 早早\n");
	printf("早早                  早                        早                                       早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                  早  ID  仃                早                                       早早                              早早\n");
	printf("早早                  早                        早                                       早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                  早  PW  仃                早                                       早早 早      ( G ) 菴煎 陛晦     早 早早\n");
	printf("早早                  早                        早                                       早早 早                          早 早早\n");
	printf("早早                  曲收收收收收收收收收收收收收收收收收收收收收收收收旭                                       早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_calendar(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早   Calender                                                                                                          早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("早                                                                                                                     早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_fast_task_manager(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收--收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早                                         早                Result      <(1 / 2)>    早早             MENU             早早\n");
	printf("早早              Search / Add               早                                         早早                              早早\n");
	printf("早早                                         早  旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬  早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早  早 仃 譆辨撩                         早  早早 早          彌營⑽          早 早早\n");
	printf("早早  旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬  早  曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭  早早 早                          早 早早\n");
	printf("早早  早 仃                                早  早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早  早                                   早  早  旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬  早早                              早早\n");
	printf("早早  曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭  早  早 仃                                早  早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早  曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭  早早 早          檜蝓團          早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早  旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬  早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早  早 仃 檜遽膨                         早  早早                              早早\n");
	printf("早早                                         早  曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭  早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早收收收收收收收收收收收收收收收收收收收收收收收收--收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收早早 早                          早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早       ( B ) 菴煎 陛晦    早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

// search -> 7, 7
// result -> 49 ,5 縑憮 4蘊噶 頂溥馬
// �香嚂� -> 75, 3 檜剪 瞪羹陛 120檜賊 85

void design_group_manager(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收--收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早                                         早                                         早早             MENU             早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早 ( D ) ⑷營斜瑜 嬪纂滲唳  早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早  ( T ) ⑷營斜瑜 鷓蝶觼   早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( M ) 斜瑜 煽м      早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早   ( P ) 斜瑜 Щ煎ぷじ    早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( B ) 菴煎 陛晦      早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void design_current_group_task(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收--收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早                                         早                                         早早             MENU             早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( A ) 鷓蝶觼 蹺陛    早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( M ) 鷓蝶觼 熱薑    早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( D ) 鷓蝶觼 餉薯    早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早       ( I ) 蟾晦��       早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早早                                         早                                         早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                         早                                         早早 早     ( B ) 菴煎 陛晦      早 早早\n");
	printf("早早                                         早                                         早早 早                          早 早早\n");
	printf("早早                                         早                                         早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                         早                                         早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
// 斜瑜婦葬嫌 謝ル 翕橾

void design_set_group_property(void) {
	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早旨收收收收收收收收收收收收收收收收收收收收收收收收--收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬早\n");
	printf("早早                                                                                   早早             MENU             早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早    ( V ) 綠輿橡塭檜盪    早 早早\n");
	printf("早早  旨收收收收收收收收收收收收收收收收收收收收收收旬    旨收收收收收收收收收收收收收收收收收收收收收收旬    旨收收收收收收收收收收收收收收收收收收收收收旬  早早 早                          早 早早\n");
	printf("早早  早        蝶 鷗         早    早          聽          早    早          ��         早  早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早  曲收收收收收收收收收收收收收收收收收收收收收收旭    曲收收收收收收收收收收收收收收收收收收收收收收旭    曲收收收收收收收收收收收收收收收收收收收收收旭  早早                              早早\n");
	printf("早早                                                                                   早早 旨收收收收收收收收收收收收收收收收收收收收收收收收收收旬 早早\n");
	printf("早早                                                                                   早早 早      ( B ) 菴煎 陛晦     早 早早\n");
	printf("早早                                                                                   早早 早                          早 早早\n");
	printf("早早                                                                                   早早 曲收收收收收收收收收收收收收收收收收收收收收收收收收收旭 早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早早                                                                                   早早                              早早\n");
	printf("早曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
// 蝶鷗 陛遴等 嬪纂 -> 15, 7
// 聽 嬪纂 -> 42, 7
// �� 嬪纂 -> 69, 7