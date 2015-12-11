//
//  ofxTextInputField.cpp
//  textInput
//
//  Created by Enrico Becker on 30.11.15.
//
//

#include "ofxSimpleTextInputField.h"

ofxTextInputField::ofxSimpleTextInputField(){}

ofxTextInputField::~ofxSimpleTextInputField(){}

void ofxSimpleTextInputField::setup(ofRectangle area, int fS, string t, string d)
{
    bounds = area;
    fSize = fS;
    text = t;
    description = d;
    font.loadFont("verdana.ttf", fSize);
    col = ofColor(255,255,255);
    float y = +((area.height - fSize) / 2) + fSize;
    tPos = ofVec2f(4,y);
    cIndex = text.size();
    calculateDrawIndices();
}

bool ofxSimpleTextInputField::isInside(int &x, int &y)
{
    if(bounds.inside(x, y))
    {
        activate();
        cIndex = calCPos(x);
        return true;
    }
    return false;
}

int ofxSimpleTextInputField::calCPos(int &x)
{
    int pos = x - bounds.x - 10;
    int index = 0;
    int px = 0;
    while (px < pos)
    {
        if (index >= text.size())     return text.size();
        px = font.getStringBoundingBox(text.substr(0,index), 0, 0).width;
        index++;
    }
    return index;
}


void ofxSimpleTextInputField::activate()
{
    active = true;
}

void ofxSimpleTextInputField::deactivate()
{
    active = false;
}

void ofxSimpleTextInputField::addKey(int key)
{
    if (active)
    {
        //add charachter
        if (key >= 32 && key <= 126)
        {
            text.insert(text.begin() + cIndex, key);
            cIndex++;
        }
        
        if (key == OF_KEY_RETURN)
        {
            deactivate();
            //text.insert(text.begin()+cPos, '\n');
            //cPos++;
        }
        
        if (key == OF_KEY_BACKSPACE)
        {
            if (cIndex > 0) {
                text.erase(text.begin() + cIndex - 1);
                --cIndex;
            }
        }
        
        if (key == OF_KEY_DEL)
        {
            if (text.size() > cIndex)
            {
                text.erase(text.begin() + cIndex);
            }
        }
        
        if (key == OF_KEY_LEFT)
            if (cIndex > 0)
                --cIndex;
        
        if (key == OF_KEY_RIGHT)
            if (cIndex < text.size())
                ++cIndex;
        
        calculateDrawIndices();
    }
}


void ofxSimpleTextInputField::calculateDrawIndices()
{
    //show the last characters
    firstDrawIndex = 0;
    for (int i = 0; i < text.size(); i++)
    {
        int l = font.getStringBoundingBox(text.substr(i,text.size()), 0, 0).width;
        if (l < bounds.width - 8)
        {
            firstDrawIndex = i;
            break;
        }
    }
    //if deactivated show from the beginning
    lastDrawIndex = text.size();
    for (int i = 0; i < text.size(); i++)
    {
        int l = font.getStringBoundingBox(text.substr(0,i), 0, 0).width;
        if (l > bounds.width - 8)
        {
            lastDrawIndex = i - 1;
            break;
        }
    }
}

void ofxSimpleTextInputField::setBounds(ofRectangle area)
{
    bounds = area;
    float y = +((area.height - fSize) / 2) + fSize;
    tPos = ofVec2f(2,y);
    calculateDrawIndices();
}

void ofxSimpleTextInputField::setText(string t)
{
    text = t;
    calculateDrawIndices();
}

void ofxSimpleTextInputField::setColor(ofColor c)
{
    col = c;
}

void ofxSimpleTextInputField::draw()
{
    ofSetLineWidth(1);
    ofSetColor(col);
    ofNoFill();
    ofRect(bounds);
    font.drawString(description, bounds.x + bounds.width + 5, bounds.y + tPos.y);
    //check if it is inside boundaries
    if(active) font.drawString(text.substr(firstDrawIndex,text.size()), bounds.x + tPos.x, bounds.y + tPos.y);
    else font.drawString(text.substr(0,lastDrawIndex), bounds.x + tPos.x, bounds.y + tPos.y);
    if (active) //draw cursor
    {
        ofSetLineWidth(2);
        float x1 = bounds.x + 6 + font.getStringBoundingBox(text.substr(firstDrawIndex,cIndex), 0, 0).width;
        float y1 = bounds.y + 3;
        float x2 = x1;
        float y2 = bounds.y + bounds.height - 3;
        float alpha = (sin(ofGetElapsedTimef() * 5) + 1) * 127;
        ofSetColor(col,alpha);
        ofLine(x1,y1,x2,y2);
    }
}

string ofxSimpleTextInputField::getText()
{
    return text;
}

bool &ofxSimpleTextInputField::getState()
{
    return active;
}
