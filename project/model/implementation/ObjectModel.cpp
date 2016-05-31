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
   
    CameraPtr camera = bRenderer.getObjects()->getCamera(cameraName);
    vmml::Matrix4f viewMatrix = camera->getViewMatrix();
    ShaderPtr shader = bRenderer.getObjects()->getShader(modelName);
    if (shader.get())
    {
        shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        shader->setUniform("ViewMatrix", viewMatrix);
        shader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)),normalMatrix);
        shader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(camera->getPosition(), 1.0f);
        shader->setUniform("EyePos", eyePos);
        
//        shader->setUniform("LightPos", vmml::Vector4f(-4.f, 1.f, .5f, 1.f));
        shader->setUniform("Ia", vmml::Vector3f(1.0f));
        shader->setUniform("Id", vmml::Vector3f(1.0f));
        shader->setUniform("Is", vmml::Vector3f(1.0f));
        
        std::vector<std::string> cubeMapSpecularFileNames;
        cubeMapSpecularFileNames.push_back("skyboxSideLeft.png");
        cubeMapSpecularFileNames.push_back("skyboxSideRight.png");
        cubeMapSpecularFileNames.push_back("skyboxSideBottom.png");
        cubeMapSpecularFileNames.push_back("skyboxSideTop.png");
        cubeMapSpecularFileNames.push_back("skyboxSideFront.png"); //not visible
        cubeMapSpecularFileNames.push_back("skyboxSideBack.png");
        
        CubeMapPtr cubeMap = bRenderer.getObjects()->loadCubeMap(modelName, cubeMapSpecularFileNames);
        shader->setUniform("skyboxSpecular", cubeMap);
        
        std::vector<std::string> cubeMapDiffuseFileNames;
        cubeMapDiffuseFileNames.push_back("skyboxSideLeft_diffuse.png");
        cubeMapDiffuseFileNames.push_back("skyboxSideRight_diffuse.png");
        cubeMapDiffuseFileNames.push_back("skyboxSideBottom_diffuse.png");
        cubeMapDiffuseFileNames.push_back("skyboxSideTop_diffuse.png");
        cubeMapDiffuseFileNames.push_back("skyboxSideFront_diffuse.png"); //not visible
        cubeMapDiffuseFileNames.push_back("skyboxSideBack_diffuse.png");
        
        CubeMapPtr cubeMapDiffuse = bRenderer.getObjects()->loadCubeMap(modelName, cubeMapDiffuseFileNames);
        shader->setUniform("skyboxDiffuse", cubeMapDiffuse);
    }
    else
    {
        bRenderer::log("No shader available.");
    }
    
    std::cout << "Draw object model " << modelName << "\n";
    bRenderer.getModelRenderer()->drawModel(modelName, cameraName, modelMatrix, lightNames);
    
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
