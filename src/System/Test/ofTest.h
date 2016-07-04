//
//  ofTest.h
//
//  oFアプリでのテストクラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ofTest_h
#define ofTest_h

/**
 @brief oFアプリ層に乗せてのクラスを行うクラスです。
 */
class ofTest
{
public:
    //! コンストラクタ
    ofTest();
    //! デストラクタ
    ~ofTest();
    //! 初期化処理
    bool    Initialize();
    //! テスト実施
    bool    DoTest();
};

#endif /* ofTest_h */
