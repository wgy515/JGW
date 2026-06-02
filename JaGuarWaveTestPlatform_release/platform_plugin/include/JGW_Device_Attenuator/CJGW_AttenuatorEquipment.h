#pragma once
#include <JGW_NIGPIBControllImpl/iequipcommtl.h>
#include <JGW_Device_Attenuator/JGW_DEVICE_ATTENUATOR_DEFINE.h>

namespace JGW
{
    class JGW_DEVICE_ATTENUATOR_CLASS CCJGW_AttenuatorEquipment
    {
    public:
        CCJGW_AttenuatorEquipment(void);
        ~CCJGW_AttenuatorEquipment(void);
    public:
        virtual void CloseAttenuatorEquipment();

        virtual void SetGPIB(BYTE bGPIBType = NI_GPIB_TYPE, DWORD dwBoardIndex = 0, DWORD dwPrimaryAddress = 20,DWORD secAddress = 96);

        virtual bool AttenuatorEquipmentIsOpen();
//         virtual void SetTCPIP(const char* strIPAddress,int nSubInstrument = 0);
// 
        virtual void SetVisa(const char* visaAddress = "TCPIP0::CMW50050-134371::inst0::INSTR");
// 
        virtual void SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument = 0);
    public:
        //! dbm 
        virtual void SetAuttenuatorValue(float fAuttenuatorValue);
    protected:
        IEquipCommTL*	m_pEquipInter;  
    };
}