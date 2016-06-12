//
//  KSVTKExample.h
//
//  statismoのVTK Exampleの関数群をC++ライブラリとして実装するクラスです.
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSVTKExample_h
#define KSVTKExample_h

namespace Kosakasakas {
    
    /**
     @brief VTK Exampleクラス
     
     statismoのVTK Exampleの関数群をC++ライブラリとして実装するクラスです.
    */
    class KSVTKExample
    {
    public:
        static void vtkBuildShapeModelExample(const char* srcDir,
                                              const char* dstName);
        static void vtkBuildIntensityModelExample(const char* dataDir,
                                                  const char* modelName);
        static void vtkBasicSamplingExample(const char* modelName,
                                            const char* resDir);
        static void vtkCrossValidationExample(const char* dataDir);
        static void vtkBuildPosteriorModelExample(const char* inModelName,
                                                  const char* inPartialShapeMeshName,
                                                  const char* inPosteriorModelName,
                                                  const char* outReconstructedShapeName);
        static void vtkBuildConditionalModelExample(const char* dataDir,
                                                    const char* modelName);
        
        
    private:
    };
    
} //namespace Kosakasakas {

#endif /* KSVTKExample_h */
