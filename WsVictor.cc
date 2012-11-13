#include "WsVictor.hh"
#include "WsLogger.hh"
#include <stdio.h>
#include <errno.h>

#define LINE_BUFFER_SIZE 50

WsVictor::WsVictor(UINT32 channel)
    : Victor(channel)
    , a_mapUserInputFlag(false)
    , ap_mappingFileName(NULL)
    , a_mappingTableSize(-1)
    , ap_positiveInputMappingTable(NULL)
    , ap_negativeInputMappingTable(NULL)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}

WsVictor::WsVictor(UINT8 moduleNumber, UINT32 channel)
    : Victor(moduleNumber, channel)
    , a_mapUserInputFlag(false)
    , ap_mappingFileName(NULL)
    , a_mappingTableSize(-1)
    , ap_positiveInputMappingTable(NULL)
    , ap_negativeInputMappingTable(NULL)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}


WsVictor::~WsVictor(void)
{
    WS_LOG_TRACE("Begin");
    
    if (NULL != ap_mappingFileName)
    {
        delete [] ap_mappingFileName;
    }
    
    WS_LOG_TRACE("End");
}

void
WsVictor::Set(float value, UINT8 syncGroup)
{
    WS_LOG_TRACE("Begin");
    
    float mapped_value = 0.0f;
    
    if (true == a_mapUserInputFlag)
    {
        WS_LOG_DEBUG("Mapping Enabled");
        
        int idx = 0;
        float x0, x1, y0, y1 = 0.0f;
        float x, y, weight;
        
        if (value > 0.0f)
        {
            WS_LOG_DEBUG("Positive Mapping Needed, value = %.05f", value);
            
            // Get the index into the positive table
            idx = static_cast<int>(value * a_mappingTableSize);
            if (idx < a_mappingTableSize)
            {
                // Set up the initial known points on the line
                x0 = (float)idx / (float)a_mappingTableSize;
                x1 = (float)(idx + 1) / (float)(a_mappingTableSize);
                y0 = ap_positiveInputMappingTable[idx];
                y1 = ap_positiveInputMappingTable[idx + 1];
                
                //
                // Now do the mapping by using linear interpolation
                //  See http://en.wikipedia.org/wiki/Linear_interpolation for the algorithm
                //
                x = value;
                weight = (x - x0) / (x1 - x0);
                y = y0 + (weight * (y1 - y0));
            }
            else
            {
                // We are pegged at the top, just choose the top value
                y = ap_positiveInputMappingTable[idx];
            }
        }
        else
        {
            WS_LOG_DEBUG("Negative Mapping Needed, value = %.05f", value);
            
            // Get the index into the positive table
            idx = static_cast<int>((-1.0f * value) * a_mappingTableSize);
            if (idx < a_mappingTableSize)
            {
                // Set up the initial known points on the line
                x0 = -1.0f * (float)idx / (float)a_mappingTableSize;
                x1 = -1.0f * (float)(idx + 1) / (float)(a_mappingTableSize);
                y0 = ap_negativeInputMappingTable[idx];
                y1 = ap_negativeInputMappingTable[idx + 1];
                
                //
                // Now do the mapping by using linear interpolation
                //  See http://en.wikipedia.org/wiki/Linear_interpolation for the algorithm
                //
                x = value;
                weight = (x - x0) / (x1 - x0);
                y = y0 + (weight * (y1 - y0));
            }
            else
            {
                // We are pegged at the top, just choose the top value
                y = ap_negativeInputMappingTable[idx];
            }
        }
        
        // 'y' is the interpolated value, which is our mapped value
        mapped_value = y;
        WS_LOG_DEBUG("<x0,y0> <x1,y1> <x,y> = <%.05f,%.05f> <%.05f,%.05f> <%.05f,%.05f>",
                     x0, y0, x1, y1, x, y);
    }
    else
    {
        // No mapping requested, just pass it along
        WS_LOG_DEBUG("Mapping Disabled");
        mapped_value = value;
    }
    
    // Set the value back up in the base class...
    Victor::Set(mapped_value, syncGroup);
    
    WS_LOG_TRACE("End");
}

void
WsVictor::setMappingFileName(const char* pc_filename)
{
    WS_LOG_TRACE("Begin");
    
    // Clean up in case someone called this twice
    if (NULL != ap_mappingFileName)
    {
        delete [] ap_mappingFileName;
        ap_mappingFileName = NULL;
    }
    
    if (NULL != ap_positiveInputMappingTable)
    {
        delete [] ap_positiveInputMappingTable;
        ap_positiveInputMappingTable = NULL;
    }
    
    if (NULL != ap_negativeInputMappingTable)
    {
        delete [] ap_negativeInputMappingTable;
        ap_negativeInputMappingTable = NULL;
    }
    
    // Duplicate the incoming filename string for safe keeping
    int size = strlen(pc_filename);
    ap_mappingFileName = new char[size];
    ::memset(ap_mappingFileName, 0x00, size);
    ::strcpy(ap_mappingFileName, pc_filename);
    
    // Open the file so we can read the contents
    FILE* p_file = NULL;
    p_file = ::fopen(const_cast<const char*>(ap_mappingFileName), "r");
    
    // Check for a valid file handle
    if (NULL != p_file)
    {
        //
        // File opened successfully, read in the contents...
        //  The file format is as follows:
        //  # This is a comment...
        //  res=<NUMBER>
        //  float,float
        //  float,float
        //  ...
        //  float,float
        //
        int idx = 1;
        char line_buffer[LINE_BUFFER_SIZE];
        while (0 == feof(p_file))
        {
            ::memset(line_buffer, 0x00, sizeof(line_buffer));
            ::fgets(line_buffer, sizeof(line_buffer), p_file);
            
            if (('#' == line_buffer[0]) ||
                ('\n' == line_buffer[0]))
            {
                // Ignore, this is a blank or comment line
            }
            else if (0 == ::strncmp("res=", line_buffer, strlen("res=")))
            {
                // Found our resolution
                if (1 != ::sscanf(line_buffer, "res=%d\n", &a_mappingTableSize))
                {
                    WS_LOG_ERROR("Unrecognized line: %s", line_buffer);
                }
                
                // Break out of the while loop, start reading lines...
                break;
            }
            else
            {
                // Some other line...
                WS_LOG_ERROR("Unrecognized line before 'res=': %s", line_buffer);
                break;
            }
        } // end while()
        
        // Check to see if we read a good res value
        if (a_mappingTableSize > 0)
        {
            // Create the tables with N+1 entries (the 1st entry is always 0.0)
            ap_positiveInputMappingTable = new float[a_mappingTableSize + 1];
            ap_negativeInputMappingTable = new float[a_mappingTableSize + 1];
            ::memset(ap_positiveInputMappingTable, 0x00, sizeof(ap_negativeInputMappingTable));
            ::memset(ap_negativeInputMappingTable, 0x00, sizeof(ap_negativeInputMappingTable));
            
            // Read in the 'N' entries
            for (; (idx < a_mappingTableSize + 1) && (0 == feof(p_file)); idx++)
            {
                char line_buffer[LINE_BUFFER_SIZE];
                ::memset(line_buffer, 0x00, sizeof(line_buffer));
                ::fgets(line_buffer, sizeof(line_buffer), p_file);
                
                
                // Assume this is a value line... read it
                float pos = 0.0f;
                float neg = 0.0f;
                
                int scan_val = ::sscanf(line_buffer, "%f,%f\n", &pos, &neg);
                if (2 == scan_val)
                {
                    if (pos > 1.0)
                    {
                        WS_LOG_ERROR("Mapping data out of range!! pos = %.05f, idx = %d", pos, idx);
                        pos = 1.0;
                    }
                    
                    if (neg < -1.0)
                    {
                        WS_LOG_ERROR("Mapping data out of range!! neg = %.05f, idx = %d", neg, idx);
                        neg = -1.0;
                    }
                    
                    // Make sure we have a place to put this data
                    if ((NULL != ap_positiveInputMappingTable) &&
                        (NULL != ap_negativeInputMappingTable) &&
                        (a_mappingTableSize > 0))
                    {
                        ap_positiveInputMappingTable[idx] = pos;
                        ap_negativeInputMappingTable[idx] = neg;
                        WS_LOG_DEBUG("New values:  idx=%03d, %.05f,%.05f", idx, pos, neg);
                    }
                    else
                    {
                        // This is not expected...
                        WS_LOG_ERROR("Error when reading mapping file");
                        WS_LOG_ERROR("idx = %03d, ap_pimt = 0x%08x, ap_nimt = 0x%08x, a_mts = %d",
                                     idx,
                                     ap_positiveInputMappingTable,
                                     ap_negativeInputMappingTable,
                                     a_mappingTableSize);
                                     
                        // Get out of the loop
                        break;
                    }
                }
                else
                {
                    // sscanf failed, bail out
                    WS_LOG_ERROR("sscanf() failed, scan_val = %d", scan_val);
                    break;
                }
            } // end for()
            
            // Did we read successfully?  If so, enable mapping
            if (a_mappingTableSize == idx - 1)
            {
                a_mapUserInputFlag = true;
            }
        }
        else
        {
            // This is really bad...
            WS_LOG_ERROR("Invalid mapping file resolution detected:  %d", a_mappingTableSize);
            a_mapUserInputFlag = false;
        }
    }
    else
    {
        // This is really bad...
        WS_LOG_ERROR("Unable to open mapping file '%s', errno(%d): '%s'",
                     ap_mappingFileName,
                     errno,
                     ::strerror(errno));
        a_mapUserInputFlag = false;
    }
    
    // Don't forget to close the file
    if (NULL != p_file)
    {
        ::fclose(p_file);
    }
    
    WS_LOG_TRACE("End");
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

