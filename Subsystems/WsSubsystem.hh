#ifndef __WsSubsystem_hh__
#define __WsSubsystem_hh__

#include <string>

class WsSubsystem
{
    public:
        virtual ~WsSubsystem(void);
        virtual std::string GetName(void);
        
        // All WsSubsystems must implement this method
        virtual void update(void) = 0;
        
    protected:
        WsSubsystem(const char* pc_name);
        
    private:
        WsSubsystem(const WsSubsystem& rc_rhs);
        const WsSubsystem& operator=(const WsSubsystem& rc_rhs);
        
        std::string a_subsystemName;
};

#endif //__WsSubsystem_hh__


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

