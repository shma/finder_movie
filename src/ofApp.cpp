#include "ofApp.h"

bool apiMode = false;
string b;
int cacheCount;
int currentRate;
float currentTaion;
int acceleration;

vector<string> extractedWords;

// ここに該当ファイルを入れるだけでよいようにする。
string targetDate = "20160807";

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Init
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    myFbo.allocate(640,360);
    myGlitch.setup(&myFbo);
    font.load("Futura.ttc", 36);
    fontSmall.load("Futura.ttc", 18);
    
    // Videos loading
    ofVideoPlayer v;
    videos.push_back(v);
    
    videos[0].load("20160727/20160727.mov");
    videos[0].play();
    
    video.load("20160807/0807_yanagase.mov");
    video.play();
    
    ofFbo f;
    f.allocate(640,360);
    fbos.push_back(f);
    
    string rateFile = "20160807/2016_0807_rate.json";
    bool rateSuccessful = rates.open(rateFile);
    
    if (!rateSuccessful) {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    string taionFile = "20160807/2016_0807_taion.json";
    bool taionSuccessful = taions.open(taionFile);
    
    if (!taionSuccessful) {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }

    if (!apiMode) {
        string visionFile = "20160727_vision.json";
        bool visionSuccessful = vision.open(visionFile);
        if (visionSuccessful) {
            ofLogNotice("ofApp::setup") << vision.getRawString();
        } else {
            ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
        }
    }
    
    // Vision API
    for(int i; i < vision.size(); i++) {
        cout << vision[i].size() << endl;
    }
    
    loading = false;
}

//--------------------------------------------------------------
void ofApp::update(){

    if (timer > 300) {
        videoSpeed = 10;
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);
        display = false;
    }
    
    videos[0].update();
    
    video.setSpeed(videoSpeed);
    video.update();
    
    int oneMin = 60;
    
    if (video.getCurrentFrame() >= oneMin) {
        int sec = video.getCurrentFrame() / oneMin;
        
        if (sec == 10) {
            beforeRate = rates[sec][1].asInt();
        }
        
        if (sec > 10) {
            currentRate = rates[sec][1].asInt();
            int beforeRate = rates[sec - 10][1].asInt();
            acceleration = currentRate - beforeRate;
            if (abs(acceleration) >= 9 && timer > 300) {
                videoSpeed = 1;
                myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,false);
                timer = 0;
                display = true;
                loading = false;
            }
        }
        
        currentTaion = taions[sec][1].asFloat();
    }
    
    fbos[0].begin();
    videos[0].draw(0, 0, 640, 360);
    fbos[0].end();

    myFbo.begin();
    video.draw(0, 0, 640, 360);
    myFbo.end();
    
    timer++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    myGlitch.generateFx();
    myFbo.draw(10,10);
    
    fbos[0].draw(650,10);
    
    if (apiMode) {
        if (display) {
            if(!loading){
                cout << "movie come" << endl;
                video.setPaused(true);

                visionImage.grabScreen(10, 10, 1280, 720);
                visionImage.save("visionImage.png");
                
                cout << "API CALL" << endl;
                string json = ofSystem("php /Users/shma/work/vision/vision.php " + ofFilePath::getAbsolutePath("visionImage.png") + " LABEL_DETECTION");
                vision.parse(json);
                loading = true;
            
                words.clear();
                for(int i = 0; i < vision["responses"][0]["labelAnnotations"][0].size(); i++) {
                    words.push_back(vision["responses"][0]["labelAnnotations"][i]["description"].asString());
                }
            
                string word = ofJoinString(words, ",");
                string fileName = "abc.txt";
                int frame = video.getCurrentFrame();
                
                b =  b + ("[" + ofToString(frame) + "," + word + "]\r\n,");
                ofBuffer buffer = ofBuffer( b );
                ofBufferToFile( fileName, buffer );
            
                video.setPaused(false);
                cout << "movie restart" << endl;
            }
        }
    } else {
        if (display && timer==1) {
            visionImage.grabScreen(10, 10, 1280, 720);
            
            words.clear();
            for (int i = 0; i < vision[cacheCount].size(); i++ ) {
                words.push_back(vision[cacheCount][i].asString());
                extractedWords.push_back(vision[cacheCount][i].asString());
            }
            cacheCount++;
        }
    }
    
    ofSetColor(255, 255, 255);
    if (display) {
        string word = ofJoinString(words, ",");
        visionImage.draw(100, 850, 320, 180);
        fontSmall.drawString("Extracted New Words : ", 480, 880);
        fontSmall.drawString(word , 480, 920);
    }
    
    font.drawString("Current HR : " + ofToString(currentRate), 700, 100);
    font.drawString("Current Taion : " + ofToString(currentTaion), 700, 160);
    
    /**
    if (acceleration >= 9) {
        font.drawString("Acceleration : " + ofToString(acceleration) + "!", 700, 160);
    } else {
        font.drawString("Acceleration : " + ofToString(acceleration), 700, 160);
    }
     **/
    
    font.drawString("Extracted Words", 700, 240);
    
    for(int i; i < extractedWords.size(); i++) {
        if (i >= 20) {
            fontSmall.drawString(extractedWords[i], 1600, 250 + ((i - 20 + 1) * 30));
        } else {
            fontSmall.drawString(extractedWords[i], 1300, 250 + ((i + 1) * 30));
        }
    }
}

void ofApp::callApi() {
    
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
