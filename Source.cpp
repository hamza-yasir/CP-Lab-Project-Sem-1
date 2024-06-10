#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")  //to link with windows mmultimedia library

	#define SCREEN_WIDTH 90       //defined macros set for the console screen width, height and the playable field width
	#define SCREEN_HEIGHT 26
	#define WIN_WIDTH 70
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);    // handle to reference console output
COORD CursorPosition;       //structure for cursor position

struct Player {
	string name;        //structure for user info which is stored in player.data file
	int age;
} player1;      // a variable made out of player struct

int enemyY[3];      // array to hold enemy y-coordinates
int enemyX[3];       // array to hold enemy x-coordinates
bool enemyFlag[3];	  // array to track enemy states (active/inactive)
char car[4][4] = {      // 2D array to print desired shape of the cafr
	{' ', 'O', 'O', ' '},
	{'O', '*', '*', 'O'},
	{' ', '*', '*', ' '},
	{'O', '*', '*', 'O'}
};
int carPos = WIN_WIDTH / 2;    //starting pos of user car made to be in the centre of playable area
int score = 0;
int highScore = 0;

void gotoxy(int x, int y) {     // function to set cursor position
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {    // function to set cursor visibility and size
	if (size == 0)
		size = 20;		//default size of cursor
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {					// for loop to print the side borders
	for (int i = 0; i < SCREEN_HEIGHT; i++) {      
		for (int j = 0; j < 17; j++) {
			gotoxy(0 + j, i); cout << "*";				//left side
			gotoxy(WIN_WIDTH - j, i); cout << "*";		//right side
		}
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {			//bottom
		gotoxy(SCREEN_WIDTH, i); cout << "*";
	}
}

void genEnemy(int ind) {            //enemy generation at random x coordinate
	enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind) {				//drawing enemy caar at the coordinate
	if (enemyFlag[ind]) {
		gotoxy(enemyX[ind], enemyY[ind]); cout << "|  |";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "0**0";
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout << " ** ";
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "0**0";
	}
}

void eraseEnemy(int ind) {			//replacing enemny car printed at coordinate with spaces to erase car
	if (enemyFlag[ind]) {
		gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
	}
}

void resetEnemy(int ind) {				
	eraseEnemy(ind);		//erasing enemy as shown above and then resetting y position to generate new enemny at random x coord 
	enemyY[ind] = 1;		
	genEnemy(ind);
}

void drawCar() {		
	for (int i = 0; i < 4; i++) {			// Loop through each row of the car
		for (int j = 0; j < 4; j++) {		// Loop through each column of the car
			gotoxy(j + carPos, i + 22);		// Set the cursor position for each car part
			cout << car[i][j];			// Print the corresponding character from the car array
		}
	}
}

void eraseCar() {
	for (int i = 0; i < 4; i++) {			// Loop through each row of the car
		for (int j = 0; j < 4; j++) {		// Loop through each col of the car
			gotoxy(j + carPos, i + 22);		// Set the cursor position for each car part
			cout << " ";					//print space for each car part to erase
		}
	}
}

bool collision() {
	for (int i = 0; i < 3; i++) {
		if (enemyFlag[i]) {
			if (enemyY[i] + 4 >= 23) {
				if (enemyX[i] + 4 - carPos >= 0 && enemyX[i] + 4 - carPos < 9) {
					return true;
				}
			}
		}
	}
	return false;
}

void gameOver() {
	system("cls");
	system("color 0D");

	// high score 
	if (score > highScore) {
		highScore = score;
		ofstream fileout("highscore.dat");
		fileout << highScore;
		fileout.close();
	}
	cout << endl;
	cout << "\t\t\t\t\t\t--------------------------" << endl;
	cout << "\t\t\t\t\t\t-------- Game Over -------" << endl;
	cout << "\t\t\t\t\t\t--------------------------" << endl;
	cout << "\t\t\t\t\t\t--------------------------" << endl;
	if (score < highScore) {
		cout << "\t\t\t\t\t\t-------- Get better ------" << endl;
		cout << "\t\t\t\t\t\t--------------------------" << endl << endl;
	}
	else {
		cout << "\t\t\t\t\t\t--- yay new high score ---" << endl;
		cout << "\t\t\t\t\t\t--------------------------" << endl;
		cout << "\t\t\t\t\t\t--------- cheat? ---------" << endl;
		cout << "\t\t\t\t\t\t--------------------------" << endl;
		cout << "\t\t\t\t\t\t--------------------------" << endl << endl;
	}
	cout << "\t\t\t\t\t\t\tYour Score: " << score << endl;
	cout << "\t\t\t\t\t\t\tHigh Score: " << highScore << endl << endl;
	cout << "\t\t\t\t\t\tPress any key to return to menu";
	_getch();
}

void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5);
	cout << "Score: " << score << endl;
}

void instructions() {
	system("color 0D");
	system("cls");
	cout << "Instructions";
	cout << "\n----------------";
	cout << "\n Avoid Cars by moving left or right. ";
	cout << "\n\n Press 'a' to move left";
	cout << "\n Press 'd' to move right";
	cout << "\n Press 'escape' to exit";
	cout << "\n\nPress any key to go back to menu";
	_getch();
}

void play() {
	system("color 0D");
	carPos = -1 + WIN_WIDTH / 2;
	score = 0;
	enemyFlag[0] = true;
	enemyFlag[1] = false;
	enemyFlag[2] = false;
	enemyY[0] = enemyY[1] = enemyY[2] = 1;
	system("cls");
	drawBorder();
	updateScore();
	genEnemy(0);
	genEnemy(1);
	gotoxy(WIN_WIDTH + 7, 2); cout << "BUMP3RS";
	gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); cout << "Controls ";
	gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); cout << " a to move left ";
	gotoxy(WIN_WIDTH + 2, 15); cout << " d to move right ";
	gotoxy(18, 5); cout << "Press any key to start";
	_getch();
	gotoxy(18, 5); cout << "                            ";
	// where the magic is happening //
	while (1) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'a' || ch == 'A') {
				if (carPos > 18)
					carPos -= 4;
			}
			if (ch == 'd' || ch == 'D') {
				if (carPos < 50)
					carPos += 4;
			}
			if (ch == 27) {
				break;
			}
		}
		drawCar();
		drawEnemy(0);
		drawEnemy(1);
		if (collision()) {
			gameOver();
			return;
		}
		Sleep(30);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);
		if (enemyY[0] == 10 && !enemyFlag[1])
			enemyFlag[1] = true;
		if (enemyFlag[0])
			enemyY[0] += 1;
		if (enemyFlag[1])
			enemyY[1] += 1;
		if (enemyY[0] > SCREEN_HEIGHT - 4) {
			resetEnemy(0);
			score++;
			updateScore();
		}
		if (enemyY[1] > SCREEN_HEIGHT - 4) {
			resetEnemy(1);
			score++;
			updateScore();
		}
	}
}

void loadHighScore() {
	ifstream filein("highscore.dat");
	if (filein.is_open()) {
		filein >> highScore;
		filein.close();
	}
}

int main() {
	setcursor(0, 0);
	srand((unsigned)time(NULL));
	system("color 0D");
	loadHighScore();
	cout << "Enter player name: ";
	cin >> player1.name;
	cout << "Enter player age: ";
	cin >> player1.age;
	ofstream fileout("player.dat");
	fileout << player1.name << endl;
	fileout << player1.age << endl;
	fileout.close();
	while (true) {
		system("color 0D");
		system("cls");
		gotoxy(10, 5); cout << " \t\t\t\t\t-------------------------- ";
		gotoxy(10, 6); cout << " \t\t\t\t\t|         BUMP3RS        | ";
		gotoxy(10, 7); cout << " \t\t\t\t\t--------------------------";
		gotoxy(10, 9); cout << "\t\t\t\t\t1. Start Game";
		gotoxy(10, 10); cout << "\t\t\t\t\t2. Instructions";
		gotoxy(10, 11); cout << "\t\t\t\t\t3. Quit";
		gotoxy(10, 13); cout << "\t\t\t\t\tSelect option: ";
		char op = _getche();
		if (op == '1') {
			play();
		}
		else if (op == '2') {
			instructions();
		}
		else if (op == '3') {
			exit(0);
		}
	}
	return 0;
}
