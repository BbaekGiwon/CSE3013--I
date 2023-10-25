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
//#include <deque>
using namespace std;

vector<string> maze_input;
vector<pair<int, int>> dfs_root;//answer dfs_root
vector<pair<int, int>> trial_dfs_root;//all trail


//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
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
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

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
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			if (!isdfs) {
				isdfs=DFS();
				bShowInfo = bChecked;
			}
			
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	//cout << maze_input.size();

	for (int i = 0; i < maze_input.size(); i++) {
		for (int j = 0; j < maze_input[0].size(); j++) {
			if (maze_input[i][j] == '-') {
				ofDrawLine((j - 1) * 10, i * 10, (j + 1) * 10, i * 10);
			}
			else if (maze_input[i][j] == '|') {
				ofDrawLine(j * 10, (i - 1) * 10, j * 10, (i + 1) * 10);
			}
		}
	}


	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			//cout << "we can draw!\n";
			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
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

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	if (key == 'd') {
		if (isOpen)
		{
			if (!isdfs) {
				isdfs = DFS();
			}

		}
		else
			cout << "you must open file first" << endl;

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
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << "\n";
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			freeMemory();

			for (auto line : buffer.getLines()) {
				maze_input.push_back(line);
			}

			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수

	maze_input.clear();
	maze_input.shrink_to_fit();
	dfs_root.clear();
	dfs_root.shrink_to_fit();
	trial_dfs_root.clear();
	trial_dfs_root.shrink_to_fit();
	isdfs = false;
}


bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	
	//initialize visited array
	int** visited;
	visited = new int*[maze_input.size()];
	for (int i = 0; i < maze_input.size(); i++) {
		visited[i] = new int[maze_input[0].size()];
	}
	for (int i = 0; i < maze_input.size(); i++) {
		for (int j = 0; j < maze_input[0].size(); j++) {
			visited[i][j] = 0;
		}
	}

	//four directions
	int x[4] = { 2,0,-2,0 }, y[4] = { 0,2,0,-2 };
	//present coordinate
	int loc_x = 1, loc_y = 1;

	dfs_root.push_back(make_pair(loc_y, loc_x));
	trial_dfs_root.push_back(make_pair(loc_y, loc_x));
	visited[1][1] = 1;

	while (dfs_root.size() > 0) {
		int b_flag = 0;
		loc_y = dfs_root[dfs_root.size() - 1].first;
		loc_x = dfs_root[dfs_root.size() - 1].second;
		trial_dfs_root.push_back(make_pair(loc_y, loc_x));

		//if it come to end, return
		if (loc_y == maze_input.size() - 2 && loc_x == maze_input[0].size() - 2) {
			/*for debug
			for (int i = 0; i < dfs_root.size(); i++) {
				cout << dfs_root[i].first << ", " << dfs_root[i].second << "\n";
			}
			*/
			for (int i = 0; i < maze_input.size(); i++) {
				delete[] visited[i];
			}
			delete[] visited;

			return true;
		}
		
		//search four directions
		for (int i = 0; i < 4; i++) {
			if (loc_y + y[i] >= 0 && loc_y + y[i] < maze_input.size() && loc_x + x[i] >= 0 && loc_x + x[i] < maze_input[0].size()){
				if (visited[loc_y + y[i]][loc_x + x[i]] == 0&&maze_input[loc_y + y[i]/2][loc_x + x[i]/2]!='-' && maze_input[loc_y + y[i] / 2][loc_x + x[i] / 2] != '|') {
					visited[loc_y+y[i]][loc_x+x[i]] = 1;
					dfs_root.push_back(make_pair(loc_y+y[i], loc_x+x[i]));
					b_flag = 1;
					break;
				}
			}
		}
		//if it can move, continue
		if (b_flag == 1) continue;
		//if it can't move, go back
		dfs_root.pop_back();

	}
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	
	ofSetLineWidth(5);
	ofSetColor(256, 0, 0);
	//first draw every trial with red
	for (int i = 0; i < trial_dfs_root.size() - 1; i++) {
		ofDrawLine(trial_dfs_root[i].second * 10, trial_dfs_root[i].first * 10, trial_dfs_root[i + 1].second * 10, trial_dfs_root[i + 1].first * 10);
	}
	ofSetColor(0, 256, 256);
	//then draw answer dfs_root over every trail
	for (int i = 0; i < dfs_root.size() - 1; i++) {
		ofDrawLine(dfs_root[i].second * 10, dfs_root[i].first * 10, dfs_root[i+1].second * 10, dfs_root[i+1].first * 10);
		//cout << dfs_root[i].second * 10 << " " << dfs_root[i].first * 10 << " " << dfs_root[i + 1].second * 10 << " " << dfs_root[i + 1].first * 10 << "\n";
		//cout << dfs_root.size() << "\n";
	}
	
}