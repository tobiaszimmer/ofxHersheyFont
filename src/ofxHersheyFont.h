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
    
    void draw(string stringValue, float x, float y, float scale){
        draw(stringValue, x, y, scale, false, 0);
    }
    void draw(string stringValue, float x, float y, float scale, bool centered){
        draw(stringValue, x, y, scale, centered, 0);
    }
    void draw(string stringValue, float x, float y, float scale, bool centered, float angle);
    void setColor(ofColor c);
    float getWidth(string stringValue, float scale);
    float getHeight(float scale);
    ofPath getPath(string stringValue, float scale, float yFlip = -1){
        return getPath(stringValue, 0, 0, scale, yFlip);
    };
    ofPath getPath(string stringValue, float xPos, float yPos, float scale, float yFlip = -1);
    
  
    filesystem::path fontPath;
    string fontName;
    
    ofColor color;
    
    //stephan
    string getPath_asPythonString(string stringValue, string prefix, float xPos, float yPos, float scale);
    string getPath_AsciiAsPythonString(string stringValue, string prefix, float xPos, float yPos, float scale);
    
    ofXml svgFontFile;
    void loadSVGFont(filesystem::path _fontPath);

private:
    void drawChar(ofXml xmlElement);
};


