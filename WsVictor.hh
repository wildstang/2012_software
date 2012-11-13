#ifndef __WsVictor_hh__
#define __WsVictor_hh__

#include "Victor.h"

class WsVictor : public Victor
{
    public:
        explicit WsVictor(UINT32 channel);
        WsVictor(UINT8 moduleNumber, UINT32 channel);
        virtual ~WsVictor(void);
        
        // Re-implemented from WPI::Victor
        virtual void Set(float value, UINT8 syncGroup = 0);
        
        // If no file is set, no mapping is done
        void setMappingFileName(const char* pc_filename);
        
        
    private:
        bool   a_mapUserInputFlag;
        char*  ap_mappingFileName;
        int    a_mappingTableSize;
        float* ap_positiveInputMappingTable;
        float* ap_negativeInputMappingTable;
        
        WsVictor(const WsVictor& rc_rhs);
        WsVictor& operator=(const WsVictor& rc_rhs);
};

#endif // __WsVictor_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

