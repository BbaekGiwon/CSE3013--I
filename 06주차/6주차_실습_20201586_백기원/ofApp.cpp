#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255, 255, 255);
    ofSetLineWidth(4);

    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(127, 23, 31);  // Set the drawing color to brown

    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);


    ofSetLineWidth(5);
    if (draw_flag) {


        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        for (int i = 0; i < num_of_line; i++) {
            mydata mm = q_line[i];
            ofDrawLine(mm.x1, mm.y1, mm.x2, mm.y2);
        }


        ofSetColor(0);
        for (int i = 0; i < num_of_dot; i++) {
            ofDrawCircle(q_dot[i].first, q_dot[i].second, 10);
        }

        ofSetColor(255, 0, 0);
        ofDrawCircle(q_dot[index].first, q_dot[index].second, 10);




        // 2nd week portion.
        ofSetLineWidth(2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    if (key == 'q') {
        // Reset flags
        draw_flag = 0;

        // Free the dynamically allocated memory exits.
        q_line.clear();
        q_line.shrink_to_fit();
        q_dot.clear();
        q_dot.shrink_to_fit();

        cout << "Dynamically allocated memory has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;

        draw_flag = 1;
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
    }
    if (key == 's') {
        // 2nd week portion.
    }
    if (key == 'e') {
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 'l') {
        // Open the Open File Dialog
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

        // Check whether the user opened a file
        if (openFileResult.bSuccess) {
            ofLogVerbose("User selected a file");

            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }

    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */

    if (key == OF_KEY_RIGHT) {
        index++;
        if (index == num_of_dot) index = 0;
        cout << "Selcted Dot Coordinate is (" << q_dot[index].first << ", " << q_dot[index].second << ")" << endl;
    }
    if (key == OF_KEY_LEFT) {
        index--;
        if (index < 0) index = num_of_dot - 1;
        cout << "Selcted Dot Coordinate is (" << q_dot[index].first << ", " << q_dot[index].second << ")" << endl;

    }
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
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    //Path to the comma delimited file
    //string fileName = "input.txt";

    string file_Path = openFileResult.getPath();
    ofFile file(file_Path);
    if (!file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;

    ofBuffer buffer(file);

    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0, pass_the_line = 0;


    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */



    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

        // Split line into strings
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) {
            if (pass_the_line != 0) {
                input_type = 1;
            }

            if (input_type == 0) { // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                pass_the_line++;
            }
            else { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
            }
        }
        else if (words.size() >= 2) {
            int x1, y1, x2, y2;
            mydata mm;
            if (input_type == 0) { // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                if (x1 < 0 || x1>1024 || x2 < 0 || x2>1024) {
                    num_of_line--;
                    continue;
                }
                if (y1 < 40 || y1>728 || y2 < 40 || y2>728) {
                    num_of_line--;
                    continue;
                }
                mm = { x1,y1,x2,y2 };
                q_line.push_back(mm);
            }
            else { // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                if (x1 < 0 || x1>1024 || y1 < 40 || y1>728) {
                    num_of_dot--;
                    continue;
                }
                q_dot.push_back({ x1, y1 });
            }




            //좌표저장
        } // End of else if.
        //라인 다 입력 받은 후 점 받을 준비
    } // End of for-loop (Read file line by line).

    sort(q_dot.begin(), q_dot.end());

    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    ;
}