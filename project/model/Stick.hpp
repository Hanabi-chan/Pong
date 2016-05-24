//
//  Stick.hpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#ifndef Stick_hpp
#define Stick_hpp

#include <stdio.h>
#include "ObjectModel.h"
#include "Field.hpp"

class Stick : public ObjectModel {
    
public:
    
    Stick(const Stick &stick, bool left);
    
    Stick(Field *field, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale, bool left = false);
    
    struct Translation {
        
        GLfloat x, y, z;
        
        vmml::Vector3f toVector(){
            return vmml::Vector3f(x, z, y);
        }
        
    };
    Translation translation;
    
    bool left;
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
    const vmml::Vector3f xAxis;
    
    const GLfloat rotation, trans_x, trans_z;

private:

//    const vmml::Vector3f xAxis;
//    
//    const GLfloat rotation, trans_x, trans_z;
    
    Field *field;
    
    void makeMovement(Renderer &bRenderer);
    
};
#endif /* Stick_hpp */
