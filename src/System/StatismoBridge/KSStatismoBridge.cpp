//
//  KSStatismoBridge.h
//
//  非線形最小二乗問題のための最適化計算クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "KSStatismoBridge.h"

#include <boost/scoped_ptr.hpp>

using namespace Kosakasakas;
using namespace statismo;

KSStatismoBridge::KSStatismoBridge()
{}

KSStatismoBridge::~KSStatismoBridge()
{}

bool    KSStatismoBridge::Initialize()
{
    vtkBuildShapeModelExample();
    return true;
}

void    KSStatismoBridge::Finalize()
{}

int getdir (std::string dir, std::vector<std::string> &files, const std::string& extension=".*") {
    vtkDirectory *directory = vtkDirectory::New();
    directory->Open(dir.c_str());
    
    for (unsigned i = 0; i < directory->GetNumberOfFiles(); i++) {
        const char* filename = directory->GetFile(i);
        if (extension == ".*" || std::string(filename).find(extension) != std::string::npos)
            files.push_back(filename);
    }
    directory->Delete();
    return 0;
}

vtkPolyData* loadVTKPolyData(const std::string& filename) {
    vtkPolyDataReader* reader = vtkPolyDataReader::New();
    reader->SetFileName(filename.c_str());
    reader->Update();
    vtkPolyData* pd = vtkPolyData::New();
    pd->ShallowCopy(reader->GetOutput());
    return pd;
}

void    KSStatismoBridge::vtkBuildShapeModelExample()
{
    std::string datadir = "";
    std::string modelname = "";
    
    typedef std::vector<std::string> StringVectorType;
    StringVectorType filenames;
    getdir(datadir, filenames, ".vtk");
    if (filenames.size() == 0) {
        std::cerr << "did not find any vtk files in directory " << datadir << " exiting.";
        exit(-1);
    }
    try {
        
        // We create a new representer object. For the vtkPolyDataRepresenter, we have to set a reference
        // and the alignmentType. The alignmenttype (which is here RIGID) determines how the dataset that we
        // will use will later be aligned to the reference.
        
        vtkPolyData* reference = loadVTKPolyData(datadir + "/" + filenames[0]);
        boost::scoped_ptr<RepresenterType> representer(RepresenterType::Create(reference));
        
        // We create a datamanager and provide it with a pointer  to the representer
        boost::scoped_ptr<DataManagerType> dataManager(DataManagerType::Create(representer.get()));
        
        
        // Now we add our data to the data manager
        // load the data and add it to the data manager. We take the first 17 hand shapes that we find in the data folder
        for (unsigned i = 0; i < filenames.size() ; i++) {
            vtkPolyData* dataset = loadVTKPolyData(datadir + "/" + filenames[i]);
            
            // We provde the filename as a second argument.
            // It will be written as metadata, and allows us to more easily figure out what we did later.
            dataManager->AddDataset(dataset, filenames[i]);
            
            // it is save to delete the dataset after it was added, as the datamanager direclty copies it.
            dataset->Delete();
        }
        
        // To actually build a model, we need to create a model builder object.
        // Calling the build model with a list of samples from the data manager, returns a new model.
        // The second parameter to BuildNewModel is the variance of the noise on our data
        boost::scoped_ptr<ModelBuilderType> modelBuilder(ModelBuilderType::Create());
        
        boost::scoped_ptr<StatisticalModelType> model(modelBuilder->BuildNewModel(dataManager->GetData(), 0.01));
        
        // Once we have built the model, we can save it to disk.
        model->Save(modelname);
        std::cout << "Successfully saved shape model as " << modelname << std::endl;
        
        reference->Delete();
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the shape model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}