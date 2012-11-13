#include "WsSubject.hh"
#include "WsObserver.hh"
#include "WsLogger.hh"
#include "Synchronized.h"

WsSubject::WsSubject(std::string name, SEM_ID p_sem)
    : ap_observerListMutex(p_sem)
    , a_observerList()
    , a_subjectName(name)
{
    WS_LOG_TRACE("Begin");
    
    if (NULL == ap_observerListMutex)
    {
        WS_LOG_FATAL("A valid mutex must be provided!!");
    }
    
    WS_LOG_DEBUG("Created Subject 0x%08x, name = '%s'", (UINT32)this, a_subjectName.c_str());
    
    WS_LOG_TRACE("End");
}

WsSubject::~WsSubject(void)
{
    WS_LOG_TRACE("Begin");
    
    // Don't delete the observers, we don't own them
    a_observerList.clear();
    semFlush(ap_observerListMutex);
    semDelete(ap_observerListMutex);
    ap_observerListMutex = NULL;
    
    WS_LOG_DEBUG("Destroyed Subject 0x%08x, name = '%s'", (UINT32)this, a_subjectName.c_str());
    a_subjectName.clear();
    
    WS_LOG_TRACE("End");
}

void
WsSubject::attach(WsObserver* p_observer)
{
    WS_LOG_TRACE("Begin");
    
    // Check to make sure we weren't given garbage
    if (NULL == p_observer)
    {
        WS_LOG_ERROR("Cannot attach a NULL observer!!");
        return;
    }
    
    // Add this observer to the list
    CRITICAL_REGION(ap_observerListMutex)
    {
        a_observerList.push_back(p_observer);
        WS_LOG_DEBUG("WsObserver '%s' now watching WsSubject '%s'",
                     p_observer->getName().c_str(),
                     a_subjectName.c_str());
    }
    END_REGION
    
    WS_LOG_TRACE("End");
}

void
WsSubject::detach(WsObserver* p_observer)
{
    WS_LOG_TRACE("Begin");
    
    // Check to make sure we weren't given garbage
    if (NULL == p_observer)
    {
        WS_LOG_ERROR("Cannot detach a NULL observer!!");
        return;
    }
    
    // Remove this observer from the list
    CRITICAL_REGION(ap_observerListMutex)
    {
        a_observerList.remove(p_observer);
        WS_LOG_DEBUG("WsObserver '%s' no longer watching WsSubject '%s'",
                     p_observer->getName().c_str(),
                     a_subjectName.c_str());
    }
    END_REGION
    
    WS_LOG_TRACE("End");
}

void
WsSubject::notify(void)
{
    WS_LOG_TRACE("Begin");
    
    // Call update on each observer
    CRITICAL_REGION(ap_observerListMutex)
    {
        std::list<WsObserver*>::iterator iter = a_observerList.begin();
        std::list<WsObserver*>::iterator end_iter = a_observerList.end();
        
        while (iter != end_iter)
        {
            WsObserver* p_observer = *iter;
            p_observer->update();
            WS_LOG_DEBUG("WsSubject '%s' caused update on WsObserver '%s'",
                         a_subjectName.c_str(),
                         p_observer->getName().c_str());
                         
            // Next observer
            iter++;
        }
    }
    END_REGION
    
    WS_LOG_TRACE("End");
}

std::string
WsSubject::getName(void)
{
    return a_subjectName;
}

SEM_ID
WsSubject::getMutex(void)
{
    return ap_observerListMutex;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

