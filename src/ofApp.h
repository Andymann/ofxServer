#pragma once

#include "ofMain.h"
#include "pixelServer.h"
#include "ofxImageSequencePlayback.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxNetworkUtils.h"
#include "ofxOsc.h"



class ofApp : public ofBaseApp{
    
    

	public:
    
    //Poco::Net::HostEntry thisHost;
    //std::string nodeName;
    //Poco::Net::IPAddress publicIp;
    //Poco::Net::NetworkInterface::List siteLocalInterfaces;
    
		void setup();
		void update();
		void draw();
    
    

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    void processTexture(ofTexture pTexture, int pCompressionType);
    
    
    
    pixelServer server;
    ofxImageSequencePlayback mySequence;
    
    ofImage testImg;
    int iCounter = 0;
    
    
    ofVideoGrabber vidGrabber;
    ofPixels videoPixels;
    ofTexture videoTexture;
    int camWidth;
    int camHeight;
		
    ofTrueTypeFont font;
    ofxDatGui* gui;
    //ofxDatGuiDropdown* cmbNetwork;
    //ofxDatGuiButton* btnClear;
    ofxDatGuiSlider* sldMessageSize;
    ofxDatGuiLabel* lblStatus;
    ofxDatGuiLabel* lblInfo;
    
    ofxXmlSettings xmlSettings;
    void readSettings();
    void saveSettings();
    void setupGui();
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 12;
        //font.file = "path/to/font.ttf";
        init();
    }
};
