//
//  Model.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 03.05.16.
//
//

#include <stdio.h>
#include "ObjectModel.h"

const std::string ObjectModel::CAMERA_NAME("camera");

ObjectModel::ObjectModel(const std::string &model_name) : MODEL_NAME(model_name) {};

void ObjectModel::drawModel(Renderer &bRenderer, const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames){
   
    std::cout << "Draw object model " << modelName << "\n";
    bRenderer.getModelRenderer()->drawModel(modelName, cameraName, modelMatrix, lightNames);
    
    vmml::Matrix4f viewMatrix = bRenderer.getObjects()->getCamera("camera")->getViewMatrix();
    ShaderPtr shader = bRenderer.getObjects()->getShader(modelName);
    if (shader.get())
    {
        shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        shader->setUniform("ViewMatrix", viewMatrix);
        shader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)),normalMatrix);
        shader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        shader->setUniform("EyePos", eyePos);
        
        shader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        shader->setUniform("Ia", vmml::Vector3f(1.f));
        shader->setUniform("Id", vmml::Vector3f(1.f));
        shader->setUniform("Is", vmml::Vector3f(1.f));
        
        std::vector<std::string> cubeMapFileNames;
        cubeMapFileNames.push_back("skyboxSide5.png");
        cubeMapFileNames.push_back("skyboxSide2.png");
        cubeMapFileNames.push_back("skyboxSide4.png");
        cubeMapFileNames.push_back("skyboxSide1.png");
        cubeMapFileNames.push_back("skyboxSide3.png"); //not visible
        cubeMapFileNames.push_back("skyboxSide6.png");
        
        CubeMapPtr cubeMap = bRenderer.getObjects()->loadCubeMap(modelName, cubeMapFileNames);
        shader->setUniform("skybox", cubeMap);
    }
    else
    {
        bRenderer::log("No shader available.");
    }
}

//void ObjectModel::drawModel(Renderer &bRenderer, const std::string &cameraName){
//    // do nothing
//}

//vmml::Vector3f ObjectModel::getScale() {
//    return this->scale;
//}
//
//vmml::Vector3f ObjectModel::getTranslation(){
//    return this->translation;
//}
