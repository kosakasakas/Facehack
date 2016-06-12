//
//  ofVTKExample.cpp
//
//  statismoのrunVTKExamplesをC++ライブラリとして実装するクラスです.
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/06/12.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ofApp.h"
#include "ofVTKExample.hpp"
#include "System/StatismoBridge/KSVTKExample.h"

using namespace Kosakasakas;

/**
 @brief コンストラクタ
 */
ofVTKExample::ofVTKExample()
{}

/**
 @brief デストラクタ
 */
ofVTKExample::~ofVTKExample()
{}

/**
 @brief 初期化
 
 @return 成功可否
 */
bool    ofVTKExample::Initialize()
{
    return true;
}

/**
 @brief 終了処理
 */
void    ofVTKExample::Finalize()
{}

/**
 @brief 実行
 */
void    ofVTKExample::Run()
{
    ofDirectory dataDir("");
    string dataPath = dataDir.getAbsolutePath();
    string resDir               = dataPath + "/model";
    string handPolyPath         = dataPath + "/hand_polydata/";
    string handImagePath        = dataPath + "/hand_images/";
    string vtkHandPartModel     = dataPath + "/hand_polydata/partial/hand-0-part.vtk";
    string vtkShapeModel        = resDir + "/vtkShapeModel.h5";
    string vtkIntensityModel    = resDir + "/vtkIntensityModel.h5";
    string vtkPosteriorModel    = resDir + "/vtkPosteriorModel.h5";
    string handReconstructModel = resDir + "/hand-0-reconstruction.vtk";
    string vtkConditionalModel  = resDir + "/vtkConditionalModel.h5";
    
    // build a shape model from the hand data
    KSVTKExample::vtkBuildShapeModelExample(handPolyPath.c_str(),
                                            vtkShapeModel.c_str());
    
    // build an intensity model
    KSVTKExample::vtkBuildIntensityModelExample(handImagePath.c_str(),
                                                vtkIntensityModel.c_str());
    
    // sample from the model and save results
    KSVTKExample::vtkBasicSamplingExample(vtkShapeModel.c_str(),
                                          resDir.c_str());
    
    // Crossvalidation
    KSVTKExample::vtkCrossValidationExample(handPolyPath.c_str());
    
    // Build a partially fixed model
    KSVTKExample::vtkBuildPosteriorModelExample(vtkShapeModel.c_str(),
                                                vtkHandPartModel.c_str(),
                                                vtkPosteriorModel.c_str(),
                                                handReconstructModel.c_str());
    
    // Build a conditional model
    KSVTKExample::vtkBuildConditionalModelExample(handImagePath.c_str(),
                                                  vtkConditionalModel.c_str());
    
}

