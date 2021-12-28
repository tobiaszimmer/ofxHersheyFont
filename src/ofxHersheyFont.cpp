/*
 *  ofxHersheyFont.cpp
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

#include "ofxHersheyFont.h"
#include "simplexCharacterSet.h"


//--------------------------------------------------------------
ofxHersheyFont::ofxHersheyFont(){
    color = ofColor(0);
}

void ofxHersheyFont::loadSVGFont(string _fontPath){
    
    if( svgFontFile.load(_fontPath) ){
        ofLog()<<"loaded "<<_fontPath;
        useSVGfont = true;
    }else{
        ofLog()<<"unable to load "<<_fontPath<<" check data/ folder";
    }
    
    auto svgXml = svgFontFile.find("svg");
    /*
    ofLog()<<"svgXml ";
    for(auto & svgElement: svgXml){
        ofLog()<<svgElement;
        
        auto defsXml = svgElement.find("defs");
        ofLog()<<"defsXml ";
        for(auto & defElement: defsXml){
            ofLog()<<defElement;
            
            auto fontXml = defElement.find("font");
            ofLog()<<"fontXml ";
            for(auto & fontElement: fontXml){
                ofLog()<<fontElement;
                
                auto glyphXml = fontElement.find("glyph");
                ofLog()<<"glyphXml ";
                for(auto & glyphElement: glyphXml){
                    ofLog()<<glyphElement ; //.getValue();
                    cout<<"unicode "<<glyphElement.getAttribute("unicode").getValue()<<" "<<endl;
                    cout<<"glyph-name "<<glyphElement.getAttribute("glyph-name").getValue()<<" "<<endl;
                    cout<<"horiz-adv-x "<<glyphElement.getAttribute("horiz-adv-x").getValue()<<" "<<endl;
                    cout<<"d "<<glyphElement.getAttribute("d").getValue()<<" "<<endl;
                    characterMap[glyphElement.getAttribute("unicode").getValue()] = glyphElement.getAttribute("d").getValue();
                }
                
                //                ofLog()<<"fontElement.findFirst "<<fontElement.findFirst("glyph[@unicode='$']").getAttribute("glyph-name").getValue();
                
            }//end fontXml
            
        }//end defsXml
        
    }//end svgXml
    */
    
    //    ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']"); 
    //    ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']").getAttribute("glyph-name").getValue(); 
    
}
//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float scale) {
    draw(stringValue, xPos, yPos, scale, false, 0);
}

//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float scale, bool centered) {
    draw(stringValue, xPos, yPos, scale, centered, 0);
}

//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float scale, bool centered, float angle) {
    
    float characterXPos = 0;
    float center = 0;
    if (centered) center = getWidth(stringValue, scale) / 2;
    
    ofPushMatrix();
    ofTranslate(xPos, yPos);
    ofRotateDeg(angle);
    ofTranslate(-center, 0);
    
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        ofPushMatrix();
        ofTranslate(characterXPos, 0);
        ofScale(scale, -scale);
        
        float charWidth = 0;
        if(useSVGfont == false){
            //get ascii value of specific character from the input string
            int asciiValue = stringValue.at(i);
            
            //if character is not available, use questionmark
            if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
            
            //draw the character
            drawChar(asciiValue);
            
            //update xPos / starting position for the next character
            charWidth = simplex[asciiValue - 32][1];// * scale;
//            characterXPos += charWidth;
        }else{
            string nextChar = ofToString(stringValue.at(i));
            
            //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']"); 
            //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']").getAttribute("glyph-name").getValue(); 
            string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
           
//            ofLog()<<"svg character:"<<nextChar;
            
            if(svgFontFile.findFirst(elementPath) == 1 ){
                drawSVGchar(svgFontFile.findFirst(elementPath));
                charWidth = ofToFloat(svgFontFile.findFirst(elementPath).getAttribute("horiz-adv-x").getValue());
//                ofLog()<<"width "<<charWidth;
            }else{
                int asciiValue = 63;
                drawChar(asciiValue);
                //update xPos / starting position for the next character
                charWidth = simplex[asciiValue - 32][1] * scale;
                
                ofLog()<<"char not in svg replace with simplexCharacter ?";
            }
            //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']").getAttribute("glyph-name").getValue(); 
            
        }
        
        characterXPos += (charWidth * scale);
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofxHersheyFont::drawSVGchar(ofXml xmlElement) {
    ofLog()<<"drawChar() "<<xmlElement.getAttribute("glyph-name").getValue(); 
    
    string path_str = xmlElement.getAttribute("d").getValue();
    ofLog()<<"path_str "<<path_str;
    
    cout<<"horiz-adv-x "<<xmlElement.getAttribute("horiz-adv-x").getValue()<<" "<<endl;
    cout<<"d "<<xmlElement.getAttribute("d").getValue()<<" "<<endl;
   
    ofPath chPath;
    //move to first coordinate
//    chPath.moveTo(simplex[asciiValue - 32][2], simplex[asciiValue - 32][3]);
    vector<string> splitString = ofSplitString(xmlElement.getAttribute("d").getValue(), " ");
    
    for(int i=0; i<splitString.size(); i+=3){
        if(splitString[i] == "M"){
            chPath.moveTo(ofToFloat(splitString[i+1]), ofToFloat(splitString[i+2]));
        }else if(splitString[i] == "L"){
            chPath.lineTo(ofToFloat(splitString[i+1]), ofToFloat(splitString[i+2]));
        }
    }
    
    chPath.setStrokeColor(color);
    chPath.setStrokeWidth(1);
    chPath.setFilled(false);
    chPath.draw();
}
//--------------------------------------------------------------
void ofxHersheyFont::drawChar(int asciiValue) {
    
    ofPath chPath;
    
    //move to first coordinate
    chPath.moveTo(simplex[asciiValue - 32][2], simplex[asciiValue - 32][3]);
    
    //iterate through points of the character
    for (int i = 4; i <= simplex[asciiValue - 32][0] * 2; i += 2)
    {
        int x = simplex[asciiValue - 32][i];
        int y = simplex[asciiValue - 32][i + 1];
        
        if (x != -1) chPath.lineTo(x, y);
        
        //skip -1,-1 value -> equals pen up operation / end of a line
        //and move to next point
        if (x == -1) {
            chPath.moveTo(simplex[asciiValue - 32][i + 2], simplex[asciiValue - 32][i + 3]);
            i += 2;
        }
    }
    
    chPath.setStrokeColor(color);
    chPath.setStrokeWidth(1);
    chPath.setFilled(false);
    chPath.draw();
}


//--------------------------------------------------------------
void ofxHersheyFont::setColor(ofColor c) {
    color = c;
}

//--------------------------------------------------------------
float ofxHersheyFont::getWidth(string stringValue, float scale){
    float stringWidth = 0;
    
    for (int i = 0; i < stringValue.size(); i++)
    {
        int asciiValue = stringValue.at(i);
        
        //questionmark if character not available
        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63; 
        
        stringWidth += (float)simplex[asciiValue - 32][1] * scale;
    }
    
    return stringWidth;
}

//--------------------------------------------------------------
float ofxHersheyFont::getHeight(float scale) {
    //the height of a capital letter is 21px (scale 1)
    float stringHeight = (float) 21 * scale;
    
    return stringHeight;
}


//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float scale) {
    return getPath(stringValue, 0, 0, scale);
}

//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float xPos, float yPos, float scale) {
    
    ofPath path;
    
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        //get ascii value of specific character from the inout string
        int asciiValue = stringValue.at(i);
        
        //only draw character if vectors are available, otherwise draw questionmark
        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
        
        //moveto first coordinate of the character
        path.moveTo(xPos + simplex[asciiValue - 32][2] * scale, yPos + (-1) * simplex[asciiValue - 32][3] * scale);
        
        //iterate through points of each character
        for (int j = 4; j <= simplex[asciiValue - 32][0] * 2; j += 2)
        {
            int x = simplex[asciiValue - 32][j];
            int y = (-1) * simplex[asciiValue - 32][j + 1];
            
            if (x != -1) path.lineTo(xPos + x * scale, yPos + y * scale);
            
            if (x == -1) {
                path.moveTo(xPos + simplex[asciiValue - 32][j + 2] * scale, yPos + (-1) * simplex[asciiValue - 32][j + 3] * scale);
                j += 2;
            }
        }
        
        //at the end of each character, set xPos to starting coordinate of next character
        xPos += (float)simplex[asciiValue - 32][1] * scale;
    }
    
    path.setStrokeColor(color);
    path.setStrokeWidth(1);
    path.setFilled(false);
    
    return path;
}
