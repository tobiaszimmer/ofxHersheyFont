/*
*  Created by Tobias Zimmer,  August 2016.
*  www.tobiaszimmer.net
*
*  Font originally developed by Dr. Allen V. Hershey in 1967.
*  Font vectors made available by Paul Bourke.
*  paulbourke.net/dataformats/hershey/
*
*/

#pragma once

#include "ofMain.h"
#include "ofxHersheyFont.h"

#include <Python/Python.h>

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyReleased(int key);
    
    string myString;
    ofxHersheyFont hersheyFont;
    string pythonString;
};
