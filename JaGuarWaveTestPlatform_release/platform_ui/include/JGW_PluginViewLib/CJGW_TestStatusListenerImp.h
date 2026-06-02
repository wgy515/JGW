#pragma once

class CCJGW_TestStatusListenerImp
{
public:
    virtual bool OnIdelTestStatus() = 0;
    virtual bool OnBusyTestStatus() = 0;
    virtual bool OnPassTestStatus() = 0;
    virtual bool OnFailTestStatus() = 0;
};