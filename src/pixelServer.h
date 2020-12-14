//
//  pixelServer.h
//  ofxServer
//
//  Created by Andy Fischer on 11.12.20.
//

//#ifndef pixelServer_h
//#define pixelServer_h


//#endif /* pixelServer_h */
#include "ofxNetwork.h"
//#include "ofAddons.h"
#include "ofMain.h"

//#define NUM_BYTES_IN_IMG (750 * 450 * 3)

enum serverState{
    NONE, READY, SENDING, COMPLETE
};

class pixelServer{
    public:
    
    //-----------------------------------------------------
    pixelServer(){
        state        = NONE;
    }

    //-----------------------------------------------------
    int getState(){
        return state;
    }

    //-----------------------------------------------------
    string getStateStr(){
        if(state == NONE)return "NONE";
        else
        if(state == READY)return "READY";
        else
        if(state == SENDING)return "SENDING";
        else
        if(state == COMPLETE)return "COMPLETE";
        
        return "ERR";
    }
    
    //-----------------------------------------------------
    float getPctSent(){
        return ( (float)numSentBytes / pixels.size() ) * 100;
    }
    
    int getPixelsSize(){
        return pixels.size();
    }

    //-----------------------------------------------------
    void setup(string pAddress, int port = 11999){
       //setup the server to listen on 11999
        ofxTCPSettings settings = ofxTCPSettings(pAddress, port);
        tcpServer.setup(settings);
        state = READY;
    }

    //-----------------------------------------------------
    void sendPixels(vector<unsigned char> * pixelData) {
        if( state == NONE ) {
            return;
        }else if(state == READY){
            pixels.clear();
        }else if(state == SENDING){
            return;
        }else if (state == COMPLETE){
            pixels.clear();
        }
                
//        memcpy(pixels, pixelData, totalBytes);
        copy(pixelData->begin(), pixelData->end(), back_inserter(pixels));
        state            = SENDING;
        numSentBytes    = 0;
    }

    //-----------------------------------------------------
    void update(int numToSend = 1024){
        
        if( state == SENDING && numSentBytes < pixels.size() ){
            if( numToSend + numSentBytes > pixels.size() ){
                numToSend -= ( (numToSend + numSentBytes) - pixels.size() );
            }
        
            tcpServer.sendRawBytesToAll( (char *)&pixels[numSentBytes], numToSend);
            numSentBytes += numToSend;
        }
        
        if( numSentBytes >= pixels.size() ){
            state = COMPLETE;
        }

    }

    void saveImage(){
        
    }

    serverState state;
    int numSentBytes;
    //int totalBytes;
    
    ofxTCPServer tcpServer;
    
    //unsigned char pixels[NUM_BYTES_IN_IMG];
    vector<unsigned char>pixels;
};
