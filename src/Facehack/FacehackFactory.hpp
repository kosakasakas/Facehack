//
//  FacehackFactory.hpp
//  Facehack
//
//  Created by Takahiro Kosaka on 2016/07/04.
//
//

#ifndef FacehackFactory_hpp
#define FacehackFactory_hpp

#include "FacehackModel.hpp"
#include "FacehackParams.hpp"

namespace Facehack {

    typedef std::shared_ptr<FacehackParams> ParamsPtr;
    typedef std::shared_ptr<FacehackModel>  FacehackModelPtr;
    
    class FacehackFactory
    {
    public:
        FacehackFactory();
        virtual ~FacehackFactory();
        bool    Initialize(int width, int height);
        void    Finalize();
        
        FacehackModelPtr    Create(const ParamsPtr& pParam);
        FacehackModelPtr    Create();
    private:
        int m_Width;
        int m_Height;
    };
}

#endif /* FacehackFactory_hpp */
