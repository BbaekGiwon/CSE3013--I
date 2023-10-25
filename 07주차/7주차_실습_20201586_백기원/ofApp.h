#pragma once

#include "ofMain.h"
#include "water_line.h"
#include <cstdlib>

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    // waterfall-related member function
    void resetWater();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // Line segment and Dot member variables
    LineSegment* lineseg;    
    Dot* dot;

    // File Input member function
    void processOpenFileSelection(ofFileDialogResult openFileResult);


    int currentIndex;
    int num_of_line;
    int num_of_dot;
    int draw_flag;
    int load_flag;
    int waterfall_start_flag;
    int selection_dot;

    string originalFileExtension;

    ofPoint target_dot;
    float water_radius;
    float dot_diameter;

    vector <WaterLine> wl;


};

