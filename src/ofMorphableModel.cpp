//
//  ofMorphableModel.cpp
//  Facehack
//
//  Created by 小坂 昂大 on 2016/06/29.
//
//

#include "ofApp.h"
#include "ofMorphableModel.hpp"
#include "memory.h"

#include <itkCommand.h>
#include <itkEuclideanDistancePointMetric.h>
#include <itkLevenbergMarquardtOptimizer.h>
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkPointSetToPointSetRegistrationMethod.h>

#include "itkStandardMeshRepresenter.h"
#include "itkStatisticalModel.h"
#include "itkStatisticalShapeModelTransform.h"


const unsigned Dimensions = 3;
typedef itk::Mesh<float, Dimensions  > MeshType;

typedef itk::StandardMeshRepresenter<float, Dimensions> RepresenterType;

typedef itk::MeshFileReader<MeshType> MeshReaderType;
typedef itk::EuclideanDistancePointMetric<MeshType, MeshType> MetricType;

// As a transform, we use the StatisticalShapeModelTransform, that comes with statismo
typedef itk::StatisticalShapeModelTransform<MeshType, double, Dimensions> TransformType;


typedef itk::PointSetToPointSetRegistrationMethod<MeshType, MeshType> RegistrationFilterType;

typedef  itk::LevenbergMarquardtOptimizer OptimizerType;

typedef itk::StatisticalModel<MeshType> StatisticalModelType;



ofMorphableModel::ofMorphableModel()
{}

ofMorphableModel::~ofMorphableModel()
{}

bool ofMorphableModel::Initialize()
{
    ofDirectory dataDir("unpublic");
    string dataPath = dataDir.getAbsolutePath() + "/model2009-publicmm1-bfm.h5";
    
    // load the model
    //RepresenterType::Pointer representer = RepresenterType::New();
    //StatisticalModelType::Pointer model = StatisticalModelType::New();
    //model->Load(representer, dataPath.c_str());
    //MeshType::Pointer fixedPointSet  = model->GetRepresenter()->GetReference();
    //std::cout << "model succesully loaded " << std::endl;
  
    return true;
}