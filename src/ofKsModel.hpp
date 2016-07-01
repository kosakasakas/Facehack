//
//  ofKsModel.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/01.
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ofKsModel_hpp
#define ofKsModel_hpp

#include "ofMain.h"

namespace Kosakasakas
{
    /**
     @brief ofMeshと行列の管理クラス
     ofMeshと行列を管理するためのベースクラスです.
     */
    class ofKsModel
    {
    public:
        //! コンストラクタ
        ofKsModel();
        //! デストラクタ
        virtual ~ofKsModel();
        
        //! 初期化
        virtual bool    Initialize();
        //! 終了
        virtual void    Finalize();
        //! メッシュを取得
        const ofMesh& GetMesh() const
        {
            return m_Mesh;
        }
        //! モデル行列を取得
        const ofMatrix4x4& GetModelMatrixConst() const
        {
            return m_ModelMat;
        }
        //! モデル行列を取得
        ofMatrix4x4& GetModelMatrix()
        {
            return m_ModelMat;
        }
        
    protected:
        //! メッシュのロードのためのアブストラクトファクトリメソッド
        virtual bool    LoadMesh() = 0;
        
    protected:
        //! メッシュ
        ofMesh      m_Mesh;
        //! モデル行列
        ofMatrix4x4 m_ModelMat;
    };
}

#endif /* ofKsModel_hpp */
