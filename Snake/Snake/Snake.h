#include <iostream>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include <conio.h>
#include <fstream>
using namespace std;

#define INDEX 6
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3
#define MAX_SIZE_SNAKE 6+MAX_SIZE_FOOD*MAX_SPEED
#define MAX 100

char MSSV[] = "1612842";
POINT snake[MAX_SIZE_SNAKE];
POINT food[MAX_SIZE_FOOD];
POINT gate[5];

int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGH_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
int SCORE = 0;
int XUAT_HIEN_GATE = 0;
int SIZE_TEMP = 0;
char track[MAX_SIZE_SNAKE];
int HINTS = 0;

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX)& ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void ShowCur(bool CursorVisibility)
{//vietgamedev.net Ham an hien con tro trong Console
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

void SetColor(WORD color)
{//vietgamedev.net
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++)
	if (snake[i].x == x && snake[i].y == y)
		return false;
	return true;
}

void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do
		{
			x = rand() % (WIDTH_CONSOLE - 5) + 3;
			y = rand() % (HEIGH_CONSOLE - 5) + 3;

		} while (IsValid(x, y) == false);
		food[i] = { x, y };
	}
}
void GenerateGate(){
	int x, y;
	srand(time(NULL));
	do
	{
		x = rand() % (WIDTH_CONSOLE - 5) + 3;
		y = rand() % (HEIGH_CONSOLE - 5) + 3;
	} while
		(IsValid(x, y) == false
		|| IsValid(x - 1, y + 1) == false
		|| IsValid(x - 1, y) == false
		|| IsValid(x + 1, y) == false
		|| IsValid(x + 1, y + 1) == false
		|| y == snake[SIZE_SNAKE - 1].y
		|| y + 1 == snake[SIZE_SNAKE - 1].y);

	gate[2] = { x, y };
	gate[0] = { x - 1, y + 1 };
	gate[1] = { x - 1, y };
	gate[3] = { x + 1, y };
	gate[4] = { x + 1, y + 1 };
}

void drawGate(char *str)
{
	for (int i = 0; i < 5; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		printf(str);
	}
}

void ResetData() {
	//Khởi tạo các giá trị toàn cục
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; HINTS = 0;
	FOOD_INDEX = 0, WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 6;
	XUAT_HIEN_GATE = 0; SIZE_TEMP = 0;
	//Khởi tạo giá trị mặc định cho snake
	snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
	snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
	snake[4] = { 14, 5 }; snake[5] = { 15, 5 };

	GenerateFood();//Tạo mảng thức ăn food
	GenerateGate();
}

void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0){
	SetColor(13);
	GotoXY(x, y); cout << char(219);
	for (int i = 1; i < width; i++) cout << char(223);
	cout << char(219);
	GotoXY(x, height + y); cout << char(223);
	for (int i = 1; i < width; i++) cout << char(223);
	cout << char(223);
	for (int i = y + 1; i < height + y; i++){
		GotoXY(x, i); cout << char(219);
		GotoXY(x + width, i); cout << char(219);
	}
	GotoXY(curPosX, curPosY);
	SetColor(15);
}
void DrawRectangleThin(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0){

	GotoXY(x, y); cout << char(218);
	for (int i = 1; i < width; i++) cout << char(196);
	cout << char(191);
	GotoXY(x, height + y); cout << char(192);
	for (int i = 1; i < width; i++) cout << char(196);
	cout << char(217);
	for (int i = y + 1; i < height + y; i++){
		GotoXY(x, i); cout << char(179);
		GotoXY(x + width, i); cout << char(179);
	}
	GotoXY(curPosX, curPosY);

}
void DrawRectangleBold(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0){

	GotoXY(x, y); cout << char(201);
	for (int i = 1; i < width; i++) cout << char(205);
	cout << char(187);
	GotoXY(x, height + y); cout << char(200);
	for (int i = 1; i < width; i++) cout << char(205);
	cout << char(188);
	for (int i = y + 1; i < height + y; i++){
		GotoXY(x, i); cout << char(186);
		GotoXY(x + width, i); cout << char(186);
	}
	GotoXY(curPosX, curPosY);
}
void DrawHintMenu(){
	GotoXY(WIDTH_CONSOLE + 3, 12);
	cout << "LE THANH CONG - MSSV: 1612842";
	GotoXY(WIDTH_CONSOLE + 3, 14);
	cout << "DO AN TH KTLT - LOP: 16CNTN";
	GotoXY(WIDTH_CONSOLE + 3, 16);
	cout << "Press Any Key - New Game";
	GotoXY(WIDTH_CONSOLE + 3, 18);
	cout << "Press Key T   - Tai Lai";
}
void DrawHintGame(){
	GotoXY(WIDTH_CONSOLE + 3, 12);
	cout << " Press Key L   - Luu Game    ";
	GotoXY(WIDTH_CONSOLE + 3, 14);
	cout << " Press Key T   - Tai Lai   ";
	GotoXY(WIDTH_CONSOLE + 3, 16);
	cout << " Press Key P   - Pause Game";
	GotoXY(WIDTH_CONSOLE + 3, 18);
	cout << " Press Key ESC - Exit Game";
	HINTS++;
}
void DrawInfo(){
	//ve bang ten
	DrawRectangleBold(WIDTH_CONSOLE + 2, 0, 30, 2);
	GotoXY(WIDTH_CONSOLE + 2 + 13, 1);
	cout << "SNAKE";
	//ve bang diem va level
	DrawRectangleThin(WIDTH_CONSOLE + 2, 3, 15, 6);
	DrawRectangleThin(WIDTH_CONSOLE + 2, 3, 30, 6);

	GotoXY(WIDTH_CONSOLE + 7, 1 + 3); cout << "SCORE";
	GotoXY(WIDTH_CONSOLE + 7 + 2, 4 + 3); cout << SCORE;
	GotoXY(WIDTH_CONSOLE + 7 + 15, 1 + 3); cout << "LEVEL";
	GotoXY(WIDTH_CONSOLE + 7 + 15 + 2, 4 + 3); cout << SPEED;

	DrawRectangleThin(WIDTH_CONSOLE + 2, 3, 15, 2);
	DrawRectangleThin(WIDTH_CONSOLE + 2 + 15, 3, 15, 2);

	GotoXY(WIDTH_CONSOLE + 2 + 15, 6 + 3); cout << char(193);
	GotoXY(WIDTH_CONSOLE + 2 + 15, 2 + 3); cout << char(197);
	GotoXY(WIDTH_CONSOLE + 2 + 15, 0 + 3); cout << char(194);
	GotoXY(WIDTH_CONSOLE + 2, 2 + 3); cout << char(195);
	GotoXY(WIDTH_CONSOLE + 2 + 30, 2 + 3); cout << char(180);

	DrawRectangleThin(WIDTH_CONSOLE + 2, 10, 30, 10);//Ve bang hint
	GotoXY(WIDTH_CONSOLE + 2 + 13, 11);

	cout << "HINTS";
	DrawHintMenu();
	//Ve bang Notifications
	DrawRectangleThin(0, HEIGH_CONSOLE + 1, WIDTH_CONSOLE + 32, 2);
}

void StartGame() {
	ShowCur(0);
	system("cls");
	ResetData(); // Khởi tạo dữ liệu gốc
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // Vẽ màn hình game
	DrawInfo();
	STATE = 1;//Bắt đầu cho Thread chạy
	GotoXY(WIDTH_CONSOLE + 3, 0);
}
void DrawSnake(char*str){

	if (STATE == 0) SetColor(12);
	//co gate
	if (XUAT_HIEN_GATE == 1)
	{
		if (SIZE_TEMP == 0)
		{
			int dem = 0;
			for (int i = SIZE_SNAKE - 1; i >= 0; i--)
			{
				track[i] = MSSV[dem % 7];
				dem++;
			}
		}
		for (int i = SIZE_SNAKE - 1; i >= 0; i--)
		{
			GotoXY(snake[i].x, snake[i].y);
			if (strcmp(str, MSSV) != 0) cout << str;
			else if (i == SIZE_SNAKE - 1 && SIZE_TEMP == 0)
			{
				SetColor(11);
				GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
				cout << track[SIZE_SNAKE - 1];
				SetColor(15);
			}
			else cout << track[i];
		}
		return;
	}
	//ko co gate
	int dem = 0;
	for (int i = SIZE_SNAKE - 1; i >= 0; i--){

		GotoXY(snake[i].x, snake[i].y);
		if (strcmp(str, MSSV) == 0)
		{
			printf("%c", MSSV[dem % 7]);
			dem++;
			if (i == SIZE_SNAKE - 1)
			{
				SetColor(11);
				GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
				cout << MSSV[0];
				SetColor(15);
			}
		}
		else
			printf(str);
	}

	SetColor(15);
}
void ProcessDead() {

	STATE = 0;
	GotoXY(2, HEIGH_CONSOLE + 2);
	printf("Dead, type y to continue or anykey to exit");
	XUAT_HIEN_GATE = 0;
}

void ResetPos(){
	for (int i = 0; i < SIZE_SNAKE; i++){
		snake[i].x = 10 + i;
		snake[i].y = 5;
	}
	CHAR_LOCK = 'A';
	MOVING = 'D';
}

void Eat() {
	snake[SIZE_SNAKE] = food[FOOD_INDEX];

	if (FOOD_INDEX + 1 == MAX_SIZE_FOOD - 1) GenerateGate();

	if (FOOD_INDEX + 1 == MAX_SIZE_FOOD){
		XUAT_HIEN_GATE = 1;
		food[FOOD_INDEX] = gate[2];

		if (snake[SIZE_SNAKE].x == gate[2].x && snake[SIZE_SNAKE].y == gate[2].y){

			if (snake[SIZE_SNAKE - 1].x - 1 == gate[1].x&&snake[SIZE_SNAKE - 1].y + 1 == gate[1].y)
				ProcessDead();
			SIZE_SNAKE--;
			SIZE_TEMP++;
		}

		if (SIZE_TEMP == 0) {
			SCORE++;
			SIZE_SNAKE++;
		}
		if (SIZE_SNAKE != 0) return;
		else drawGate(" ");
	}
	XUAT_HIEN_GATE = 0;

	if (FOOD_INDEX + 1 == MAX_SIZE_FOOD){
		FOOD_INDEX = 0;
		SPEED++;
		SIZE_SNAKE = SIZE_TEMP;
		SIZE_TEMP = 0;
		ResetPos();
		if (SPEED == MAX_SPEED + 1)
			ResetData();
		GenerateFood();
		GenerateGate();
	}
	else
	{
		SCORE++;
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}

void DrawFood(char*str)
{
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf(str);
}
bool EatSnake()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[SIZE_SNAKE - 1].x == snake[i].x&&snake[SIZE_SNAKE - 1].y == snake[i].y)
			return true;
	}
	return false;
}

void PauseGame(thread &t) {
	SuspendThread(t.native_handle());
}
void ResumeGame(thread &t){
	ResumeThread(t.native_handle());
}
void Pause(char *message = "Press any key to continue . . . ") {
	//https://www.daniweb.com/programming/software-development/threads/169073/how-to-hide-the-press-any-key-to-continue
	//Ham an dung man hinh va an Press any key to continue
	std::cout << message;
	HANDLE hStdin;
	DWORD cNumRead;
	INPUT_RECORD irInBuf[1];
	if (HANDLE(hStdin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return;
	while (true)  {
		if (!ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead))
			return;
		for (DWORD i = 0; i < cNumRead; ++i)
		if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown) {
			std::cout << '\n';
			return;
		}
	}
}
void ExitGame(thread &t) {

	DrawFood(" ");
	drawGate(" ");
	DrawSnake(" ");

	GotoXY(WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2);
	cout << "GAME OVER";
	GotoXY(WIDTH_CONSOLE / 2 - 2 - 7, HEIGH_CONSOLE / 2 + 1);
	cout << "THANK YOU FOR PLAYING " << (char)254;
	GotoXY(2, HEIGH_CONSOLE + 2);
	for (int i = 0; i < WIDTH_CONSOLE; i++) cout << " ";
	GotoXY(2, HEIGH_CONSOLE + 2);
	cout << "Your score is: " << SCORE;
	Pause("");
	system("cls");
	t.detach();
}

void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 >= WIDTH_CONSOLE) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}

		snake[SIZE_SNAKE - 1].x++;
	}
}
void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 <= 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y){
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}
void MoveDown() {
	if (snake[SIZE_SNAKE - 1].y + 1 >= HEIGH_CONSOLE) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}
void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 <= 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}

void ThreadFunc() {
	while (1) {

		if (STATE == 1) {
			DrawFood(" ");
			DrawSnake(" ");
			switch (MOVING){
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			DrawFood("0");
			if (XUAT_HIEN_GATE == 1){
				drawGate("0");
				for (int i = 0; i < 5; i++)
				{
					if (i != 2)
					{
						if (snake[SIZE_SNAKE - 1].x == gate[i].x && snake[SIZE_SNAKE - 1].y == gate[i].y)
						{
							ProcessDead();
						}
					}
				}
			}
			if (HINTS <= 1)
				DrawHintGame();
			//Xử lý rắn ăn chính nó
			if (EatSnake()) ProcessDead();

			if (STATE == 0) {
				DrawSnake("#");
			}
			else
				DrawSnake(MSSV);
			//hieu ung chet
			if (STATE == 0) {
				for (int i = 0; i < 5; i++)
				{
					DrawSnake("#");
					Sleep(100);
					DrawSnake(" ");
					Sleep(100);
				}
				DrawFood(" ");
				drawGate(" ");

			}

			//In ra thông tin đang chơi

			GotoXY(WIDTH_CONSOLE + 7 + 2, 4 + 3); cout << SCORE << "  ";
			GotoXY(WIDTH_CONSOLE + 7 + 15 + 2, 4 + 3); cout << SPEED << "  ";

			if (STATE == 1)
			{
				GotoXY(2, HEIGH_CONSOLE + 2);
				for (int i = 0; i < WIDTH_CONSOLE; i++) cout << " ";
			}
			Sleep(1000 / (SPEED*INDEX));
		}
	}
}
void SaveGame(char* fileName)
{
	fstream fileSave(fileName, ios::out);

	fileSave << SCORE << endl;
	fileSave << SPEED << endl;
	fileSave << CHAR_LOCK << endl;
	fileSave << MOVING << endl;
	fileSave << FOOD_INDEX << endl;
	fileSave << SIZE_SNAKE << endl;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		fileSave << snake[i].x << " " << snake[i].y << " ";
	}
	fileSave << XUAT_HIEN_GATE << endl;
	fileSave << SIZE_TEMP << endl;
	for (int i = 0; i < 5; i++)
	{
		fileSave << gate[i].x << " " << gate[i].y << " ";
	}
	fileSave.close();
}
bool LoadGame(char*fileName)
{
	HINTS = 0;
	fstream fileSave(fileName, ios::in);
	if (fileSave.fail())
	{
		GotoXY(2, HEIGH_CONSOLE + 2);
		cout << "Khong co fileSave!";
		for (int i = 0; i < 10 + strlen(fileName); i++) cout << "  ";
		GotoXY(0, 0);
		return false;
	}

	fileSave >> SCORE;
	fileSave >> SPEED;
	fileSave >> CHAR_LOCK;
	fileSave >> MOVING;
	fileSave >> FOOD_INDEX;
	fileSave >> SIZE_SNAKE;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		fileSave >> snake[i].x >> snake[i].y;
	}
	fileSave >> XUAT_HIEN_GATE;
	fileSave >> SIZE_TEMP;
	for (int i = 0; i < 5; i++)
	{
		fileSave >> gate[i].x >> gate[i].y;
	}
	food[FOOD_INDEX] = gate[2];
	fileSave.close();

	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // Vẽ màn hình game
	DrawInfo();
	STATE = 1;//Bắt đầu cho Thread chạy	
	GotoXY(WIDTH_CONSOLE + 3, 0);
	return true;
}
void NewGame(char*str)
{
	StartGame();
	GotoXY(2, HEIGH_CONSOLE + 2);
	cout << str;
	for (int i = 3; i >= 1; i--)
	{
		GotoXY(strlen(str) + 2 + 2, HEIGH_CONSOLE + 2);
		cout << i;
		Sleep(i * 150);
	}
}
void Menu(){
	int temp;
	GotoXY(WIDTH_CONSOLE / 2 - 8, HEIGH_CONSOLE / 2);
	cout << (char)254 << " New Game";
	GotoXY(WIDTH_CONSOLE / 2 - 8, HEIGH_CONSOLE / 2 + 1);
	cout << (char)254 << " Tai lai";
	temp = toupper(getch());
	if (temp == 'T')
	{
		StartGame();
		GotoXY(2, HEIGH_CONSOLE + 2);
		cout << "Nhap ten file da luu: ";
		char* fileName = new char[MAX];
		gets(fileName);
		if (!LoadGame(fileName))
		{
			NewGame("Khong co file nay, chuyen sang New Game...");
		}
		LoadGame(fileName);
		system("cls");
		DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
		DrawInfo();
		GotoXY(2, HEIGH_CONSOLE + 2);
		cout << "Ban da tai Game thanh cong!";
		Sleep(1000);
	}
	else
	{
		NewGame("Dang vao che do New Game");
	}
}