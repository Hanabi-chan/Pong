//
//  Skybox.cpp
//  bRenderer_ios
//
//  Created by Sarah Zurmühle on 14.05.16.
//
//

#include "Skybox.hpp"

Skybox::Skybox(GLfloat dimensionX, GLfloat dimensionY, GLfloat scale) : ObjectModel("skybox"), skyboxMatrix(vmml::create_scaling(vmml::Vector3f(1.3,1.3,1.3))) {
    this->scale(scale);
    this->dimension.x = dimensionX * scale;
    this->dimension.y = dimensionY * scale;
}

void Skybox::drawModel(Renderer &bRenderer, const std::string &cameraName = ObjectModel::CAMERA_NAME){
    ShaderPtr skyboxShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    this->ObjectModel::drawModel(bRenderer, MODEL_NAME, cameraName, this->skyboxMatrix, std::vector<std::string>({ }));
    
    vmml::Matrix4f skyboxModelMatrix = vmml::create_translation(vmml::Vector3f(0,0,0)) * vmml::create_scaling(vmml::Vector3f(1.3,1.3,1.3));
    
    vmml::Matrix4f viewMatrixSkybox = bRenderer.getObjects()->getCamera("camera")->getViewMatrix();
    
    if (skyboxShader.get())
    {
        skyboxShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        skyboxShader->setUniform("ViewMatrix", viewMatrixSkybox);
        skyboxShader->setUniform("ModelMatrix", skyboxModelMatrix);
                
        vmml::Matrix3f normalMatrixSkybox;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(skyboxModelMatrix)), normalMatrixSkybox);
        skyboxShader->setUniform("NormalMatrix", normalMatrixSkybox);
    }
    else
    {
        bRenderer::log("No shader available.");
    }
    skyboxShader->setUniform("ModelMatrix", skyboxModelMatrix);
    
}
