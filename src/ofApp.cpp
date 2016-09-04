#include "ofApp.h"

string targetDate = "20160829";

int normalPlayTime = 300;

ofVec2f videoSize;
ofVec2f thumbVideoSize;

int paddingHeight;

vector<ofPath> paths;
float angles[] = {0,0,0,0};

int currentCluster;

//--------------------------------------------------------------
void ofApp::setup(){
    // Init
    ofBackground(255, 255, 255);
    ofSetFrameRate(59.94);
    
    font.load("MuseoSans_500.otf", 26);
    fontSmall.load("MuseoSans_500.otf", 22);
    
    // Videos loading
    videoSize.set(1280, 720);
    ofFbo f;
    f.allocate(videoSize.x, videoSize.y);
    fbos.push_back(f);
    
    thumbVideoSize.set(320, 180);
    
    for (int i = 0; i < 1; i++) {
        ofVideoPlayer v;
        videos.push_back(v);
        
        videos[i].load(targetDate + "/movie.mov");
        videos[i].play();
        videos[i].setLoopState(OF_LOOP_NONE);
        
        float speed = 1;
        videoSpeeds.push_back(videoSpeed);
        
        f.allocate(thumbVideoSize.x, thumbVideoSize.y);
        fbos.push_back(f);
    }
    
    myGlitch.setup(&fbos[0]);
    
    // GUI Set up
    gui.setup();
    gui.add(rateThreshold.setup("rate", ofVec2f(50, 120), ofVec2f(0, 0), ofVec2f(180, 180)));
    gui.add(taionThreshold.setup("taion", 35, 30.0, 40.0));
    gui.setPosition(videoSize.x + 20, 300);
    
    setupJsons();
    averageTaion = getAverage(taions);
    minMaxTaion = getMinMax(taions);
    minMaxRate  = getMinMax(rates);
    
    // Vision API
    for(int i; i < vision.size(); i++) {
 //       cout << vision[i].size() << endl;
    }
    
    loading = false;

    if (csv.load(targetDate + "/rt.csv")) {
        ofLog() << csv[1][1];
    }
    
    afont.loadFont("fontawesome-webfont.ttf", 40);
    
    setPieChart();
    
    for (int i = 0; i < 4; i++) {
        ofPath path;
        switch (i) {
            case 0:
                path.setColor(ofColor(69,105,144));
                break;
            case 1:
                path.setColor(ofColor(239,118,122));
                break;
            case 2:
                path.setColor(ofColor(73,190,170));
                break;
            case 3:
                path.setColor(ofColor(238,184,104));
                break;
                
            default:
                break;
        }

        path.setCircleResolution(360);
        path.moveTo(150, 150);
        path.arc( 150, 150, 150, 150, lastAngle, lastAngle + angles[i]);
        lastAngle += angles[i];
        
        paths.push_back(path);
    }
}



//--------------------------------------------------------------
void ofApp::update(){
    videos[0].update();
    videos[0].setSpeed(videoSpeeds[0]);
    
    int oneMin = 60;
    int sec = 0;
    
    if (videos[0].getCurrentFrame() >= oneMin) {
        sec = videos[0].getCurrentFrame() / oneMin;
        if (sec == 10) {
            beforeRate = rates[sec][1].asInt();
        }
    }
    
    currentCluster = ofToInt(csv[sec][3]);
    
//    cout << currentCluster << endl;
    videoSpeed = 5;
    for (int i = 0; i < 4; i++) {
        videoSpeeds[i] = 5;
    }
    
    currentRate = rates[sec][1].asInt();
    scoreRate =  ofMap(currentRate, minMaxRate.x , minMaxRate.y , -1.0, 1.0);
    
    /**
    if (abs(scoreRate) > 0.5) {
        videoSpeed = 1;
    } else {
        videoSpeed = 1;
    }
     **/
    
    /**
    if (scoreRate <= 0.1) {
        videoSpeed = 1;
    } else {
        videoSpeed = 10;
    }
     **/
    
    currentTaion = taions[sec][1].asFloat();
    scoreTaion = ofMap(currentTaion, minMaxTaion.x , minMaxTaion.y, -1.0, 1.0);
    myGlitch.ShadeVal[0] =  scoreTaion / 2;
    //cout << myGlitch.ShadeVal[0] << endl;
    
    myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);
    if (currentCluster == 1) {
        /**
        visionImage.grabScreen(10, 10, videoSize.x, videoSize.y);
        string imageName = targetDate + "_oneImage" + ofToString(videos[0].getCurrentFrame()) + ".png";
        visionImage.save(imageName);

        fileName = targetDate + "_active_images.csv";
        ofBuffer buffer = ofBufferFromFile(fileName);
        buffer.append(ofBuffer(ofToString(videos[0].getCurrentFrame()) + "," + "word," + imageName +  "\r\n" ));
        
        ofBufferToFile( fileName, buffer );
        **/
        videoSpeeds[0] = 1;
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,false);
    }
    
    /**
    if (scoreRate > 0.5 && timer > 600) {
        visionImage.grabScreen(10, 10, videoSize.x, videoSize.y);
        
        string imageName = targetDate + "_activeImage" + ofToString(videos[0].getCurrentFrame()) + ".png";
        visionImage.save(imageName);
        
        fileName = targetDate + "_active_images.csv";
        ofBuffer buffer = ofBufferFromFile(fileName);
        buffer.append(ofBuffer(ofToString(videos[0].getCurrentFrame()) + "," + "word," + imageName +  "\r\n" ));
        
        ofBufferToFile( fileName, buffer );
        
        timer = 0;
    } else if (scoreRate < -0.5 && timer > 600) {
        visionImage.grabScreen(10, 10, videoSize.x, videoSize.y);
        visionImage.save(targetDate + "_calmImage" + ofToString(videos[0].getCurrentFrame()) + ".png");
        
        string imageName = targetDate + "_calmImage" + ofToString(videos[0].getCurrentFrame()) + ".png";
        fileName = targetDate + "_calm_images.csv";
        ofBuffer buffer = ofBufferFromFile(fileName);
        buffer.append(ofBuffer(ofToString(videos[0].getCurrentFrame()) + "," + "word," + imageName +  "\r\n" ));
        
        ofBufferToFile( fileName, buffer );
        timer = 0;
    }
     **/
    
    
    fbos[0].begin();
    videos[0].draw(0, 0, videoSize.x, videoSize.y);
    fbos[0].end();
    

    
    timer++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    paddingHeight = (ofGetHeight() - videoSize.y) / 2 - 100;

    ofPushStyle();
    ofPushMatrix();
    ofTranslate(40, 920);
        ofSetColor(74, 74, 72);
    
        font.drawString("Bio-information", 0, 0);
        int descY = 80;
        afont.drawString( ofxFontAwesome::heart, 0, descY);
        font.drawString(ofToString(currentRate) + " bpm", 80, descY);
    
        ofSetColor(74, 74, 72);
        afont.drawString( ofxFontAwesome::line_chart , 300, descY);
        font.drawString(ofToString(currentTaion) + " °C", 380, descY);
        //font.drawString("Current Cluster      : " + ofToString(currentCluster), videoSize.x + 50, paddingHeight + 100);
    
        afont.drawString( ofxFontAwesome::pie_chart , 700, descY);
        ofFill();
        switch (currentCluster) {
            case 1:
                ofSetColor(69,105,144);
                break;
            case 2:
                ofSetColor(239,118,122);
                break;
            case 3:
                ofSetColor(73,190,170);
                break;
            case 4:
                ofSetColor(238,184,104);
                break;

            default:
                break;
        }
        ofDrawRectangle(780, descY - 30, 48.54, 30);
    
    ofPopMatrix();
    ofPopStyle();

    
    // 円グラフ
    ofPushStyle();
    ofPushMatrix();
        ofTranslate(videoSize.x +  150, 120);
    
        for (int i =0; i < 4; i++) {
            paths[i].draw();
        }
    
        ofSetColor(255, 255, 255);
        ofSetCircleResolution(360);
        ofDrawCircle(150, 150, 50);
    ofPopMatrix();
    ofPopStyle();

    
    /**
    ofPushStyle();
    ofPushMatrix();
        ofTranslate(videoSize.x +  150, 500, 0);
        ofSetColor(74, 74, 72);
        ofNoFill();
        ofSetLineWidth(3);
        //    ofDrawRectangle(0, 0, 300, 300);
    
        ofDrawLine(0, 150, 300, 150);
        ofDrawLine(150, 0, 150, 300);
    
        ofDrawCircle(150 + (150 * scoreTaion), 150 - (150 * scoreRate), 3);
    ofPopMatrix();
    ofPopStyle();
     **/

    myGlitch.generateFx();
    fbos[0].draw(30, paddingHeight);
    
    if (apiMode) {
        if (display) {
            if(!loading){
                callApi();
            }
        }
    }
    
    //gui.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'x') {
        visionImage.grabScreen(10, 10, videoSize.x, videoSize.y);
        visionImage.save("visionImage.png");
    }
    
    if (key == ' ') {
        
        videos[0].setPaused(true);
    }
    if (key == 'n') {
        
        videos[0].setPaused(false);
    }
}

void ofApp::setupJsons() {
    // ÂøÉÊãçÊï∞
    string rateFile = targetDate + "/" + targetDate + "_rate.json";
    bool rateSuccessful = rates.open(rateFile);
    
    if (!rateSuccessful) {
        ofLogError("ofApp::setup")  << "rate Failed to parse JSON" << endl;
    }
    
    // ‰ΩìÊ∏©
    string taionFile = targetDate + "/" + targetDate + "_taion.json";
    bool taionSuccessful = taions.open(taionFile);
    
    if (!taionSuccessful) {
        ofLogError("ofApp::setup")  << "taion Failed to parse JSON" << endl;
    }

    // MEME
    string memeFile = targetDate + "/" + targetDate + "_meme.json";
    bool memeSuccessful = memes.open(taionFile);
    
    if (!memeSuccessful) {
        ofLogError("ofApp::setup")  << "MEME Failed to parse JSON" << endl;
    }

    // MEME
    string locationFile = targetDate + "/" + targetDate + "_place.json";
    bool locationSuccessful = locations.open(taionFile);
    
    if (!locationSuccessful) {
        ofLogError("ofApp::setup")  << "location Failed to parse JSON" << endl;
    }
    
}

//
float ofApp::getAverage(ofxJSONElement vals) {
    float sum = 0;
    for (int i = 0; i < vals.size(); i++) {
        sum += vals[i][1].asFloat();
    }
    
    return sum / vals.size();
}

ofVec2f ofApp::getMinMax(ofxJSONElement vals) {
    ofVec2f minMax;
    float min;
    float max;
    for (int i = 0; i < vals.size(); i++) {
        if (i == 0) {
            min = vals[i][1].asFloat();
            max = vals[i][1].asFloat();
        }
        
        if (min > vals[i][1].asFloat()) {
            min = vals[i][1].asFloat();
        }
        
        if (max < vals[i][1].asFloat()) {
            max = vals[i][1].asFloat();
        }
    }
    
    minMax.set(min,max);
    return minMax;
}


void ofApp::callApi() {
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
}


void ofApp::setPieChart() {
    
    
    
    int maxCl;
    
    
    int rows = csv.getNumRows();



    for(int i = 0; i < rows; i++) {
        int cl = ofToInt(csv[i][3]);
        
        angles[cl - 1] = 1 + angles[cl - 1];
    }
    
    cout << angles[0] << endl;
    cout << angles[1] << endl;
    cout << angles[2] << endl;
    cout << angles[3] << endl;
    
    for (int i = 0; i < 4; i++) {
        angles[i] = round(angles[i] / rows * 360);
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