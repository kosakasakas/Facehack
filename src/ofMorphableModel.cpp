//
//  ofMorphableModel.cpp
//  Facehack
//
//  Created by 小坂 昂大 on 2016/06/29.
//
//

#include "ofApp.h"
#include "ofMorphableModel.hpp"
#include "vtkStandardMeshRepresenter.h"
#include "StatisticalModel.h"
#include "memory.h"
#include <boost/scoped_ptr.hpp>

using namespace statismo;

ofMorphableModel::ofMorphableModel()
{}

ofMorphableModel::~ofMorphableModel()
{}

bool ofMorphableModel::Initialize()
{
    ofDirectory dataDir("unpublic");
    string dataPath = dataDir.getAbsolutePath() + "/model2009-publicmm1-bfm.h5";
    
    typedef vtkStandardMeshRepresenter RepresenterType;
    typedef StatisticalModel<vtkPolyData> StatisticalModelType;
    
    RepresenterType* representer = RepresenterType::Create();
     boost::scoped_ptr<StatisticalModelType> model(StatisticalModelType::Load(representer, dataPath.c_str()));
    std::cout << "loaded model with " << model->GetNumberOfPrincipalComponents() << " Principal Components" << std::endl;
    
    return true;
}