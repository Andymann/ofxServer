#include "ofApp.h"

const int FRAMERATE = 30.0f;
static int COMPRESSION_TURBOJPEG = 0x04;
static int COMPRESSION_JPEG = 0x00;
int iMessageSize;//0--255; will internally be multiplied by 1000
string sActiveNic;



static string sMessageSize = "MessageSize";
static string sNic = "Network Interface";

static string sStatus_Sending = "Status: Sending (hit <space> to toggle)";
static string sStatus_Quite = "Status: Not sending (hit <space> to toggle)";
static string sXmlFile = "settings.xml";

ofTexture tex;

bool bActive = false;
bool bToggle = false;

static std::vector<unsigned char> bufImg;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(FRAMERATE);
    ofSetVerticalSync(true);
    ofSetWindowTitle("Server");
    ofSetEscapeQuitsApp(false);
    font.load(OF_TTF_MONO, 23);
    readSettings();
    setupGui();
    
    ofBackground(20,20,20);
    server.setup(sActiveNic, 44999);
    
    bonjourPublisher.setup();
    bonjourPublisher.publish("_ofxbonjour_example._tcp", "ofxServer", 44999);

    
 /*
    mySequence.loadSequence("sequenceBig",FRAMERATE);
    mySequence.setShouldLoop(true);
    mySequence.play();
*/
    
    camWidth = 640;  // try to grab at this size.
    camHeight = 480;

    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(1);
    vidGrabber.setDesiredFrameRate(FRAMERATE);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    
    videoPixels.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
    videoTexture.allocate(videoPixels);
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );

    vector<string> optsNic;
    siteLocalInterfaces = ofxNet::NetworkUtils::listNetworkInterfaces(ofxNet::NetworkUtils::SITE_LOCAL);

    optsNic.push_back("127.0.0.1");
    for (const auto& interface: siteLocalInterfaces)
    {
        optsNic.push_back( interface.broadcastAddress().toString() );
        optsNic.push_back( interface.address().toString());
    }
    cmbNic = gui->addDropdown(sNic, optsNic);
    if (std::find(optsNic.begin(), optsNic.end(), sActiveNic) != optsNic.end()){
        cmbNic->setLabel(sActiveNic);
    }else{
        cmbNic->setLabel("127.0.0.1");
    }
    
    
    
    sldMessageSize = gui->addSlider(sMessageSize, 5, 255);
    sldMessageSize->setPrecision(0);
    sldMessageSize->setValue((float)iMessageSize);
    
    lblInfo = gui->addLabel("Info");
    
    //lblStatus = gui->addLabel(sStatus_Quite);
    
    
    gui->setTheme(new myCustomTheme() );
    gui->setWidth(640);
    
    // once the gui has been assembled, register callbacks to listen for component specific events
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
}

//--------------------------------------------------------------
void ofApp::readSettings(){
    if (xmlSettings.loadFile(sXmlFile)) {
        ofLogVerbose()<<"XML loaded"<<endl;
        //addLog("XML loaded");
        iMessageSize = xmlSettings.getValue("MessageSize", 200);
        sActiveNic = xmlSettings.getValue("Network", "127.0.0.1");
    }else{
        ofLogVerbose("Could not load xml. Reverting to default values.");
        //addLog("Could not load xml. Reverting to default values.");
    }
    
    
}

//--------------------------------------------------------------
void ofApp::saveSettings(){
    xmlSettings.setValue("MessageSize", iMessageSize);
    xmlSettings.setValue("Network",sActiveNic);
    xmlSettings.save(sXmlFile);
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    string sTarget = ofToString(e.target->getName());
    if(sTarget == sMessageSize){
        iMessageSize = e.target->getValue();
    }
}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    string sTarget = ofToString(e.target->getName());
    if(sTarget == sNic){
        //ofLogNotice(e.target->getLabel());
        bActive = false;
        sActiveNic = e.target->getLabel();
        server.setup(sActiveNic);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    server.update(91920);
    
  /*
    mySequence.update();
    tex = *mySequence.getCurrentTexture();
  */
 
    vidGrabber.update();

    if(vidGrabber.isFrameNew()){
        ofPixels & pixels = vidGrabber.getPixels();
        //if(pixels.isAllocated()){
            for(size_t i = 0; i < pixels.size(); i++){
                //invert the color of the pixel
                videoPixels[i] = pixels[i];
            }
            //load the inverted pixels
            tex.loadData(videoPixels);
        //}
    }

    
    
    processTexture( tex, COMPRESSION_JPEG );
   
    //if(iCounter<1){
    //    iCounter++;
    //}else{
    //    iCounter=0;
//        server.sendPixels( & bufImg );
    //}

    if (bActive){
        server.sendPixels( & bufImg );
        //ofLogNotice("Sending");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    /*
    mySequence.draw();
    */
    
    //if(server.getState()== SENDING){
      //  if(server.getPctSent()>50){
            vidGrabber.draw(20, 50);
      //  }
   //}
    
    
    string statusStr =  "status: " + server.getStateStr();
    statusStr += " -- sent "+ofToString(server.getPctSent(), 2)+"% of " + ofToString(server.getPixelsSize()) + " bytes.";
       
    lblInfo->setLabel(statusStr);
    
    ofSetColor(255, 255, 255);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' '){
        server.sendPixels( & bufImg );
        bActive = !bActive;
    }
             
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void ofApp::exit(){
    saveSettings();
}


void ofApp::processTexture(ofTexture pTexture, int pCompressionType){
    
    ofBuffer ofBuff;
    //ofImage image;
    ofPixels pixels;
    
    
    bufImg.resize(0);
    pTexture.readToPixels(pixels);

    //---Imagedata into Buffer;
    ofSaveImage(pixels,ofBuff,OF_IMAGE_FORMAT_JPEG);
    
    
    char * p = ofBuff.getBinaryBuffer();
    
    
    bufImg.resize(0);
    bufImg.push_back(pCompressionType);
    bufImg.push_back(0xAF); //"Signature"
    bufImg.push_back(0xAF);
    bufImg.push_back( iMessageSize );
    bufImg.push_back((int)pTexture.getWidth() & 0xff);
    bufImg.push_back((int)pTexture.getWidth() >> 8);
    bufImg.push_back(0);
    bufImg.push_back(0);
    bufImg.push_back((int)pTexture.getHeight() & 0xff);
    bufImg.push_back((int)pTexture.getHeight() >> 8);
    bufImg.push_back(0);
    bufImg.push_back(0);
    bufImg.push_back((int)(ofBuff.size()+16) & 0xff);
    bufImg.push_back((int)(ofBuff.size()+16) >> 8);
    bufImg.push_back((int)(ofBuff.size()+16) >> 16);
    bufImg.push_back((int)(ofBuff.size()+16) >> 24);
    
    int iImageSize = ofBuff.size() / sizeof(char);    //----Just the picture without header
    for (int i = 0; i < iImageSize; i++) {
        bufImg.push_back(p[i]);
    }

}
