#ifndef __WsSubject_hh__
#define __WsSubject_hh__

#include "semLib.h"
#include <list>
#include <string>

class WsObserver;
class WsSubject
{
    public:
        WsSubject(std::string name, SEM_ID p_sem);
        virtual ~WsSubject(void);
        
        void attach(WsObserver* p_observer);
        void detach(WsObserver* p_observer);
        void notify(void);
        std::string getName(void);
        
    protected:
        SEM_ID getMutex(void);
        
    private:
        // For tracking all interested observers of this subject
        SEM_ID                  ap_observerListMutex;
        std::list<WsObserver*>  a_observerList;
        std::string             a_subjectName;
        
    private:
        WsSubject(const WsSubject& rc_rhs);
        WsSubject& operator=(const WsSubject& rc_rhs);
};

#endif // __WsSubject_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

