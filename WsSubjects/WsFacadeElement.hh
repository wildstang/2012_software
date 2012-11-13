#ifndef __WsFacadeElement_hh__
#define __WsFacadeElement_hh__

#include "WsSubject.hh"
#include "semLib.h"
#include <list>
#include <string>

template <class TypeName>
class WsFacadeElement : public WsSubject
{
    public:
        WsFacadeElement(std::string name,
                        int         elementSize,
                        TypeName*   p_defaultValue,
                        SEM_ID      p_mutex);
        virtual ~WsFacadeElement(void);
        
        void getState(TypeName& r_state);
        void setState(const TypeName& rc_state);
        
    private:
        UINT8*  ap_data;
        int     a_dataSize;
};

#include "WsFacadeElement.i"

#endif // __WsFacadeElement_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

