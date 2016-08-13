#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxPostGlitch.h"
#include "visionLoader.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
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
    
        // 独自関数
        void callApi();
        void setupJsons();
    
        vector<ofVideoPlayer> videos;
    
        ofVideoPlayer video;
        ofImage visionImage;
    
        // 生体情報
        ofxJSONElement rates;
        ofxJSONElement taions;
        ofxJSONElement memes;
        ofxJSONElement locations;
    
        // Google Cloud Vision APIの結果を格納する
        ofxJSONElement vision;
    
        // Extractorの効果を定義する。
        vector<ofFbo> fbos;
        ofFbo myFbo;
        ofxPostGlitch myGlitch;
    
    
        ofTrueTypeFont font;
        ofTrueTypeFont fontSmall;
    
        bool loading;
        visionLoader visionLoader;
    
        ofImage image;
    
        float videoSpeed = 1;
    
        int beforeRate = 0;
        int timer = 400;
    
        bool display;
    
        vector<string> words;
    
        // Google Cloud Vision APIを叩くかどうか。
        bool apiMode = false;
        string b;
        int cacheCount;
        int currentRate;
        float currentTaion;
        int acceleration;
    
        vector<string> extractedWords;
    
        ofxPanel gui;
        ofxButton extractButton;
        ofxFloatSlider rateThreshold;
        ofxFloatSlider taionThreshold;
};
