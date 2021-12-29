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

//--------------------------------------------------------------
ofxHersheyFont::ofxHersheyFont(){
    color = ofColor(0);
}

void ofxHersheyFont::loadSVGFont(filesystem::path _fontPath){
 
    fontPath = _fontPath;
    fontName = fontPath.stem().string();
    
    //    Poco::Timestamp fileTime = Poco::File(file.string()).getLastModified();
    //    ofLog()<<file.filename().string()<<" : "<<print_pretty_datetime(fileTime);
    //        ofLog()<<"isDirectory "<<Poco::File(file.string()).isDirectory();
    //        ofLog()<<"root_name "<<file.root_name().string();
    //        ofLog()<<"stem "<<file.stem().string();
    //        ofLog()<<"extension "<<file.extension().string();
    
    if( svgFontFile.load(fontPath) ){
        ofLog()<<"loaded "<<fontPath;
    }else{
        ofLog()<<"unable to load "<<fontPath<<" check data/ folder";
    }
    
    
    /*
     //examples on how to read/parse SVG/XML data
     auto svgXml = svgFontFile.find("svg");
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
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float scale, bool centered, float angle) {
    
    float characterXPos = 0;
    float center = 0;
    if (centered) center = getWidth(stringValue, scale) / 2;
    
    ofPushMatrix();
    ofTranslate(xPos, yPos);
    ofRotateDeg(angle);
    ofTranslate(-center, 0);
    
    ofPath charPath = getPath(stringValue,scale);
    charPath.draw();
   
    ofPopMatrix();
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
        string nextChar = ofToString(stringValue.at(i));
        string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        
        if(svgFontFile.findFirst(elementPath) == 0 ){
            ofLog()<<"char "<<nextChar<<" not in svg replace with question mark";
            nextChar = "?";
            elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        }
        
        ofXml xmlElement = svgFontFile.findFirst(elementPath);

        float charWidth = ofToFloat(xmlElement.getAttribute("horiz-adv-x").getValue());
        stringWidth += charWidth * scale;
    }
    
    return stringWidth;
}

//--------------------------------------------------------------
float ofxHersheyFont::getHeight(float scale) {
    //the height of a capital letter is 21px (scale 1)
    
    
    ofPath charPath = getPath("I",scale);
    ofLog()<<"charPath.getOutline() "<<charPath.getOutline().size();
    ofPolyline charPolyline;
    for(auto & aLine : charPath.getOutline()){
        charPolyline.addVertices(aLine.getVertices());
    }
    float stringHeight = charPolyline.getBoundingBox().getHeight();
    
    return stringHeight;
}

//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float x, float y, float scale, int yFlip){

    ofPath charPath;
    
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
    
        string nextChar = ofToString(stringValue.at(i));
        string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        
        if(svgFontFile.findFirst(elementPath) == 0 ){
            ofLog()<<"char "<<nextChar<<" not in svg replace with question mark";
            nextChar = "?";
            elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        }
        
        ofXml xmlElement = svgFontFile.findFirst(elementPath);
        
        float charWidth = ofToFloat(xmlElement.getAttribute("horiz-adv-x").getValue());
        
        vector<string> splitGlyphPath = ofSplitString(xmlElement.getAttribute("d").getValue(), " ");//glyph path data in SVG looks like this: "M 139 -9.45 L 230 18.9 L 299 22.1 L 227 25.2"
        
        for(int i=0; i<splitGlyphPath.size(); i+=3){
            if(splitGlyphPath[i] == "M"){
                charPath.moveTo(x+ofToFloat(splitGlyphPath[i+1])* scale, y+ yFlip * ofToFloat(splitGlyphPath[i+2])* scale);
            }else if(splitGlyphPath[i] == "L"){
                charPath.lineTo(x+ofToFloat(splitGlyphPath[i+1])* scale, y+ yFlip * ofToFloat(splitGlyphPath[i+2])* scale);
            }
        }
        
        x += (charWidth*scale);
    }
    //move to first coordinate
//    chPath.moveTo(simplex[asciiValue - 32][2], simplex[asciiValue - 32][3]);

    charPath.setStrokeColor(color);
    charPath.setStrokeWidth(1);
    charPath.setFilled(false);
    
    return charPath;
}

//--------------------------------------------------------------
string ofxHersheyFont::getPath_asPythonString(string stringValue, string prefix, float x, float y, float scale, int yFlip) {
    string str_path = "";

    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
    
        string nextChar = ofToString(stringValue.at(i));
        string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        
        if(svgFontFile.findFirst(elementPath) == 0 ){
            ofLog()<<"char "<<nextChar<<" not in svg replace with question mark";
            nextChar = "?";
            elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        }
        
        ofXml xmlElement = svgFontFile.findFirst(elementPath);
        
        float charWidth = ofToFloat(xmlElement.getAttribute("horiz-adv-x").getValue());
        
        vector<string> splitGlyphPath = ofSplitString(xmlElement.getAttribute("d").getValue(), " ");//glyph path data in SVG looks like this: "M 139 -9.45 L 230 18.9 L 299 22.1 L 227 25.2"
        
        for(int i=0; i<splitGlyphPath.size(); i+=3){
            str_path += prefix;
            if(splitGlyphPath[i] == "M"){
                str_path += "moveto(";
                str_path += ofToString(x+ofToFloat(splitGlyphPath[i+1])* scale,2);
                str_path += ",";
                str_path += ofToString(y+ yFlip * ofToFloat(splitGlyphPath[i+2])* scale,2);
                str_path += ")\n";
            }else if(splitGlyphPath[i] == "L"){       
                str_path += "lineto(";
                str_path += ofToString(x+ofToFloat(splitGlyphPath[i+1])* scale,2);
                str_path += ",";
                str_path += ofToString(y+ yFlip * ofToFloat(splitGlyphPath[i+2])* scale,2);
                str_path += ")\n";
            }
        }
        
        x += (charWidth*scale);
    }
    
    return str_path;
}

