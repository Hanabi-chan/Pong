//
//  Cushion.cpp
//  bRenderer_ios
//
//  Created by Björn Hasselmann on 16.05.16.
//
//

#include "Cushion.hpp"

Cushion::Cushion() : MODEL_NAME("bande"){};

void Cushion::drawModel(vmml::Matrix4f fieldMatrix, Renderer &bRenderer, const std::string &cameraName){
    vmml::Matrix4f modelMatrix = fieldMatrix;// * vmml::create_scaling(vmml::Vector3f(10.0f, 10.0f, 10.0f));
    ObjectModel::drawModel(bRenderer, Cushion::MODEL_NAME, cameraName, modelMatrix, std::vector<std::string>({}));
};