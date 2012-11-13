#include "WsLogger.hh"
#include "Utility.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <errno.h>
#include <sockLib.h>
#include <inetLib.h>

// String array indexed by LogLevelType
static const char* LOG_LEVEL_TYPE_NAMES[WsLogger::NUM_LEVELS] =
{
    "FATAL",
    "ERROR",
    "WARNING",
    "NOTICE",
    "INFO",
    "DEBUG",
    "TRACE",
    "ALWAYS"
};


#define DEFAULT_LOGGING_BITMAP \
    (1 << WsLogger::LEVEL_FATAL) + \
    (1 << WsLogger::LEVEL_ERROR) + \
    (1 << WsLogger::LEVEL_WARNING) + \
    (1 << WsLogger::LEVEL_NOTICE) + \
    (1 << WsLogger::LEVEL_INFO) + \
    (1 << WsLogger::LEVEL_DEBUG) + \
    (1 << WsLogger::LEVEL_TRACE) + \
    (1 << WsLogger::LEVEL_ALWAYS)

#define LOG_METADATA_BUF_SIZE 120
#define LOG_DATA_BUF_SIZE     136

#define WS_LOGGER_SEND_PORT 17654
#define WS_LOGGER_SEND_ADDR 0xE0010B6A  // (224.1.11.106)


WsLogger* WsLogger::asp_instance = NULL;
WsLogger::WsLogger(void)
    : a_logLevelBitmap(DEFAULT_LOGGING_BITMAP)
    , a_numEventsPosted(0)
    , a_socketDescriptor(-1)
    , ap_sendToAddress(NULL)
{
    // Create a UDP socket for sending
    a_socketDescriptor = ::socket(AF_INET,
                                  SOCK_DGRAM,
                                  IPPROTO_UDP);
                                  
    // This socket should not be able to receive data
    ::shutdown(a_socketDescriptor, SHUT_RD);
    
    // Allow broadcast sending on this socket if not enabled by default
    char bcast_flag = 1;
    ::setsockopt(a_socketDescriptor,
                 SOL_SOCKET,
                 SO_BROADCAST,
                 &bcast_flag,
                 sizeof(bcast_flag));
                 
    // We will be sending data to the multicast address (224.1.11.106:17654)
    ap_sendToAddress = new struct sockaddr_in;
    ::memset(ap_sendToAddress, 0x00, sizeof(struct sockaddr_in));
    ap_sendToAddress->sin_family = AF_INET;
    ap_sendToAddress->sin_port = htons(WS_LOGGER_SEND_PORT);
    ap_sendToAddress->sin_addr.s_addr = htonl(WS_LOGGER_SEND_ADDR);
}

WsLogger::~WsLogger(void)
{
    a_logLevelBitmap = 0;
    a_numEventsPosted = 0;
    
    // Free up resources
    ::close(a_socketDescriptor);   a_socketDescriptor = -1;
    delete ap_sendToAddress;       ap_sendToAddress = NULL;
}

WsLogger*
WsLogger::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsLogger(); }
    return asp_instance;
}

void
WsLogger::enableLoggingForLevel(const LogLevelType c_level)
{
    if (c_level < NUM_LEVELS)
    {
        a_logLevelBitmap |= (1 << c_level);
    }
}

void
WsLogger::enableLoggingForAllLevels(void)
{
    for (int level = 0; level < WsLogger::NUM_LEVELS; level++)
    {
        a_logLevelBitmap |= (1 << level);
    }
}

void
WsLogger::disableLoggingForLevel(const LogLevelType c_level)
{
    // @todo:  This can disable the "ALWAYS" log, this is bad
    if (c_level < NUM_LEVELS)
    {
        a_logLevelBitmap &= ~(1 << c_level);
    }
}

void
WsLogger::disableLoggingForAllLevels(void)
{
    // @todo:  This can disable the "ALWAYS" log, this is bad
    for (int level = 0; level < WsLogger::NUM_LEVELS; level++)
    {
        a_logLevelBitmap &= ~(1 << level);
    }
}

void
WsLogger::setLoggingBitmap(const unsigned char c_newBitmap)
{
    a_logLevelBitmap = c_newBitmap;
}

bool
WsLogger::isLoggingEnabledForLevel(const LogLevelType c_level) const
{
    unsigned char masked_level = 0xFF;
    if (LEVEL_ALWAYS != c_level)
    {
        masked_level = (a_logLevelBitmap & (1 << c_level));
    }
    
    return !(0 == masked_level);
}

void
WsLogger::postEvent(const LogLevelType c_logLevel,
                    const char*        pc_fileName,
                    const char*        pc_functionName,
                    const int          c_lineNumber,
                    const char*        pc_formatString,
                    ...)
{
    // If the log level is not enabled, bail out early
    if (false == this->isLoggingEnabledForLevel(c_logLevel))
    {
        return;
    }
    
    //
    // If we get here, the log level is enabled, and we need to post the log.
    // Declare a string buffer for the log metadata and clear it out.
    //  Note:  We are using a stack-local variable for speed
    //
    char whence_data_buffer[LOG_METADATA_BUF_SIZE];
    ::memset(whence_data_buffer, 0x00, sizeof(whence_data_buffer));
    
    //
    // Here, we are retrieving the current time.  The current time will be sent
    //  as an ascii floating point string in the log data.
    //
    struct timespec tp;
    ::memset(&tp, 0x00, sizeof(struct timespec));
    
    double current_timestamp = 0.0;
    int status = ::clock_gettime(CLOCK_REALTIME, &tp);
    
    if (0 == status)
    {
        current_timestamp = (double) tp.tv_sec +
                            (double) tp.tv_nsec / 1000000000.0;
    }
    
    //
    // Here, we are hunting for the filename that was passed in.  Depending on
    //  the structure of the build environment and compiler, the filename might
    //  be a fully-qualified path, a relative path, or just the filename alone.
    //  So, we look for a directory separator character '/'.  If we find the '/'
    //  character, then the first letter of the filename is just after the last
    //  directory separator.
    //
    char* p_last_slash = ::strrchr(pc_fileName, '/');
    const char* pc_fname = (NULL == p_last_slash)
                           ? pc_fileName
                           : p_last_slash + 1;
                           
    //
    // Now, we need to look up the "stringified" version of the numerical log
    //  level.  These strings are only going to be valid if the passed-in log
    //  level is in range of the array that defines the log level names.
    //
    const char* pc_log_level_string =
        (c_logLevel < sizeof(LOG_LEVEL_TYPE_NAMES) / sizeof(LOG_LEVEL_TYPE_NAMES[0]))
        ? LOG_LEVEL_TYPE_NAMES[c_logLevel]
        : "UNKNOWN_LEVEL";
        
    //
    // Now that we have all of our log "metadata", let's create the "header"
    //  section of our log - this serves as an easy identifier as to what place
    //  in the code the event was posted from.
    //
    int bytes_written = 0;
    bytes_written = ::snprintf(whence_data_buffer,
                               sizeof(whence_data_buffer),
                               "0x%04x|%017.6f|%s|%s|%d|%s|",
                               ++a_numEventsPosted,
                               current_timestamp,
                               pc_log_level_string,
                               pc_fname,
                               c_lineNumber,
                               pc_functionName);
                               
    //
    // If we would have overrun the whence_data_buffer, back up and insert a '+'
    //  character so that this scenario is visible to the end-user.
    //
    if ((unsigned int)bytes_written >= sizeof(whence_data_buffer))
    {
        whence_data_buffer[sizeof(whence_data_buffer) - 3] = '+';
        whence_data_buffer[sizeof(whence_data_buffer) - 2] = '|';
    }
    
    // And make sure that our 'whence' buffer does not overrun
    whence_data_buffer[sizeof(whence_data_buffer) - 1] = '\0';
    
    //
    // At this point, we need to collect the actual log data.  So, we declare
    //  another character buffer for the log data and clear it out.
    //  Note:  We are using a stack-local variable (instead of new) for speed.
    //
    char log_data_buffer[LOG_DATA_BUF_SIZE];
    ::memset(log_data_buffer, 0x00, sizeof(log_data_buffer));
    
    //
    // Print all variable arguments to the log data buffer, while being careful
    //  not to overrun the buffer we have.
    //
    if (::strlen(pc_formatString) < sizeof(log_data_buffer))
    {
        va_list args;
        va_start(args, pc_formatString);
        bytes_written = ::vsnprintf(log_data_buffer,
                                    sizeof(log_data_buffer),
                                    pc_formatString,
                                    args);
        va_end(args);
    }
    else
    {
        //
        // The format string itself is bigger than the buffer we have reserved
        //  for the log data.  In this case, we cannot even attempt to parse the
        //  given data.  Instead, we will simply print an indication that the
        //  format string is not valid.
        //
        //  Note:  Because we are replacing the user-provided format string with
        //         the warning text below, the minimum size of the log data
        //         buffer is dictated by this warning text.
        //
        ::snprintf(log_data_buffer,
                   sizeof(log_data_buffer),
                   "%s",
                   "*** Invalid format string ***");
    }
    
    //
    // Here we are checking to see if the vsnprintf() function would have
    //  written more bytes than the buffer has room for.  If so, indicate that
    //  this truncation occurred by replacing the last visible character with a
    //  '+' character.  If there are a large number of log events that show this
    //  truncation indication ('+' character), then the developer should consider
    //  these possibilities:
    //  1.  Reduce the amount of data supplied in the various WS_LOG_*() macros.
    //  2.  Re-compile this code with LOG_DATA_BUF_SIZE increased.
    //
    
    //
    // If we would have overrun the log_data_buffer, back up and insert a '+'
    //  character so that this scenario is visible to the end-user.
    //
    if ((unsigned int)bytes_written >= sizeof(log_data_buffer))
    {
        log_data_buffer[sizeof(log_data_buffer) - 2] = '+';
    }
    
    // Make sure we don't overrun our buffer
    log_data_buffer[sizeof(log_data_buffer) - 1] = '\0';
    
    
    //
    // Here, we are assembling the complete log buffer.  This consists of the
    //  metadata, followed by the actual log data.
    //
    char log_buffer[LOG_METADATA_BUF_SIZE + LOG_DATA_BUF_SIZE];
    ::memset(log_buffer, 0x00, sizeof(log_buffer));
    ::snprintf(log_buffer,
               sizeof(log_buffer),
               "%s%s",
               whence_data_buffer,
               log_data_buffer);
               
    //
    // Here, we are sending the log data to the various destinations that are
    //  supported:
    //  1. stdout - not on cRIO - takes up WAY too much CPU
    //  2. UDP Network Socket
    //
    //  CONSOLE OUTPUT  ::printf("%s\n", log_buffer);
    //  CONSOLE OUTPUT  ::fflush(stdout);
    //
    int send_status =
        ::sendto(a_socketDescriptor,
                 log_buffer,
                 strlen(log_buffer),
                 0,
                 (struct sockaddr*) ap_sendToAddress,
                 sizeof(*ap_sendToAddress));
                 
    // Check if network send failed
    if (-1 == send_status)
    {
        ::printf("\nSENDTO ERROR... %d\n", errno);
    }
    
    //
    // Check to see if the caller told us the severity was FATAL, if so we need
    //  to make sure we can stop program execution here.
    //
    if (WsLogger::LEVEL_FATAL == c_logLevel)
    {
        this->implementDeviceReset(log_buffer);
    }
}

void
WsLogger::implementDeviceReset(const char* pc_message)
{
    //
    // This method will implement an appropriate "reset" algorithm for the
    //  target.  If this is the cRIO, the wpi_assert() method should do the
    //  trick.  If this is an Xcode/DevCpp/MinGw bench test, then the regular
    //  old assert should work.
    //
    wpi_suspendOnAssertEnabled(true);
    wpi_assertWithMessage(false, pc_message);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

