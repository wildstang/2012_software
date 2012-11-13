#ifndef __WsPidInput_hh__
#define __WsPidInput_hh__

class WsPidInput
{
    public:
        // All WsPidInput classes must implement this method
        virtual float pidRead(void) = 0;
};

#endif  // __WsPidInput_hh__

