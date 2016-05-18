//
//  Field.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Field.hpp"

Field::Field(Cushion *cushion, GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("field"), fieldMatrix(vmml::create_rotation(-0.2f, vmml::Vector3f(1,0,0)) * vmml::create_rotation(-0.7f, vmml::Vector3f(0,1,0))) {
    this->scale(scale);
    this->dimension.x = dimensionX * scale;
    this->dimension.y = dimensionY * scale;
    this->cushion = cushion;
}


void Field::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
   
    ShaderPtr fieldShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    if (fieldShader.get())
    {
        vmml::Matrix4f viewMatrixField = bRenderer.getObjects()->getCamera("camera")->getViewMatrix();
        
        fieldShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        fieldShader->setUniform("ViewMatrix", viewMatrixField);
        fieldShader->setUniform("ModelMatrix", this->fieldMatrix);
        
        vmml::Matrix3f normalMatrixField;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(this->fieldMatrix)), normalMatrixField);
        fieldShader->setUniform("NormalMatrix", normalMatrixField);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        fieldShader->setUniform("EyePos", eyePos);
        
        fieldShader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        fieldShader->setUniform("Ia", vmml::Vector3f(1.f));
        fieldShader->setUniform("Id", vmml::Vector3f(1.f));
        fieldShader->setUniform("Is", vmml::Vector3f(1.f));
    }
    else
    {
        bRenderer::log("No shader available.");
    }
    
    /*
     *
     *
     *
     */
    /*
    CameraPtr reflCamPtr;
    reflCamPtr->setPosition(vmml::Vector3f(0.0f, -1.0f, 0.0f));
    // bad access
//    bRenderer.getObjects()->addCamera("reflection", reflCamPtr);
    
    glEnable(GL_CULL_FACE);
    
    // Use 0 for clear stencil, enable stencil test
    glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );
    
    // Set 1's in very location where the portal is.
    glStencilFunc( GL_NEVER, 0, 0xFF );
    glStencilOp( GL_INCR, GL_KEEP, GL_KEEP );
    
    glDisable(GL_DEPTH_TEST);
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, "reflection", this->fieldMatrix, std::vector<std::string>({ }));
    glEnable(GL_DEPTH_TEST);
    
    // Render the entire scene at every location where there's a 1 (aka, a portal).
    glStencilFunc( GL_LEQUAL, 1, 0xFF );
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    
    // Set up CLIP PLANE ?
    
    // Render the actual scene inside of the portal.
    // LOOP
     bRenderer.getModelRenderer()->drawQueue(/*GL_LINES);
    
    // Done with stenciling.
    glDisable( GL_STENCIL_TEST );
    */
    /*
     *
     *
     *
     *
     */
    
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->fieldMatrix, std::vector<std::string>({ }));
    vmml::Matrix4f cushionMatrix = this->cushion->drawModel(this->fieldMatrix, bRenderer, cameraName);
    this->ObjectModel::drawModel(bRenderer, this->cushion->MODEL_NAME, cameraName, cushionMatrix, std::vector<std::string>({ }));
    
}