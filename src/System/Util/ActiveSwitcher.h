//
//  ActiveSwitcher.h
//  Face2Face
//
//  Created by Takahiro Kosaka on 2016/04/14.
//
//

#ifndef ActiveSwitcher_h
#define ActiveSwitcher_h

#include "memory.h"

namespace Kosakasakas
{
    template<class T>
    class ActiveSwitcher
    {
    public:
        ActiveSwitcher()
        : m_pActiveRef(nullptr)
        {};
        virtual ~ActiveSwitcher()
        {};
        inline bool    Initialize()
        {
            m_RefMap.clear();
            return true;
        }
        inline virtual void    Finalize()
        {
            m_RefMap.clear();
        }
        inline bool RegistObj(const char* name, T* pObj, bool isActive = false)
        {
            if(!name) return false;
            
            auto map    = m_RefMap.find(name);
            if (map != m_RefMap.end())
            {
                return false;
            }
            
            m_RefMap[name]  = pObj;
            
            if (isActive)
            {
                m_pActiveRef    = pObj;
            }
            return true;
        }
        inline bool RemoveObj(const char* name)
        {
            if(!name) return false;
            
            auto map    = m_RefMap.find(name);
            if (map == m_RefMap.end())
            {
                return true;
            }
            
            if((*map).second  == m_pActiveRef)
            {
                return false;
            }
            
            m_RefMap.erase(name);
            return true;
        }
        inline bool    Activate(const char* name)
        {
            if(!name) return false;
            
            auto map    = m_RefMap.find(name);
            if (map == m_RefMap.end())
            {
                return false;
            }
            m_pActiveRef    = (*map).second;
            return true;
        }
        inline T*  GetActiveObj()
        {
            return m_pActiveRef;
        }
        inline T*   GetObj(const char* name) const
        {
            return m_RefMap[name];
        }
        inline bool HasObj(const char* name) const
        {
            if(!name) return false;
            
            auto map    = m_RefMap.find(name);
            if (map == m_RefMap.end())
            {
                return false;
            }
            return true;
        }
        
    private:
        typedef std::shared_ptr<T>  TPtr;
        std::map<const char*, TPtr> m_RefMap;
        TPtr    m_pActiveRef;
    };
}



#endif /* ActiveSwitcher_h */
