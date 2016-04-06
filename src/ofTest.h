//
//  ofTest.h
//
//  oFアプリでのテストクラス
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

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
