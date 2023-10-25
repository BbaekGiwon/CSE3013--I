//
//  water_line.cpp
//  firstSketch
//
//  Created by 민동현 on 26/09/2019.
//

#include "water_line.h"


void WaterLine::draw() {
    if( calc_complete){
        ofSetLineWidth(5);
        
        ofSetColor(uniqueColor_r, uniqueColor_g, uniqueColor_b);
        for( int i=0 ; i<path_idx-1 ; i++){
            uniqueColor_r = ofRandom(0,100);
            uniqueColor_g = ofRandom(0,100);
            uniqueColor_b = ofRandom(185, 255);
            ofSetColor(uniqueColor_r, uniqueColor_g, uniqueColor_b);
            ofDrawLine(path[i].x1-1, path[i].y1-1, path[i+1].x1+1, path[i+1].y1+1);
        }
    }
    draw_complete = 1;
}
void WaterLine::update(){

}
void WaterLine::reset() {
    //the unique val allows us to set color for each water line.
    uniqueColor_r = ofRandom(0,100);
    uniqueColor_g = ofRandom(0,100);
    uniqueColor_b = ofRandom(185, 255);

    calc_complete = 0;
    draw_complete = 0;
    
    path_idx = 0;
}
void WaterLine::calculate_path(LineSegment *lineseg, int num_of_line) {
    path[path_idx].x1 = start_dot.x;
    path[path_idx].y1 = start_dot.y;
    path_idx++;
    
    for( ; start_dot.y <=ofGetHeight()-50 ; start_dot.y++){
        for( int i=0 ; i<num_of_line ; i++){
            // 1) Ignore line that located higher than water particle.
            if( start_dot.y >= lineseg[i].y1 && start_dot.y >= lineseg[i].y2 ) continue;
            // 2) Check whether the dot point exists between the each end of line segment.
            // If pos.x exists btw. y1 and y2 then, eventually hit the line segment.q
            if( lineseg[i].x1 < lineseg[i].x2){
                if( start_dot.x <= lineseg[i].x1 || start_dot.x >= lineseg[i].x2)
                    continue;
            }
            else if ( lineseg[i].x1 > lineseg[i].x2){
                if( start_dot.x <= lineseg[i].x2 || start_dot.x >= lineseg[i].x1)
                    continue;
            }

            // Slope 계산
            double temp_slope = (double)(start_dot.y - lineseg[i].y1)/(start_dot.x - lineseg[i].x1);
            // Dot exists in line
            
            if( abs(temp_slope - lineseg[i].slope) <= EPSILON){
                path[path_idx].x1 = start_dot.x;
                path[path_idx].y1 = start_dot.y+2;
                path_idx++;
                
                // Debug output
                //cout << "[" << i << "]" << "x: " << start_dot.x << " " << "y: " << start_dot.y << endl;
                if( lineseg[i].slope < 0){
                    path[path_idx-1].x1++;
                    start_dot.x = lineseg[i].x1;
                    start_dot.y = lineseg[i].y1-2;
                }
                else{
                    path[path_idx-1].x1--;
                    start_dot.x = lineseg[i].x2;
                    start_dot.y = lineseg[i].y2-2;
                }
                path[path_idx].x1 = start_dot.x;
                path[path_idx].y1 = start_dot.y;
                path_idx++;
                
                // Debug output
                //cout << "[" << i << "]" << "x: " << start_dot.x << " " << "y: " << start_dot.y << endl;
            }
        }
    }
    // Last path
    path[path_idx].x1 = start_dot.x;
    path[path_idx].y1 = start_dot.y;
    path_idx++;
    
    calc_complete = 1;
}

WaterLine::WaterLine(int num_of_line) {
    draw_complete = 0;
    calc_complete = 0;
    
    uniqueColor_r = ofRandom(0,255);
    uniqueColor_g = ofRandom(0,255);
    uniqueColor_b = ofRandom(185, 255);
    
    
    path_idx = 0;
    path = nullptr;
    if( !path)
        path = (Dot*)malloc(sizeof(Dot) * (2*num_of_line + 4));
}
WaterLine::~WaterLine() {
    //free(path);
}
