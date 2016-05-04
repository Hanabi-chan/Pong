//
//  Puck.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#ifndef Puck_hpp
#define Puck_hpp

#include <stdio.h>
#include "ObjectModel.h"
#include "Field.hpp"

class Puck : public ObjectModel {
    
public:
    
    Puck(Field *field, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale);
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
private:
    
    const GLfloat trans_z;
    
    Field *field;
    
    GLfloat radius;
    
};


#endif /* Puck_hpp */
