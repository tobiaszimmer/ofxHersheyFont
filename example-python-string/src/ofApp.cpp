/*
*  Created by Tobias Zimmer, August 2016.
*  www.tobiaszimmer.net
*
*  Font originally developed by Dr. Allen V. Hershey in 1967.
*  Font vectors made available by Paul Bourke.
*  paulbourke.net/dataformats/hershey/
*
*/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(ofColor::pink);

    hersheyFont.loadSVGFont(ofToDataPath("../../../svg-fonts/fonts/Hershey/HersheySans1.svg",true));
    //set color of the font (default is black)
	hersheyFont.setColor(ofColor(0));
    
    myString = "Sphink of black quartz, judge my vow.";
    pythonString = hersheyFont.getPath_asPythonString(myString, "ad.", 0, 0, 0.03);
    
    cout<<"pythonString:"<<endl;
    cout<<pythonString<<endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    ofSetColor(0);
    int tempY = 30;
    ofPath p = hersheyFont.getPath(myString, 0.03);
    p.draw(200,tempY+=60);
    ofDrawBitmapString(hersheyFont.fontName+" as path", 5, tempY);

    ofDrawBitmapString(pythonString, ofGetWidth()/2, 10);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'p'){
        string python_str = "";
        
        Py_Initialize();
        
        python_str = "";
        python_str += "import time\n";
        python_str += "from pyaxidraw import axidraw\n";
        python_str += "ad = axidraw.AxiDraw()\n";
        
        python_str += "ad.interactive()\n";
        python_str += "ad.options.model = "+ofToString(2)+"\n"; //AxiDraw SE/A3 https://axidraw.com/doc/py_api/#model 
        python_str += "ad.options.speed_pendown = "+ofToString(90)+"\n";// Integers from 1 to 110. speed limit for the XY carriage when the pen is down
        python_str += "ad.options.speed_penup = "+ofToString(90)+"\n"; //speed limit for the XY carriage when the pen is up
        
        python_str += "ad.connect()\n";
        
        // Change some options, just to show how we do so:
        python_str += "ad.options.units = 1\n"; // 0 inch, 1 cm, 2 mm
        python_str += "ad.options.pen_pos_down = "+ofToString(90)+"\n";
        python_str += "ad.options.pen_pos_up = "+ofToString(90)+"\n";
        python_str += "ad.update()\n";                 // Process changes to options 
        
        python_str += pythonString;
        
        python_str += "ad.moveto(0,0)\n"; //Return home
        
        python_str += "ad.disconnect()\n";
        
        PyRun_SimpleString(python_str.c_str());
        Py_Finalize();
    }
}
