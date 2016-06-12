//
//  ofVTKExample.hpp
//
//  statismoのrunVTKExamplesをC++ライブラリとして実装するクラスです.
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/06/12.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ofVTKExample_hpp
#define ofVTKExample_hpp

/**
 @brief ofVTK Exampleクラス
 
 statismoのrunVTKExamplesをC++ライブラリとして実装するクラスです.
 */
class ofVTKExample
{
public:
    //! コンストラクタ
    ofVTKExample();
    //! デストラクタ
    virtual ~ofVTKExample();
    //! 初期化処理
    bool    Initialize();
    //! 終了処理
    void    Finalize();
    //! 実行
    void    Run();
};


#endif /* ofVTKExample_hpp */
