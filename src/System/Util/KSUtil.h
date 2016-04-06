//
//  Util.h
//
//  ユーティリティを定義するヘッダファイル
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//

#ifndef KSUtil_h
#define KSUtil_h

#include "ofLog.h"

class KSUtil
{
public:
    /**
     @brief oF用のアサート
     oFのコーディング規約だとアサートするなって書いてあるけど。
     */
    static void of_Assert(bool passed, const char* assert, const char* file, long line)
    {
        if (passed == false)
            ofLog(OF_LOG_ERROR, "fail in %s, at %ld. (%s)", file, line, assert);
    }
#ifdef NDEBUG
#define ofASSERT(x,y) of_Assert(true, "", "", 0)
#else
#define ofASSERT(x,y) of_Assert(x, y , __FILE__, __LINE__ )
#endif
};

#endif /* KSUtil_h */
