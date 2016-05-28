
//
//  Skybox.hpp
//  bRenderer_ios
//
//  Created by Sarah Zurmühle on 14.05.16.
//
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <stdio.h>
#include "ObjectModel.h"

class Skybox : public ObjectModel {
    
public:
    
    const vmml::Matrix4f skyboxMatrix;
    
    Skybox(GLfloat dimensionX, GLfloat dimensionY, GLfloat scale);
    
    void drawModel(Renderer &bRenderer, const std::string &cameraName);
    
};

#endif /* Skybox_hpp */
