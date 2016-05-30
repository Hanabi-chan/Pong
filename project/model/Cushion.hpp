//
//  Cushion.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 16.05.16.
//
//

#ifndef Cushion_hpp
#define Cushion_hpp

#include <stdio.h>
#include "ObjectModel.h"

class Cushion {
    
public:
    
    const std::string MODEL_NAME;
    
    Cushion();
    
    void drawModel(vmml::Matrix4f fieldMatrix, Renderer &bRenderer, const std::string &cameraName);

};
#endif /* Cushion_hpp */
