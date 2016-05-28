//
//  Field.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Field.hpp"

Field::Field(Cushion *cushion, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("field"), fieldMatrix(vmml::create_translation(vmml::Vector3f(-0.25,-1,0)) * vmml::create_scaling(vmml::Vector3f(scale))* vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0)) ){ //* vmml::create_rotation(-0.7f, vmml::Vector3f(0,1,0))) {
    this->scale = vmml::Vector3f(scale);
    this->dimension.x = dimensionX * scale;
    this->dimension.y = dimensionY * scale;
    this->cushion = cushion;
}

void Field::setReflections(std::list<IReflection*> reflections){
    this->reflections = reflections;
}


void Field::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
    
    /* #### reflections #### */
    // enable stencil testing
    glEnable(GL_STENCIL_TEST);
    
    // set up stencil buffer, depth and color mask
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    // set the plane to draw to (here the field)
    ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->fieldMatrix, std::vector<std::string>({ }));

    // update stencil buffer
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);
    
    // read out current blend mode and restore it afterwards
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    
    // change blend function
    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    
    // draw the reflections here
    for(std::list<IReflection*>::iterator it = this->reflections.begin(); it != this->reflections.end(); ++it){
        (*it)->drawModelReflection(bRenderer);
    }
    
    ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->fieldMatrix, std::vector<std::string>({ }));
    
    // disable stencil testing
    glDisable(GL_STENCIL_TEST);
    
    // restore blend state
    (blendEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND));
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw cushion
    this->cushion->drawModel(this->fieldMatrix, bRenderer, cameraName);
}