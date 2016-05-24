//
//  Util.h
//
//  ユーティリティを定義するヘッダファイル
//
//  Copyright (c) 2016年 Takahiro Kosaka. All rights reserved.
//  Created by Takahiro Kosaka on 2016/04/05.
//
//  This Source Code Form is subject to the terms of the Mozilla
//  Public License v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KSUtil_h
#define KSUtil_h

#include "ofLog.h"
#include "../../../extAddons/Eigen/Dense"

namespace Kosakasakas
{
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
        
        /**
         @brief Eigen行列のシリアライズ
         */
        template <typename T, int R, int L>
        static std::string SerializeMat(const Eigen::Matrix<T, R, L>& M)
        {
            std::stringstream strm;
            
            // put the size of the serialized matrix
            strm << R << "," << L << ",";
            
            // put the elements of the serialized matrix
            for(int i=0; i<R; i++){
                for(int j=0; j<L; j++){
                    strm << M(i,j);
                    
                    // separator
                    if(!(i == R-1 && j == L-1))
                        strm << ",";
                }
            }
            
            return strm.str();
        }
        
        /**
         @brief Eigen行列のデシリアライズ
         */
        template <typename T>
        static Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> DeserializeMat(std::string str)
        {
            Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> M;
            int R, L;
            
            // sed "s/,/ /g" str
            replace(str.begin(), str.end(), ',', ' ');
            
            std::stringstream strm(str);
            // get the size of the output matrix
            strm >> R >> L;
            
            // get the elements of the output matrix
            M = decltype(M)(R, L);
            for(int i=0; i<R; i++){
                for(int j=0; j<L; j++){
                    strm >> M(i, j);
                }
            }
            
            return M;
        }

    };
}

#ifdef NDEBUG
#define ofASSERT(x,y) Kosakasakas::KSUtil::of_Assert(true, "", "", 0)
#else
#define ofASSERT(x,y) Kosakasakas::KSUtil::of_Assert(x, y , __FILE__, __LINE__ )
#endif

#endif /* KSUtil_h */
