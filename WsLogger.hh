#ifndef __WsLogger_hh__
#define __WsLogger_hh__

struct sockaddr_in;
class WsLogger
{
    public:
        enum LogLevels
        {
            LEVEL_FATAL = 0,
            LEVEL_ERROR,
            LEVEL_WARNING,
            LEVEL_NOTICE,
            LEVEL_INFO,
            LEVEL_DEBUG,
            LEVEL_TRACE,
            LEVEL_ALWAYS,            // Logs at this level will always be posted
            
            // This is not a log level, this is just a counter
            NUM_LEVELS
        };
        typedef unsigned char LogLevelType;
        
    public:
        static WsLogger* instance(void);
        virtual ~WsLogger(void);
        
        void enableLoggingForLevel(const LogLevelType c_level);
        void enableLoggingForAllLevels(void);
        void disableLoggingForLevel(const LogLevelType c_level);
        void disableLoggingForAllLevels(void);
        void setLoggingBitmap(const unsigned char c_newBitmap);
        bool isLoggingEnabledForLevel(const LogLevelType c_level) const;
        
        void postEvent(const LogLevelType c_logLevel,
                       const char*        pc_fileName,
                       const char*        pc_functionName,
                       const int          c_lineNumber,
                       const char*        pc_formatString = "",
                       ...);
                       
                       
    private:
        void implementDeviceReset(const char* pc_message);
        
        // A 32-bit bitmap storing on/off for each logging level
        unsigned char       a_logLevelBitmap;
        unsigned short      a_numEventsPosted;
        int                 a_socketDescriptor;
        struct sockaddr_in* ap_sendToAddress;
        static WsLogger*    asp_instance;
        
        WsLogger();
        WsLogger(const WsLogger& rc_rhs);
        WsLogger& operator=(const WsLogger& rc_rhs);
};


// Convenience macro...
#define WS_LOG_FATAL(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_FATAL, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#define WS_LOG_ERROR(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_ERROR, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#define WS_LOG_WARNING(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_WARNING, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#define WS_LOG_NOTICE(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_NOTICE, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#define WS_LOG_INFO(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_INFO, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#define WS_LOG_DEBUG(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_DEBUG, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)

#define WS_LOG_TRACE(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_TRACE, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)

#define WS_LOG_ALWAYS(format, ...) \
    do \
    { \
        WsLogger::instance()->postEvent(WsLogger::LEVEL_ALWAYS, \
                                        __FILE__, \
                                        __FUNCTION__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__); \
    } while (false)


#endif // __WsLogger_hh__


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

