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

void ObjectModel::drawModel(Renderer &bRenderer, const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f & modelMatrix, const std::vector<std::string> &lightNames){
    std::cout << "Draw object model " << modelName << "\n";
    bRenderer.getModelRenderer()->drawModel(modelName, cameraName, modelMatrix, lightNames);
}

void ObjectModel::drawModel(Renderer &bRenderer, const std::string &cameraName){
    // do nothing
}