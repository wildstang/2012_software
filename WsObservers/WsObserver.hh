#ifndef __WsObserver_hh__
#define __WsObserver_hh__

#include <string>

class WsObserver
{
    public:
        WsObserver(std::string name);
        virtual ~WsObserver(void);
        
        virtual void update(void) = 0;
        std::string getName(void);
        
    private:
        std::string a_observerName;
        
    private:
        WsObserver(const WsObserver& rc_rhs);
        WsObserver& operator=(const WsObserver& rc_rhs);
};

#endif // __WsObserver_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

