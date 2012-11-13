#ifndef _WsAutoSleeperProgram_hh__
#define _WsAutoSleeperProgram_hh__

#include "WsAutonomousProgram.hh"

class WsAutoSleeperProgram : public WsAutonomousProgram
{
    public:
        WsAutoSleeperProgram(void);
        virtual ~WsAutoSleeperProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        
    private:
        WsAutoSleeperProgram(const WsAutoSleeperProgram& rc_rhs);
        const WsAutoSleeperProgram& operator=(const WsAutoSleeperProgram& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

