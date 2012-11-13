#include "WsBtMotorValsObserver.hh"
#include "WsConfigFacade.hh"
#include "WsBallTransport.hh"
#include "WsLogger.hh"

WsBtMotorValsObserver::WsBtMotorValsObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtMotorValsObserver")
    , ap_ballTransport(p_ballTransport)
    , a_currentVals()
{
    // Zero-out the current config
    ::memset(&a_currentVals, 0x00, sizeof(a_currentVals));
    
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getBallTransportMotorValsConfig(a_currentVals);
    p_subject->attach(this);
}

WsBtMotorValsObserver::~WsBtMotorValsObserver(void)
{
    WsTypes::WsBallTransportMotorValueType new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getBallTransportMotorValsConfig(new_config);
    p_subject->detach(this);
}

void
WsBtMotorValsObserver::update(void)
{
    WsTypes::WsBallTransportMotorValueType new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsConfigFacade::instance()->getBallTransportMotorValsConfig(new_config);
    
    // Push the current value into the subsystem
    ap_ballTransport->acceptNewFixedMotorSpeeds(new_config);
    
    // Note the new value
    a_currentVals = new_config;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

