//
//  KSVTKExample.cpp
//
//  statismoのVTK Exampleの関数群をC++ライブラリとして実装するクラスです.
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "KSVTKExample.h"

#include <boost/scoped_ptr.hpp>
#include <vtkDirectory.h>
#include <vtkPolyDataReader.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkVersion.h>

#include "DataManager.h"
#include "PCAModelBuilder.h"
#include "StatisticalModel.h"
#include "vtkStandardMeshRepresenter.h"
#include "vtkStandardImageRepresenter.h"
#include "PosteriorModelBuilder.h"
#include "ConditionalModelBuilder.h"
#include "vtkStandardImageRepresenter.h"

using namespace Kosakasakas;
using namespace statismo;

typedef statismo::VectorType VectorType;
typedef statismo::MatrixType MatrixType;
typedef statismo::vtkStandardMeshRepresenter RepresenterType;
typedef statismo::StatisticalModel<vtkPolyData> StatisticalModelType;
typedef statismo::PosteriorModelBuilder<vtkPolyData> PosteriorModelBuilderType;
typedef StatisticalModelType::DomainType DomainType;
typedef DomainType::DomainPointsListType::const_iterator DomainPointsConstIterator;

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

vtkStructuredPoints* loadVTKStructuredPointsData(const std::string& filename) {
    vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
    reader->SetFileName(filename.c_str());
    reader->Update();
    vtkStructuredPoints* sp = vtkStructuredPoints::New();
    sp->ShallowCopy(reader->GetOutput());
    return sp;
}

void saveSample(const vtkPolyData* pd, const std::string& resdir, const std::string& basename) {
    std::string filename = resdir +std::string("/") + basename;
    
    vtkPolyDataWriter* w = vtkPolyDataWriter::New();
#if (VTK_MAJOR_VERSION == 5 )
    w->SetInput(const_cast<vtkPolyData*>(pd));
#else
    w->SetInputData(const_cast<vtkPolyData*>(pd));
#endif
    w->SetFileName(filename.c_str());
    w->Update();
}

void    KSVTKExample::vtkBuildShapeModelExample(const char* srcDir, const char* dstName)
{
    std::string datadir = srcDir;
    std::string modelname = dstName;
    
    // All the statismo classes have to be parameterized with the RepresenterType.
    // For building a shape model with vtk, we use the vtkPolyDataRepresenter.
    typedef vtkStandardMeshRepresenter RepresenterType;
    typedef DataManager<vtkPolyData> DataManagerType;
    typedef PCAModelBuilder<vtkPolyData> ModelBuilderType;
    typedef StatisticalModel<vtkPolyData> StatisticalModelType;
    
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


void KSVTKExample::vtkBuildIntensityModelExample(const char* dataDir, const char* modelName)
{
    std::string datadir(dataDir);
    std::string modelname(modelName);
    
    // All the statismo classes have to be parameterized with the RepresenterType.
    // For building a intensity model with vtk, we use the vtkStructuredPointsRepresenter.
    // Here, we work with unsigned character images. The second template parameter specifies
    // the pixel dimension (1 means scalar image, whereas 3 is a 3D vector image).
    typedef vtkStandardImageRepresenter<unsigned char, 1> RepresenterType;
    typedef DataManager<vtkStructuredPoints> DataManagerType;
    typedef PCAModelBuilder<vtkStructuredPoints> ModelBuilderType;
    typedef StatisticalModel<vtkStructuredPoints> StatisticalModelType;
    
    try {
        
        // Model building is exactly the same as for shape models (see BuildShapeModelExample for detailed explanation)
        vtkStructuredPoints* reference = loadVTKStructuredPointsData(datadir +"/hand-0.vtk");
        boost::scoped_ptr<RepresenterType> representer(RepresenterType::Create(reference));
        boost::scoped_ptr<DataManagerType> dataManager(DataManagerType::Create(representer.get()));
        
        // load the data and add it to the data manager. We take the first 4 hand shapes that we find in the data folder
        for (unsigned i = 0; i < 4; i++) {
            
            std::ostringstream ss;
            ss << datadir +"/hand-" << i << ".vtk";
            const std::string datasetFilename = ss.str();
            vtkStructuredPoints* dataset = loadVTKStructuredPointsData(datasetFilename);
            
            // We provde the filename as a second argument.
            // It will be written as metadata, and allows us to more easily figure out what we did later.
            dataManager->AddDataset(dataset, datasetFilename);
            
            // it is save to delete the dataset after it was added, as the datamanager direclty copies it.
            dataset->Delete();
        }
        boost::scoped_ptr<ModelBuilderType> modelBuilder(ModelBuilderType::Create());
        boost::scoped_ptr<StatisticalModelType> model(modelBuilder->BuildNewModel(dataManager->GetData(), 0.01));
        model->Save(modelname);
        
        reference->Delete();
        std::cout << "Successfully saved model as " << modelname << std::endl;
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the intensity model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void KSVTKExample::vtkBasicSamplingExample(const char* modelName, const char* resDir)
{
    std::string modelname(modelName);
    std::string resultdir(resDir);
    
    
    // All the statismo classes have to be parameterized with the RepresenterType.
    // For building a shape model with vtk, we use the vtkPolyDataRepresenter.
    typedef vtkStandardMeshRepresenter RepresenterType;
    typedef StatisticalModel<vtkPolyData> StatisticalModelType;
    
    try {
        
        // To load a model, we call the static Load method, which returns (a pointer to) a
        // new StatisticalModel object
        RepresenterType* representer = RepresenterType::Create();
        boost::scoped_ptr<StatisticalModelType> model(StatisticalModelType::Load(representer, modelname));
        std::cout << "loaded model with " << model->GetNumberOfPrincipalComponents() << " Principal Components" << std::endl;
        
        
        // get the model mean
        vtkPolyData* mean = model->DrawMean();
        saveSample(mean, resultdir, "mean.vtk");
        
        // draw a random sample
        vtkPolyData* randomSample = model->DrawSample();
        saveSample(randomSample, resultdir, "randomsample.vtk");
        
        // draw a sample with known pca coefficients (3 stddev in direction of the 1st PC)
        VectorType coefficients = VectorType::Zero(model->GetNumberOfPrincipalComponents());
        coefficients(0) = 3;
        vtkPolyData* samplePC1 = model->DrawSample(coefficients);
        saveSample(samplePC1, resultdir, "samplePC1.vtk");
        
        
        // The vtkPolyDataRepresenter returns naked pointers to vtk objcts. Therefore we have to delete all the samples
        mean->Delete();
        randomSample->Delete();
        samplePC1->Delete();
        
        std::cout << "saved samples to " << resultdir << std::endl;
        
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the shape model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void KSVTKExample::vtkCrossValidationExample(const char* dataDir)
{
    std::string datadir(dataDir);
    
    // All the statismo classes have to be parameterized with the RepresenterType.
    typedef vtkStandardMeshRepresenter RepresenterType;
    typedef DataManager<vtkPolyData> DataManagerType;
    typedef StatisticalModel<vtkPolyData> StatisticalModelType;
    typedef PCAModelBuilder<vtkPolyData> ModelBuilderType;
    typedef DataManagerType::CrossValidationFoldListType CVFoldListType;
    typedef DataManagerType::DataItemListType DataItemListType;
    
    
    try {
        vtkPolyData* reference = loadVTKPolyData(datadir +"/hand-0.vtk");
        boost::scoped_ptr<RepresenterType> representer(RepresenterType::Create(reference));
        
        // create a data manager and add a number of datasets for model building
        boost::scoped_ptr<DataManagerType> dataManager(DataManagerType::Create(representer.get()));
        
        for (unsigned i = 0; i < 17; i++) {
            
            std::ostringstream ss;
            ss << datadir +"/hand-" << i << ".vtk";
            const std::string datasetFilename = ss.str();
            vtkPolyData* dataset = loadVTKPolyData(datasetFilename);
            
            // We provde the filename as a second argument.
            // It will be written as metadata, and allows us to more easily figure out what we did later.
            dataManager->AddDataset(dataset, datasetFilename);
            
            // it is save to delete the dataset after it was added, as the datamanager direclty copies it.
            dataset->Delete();
        }
        
        std::cout << "succesfully loaded "<< dataManager->GetNumberOfSamples() << " samples "<< std::endl;
        
        // create the model builder
        boost::scoped_ptr<ModelBuilderType> pcaModelBuilder(ModelBuilderType::Create());
        
        // We perform 4-fold cross validation
        CVFoldListType cvFoldList = dataManager->GetCrossValidationFolds(4, true);
        
        // the CVFoldListType is a standard stl list over which we can iterate to get all the folds
        for (CVFoldListType::const_iterator it = cvFoldList.begin();
             it != cvFoldList.end();
             ++it) {
            // build the model as usual
            boost::scoped_ptr<StatisticalModelType> model(pcaModelBuilder->BuildNewModel(it->GetTrainingData(), 0.01));
            std::cout << "built model with  " << model->GetNumberOfPrincipalComponents() << " principal components"<< std::endl;
            
            // Now we can iterate over the test data and do whatever validation we would like to do.
            const DataItemListType testSamplesList = it->GetTestingData();
            
            for (DataItemListType::const_iterator it = testSamplesList.begin();
                 it != testSamplesList.end();
                 ++it) {
                vtkPolyData* testSample = (*it)->GetSample();
                std::cout << "probability of test sample under the model: " << model->ComputeProbabilityOfDataset(testSample) << std::endl;
                
                // We are responsible for deleting the sample.
                testSample->Delete();
            }
        }
        reference->Delete();
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the shape model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}


/**
 * Computes the mahalanobis distance of the targetPt, to the model point with the given pointId.
 */
double mahalanobisDistance(const StatisticalModelType* model, unsigned ptId, const statismo::vtkPoint& targetPt) {
    statismo::MatrixType cov = model->GetCovarianceAtPoint(ptId, ptId);
    statismo::vtkPoint meanPt = model->DrawMeanAtPoint(ptId);
    unsigned pointDim = model->GetRepresenter()->GetDimensions();
    assert(pointDim <= 3);
    
    VectorType x = VectorType::Zero(pointDim);
    for (unsigned d = 0; d < pointDim; d++) {
        x(d) = targetPt[d] - meanPt[d];
    }
    return x.transpose() * cov.inverse() * x;
}

void KSVTKExample::vtkBuildPosteriorModelExample(const char* inModelName, const char* inPartialShapeMeshName, const char* inPosteriorModelName, const char* outReconstructedShapeName)
{
    std::string inputModelName(inModelName);
    std::string partialShapeMeshName(inPartialShapeMeshName);
    std::string posteriorModelName(inPosteriorModelName);
    std::string reconstructedShapeName(outReconstructedShapeName);
    
    try {
        
        
        vtkPolyData* partialShape = loadVTKPolyData(partialShapeMeshName);
        
        RepresenterType* representer = RepresenterType::Create();
        boost::scoped_ptr<StatisticalModelType> inputModel(StatisticalModelType::Load(representer, inputModelName));
        vtkPolyData* refPd = const_cast<vtkPolyData*>(inputModel->GetRepresenter()->GetReference());
        
        
        StatisticalModelType::PointValueListType constraints;
        
        // for each point we get the closest point and check how far it is away (in mahalanobis distance).
        // If it is close, we add it as a constraint, otherwise we ignore the remaining points.
        const DomainType::DomainPointsListType& domainPoints = inputModel->GetDomain().GetDomainPoints();
        for (unsigned ptId = 0; ptId < domainPoints.size(); ptId++) {
            statismo::vtkPoint domainPoint = domainPoints[ptId];
            
            unsigned closestPointId = ptId;
            statismo::vtkPoint closestPointOnPartialShape = partialShape->GetPoint(closestPointId);
            double mhdist = mahalanobisDistance(inputModel.get(), ptId, closestPointOnPartialShape);
            if (mhdist < 5) {
                StatisticalModelType::PointValuePairType ptWithTargetPt(domainPoint, closestPointOnPartialShape);
                constraints.push_back(ptWithTargetPt);
            }
        }
        
        // build the new model. In addition to the input model and the constraints, we also specify
        // the inaccuracy of our value (variance of the error).
        
        boost::scoped_ptr<PosteriorModelBuilderType> posteriorModelBuilder(PosteriorModelBuilderType::Create());
        boost::scoped_ptr<StatisticalModelType> constraintModel(posteriorModelBuilder->BuildNewModelFromModel(inputModel.get(), constraints, 0.5));
        
        
        // The resulting model is a normal statistical model, from which we could for example sample examples.
        // Here we simply  save it to disk for later use.
        constraintModel->Save(posteriorModelName);
        std::cout << "successfully saved the model to " << posteriorModelName << std::endl;
        
        // The mean of the constraint model is the optimal reconstruction
        vtkPolyData* pmean = constraintModel->DrawMean();
        vtkPolyDataWriter* writer = vtkPolyDataWriter::New();
#if (VTK_MAJOR_VERSION == 5 )
        writer->SetInput(pmean);
#else
        writer->SetInputData(pmean);
#endif
        writer->SetFileName(reconstructedShapeName.c_str());
        writer->Update();
    } catch (statismo::StatisticalModelException& e) {
        std::cout << "Exception occured while building the intenisity model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void KSVTKExample::vtkBuildConditionalModelExample(const char* dataDir, const char* modelName)
{
    std::string datadir(dataDir);
    std::string modelname(modelName);
    
    // All the statismo classes have to be parameterized with the RepresenterType.
    // For building a intensity model with vtk, we use the vtkStructuredPointsRepresenter.
    // Here, we work with unsigned character images. The second template parameter specifies
    // the pixel dimension (1 means scalar image, whereas 3 is a 3D vector image).
    typedef vtkStandardImageRepresenter<unsigned char, 1> RepresenterType;
    typedef DataManagerWithSurrogates<vtkStructuredPoints> DataManagerWithSurrogatesType;
    typedef ConditionalModelBuilder<vtkStructuredPoints> ConditionalModelBuilderType;
    typedef StatisticalModel<vtkStructuredPoints> StatisticalModelType;
    
    
    try {
        vtkStructuredPoints* reference = loadVTKStructuredPointsData(datadir +"/hand-0.vtk");
        boost::scoped_ptr<RepresenterType> representer(RepresenterType::Create(reference));
        
        
        // We use the SurrogateDataManager, as we need to specify surrogate data in addition to the images.
        // We provide in addition to the representer also a file that contains a description of the surrogate
        // variables (e.g. whether they are categorical or continuous). See the API doc for more details.
        boost::scoped_ptr<DataManagerWithSurrogatesType> dataManager(DataManagerWithSurrogatesType::Create(representer.get(),
                                                                                                           datadir +"/surrogates/hand_surrogates_types.txt"));
        
        // add the data information. The first argument is the dataset, the second the surrogate information
        // and the 3rd the surrogate type
        
        // load the data and add it to the data manager. We take the first 4 hand images that we find in the data folder
        for (unsigned i = 0; i < 4; i++) {
            
            std::ostringstream ssFilename;
            ssFilename << datadir << "/hand-" << i << ".vtk";
            const std::string datasetFilename = ssFilename.str();
            
            std::ostringstream ssSurrogateFilename;
            ssSurrogateFilename << datadir << "/surrogates/hand-" << i << "_surrogates.txt";
            const std::string surrogateFilename = ssSurrogateFilename.str();
            
            vtkStructuredPoints* dataset = loadVTKStructuredPointsData(datasetFilename);
            
            
            // We provde the filename as a second argument.
            // It will be written as metadata, and allows us to more easily figure out what we did later.
            dataManager->AddDatasetWithSurrogates(dataset, datasetFilename, surrogateFilename);
            
            // it is save to delete the dataset after it was added, as the datamanager direclty copies it.
            dataset->Delete();
        }
        
        // Build up a list holding the conditioning information.
        typedef ConditionalModelBuilder<vtkStructuredPoints> ConditionalModelBuilderType;
        ConditionalModelBuilderType::CondVariableValueVectorType conditioningInfo;
        conditioningInfo.push_back(ConditionalModelBuilderType::CondVariableValuePair(true, 1));
        conditioningInfo.push_back(ConditionalModelBuilderType::CondVariableValuePair(false, 65));
        conditioningInfo.push_back(ConditionalModelBuilderType::CondVariableValuePair(true, 86.1));
        conditioningInfo.push_back(ConditionalModelBuilderType::CondVariableValuePair(true, 162.0));
        
        // Create the model builder and build the model
        boost::scoped_ptr<ConditionalModelBuilderType> modelBuilder(ConditionalModelBuilderType::Create());
        
        boost::scoped_ptr<StatisticalModelType> model(modelBuilder->BuildNewModel(dataManager->GetData(),
                                                                                  dataManager->GetSurrogateTypeInfo(),
                                                                                  conditioningInfo,
                                                                                  0.1));
        std::cout << "successfully built conditional model" << std::endl;
        
        // The resulting model is a normal statistical model, from which we could for example sample examples.
        // Here we simply  save it to disk for later use.
        model->Save(modelname);
        reference->Delete();
        std::cout << "save model as " << modelname << std::endl;
    } catch (StatisticalModelException& e) {
        std::cout << "Exception occured while building the conditional model" << std::endl;
        std::cout << e.what() << std::endl;
    }
}
