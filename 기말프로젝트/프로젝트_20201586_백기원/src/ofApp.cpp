/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <random>
#include <functional>
using namespace std;

typedef struct _node {
	int x, y;
	int score;
}Node;

//store player, coin information
Node player;
Node coin;

//maze information
#define maze_col 41
#define maze_row 41
#define maze_size 20
char maze_input[41][41];

deque<pair<int, int>> bfs_root;//answer of bfs_root
deque<tuple<int, int, int>> trial_bfs_root;//all trail of bfs <y,x,parent>

//--------------------------------------------------------------
void ofApp::setup() {
	//플레이어 위치 초기화
	player.x = 1;
	player.y = 1;
	player.score = 0;

	timer = ofGetElapsedTimeMillis();  // 현재 시간 저장
	interval = 1000;  // 1초 (1000밀리초)

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "GameStart", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "GameStart") {
		make_map();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}
} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {
	if (isOpen && !Gameover) {
		if (ofGetElapsedTimeMillis() - timer > interval/cur_lev) {//fog spread faster when level up
			// 1초 경과 시 동작할 함수 호출
			//cout << end << "\n";
			fog_end++;
			//cout << fog_end << "\n";
			timer = ofGetElapsedTimeMillis();  // 현재 시간 갱신
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	//DRAW MAZE; 
	if (isOpen) {
		//if player reach to end
		if (player.x == 39 && player.y == 39) {
			freeMemory();
			make_map();
			cur_lev++;//level up! - fog spread faster
		}
		//draw maze
		for (int i = 0; i < maze_row; i++) {
			for (int j = 0; j < maze_col; j++) {
				if (maze_input[i][j] == '-') {
					ofDrawLine((j - 1) * 10, i * 10, (j + 1) * 10, i * 10);
				}
				else if (maze_input[i][j] == '|') {
					ofDrawLine(j * 10, (i - 1) * 10, j * 10, (i + 1) * 10);
				}
			}
		}
		//caculate fog
		isbfs = BFS();

		//draw player
		ofSetColor(100, 200, 100);
		ofDrawCircle(player.x*10, player.y*10, 8);

		//draw fog
		bfsdraw();

		//if player had coin - add score
		if (coin.x == player.x && coin.y == player.y && coin_exist) {
			coin_exist = false;
			player.score += 500;//먹은 coin당 점수 +500
		}
		if(coin_exist) coindraw();//coin을 먹기 전이면 그려준다.

		//when game is over
		if (Gameover) {
			ofSetColor(255, 0, 0);
			ofDrawBitmapString("GameOver!!", maze_col*10 + 40, 50);  // 화면에 gameover 출력
			if (sum_is_first) {
				player.score += (cur_lev-1) * 1000;//통과한 단계*1000 점수
				sum_is_first = false;
			}
			string output = "Your score is : " + ofToString(player.score);//print score
			ofDrawBitmapString(output, maze_col*10 + 40, 60);  // 화면의 (50, 50) 좌표에 문자열 출력
		}
	}

	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(200);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'q') {

	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	//방향키로 플레이어 이동
	if (key == OF_KEY_LEFT&&!Gameover) {
		if (player.x - 2 > 0 && maze_input[player.y][player.x-1] != '|') player.x -= 2;
	}

	if (key == OF_KEY_RIGHT && !Gameover) {
		if (player.x + 2 < maze_col && maze_input[player.y][player.x + 1] != '|') player.x+=2;
	}

	if (key == OF_KEY_UP && !Gameover) {
		if (player.y - 2 > 0 && maze_input[player.y-1][player.x] != '-') player.y-=2;
	}

	if (key == OF_KEY_DOWN && !Gameover) {
		if (player.y + 2 < maze_row && maze_input[player.y + 1][player.x] != '-') player.y+=2;
	}
} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::freeMemory() {
	//memory를 free해주는 함수
	bfs_root.clear();
	bfs_root.shrink_to_fit();
	trial_bfs_root.clear();
	trial_bfs_root.shrink_to_fit();

	//코인, 안개, 플레이어 초기화
	coin_exist = true;
	fog_end = 0;
	player.x = 1;
	player.y = 1;
}

bool ofApp::BFS() {
	//initialize visited array
	int visited[maze_row][maze_col];
	for (int i = 0; i < maze_row; i++) {
		for (int j = 0; j < maze_col; j++) {
			visited[i][j] = 0;
		}
	}
	//four directions
	int x[4] = { 2,0,-2,0 }, y[4] = { 0,2,0,-2 };
	//present coordinate
	int loc_x = 1, loc_y = 1;
	//all trail root index
	int t_bfs_r_idx = 0;

	//push start point
	bfs_root.push_back(make_pair(loc_y, loc_x));
	trial_bfs_root.push_back(make_tuple(loc_y, loc_x, -1));
	visited[1][1] = 1;
	

	while (bfs_root.size() > 0) {
		int b_flag = 0;
		loc_y = bfs_root[0].first;
		loc_x = bfs_root[0].second;

		if (loc_y == maze_row - 2 && loc_x == maze_col - 2) {
			return true;
		}
		//search four directions
		for (int i = 0; i < 4; i++) {
			if (loc_y + y[i] >= 0 && loc_y + y[i] < maze_row && loc_x + x[i] >= 0 && loc_x + x[i] < maze_col) {
				if (visited[loc_y + y[i]][loc_x + x[i]] == 0 && maze_input[loc_y + y[i] / 2][loc_x + x[i] / 2] != '-' && maze_input[loc_y + y[i] / 2][loc_x + x[i] / 2] != '|') {
					visited[loc_y + y[i]][loc_x + x[i]] = 1;
					bfs_root.push_back(make_pair(loc_y + y[i], loc_x + x[i]));
					trial_bfs_root.push_back(make_tuple(loc_y + y[i], loc_x + x[i], t_bfs_r_idx));
				}
			}
		}
		t_bfs_r_idx++;
		//if it can't move, go back
		bfs_root.pop_front();

	}

	return true;
}


void ofApp::bfsdraw() {
	ofSetLineWidth(5);
	ofSetColor(256, 0, 0);
	//draw bfs algorithm's every trail
	for (int i = 1; i < fog_end-10; i++) {
		auto temp_bfs_root1 = trial_bfs_root[i];
		auto temp_bfs_root2 = trial_bfs_root[get<2>(trial_bfs_root[i])];
		ofDrawLine(get<1>(temp_bfs_root1) * 10, get<0>(temp_bfs_root1) * 10, get<1>(temp_bfs_root2) * 10, get<0>(temp_bfs_root2) * 10);
		if (get<0>(temp_bfs_root1) == player.y && get<1>(temp_bfs_root1) == player.x) Gameover = true;
		if (get<0>(temp_bfs_root2) == player.y && get<1>(temp_bfs_root2) == player.x) Gameover = true;
	}
}

void ofApp::coindraw() {
	//draw coin
	ofSetColor(0, 0, 255);
	ofDrawCircle(coin.x * 10, coin.y * 10, 5);
}

//maze generater by ellor's algorithm
void ofApp::make_map() {
	//random val
	random_device rand;
	//default_random_engine engine(rd());
	default_random_engine engine(rand());
	uniform_int_distribution<int> distribution(0, 1);

	int b[20][20]; //block set ID

	//initialize set ID
	int init = 1;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			b[i][j] = init++;
		}
	}

	//initialize maze
	for (int i = 0; i < 20 * 2 + 1; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < 20 * 2 + 1; j++) {
				if (j % 2 == 0) {
					maze_input[i][j] = '+';
				}
				else {
					maze_input[i][j] = '-';
				}
			}
		}
		else {
			for (int j = 0; j < 20 * 2 + 1; j++) {
				if (j % 2 == 0) {
					maze_input[i][j] = '|';
				}
				else {
					maze_input[i][j] = ' ';
				}
			}
		}
	}

	//until last row
	for (int i = 0; i < 19; i++) {
		//right wall
		for (int j = 0; j < 20 - 1; j++) {
			int rnd = distribution(engine);
			if (rnd == 1 && b[i][j + 1] != b[i][j]) {
				int temp = b[i][j + 1];
				maze_input[i * 2 + 1][(j + 1) * 2] = ' ';
				for (int k = 0; k < 20; k++) {
					if (b[i][k] == temp) {
						b[i][k] = b[i][j];
					}
				}
			}
		}

		//down wall
		vector<int> del_flag;//한번이라도 지운적 있으면 여기다가 id 저장함 
		for (int j = 0; j < 20; j++) {
			int cnt = 1;//같은집합의 원소 개수 카운트 
			for (int k = j + 1; k < 20; k++) {
				if (b[i][j] == b[i][k]) {//같은거 있는지 확인 
					cnt++;
					break;//같은게 존재하면 걍 나옴 
				}
			}

			if (cnt == 1) {//원소가 하나인 집합 or 하나만 남은 집합 
				//그래서 벡터뒤져서 지운적 있는지 확인 -> 만약에 있으면 굳이 얘 안지워도됨 
				if (find(del_flag.begin(), del_flag.end(), b[i][j]) == del_flag.end()) {
					//이 경우는 지운적 없는 경우에 무조건 지워주는 거 
					b[i + 1][j] = b[i][j];
					maze_input[(i + 1) * 2][(j) * 2 + 1] = ' ';
					del_flag.push_back(b[i][j]);
					continue;
				}
			}

			//하나만 남거나 하나인 집합이 아니면 그냥 랜덤으로 지울지 말지 결정			
			int rnd = distribution(engine);
			if (rnd == 1) {
				b[i + 1][j] = b[i][j];
				maze_input[(i + 1) * 2][(j) * 2 + 1] = ' ';
				del_flag.push_back(b[i][j]);//지우면 지운적 있다고 저장 
			}
		}
		del_flag.clear();
	}

	//last row
	for (int j = 0; j < 20 - 1; j++) {
		if (b[19][j + 1] != b[19][j]) {
			int temp = b[19][j + 1];
			maze_input[39][(j + 1) * 2] = ' ';
			for (int k = 0; k < 20; k++) {
				if (b[19][k] == temp) {
					b[19][k] = b[19][j];
				}
			}
		}
	}

	//for easy understanding
	maze_input[39][40] = ' ';

	//랜덤엔진 생성-아이템 생성에 활용
	uniform_int_distribution<int> distX(3, maze_col - 1);
	uniform_int_distribution<int> distY(3, maze_row - 1);
	coin.x = distX(rand);
	coin.y = distY(rand);
	if (coin.x % 2 == 0) coin.x--;
	if (coin.y % 2 == 0) coin.y--;

	isOpen = 1;//maze가 생성되었음을 알림
}