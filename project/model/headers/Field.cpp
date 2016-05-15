//
//  Field.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include "Field.hpp"

Field::Field(GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("field"), fieldMatrix(vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0))) {
    this->scale(scale);
    this->dimension.x = dimensionX * scale;
    this->dimension.y = dimensionY * scale;
}

void Field::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
   
    ShaderPtr fieldShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    if (fieldShader.get())
    {
        vmml::Matrix4f fieldModelMatrix = vmml::create_rotation(-0.5f, vmml::Vector3f(1,0,0));
        vmml::Matrix4f viewMatrixField = bRenderer.getObjects()->getCamera("camera")->getViewMatrix();
        
        fieldShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        fieldShader->setUniform("ViewMatrix", viewMatrixField);
        fieldShader->setUniform("ModelMatrix", fieldModelMatrix);
        
        vmml::Matrix3f normalMatrixField;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(fieldModelMatrix)), normalMatrixField);
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
//    fieldShader->setUniform("ModelMatrix", fieldModelMatrix);
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->fieldMatrix, std::vector<std::string>({ }));

}