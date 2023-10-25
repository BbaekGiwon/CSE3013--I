#include "ofApp.h"
#include <algorithm>

bool dot_compare(const Dot& a, const Dot& b) {
    return a.x1 < b.x1;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(15); // Limit the speed of our program to 60 frames per second

    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255, 255, 255);
    ofSetLineWidth(4);

    currentIndex = 0;
    draw_flag = 0;
    load_flag = 0;
    waterfall_start_flag = 0;
    selection_dot = 0;
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
    if (draw_flag) {
        int i;
        for (i = 0; i < num_of_line; i++) {
            ofDrawLine(lineseg[i].x1, lineseg[i].y1, lineseg[i].x2, lineseg[i].y2);
        }
        for (i = 0; i < num_of_dot; i++) {
            if (selection_dot == i)
                ofSetColor(255, 0, 0);
            else
                ofSetColor(0);

            ofDrawCircle(dot[i].x1, dot[i].y1, dot_diameter / 2);
        }
    }
    ofSetLineWidth(2);
    
    if (waterfall_start_flag) {
        for (unsigned int i = 0; i < wl.size(); i++) {
            if (wl[i].path == NULL) continue;
            if (wl[i].calc_complete == 0)
                wl[i].calculate_path(lineseg, num_of_line);
            wl[i].draw();
        }
    }
    
}

//--------------------------------------------------------------x
void ofApp::keyPressed(int key) {
    
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    
    if (key == 'q') {
        if (!load_flag) return;

        // Reset flags
        draw_flag = 0;
        waterfall_start_flag = 0;

        // Free the Dynamic_allocation entities.
        if (lineseg != NULL) {
            free(lineseg);
            lineseg = NULL;
        }
        cout << "Memory for line segement has been freed." << endl;
        if (dot != NULL) {
            free(dot);
            dot = NULL;
        }
        cout << "Memory for dot has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;
        draw_flag = 1;
        target_dot.x = dot[selection_dot].x1;
        target_dot.y = dot[selection_dot].y1;
    }
    
    if (key == 's') {
        if (load_flag) {
            resetWater();
            for (unsigned int i = 0; i < wl.size(); i++) {
                if (wl[i].calc_complete == 0)
                    wl[i].calculate_path(lineseg, num_of_line);
            }
            if (draw_flag) {
                waterfall_start_flag = 1;
            }
        }
    }
    if (key == 'e') {
        if (draw_flag)
            waterfall_start_flag = 0;
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

            //We have a file, check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    if (key == OF_KEY_RIGHT) {
        waterfall_start_flag = 0;
        selection_dot++;
        if (selection_dot == num_of_dot)
            selection_dot = 0;

        target_dot.x = dot[selection_dot].x1;
        target_dot.y = dot[selection_dot].y1;

        cout << "Selcted Dot Coordinate is (" << target_dot.x << ", " << target_dot.y << ")" << endl;
        resetWater();
    }
    if (key == OF_KEY_LEFT) {
        waterfall_start_flag = 0;
        selection_dot--;
        if (selection_dot < 0)
            selection_dot = num_of_dot - 1;

        target_dot.x = dot[selection_dot].x1;
        target_dot.y = dot[selection_dot].y1;

        cout << "Selcted Dot Coordinate is (" << target_dot.x << ", " << target_dot.y << ")" << endl;
        resetWater();
    }
}

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    //Path to the comma delimited file
    //string fileName = "input.txt";

    string fileName = openFileResult.getPath();

    ofFile file(fileName);

    if (!file.exists()) {
        cout << "Target file does not exists." << endl;
    }
    else {
        cout << "We found the target file." << endl;
    }

    ofBuffer buffer(file);

    // Input_flag is a variable for indication the type of input.
    // If input_flag is zero, then work of line input is progress.
    // If input_flag is one, then work of dot input is progress.
    int input_flag = 0;

    // Idx is a variable for index of array.
    int idx = 0;

    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

        //Split line into strings
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) {
            if (!input_flag) { // Input for number of line
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                if (!lineseg)
                    lineseg = (LineSegment*)malloc(sizeof(LineSegment) * num_of_line);
            }
            else { // Input for number of dot
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
                if (!dot)
                    dot = (Dot*)malloc(sizeof(Dot) * num_of_dot);
            }
        }
        else if (words.size() >= 2) {
            // Input for actual lines.
            if (!input_flag) {
                lineseg[idx].x1 = atoi(words[0].c_str());
                lineseg[idx].y1 = atoi(words[1].c_str());
                lineseg[idx].x2 = atoi(words[2].c_str());
                lineseg[idx].y2 = atoi(words[3].c_str());

                // Out-of-range error handling
                if (lineseg[idx].x1 < 0 || lineseg[idx].x1 > ofGetWidth()) {
                    cout << "Out-of-range: " << lineseg[idx].x1 << endl;
                    return;
                }
                else if (lineseg[idx].y1 < 0 || lineseg[idx].y1 > ofGetHeight()) {
                    cout << "Out-of-range: " << lineseg[idx].y1 << endl;
                    return;
                }
                else if (lineseg[idx].x2 < 0 || lineseg[idx].x2 > ofGetWidth()) {
                    cout << "Out-of-range: " << lineseg[idx].x2 << endl;
                    return;
                }
                else if (lineseg[idx].y2 < 0 || lineseg[idx].y2 > ofGetHeight()) {
                    cout << "Out-of-range: " << lineseg[idx].y2 << endl;
                    return;
                }

                // Slope and constant computation
                double temp_constant = 0.0f;
                
                lineseg[idx].slope = (double)(lineseg[idx].y2 - lineseg[idx].y1) / (int)(lineseg[idx].x2 - lineseg[idx].x1);
                lineseg[idx].x_coef = (double)(lineseg[idx].y2 - lineseg[idx].y1);
                lineseg[idx].y_coef = (double)((lineseg[idx].x2 - lineseg[idx].x1) * (-1));
                temp_constant = lineseg[idx].slope * (-1) * lineseg[idx].x1 + lineseg[idx].y1;
                lineseg[idx].constant = (double)temp_constant * (lineseg[idx].x2 - lineseg[idx].x1);
                
                // Debug output
                //cout << "slope: " << lineseg[idx].slope << " " << "temp_constant: " << temp_constant << endl;
                //cout << "ax: " << lineseg[idx].x_coef << " " << "by: " << lineseg[idx].y_coef << " " << "c: " <<lineseg[idx].constant << endl;

                idx++;

                // Set the flag
                if (idx >= num_of_line) {
                    input_flag = 1;
                    idx = 0;
                }
            }
            else { // Input for actual dots.
                dot[idx].x1 = atoi(words[0].c_str());
                dot[idx].y1 = atoi(words[1].c_str());

                if (dot[idx].x1 < 0 || dot[idx].x1 > ofGetWidth()) {
                    cout << "Out-of-range: " << dot[idx].x1 << endl;
                    return;
                }
                else if (dot[idx].y1 < 0 || dot[idx].y1 > ofGetHeight()) {
                    cout << "Out-of-range: " << dot[idx].y1 << endl;
                    return;
                }

                // Debug output
                //cout << dot[idx].x1 << " " << dot[idx].y1 << " " << endl;

                idx++;

                // Unset the flag
                if (idx >= num_of_dot)
                    input_flag = 0;
            }
        }
    }

    sort(dot, dot+ num_of_dot, dot_compare);

    int num = 50;
    wl.assign(num, WaterLine(num_of_line));
    dot_diameter = 20.0f;
    for (unsigned int i = 0; i < wl.size(); i++) {
        //wl[i].start_dot.x = ofRandom(dot[selection_dot].x1-dot_diameter/2, dot[selection_dot].x1+dot_diameter/2);
        wl[i].start_dot.x = dot[selection_dot].x1;
        wl[i].start_dot.y = dot[selection_dot].y1;
        wl[i].dot_radius = dot_diameter / 2;
    }
    resetWater();
}
//--------------------------------------------------------------

void ofApp::resetWater() {
    for (unsigned int i = 0; i < wl.size(); i++) {
        //wl[i].start_dot.x = ofRandom(dot[selection_dot].x1-dot_diameter/2, dot[selection_dot].x1+dot_diameter/2);
        wl[i].start_dot.x = target_dot.x;
        wl[i].start_dot.y = target_dot.y;
        wl[i].reset();
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