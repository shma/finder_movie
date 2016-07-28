#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    myFbo.allocate(1280,720);
    myGlitch.setup(&myFbo);
    
    video.load("20160727_01.mov");
    video.play();
    
    ofBackground(0, 0, 0);
    
    string rateFile = "2016_0727_rate.json";
    
    bool rateSuccessful = rates.open(rateFile);
    
    if (rateSuccessful) {
        //ofLogNotice("ofApp::setup") << rates.getRawString();
    } else {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    font.load("Futura.ttc", 36);
    loading = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (timer > 300) {
        videoSpeed = 10;
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);
        display = false;
    }
    
    video.setSpeed(videoSpeed);
    video.update();
    
    if (video.getCurrentFrame() >= 24) {
        int sec = video.getCurrentFrame() / 24;


        if (sec == 10) {
            beforeRate = rates[sec][1].asInt();
        }
        
        if (sec > 10) {
            //cout << rates[sec][1].asInt() << endl;
            int currentRate = rates[sec][1].asInt();
            int beforeRate = rates[sec - 10][1].asInt();
            if (abs(currentRate - beforeRate) >= 10) {
                videoSpeed = 1;
                myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,false);
                timer = 0;
                display = true;
            }
        }
    }

    myFbo.begin();
    video.draw(0, 0, 1280, 720);
    myFbo.end();
    
    if(loading==true && !visionLoader.isThreadRunning()){
        vision.parse(visionLoader.json);
        //cout << vision["responses"][0]["labelAnnotations"][0].size() << endl;
        loading = false;
        
        words.clear();
        
        for(int i = 0; i < vision["responses"][0]["labelAnnotations"][0].size(); i++) {
            words.push_back(vision["responses"][0]["labelAnnotations"][i]["description"].asString());
        }
    }
    
    timer++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    myGlitch.generateFx();
    myFbo.draw(10,10);
    
    if (display) {
        if(!loading){
            visionImage.grabScreen(10, 10, 1280, 720);
            visionImage.save("visionImage.png");
            

            
            visionLoader.setup(ofFilePath::getAbsolutePath("visionImage.png"));
            
            loading = true;
            visionLoader.startThread();
            
        }
    }
    
    ofSetColor(255, 255, 255);
    if (display) {
        string word = ofJoinString(words, ",");
        visionImage.draw(100, 850, 320, 180);
        font.drawString(word , 100, 800);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'x') {
        visionImage.grabScreen(10, 10, 640, 360);
        visionImage.save("visionImage.png");
        videoSpeed = 1;
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
