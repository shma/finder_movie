#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxPostGlitch.h"
#include "visionLoader.hpp"
#include "ofxGui.h"
#include "ofxCsv.h"
#include "ofxFontAwesome.h"
#include "ofxFontStash.h"

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
        float getAverage(ofxJSONElement vals);
    
        // 独自関数
        void callApi();
        void setupJsons();
        void setPieChart();
    
        ofVec2f getMinMax(ofxJSONElement vals);
   
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
    
        // 平均の値を格納
        float averageTaion;
        float averageRate;
    
        // score　各値を閾値（デフォルトは平均値）に対して-1.0 ~ 1.0でスコア化する。
        // この値が映像にエフェクトとして現れる。
        float scoreRate;
        float scoreTaion;
    
        // 最小値と最大値
        ofVec2f minMaxTaion;
        ofVec2f minMaxRate;
    
        ofTrueTypeFont font;
        ofTrueTypeFont fontSmall;
    
        bool loading;
        visionLoader visionLoader;
    
        ofImage image;
    
        vector<float> videoSpeeds;
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
        ofxVec2Slider rateThreshold;
        ofxFloatSlider taionThreshold;
    
        // 設計図を出力する先
        string fileName;
    
        ofxCsv csv;
    
        // Fontawesomeを使う
        ofxFontStash afont;
    
        // 円グラフ
        float pie_w = 100;  //円グラフの幅
        float pie_h = 300;  //円グラフの高さ
        int lastAngle = -90;   //通常の円グラフの始点にするため、スタート地点を設定
        int angle0 = 60;  //1つ目のグラフの角度
        int angle1 = 40;  //2つ目のグラフの角度

};
