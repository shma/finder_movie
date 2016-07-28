//
//  visionLoader.cpp
//  finder
//
//  Created by Shunya Matsuno on 2016/07/28.
//
//

#include "visionLoader.hpp"
void visionLoader::setup(string imagePath){
    this->path = imagePath;
}
    
void visionLoader::threadedFunction(){
        cout << "php /Users/shma/work/vision/vision.php " + this->path + " LABEL_DETECTION" << endl;
        callCount++;
        cout << "CALL API : " + ofToString(callCount) << endl;
        json = ofSystem("php /Users/shma/work/vision/vision.php " + this->path + " LABEL_DETECTION");
}
