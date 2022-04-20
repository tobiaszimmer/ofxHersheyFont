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

//    hersheyFont.loadSVGFont(ofToDataPath("svg-fonts-master/fonts/Hershey/HersheySansMed.svg"));
//    hersheyFont.loadSVGFont(ofToDataPath("svg-fonts-master/fonts/EMS/EMSSwiss.svg"));

    fontDict.listDir(ofToDataPath("../../../svg-fonts/fonts/Hershey",true));
//    fontDict.listDir(ofToDataPath("../../../svg-fonts/fonts/EMS",true));
    //set color of the font (default is black)
	hersheyFont.setColor(ofColor(0));
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    ofSetColor(0);
    string myString = "Sphink of black quartz, judge my vow.";
    int tempY = 5;
    //draw(string stringValue, float xPos, float yPos, float scale, bool centered, float angle)
    for(int i=0; i<fontDict.size();i++){
        hersheyFont.loadSVGFont(fontDict.getPath(i));
        hersheyFont.draw(myString, 200, tempY+=30, .03, false);
        ofDrawBitmapString(hersheyFont.fontName, 5, tempY);
    }
    
   
    //getPath
    //if you need more flexibility, you can get the font vectors as ofPath
    
    hersheyFont.loadSVGFont(ofToDataPath("../../../svg-fonts/fonts/Hershey/HersheySans1.svg",true));
//    hersheyFont.loadSVGFont(ofToDataPath("svg-fonts-master/fonts/EMS/EMSMistyNight.svg"));
    ofPath p = hersheyFont.getPath(myString, 0.03);
    p.draw(200,tempY+=60);
    ofDrawBitmapString(hersheyFont.fontName+" as path", 5, tempY);
    
    ofLog()<<"hersheyFont height "<<hersheyFont.getHeight(0.03);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
