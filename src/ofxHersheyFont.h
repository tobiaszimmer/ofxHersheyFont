/*
 *  ofxHersheyFont.h
 *
 *  Created by Tobias Zimmer, August 2016.
 *  www.tobiaszimmer.net
 *
 *  Font originally developed by Dr. Allen V. Hershey in 1967.
 *  Font vectors made available by Paul Bourke.
 *  paulbourke.net/dataformats/hershey/
 *
 *  A simple single line font for machines like CNC, Lasercutter, ...
 *  Available characters: ASCII codes 32 to 126.
 *
 */

#include "ofMain.h"

class ofxHersheyFont {
public:
    ofxHersheyFont();
    
    void draw(string stringValue, float x, float y, float scale);
    void draw(string stringValue, float x, float y, float scale, bool centered);
    void draw(string stringValue, float x, float y, float scale, bool centered, float angle);
    void setColor(ofColor c);
    float getWidth(string stringValue, float scale);
    float getHeight(float scale);
    ofPath getPath(string stringValue, float scale);
    ofPath getPath(string stringValue, float xPos, float yPos, float scale);
    
    ofPath getPathFromAscii(int asciiValue, float scale);
    ofPath getPathFromXML(ofXml xmlElement, float scale);
    
    ofColor color;
    
    //stephan
    string getPath_asPythonString(string stringValue, string prefix, float xPos, float yPos, float scale);
    
    ofXml svgFontFile;
    void loadSVGFont(string _fontPath);
    bool useSVGfont = false;
private:
    void drawChar(int asciiValue);
    void drawSVGchar(ofXml xmlElement);
};


