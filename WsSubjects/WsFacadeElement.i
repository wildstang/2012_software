#include "WsFacadeElement.hh"
#include "WsObserver.hh"
#include "WsLogger.hh"
#include "Synchronized.h"


template <class TypeName>
WsFacadeElement<TypeName>::WsFacadeElement(std::string name,
                                           int         elementSize,
                                           TypeName*   p_defaultValue,
                                           SEM_ID      p_mutex)
    : WsSubject(name,p_mutex)
    , ap_data(NULL)
    , a_dataSize(elementSize)
{
    WS_LOG_TRACE("Begin");

    if (NULL == p_defaultValue)
    {
        WS_LOG_FATAL("A valid default value must be provided!!");
    }

    CRITICAL_REGION(this->getMutex())
    {
        // Create new data buffer and initialize it
        ap_data = new UINT8[a_dataSize];
        ::memcpy(ap_data, p_defaultValue, a_dataSize);
    }
    END_REGION

    WS_LOG_TRACE("End");
}

template <class TypeName>
WsFacadeElement<TypeName>::~WsFacadeElement(void)
{
    WS_LOG_TRACE("Begin");

    CRITICAL_REGION(this->getMutex())
    {
        delete [] ap_data;   ap_data = NULL;
        a_dataSize = 0;
    }
    END_REGION

    WS_LOG_TRACE("End");
}

template <class TypeName>
void
WsFacadeElement<TypeName>::getState(TypeName& r_state)
{
    CRITICAL_REGION(this->getMutex())
    {
        ::memcpy(&r_state, ap_data, a_dataSize);
    }
    END_REGION
}

template <class TypeName>
void
WsFacadeElement<TypeName>::setState(const TypeName& rc_state)
{
    const UINT8* pc_data = reinterpret_cast<const UINT8*>(&rc_state);

    CRITICAL_REGION(this->getMutex())
    {
        int cmp_result = ::memcmp(pc_data, ap_data, a_dataSize);
        if (0 != cmp_result)
        {
            // The new data is different than the old data
            ::memcpy(ap_data, pc_data, a_dataSize);
            this->notify();
        }
    }
    END_REGION
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

