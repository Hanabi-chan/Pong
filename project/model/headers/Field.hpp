//
//  Field.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#ifndef Field_hpp
#define Field_hpp

#include <stdio.h>
#include "ObjectModel.h"
#include "Cushion.hpp"

class Field : public ObjectModel {
    
public:
    
    Cushion *cushion;
    
    const vmml::Matrix4f fieldMatrix;
    
    Field(Cushion *cushion, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale);
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
    
};

#endif /* Field_hpp */
