//
//  KSStatismoBridge.h
//
//  クラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSStatismoBridge_h
#define KSStatismoBridge_h

#include <vtkDirectory.h>
#include <vtkPolyDataReader.h>

#include "DataManager.h"
#include "PCAModelBuilder.h"
#include "StatisticalModel.h"
#include "vtkStandardMeshRepresenter.h"

namespace Kosakasakas {
    
    /**
     @brief
    */
    class KSStatismoBridge
    {
    public:
        //! コンストラクタ
        KSStatismoBridge();
        //! デストラクタ
        virtual ~KSStatismoBridge();
        
        /**
         @brief 初期化
         
         @return 初期化の成否
         */
        bool    Initialize();
        /**
         @brief 終了処理
         */
        void    Finalize();
        
        void    vtkBuildShapeModelExample();
        
    private:
        typedef statismo::vtkStandardMeshRepresenter RepresenterType;
        typedef statismo::DataManager<vtkPolyData> DataManagerType;
        typedef statismo::PCAModelBuilder<vtkPolyData> ModelBuilderType;
        typedef statismo::StatisticalModel<vtkPolyData> StatisticalModelType;
    };
    
} //namespace Kosakasakas {

#endif /* KSStatismoBridge_h */
