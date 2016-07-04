//
//  ofKsModel.cpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/01.
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ofKsModel.hpp"

using namespace Kosakasakas;

//! コンストラクタ
ofKsModel::ofKsModel()
{}

//! デストラクタ
ofKsModel::~ofKsModel()
{
    Finalize();
}

//! 初期化
bool    ofKsModel::Initialize()
{
    if (!LoadMesh())
    {
        ofLog(OF_LOG_ERROR, "メッシュの読み込みに失敗しました");
        return false;
    }
    
    // Identityをセット
    m_ModelMat.makeIdentityMatrix();
    
    return true;
}

//! 終了処理
void    ofKsModel::Finalize()
{
    m_Mesh.clear();
}

//! メッシュのロード
bool    ofKsModel::LoadMesh()
{
    return true;
}
