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
    ofLog()<<"center "<<center;
    ofPath charPath;
    charPath = getPath(stringValue,scale);
    charPath.draw();
    /*
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        ofPushMatrix();
        ofTranslate(characterXPos, 0);
        //        ofScale(scale, -scale);
        ofScale(1, -1);
        
        ofPath charPath;
        
        float charWidth = 0;
        
        string nextChar = ofToString(stringValue.at(i));
        
        //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']"); 
        //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']").getAttribute("glyph-name").getValue(); 
        string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
   
        if(svgFontFile.findFirst(elementPath) == 0 ){
            nextChar = "?";
            elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
            ofLog()<<"char not in svg replace with question mark";
        }
        //            ofLog()<<"svg character:"<<nextChar;
        
        charPath = getPath(nextChar,scale, 1); //svgFontFile.findFirst(elementPath),scale);
        charWidth = ofToFloat(svgFontFile.findFirst(elementPath).getAttribute("horiz-adv-x").getValue());
        
//        if(svgFontFile.findFirst(elementPath) == 1 ){
//            //                drawSVGchar(svgFontFile.findFirst(elementPath));
//            charPath = getPath(nextChar,scale); //svgFontFile.findFirst(elementPath),scale);
//            charWidth = ofToFloat(svgFontFile.findFirst(elementPath).getAttribute("horiz-adv-x").getValue());
//            //                ofLog()<<"width "<<charWidth;
//        }else{
//            //                int asciiValue = 63;
//            ////                drawChar(asciiValue);
//            //                charPath = getPathFromAscii(asciiValue,scale);
//            //                //update xPos / starting position for the next character
//            //                charWidth = simplex[asciiValue - 32][1] * scale;
//            
//            ofLog()<<"char not in svg replace with simplexCharacter ?";
//        }
        //            ofLog()<<"svgFontFile.findFirst "<<svgFontFile.findFirst("/svg/defs/font/glyph[@unicode='!']").getAttribute("glyph-name").getValue(); 
        
        
        
        charPath.draw();
        
        characterXPos += (charWidth * scale);
        ofPopMatrix();
    }
    */
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
//    ofLog()<<"getWidth() "<<stringWidth;
    return stringWidth;
}

//--------------------------------------------------------------
float ofxHersheyFont::getHeight(float scale) {
    //the height of a capital letter is 21px (scale 1)
    float stringHeight = (float) 21 * scale;
    
    return stringHeight;
}


////--------------------------------------------------------------
//ofPath ofxHersheyFont::getPath(string stringValue, float xPos, float yPos, float scale) {
//    
//    ofPath path;
//    
//    //iterate through each character of the input string
//    for (int i = 0; i < stringValue.size(); i++)
//    {
//        //get ascii value of specific character from the inout string
//        int asciiValue = stringValue.at(i);
//        
//        //only draw character if vectors are available, otherwise draw questionmark
//        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
//        
////        getPathFromAscii(asciiValue, scale);
//        //moveto first coordinate of the character
//        path.moveTo(xPos + simplex[asciiValue - 32][2] * scale, yPos + (-1) * simplex[asciiValue - 32][3] * scale);
//        
//        //iterate through points of each character
//        for (int j = 4; j <= simplex[asciiValue - 32][0] * 2; j += 2)
//        {
//            int x = simplex[asciiValue - 32][j];
//            int y = (-1) * simplex[asciiValue - 32][j + 1];
//            
//            if (x != -1) path.lineTo(xPos + x * scale, yPos + y * scale);
//            
//            if (x == -1) {
//                path.moveTo(xPos + simplex[asciiValue - 32][j + 2] * scale, yPos + (-1) * simplex[asciiValue - 32][j + 3] * scale);
//                j += 2;
//            }
//        }
//        
//        //at the end of each character, set xPos to starting coordinate of next character
//        xPos += (float)simplex[asciiValue - 32][1] * scale;
//    }
//    
//    path.setStrokeColor(color);
//    path.setStrokeWidth(1);
//    path.setFilled(false);
//    
//    return path;
//}

ofPath ofxHersheyFont::getPath(string stringValue, float x, float y, float scale, float yFlip){

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

////--------------------------------------------------------------
//string ofxHersheyFont::getPath_asPythonString(string stringValue, string prefix, float xPos, float yPos, float scale) {
//    string str_path = "";
//    
//    if(useSVGfont == false){
//        str_path = getPath_AsciiAsPythonString(stringValue,prefix,xPos,yPos,scale);
//    }else{
//        str_path = getPath_XmlAsPythonString(stringValue,prefix,xPos,yPos,scale);
//    }
//    
//    return str_path;
//}
//--------------------------------------------------------------
string ofxHersheyFont::getPath_asPythonString(string stringValue, string prefix, float xPos, float yPos, float scale) {
    string str_path = "";
    /*
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        string nextChar = ofToString(stringValue.at(i));
        string elementPath = "/svg/defs/font/glyph[@unicode='"+nextChar+"']";
        
        if(svgFontFile.findFirst(elementPath) == 1 ){
            //                drawSVGchar(svgFontFile.findFirst(elementPath));
            charPath = getPathFromXML(svgFontFile.findFirst(elementPath),scale);
            charWidth = ofToFloat(svgFontFile.findFirst(elementPath).getAttribute("horiz-adv-x").getValue());
            //                ofLog()<<"width "<<charWidth;
        }else{
            ofLog()<<"char not in svg, replace with simplexCharacter ?";
        }
        
    }
     */
    return str_path;
}

//--------------------------------------------------------------
string ofxHersheyFont::getPath_AsciiAsPythonString(string stringValue, string prefix, float xPos, float yPos, float scale) {

    string str_path = "";

    /*
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        //get ascii value of specific character from the inout string
        int asciiValue = stringValue.at(i);

        //only draw character if vectors are available, otherwise draw questionmark
        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;

        //moveto first coordinate of the character
//        path.moveTo(xPos + simplex[asciiValue - 32][2] * scale, yPos + (-1) * simplex[asciiValue - 32][3] * scale);
        str_path += prefix;
        str_path += "moveto(";
        str_path += ofToString(xPos + simplex[asciiValue - 32][2] * scale,2);
        str_path += ",";
        str_path += ofToString(yPos + (-1) * simplex[asciiValue - 32][3] * scale,2);
        str_path += ")\n";
        
        //iterate through points of each character
        for (int j = 4; j <= simplex[asciiValue - 32][0] * 2; j += 2)
        {
            int x = simplex[asciiValue - 32][j];
            int y = (-1) * simplex[asciiValue - 32][j + 1];

            if (x != -1){
//                path.lineTo(xPos + x * scale, yPos + y * scale);
                str_path += prefix;
                str_path += "lineto(";
                str_path += ofToString(xPos + x * scale,2);
                str_path += ",";
                str_path += ofToString(yPos + y * scale,2);
                str_path += ")\n";
            }
            
            if (x == -1) {
//                path.moveTo(xPos + simplex[asciiValue - 32][j + 2] * scale, yPos + (-1) * simplex[asciiValue - 32][j + 3] * scale);
                str_path += prefix;
                str_path += "moveto(";
                str_path += ofToString(xPos + simplex[asciiValue - 32][j + 2] * scale,2);
                str_path += ",";
                str_path += ofToString(yPos + (-1) * simplex[asciiValue - 32][j + 3] * scale,2);
                str_path += ")\n";
                j += 2;
            }
        }

        //at the end of each character, set xPos to starting coordinate of next character
        xPos += (float)simplex[asciiValue - 32][1] * scale;
    }
*/

    return str_path;
}
