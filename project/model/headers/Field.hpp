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

class Field : public ObjectModel {
    
public:
    
    const vmml::Matrix4f fieldMatrix;
    
    Field(GLfloat dimensionX, GLfloat dimensionY, GLfloat scale);
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
};

#endif /* Field_hpp */
