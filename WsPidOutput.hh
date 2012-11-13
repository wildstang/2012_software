
#ifndef __WsPidOutput_hh__
#define __WsPidOutput_hh__

class WsPidOutput
{
    public:
        // All WsPidOutput classes must implement this method
        virtual void pidWrite(float output) = 0;
};

#endif  // __WsPidOutput_hh__

