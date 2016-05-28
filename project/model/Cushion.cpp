//
//  Cushion.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 16.05.16.
//
//

#include "Cushion.hpp"

Cushion::Cushion() : MODEL_NAME("bande"){};

vmml::Matrix4f Cushion::drawModel(vmml::Matrix4f fieldMatrix, Renderer &bRenderer, const std::string &cameraName){
    vmml::Matrix4f modelMatrix = fieldMatrix;// * vmml::create_scaling(vmml::Vector3f(10.0f, 10.0f, 10.0f));
    ShaderPtr fieldShader = bRenderer.getObjects()->getShader(MODEL_NAME);
    if (fieldShader.get())
    {
        vmml::Matrix4f viewMatrixField = bRenderer.getObjects()->getCamera("camera")->getViewMatrix();
        
        fieldShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        fieldShader->setUniform("ViewMatrix", viewMatrixField);
        fieldShader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrixField;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrixField);
        fieldShader->setUniform("NormalMatrix", normalMatrixField);
        
//        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        
        fieldShader->setUniform("EyePos", eyePos);
        
//        fieldShader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        fieldShader->setUniform("LightPos", vmml::Vector4f(0.0023765473f, 0.00029374f, 0.000237645f, 0.1f));
        
        fieldShader->setUniform("Ia", vmml::Vector3f(1.f));
        fieldShader->setUniform("Id", vmml::Vector3f(1.f));
        fieldShader->setUniform("Is", vmml::Vector3f(1.f));
        
        std::vector<std::string> cubeMapFileNames;
        cubeMapFileNames.push_back("skyboxSide5.png");
        cubeMapFileNames.push_back("skyboxSide2.png");
        cubeMapFileNames.push_back("skyboxSide4.png");
        cubeMapFileNames.push_back("skyboxSide1.png");
        cubeMapFileNames.push_back("skyboxSide3.png"); //not visible
        cubeMapFileNames.push_back("skyboxSide6.png");
        
        CubeMapPtr cubeMap = bRenderer.getObjects()->loadCubeMap(MODEL_NAME, cubeMapFileNames);
        fieldShader->setUniform("skybox", cubeMap);
    }
    else
    {
        bRenderer::log("No shader available.");
    }
    return modelMatrix;
};