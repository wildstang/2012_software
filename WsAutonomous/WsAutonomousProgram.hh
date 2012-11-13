#ifndef __WSAUTONOMOUSPROGRAM_HH__
#define __WSAUTONOMOUSPROGRAM_HH__

#include <string>

class WsAutonomousProgram
{
    public:
        WsAutonomousProgram(const char* name);
        virtual ~WsAutonomousProgram(void);
        
        virtual void init(void) = 0;
        virtual bool run(void) = 0;
        virtual void clear(void) = 0;
        
        virtual std::string getName(void);
        virtual bool isDone(void);
        
    protected:
        std::string a_programName;
};

#endif
