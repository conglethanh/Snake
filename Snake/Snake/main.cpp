#include "Snake.h"

void main()
{
	FixConsoleWindow();
	SetConsoleTitleA("LE THANH CONG -> 1612842");
	resizeConsole(860, 420);

	StartGame();
	
	Menu();

	thread t1(ThreadFunc); //Tao thread cho snake
	char* fileName = new char[MAX];
	int temp;
	while (1){
		temp = toupper(getch());

		if (temp == 27){//ESC exit game
			PauseGame(t1);
			ExitGame(t1);
			return;
		}

		if (temp == 'L')
		{
			PauseGame(t1);
			Sleep(100);
			GotoXY(2, HEIGH_CONSOLE + 2);
			for (int i = 0; i < WIDTH_CONSOLE; i++)
			{
				cout << " ";
			}
			GotoXY(2, HEIGH_CONSOLE + 2);

			cout << "Nhap ten fileSave de luu game: ";
			gets(fileName);
			SaveGame(fileName);
			GotoXY(2, HEIGH_CONSOLE + 2);
			cout << "Ban da luu Game!";
			for (int i = 0; i < strlen(fileName) + 30; i++)
			{
				cout << " ";
			}
			GotoXY(0, 0);
			Sleep(1000);
			ResumeGame(t1);
		}

		if (temp == 'T')
		{
			PauseGame(t1);
			Sleep(100);
			GotoXY(2, HEIGH_CONSOLE + 2);
			for (int i = 0; i < MAX / 2; i++)
			{
				cout << " ";
			}
			GotoXY(2, HEIGH_CONSOLE + 2);

			cout << "Nhap ten fileSave de tai game: ";
			gets(fileName);

			if (!LoadGame(fileName)) continue;

			LoadGame(fileName);
			GotoXY(2, HEIGH_CONSOLE + 2);
			
			cout << "Ban da tai Game!";
			Sleep(1000);
			ResumeGame(t1);

		}

		if (STATE == 1) {
			if (temp == 'P'){
				PauseGame(t1);
			}
			else if (temp == 27) {
				ExitGame(t1);
				return;
			}
			else{
				ResumeGame(t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else {

			if (temp == 'Y') {
				SCORE = 0;
				StartGame();
			}
			else {
				ExitGame(t1);
				return;
			}
		}
	}
}