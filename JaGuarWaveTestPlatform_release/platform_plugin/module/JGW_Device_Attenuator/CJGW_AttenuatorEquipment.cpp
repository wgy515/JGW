#include "StdAfx.h"
#include <JGW_Device_Attenuator/CJGW_AttenuatorEquipment.h>
#include <JGW_NIGPIBControllImpl/equipcommtlv1_0_HIVE.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_gpib_agilent.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_AttenuatorEquipment::CCJGW_AttenuatorEquipment(void) : m_pEquipInter(NULL)
    {
    }


    CCJGW_AttenuatorEquipment::~CCJGW_AttenuatorEquipment(void)
    {
        CloseAttenuatorEquipment();
    }

    void CCJGW_AttenuatorEquipment::CloseAttenuatorEquipment()
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
    }

    void CCJGW_AttenuatorEquipment::SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument /* = 0 */)
    {
        char visaAddress[80] = {0};
        sprintf_s(visaAddress,"TCPIP0::%s::inst%d::INSTR",strDeviceName,nSubInstrument);
        SetVisa(visaAddress);
    }

    void CCJGW_AttenuatorEquipment::SetVisa(const char* visaAddress /* = "TCPIP0:::CMW50050-134371:::inst0:::INSTR" */)
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
        m_pEquipInter = new CEquipCommTL_VISA(visaAddress);
    }
    
    void CCJGW_AttenuatorEquipment::SetGPIB(BYTE bGPIBType /* = NI_GPIB_TYPE */, DWORD dwBoardIndex /* = 0 */, DWORD dwPrimaryAddress /* = 20 */,DWORD secAddress /* = 96 */)
    {
        CloseAttenuatorEquipment();

        if (AGILENT_GPIB_TYPE == bGPIBType)
        {
            m_pEquipInter = new CEquipCommTL_GPIB_Agilent( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
        else
        {       
            m_pEquipInter = new CEquipCommTLV1_0_HIVE( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
    }

    bool CCJGW_AttenuatorEquipment::AttenuatorEquipmentIsOpen()
    {
        return (NULL != m_pEquipInter);
    }

    void CCJGW_AttenuatorEquipment::SetAuttenuatorValue(float fAuttenuatorValue)
    {
        if (!m_pEquipInter) return ;

        std::string strCommand;
        JGW_FormatString(strCommand,"%0.2f",fAuttenuatorValue);
        m_pEquipInter->write(strCommand);
    }
}