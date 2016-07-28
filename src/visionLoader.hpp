//
//  visionLoader.hpp
//  finder
//
//  Created by Shunya Matsuno on 2016/07/28.
//
//

#ifndef visionLoader_hpp
#define visionLoader_hpp

#include <stdio.h>
#include "ofMain.h"

class visionLoader : public ofThread{
    
public:
    void setup(string imagePath);
    void threadedFunction();

    string path;
    string json;
    int callCount;
};


#endif /* visionLoader_hpp */
