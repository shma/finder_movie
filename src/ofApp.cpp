#include "ofApp.h"

string targetDate = "20160905";
int normalPlayTime = 300;

ofVec2f videoSize;
ofVec2f thumbVideoSize;

int paddingHeight;
int paddingWidth;

vector<ofPath> paths;
float angles[] = {0,0,0,0};

int currentCluster;
int choosedCluster;

vector<int> cls;

//--------------------------------------------------------------
void ofApp::setup(){
    // Init
    ofBackground(255, 255, 255);
    ofSetFrameRate(59.94);
    
    font.load("MuseoSans_500.otf", 45);
    fontSmall.load("MuseoSans_500.otf", 22);
    
    // Videos loading
    videoSize.set(1280, 720);
    ofFbo f;
    f.allocate(videoSize.x, videoSize.y);
    fbos.push_back(f);
    
    thumbVideoSize.set(320, 180);
    
    image.load(targetDate + "/kmeans.png");
    for (int i = 0; i < 1; i++) {
        ofVideoPlayer v;
        videos.push_back(v);
        
        videos[i].load(targetDate + "/movie.mov");
        videos[i].play();
        
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
    
    loading = false;

    
    csv.load(targetDate + "/rt.csv");
    
    for(int i = 0; i < csv.getNumRows(); i++) {
        // Rt mode
        int a = ofToInt(csv[i][3]);
        
        // D mode
//        int a = ofToInt(csv[i][2]) + 1;
        cls.push_back(a);
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
        path.arc( 150, 150, 200, 200, lastAngle, lastAngle + angles[i]);
        lastAngle += angles[i];
        
        paths.push_back(path);
    }
    
    choosedCluster = 1;
    //ウィンドウ初期位置
    ofSetWindowPosition(1441,0);
    
    for (int i = 0; i < cls.size(); i++) {
        cout << cls[i] << endl;
    }
}
int sec;
int oneMin = 60;
//--------------------------------------------------------------
void ofApp::update(){
    videos[0].update();
    videos[0].setSpeed(videoSpeed);
    
    sec = 0;
    
    if (videos[0].getCurrentFrame() >= oneMin) {
        sec = videos[0].getCurrentFrame() / oneMin;
        if (sec == 10) {
            beforeRate = rates[sec][1].asInt();
        }
    }
    videoSpeed = 10;
    currentCluster = cls[sec];

    currentRate = rates[sec][1].asInt();
    scoreRate =  ofMap(currentRate, minMaxRate.x , minMaxRate.y , -1.0, 1.0);
    
    currentTaion = taions[sec][1].asFloat();
    scoreTaion = ofMap(currentTaion, minMaxTaion.x , minMaxTaion.y, -1.0, 1.0);
    myGlitch.ShadeVal[0] =  scoreTaion / 2;
    //cout << myGlitch.ShadeVal[0] << endl;
    
    myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);

    if (currentCluster == choosedCluster) {
        fileName = targetDate + "_" + ofToString(choosedCluster) + "_frame.csv";
        ofBuffer buffer = ofBufferFromFile(fileName);
        buffer.append(ofBuffer(ofToString(videos[0].getCurrentFrame()) + "\r\n" ));
        
        ofBufferToFile( fileName, buffer );
        videoSpeed = 1;
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,false);
    }
    
    fbos[0].begin();
    videos[0].draw(0, 0, videoSize.x, videoSize.y);
    fbos[0].end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    paddingHeight = (ofGetHeight() - videoSize.y) / 2 - 200;
    paddingWidth = 300;

    // Bio-Informationなどなど
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(paddingWidth, paddingHeight + videoSize.y + 150);
        ofSetColor(74, 74, 72);
    
        font.drawString("Bio-information", 0, 0);
        int descY = 80;
        afont.drawString( ofxFontAwesome::heart, 0, descY);
        font.drawString(ofToString(currentRate) + " bpm", 80, descY);
    
        ofSetColor(74, 74, 72);
        afont.drawString( ofxFontAwesome::line_chart , 330, descY);
        font.drawString(ofToString(currentTaion) + " °C",410, descY);
    
        font.drawString("Classification", 740, 0);
        afont.drawString( ofxFontAwesome::pie_chart , 740, descY);
    
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
        ofDrawRectangle(805, descY - 30, 48.54, 30);
    
        ofSetColor(74, 74, 72);
        afont.drawString( ofxFontAwesome::play , 940, descY);
    
        switch (choosedCluster) {
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
        ofDrawRectangle(1005, descY - 30, 48.54, 30);
    
    ofPopMatrix();
    ofPopStyle();

    
    // 円グラフ
    ofPushStyle();
    ofPushMatrix();
        ofTranslate(paddingWidth + videoSize.x +  200, paddingHeight + 40);
    
        for (int i =0; i < 4; i++) {
            paths[i].draw();
        }
    
        ofSetColor(255, 255, 255);
        ofSetCircleResolution(360);
        ofDrawCircle(150, 150, 80);
    ofPopMatrix();
    ofPopStyle();

    
    ofPushStyle();
    ofPushMatrix();
        ofTranslate(paddingWidth + videoSize.x +  130, paddingHeight + 500);
        image.draw(0,0);
    ofPopMatrix();
    ofPopStyle();
     

    myGlitch.generateFx();
    fbos[0].draw(paddingWidth, paddingHeight);
    
    if (apiMode) {
        if (display) {
            if(!loading){
                callApi();
            }
        }
    }
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(paddingWidth, paddingHeight + videoSize.y - 10);
    
    int total = videos[0].getTotalNumFrames();
    float c = ofMap(videos[0].getCurrentFrame(), 0, total, 0, cls.size());
    
    ofSetColor(0,0,0);
    ofDrawRectangle(c * 0.24, 20, 0.24, 10);
    for (int i = 0; i < cls.size(); i++) {
        
        switch (cls[i]) {
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
                ofSetColor(255,255,255);
                break;
        }
        ofDrawRectangle(i * 0.24, 10, 0.24, 10);
    }

    ofPopMatrix();
    ofPopStyle();
    
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
    
    if (key == '1') {
        choosedCluster = 1;
    }
    
    if (key == '2') {
        choosedCluster = 2;
    }
    
    if (key == '3') {
        choosedCluster = 3;
    }
    
    if (key == '4') {
        choosedCluster = 4;
    }
    
    if (key == 'e') {
        videos[0].setFrame(310000);
    }
    
    if (key == 'a') {
        changeDate("20160905", "r");
    }
    
    if (key == 's') {
        changeDate("20160809", "r");
    }
    
    if (key == 'd') {
        changeDate("20160807", "r");
    }
    
    if (key == 'l') {
        changeDate(targetDate, "d");
    }
    
    if (key == 'k') {
        changeDate(targetDate, "r");
    }
}

void ofApp::changeDate(string t, string m) {
    targetDate = t;
    image.load(targetDate + "/kmeans.png");
    
    videos[0].close();
    videos[0].load(targetDate + "/movie.mov");
    videos[0].play();
    
    
    setupJsons();
    averageTaion = getAverage(taions);
    minMaxTaion = getMinMax(taions);
    minMaxRate  = getMinMax(rates);
    
    loading = false;
    
    csv.clear();
    if (m == "r") {
        if (csv.load(targetDate + "/rt.csv")) {
            ofLog() << csv[1][1];
        }
        
        cls.clear();
        for(int i = 0; i < csv.getNumRows(); i++) {
            cls.push_back(ofToInt(csv[i][3]));
        }
    } else if (m == "d") {
        if (csv.load(targetDate + "/d.csv")) {
            ofLog() << csv[1][1];
        }
        
        cls.clear();
        
        for(int i = 0; i < csv.getNumRows(); i++) {
            int a = ofToInt(csv[i][2]) + 1;
            cls.push_back(a);
        }
    }
    
    setPieChart();
    paths.clear();
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
        path.arc( 150, 150, 200, 200, lastAngle, lastAngle + angles[i]);
        lastAngle += angles[i];
        
        paths.push_back(path);
    }
    choosedCluster = 1;
    
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
    int rows = cls.size();

    for(int i = 0; i < rows; i++) {
        int cl = cls[i];
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