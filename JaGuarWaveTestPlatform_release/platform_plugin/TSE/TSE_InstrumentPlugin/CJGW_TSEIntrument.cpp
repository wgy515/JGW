#include "StdAfx.h"
#include "CJGW_TSEIntrument.h"
#include <JGW_NIGPIBControllImpl/equipcommtlv1_0_HIVE.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_gpib_agilent.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <JGW_NIGPIBControllImpl/DebugEquipComm.h>
#include <JGW_NIGPIBControllImpl/EquipCommtl_TcpClient.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_tcp_ip.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_serial.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_usrdll.h>

#ifdef _DEBUG
#define DEBUG_TEST
#endif
namespace JGW
{
    //IEquipCommTL* CCJGW_TSEIntrument::m_pEquipInter = NULL;
    //int CCJGW_TSEIntrument::mnIEquipCommTLType = 0;
    //CCJGW_TSEIntrument gTSEIntrument;

    CCJGW_TSEIntrument::CCJGW_TSEIntrument(void) : m_pEquipInter(NULL),mnIEquipCommTLType(0)
    {
    }


    CCJGW_TSEIntrument::~CCJGW_TSEIntrument(void)
    {
        CloseIEquipCommTL();
    }

    int CCJGW_TSEIntrument::GetIEquipCommTLType()
    {
        return mnIEquipCommTLType;
    }

    void CCJGW_TSEIntrument::CloseIEquipCommTL()
    {
        if (m_pEquipInter)
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
            mnIEquipCommTLType = 0;
        }
    }

    IEquipCommTL* CCJGW_TSEIntrument::GetIEquipCommTL()
    {
        return m_pEquipInter;
    }

    IEquipCommTL* CCJGW_TSEIntrument::OpenIEquipCommTLToVisa(const std::string& strVisaAddress)
    {
        if (m_pEquipInter) return m_pEquipInter;
        CloseIEquipCommTL();
#ifdef DEBUG_TEST
        m_pEquipInter = new CDebugEquipComm();
#else
        m_pEquipInter = new CEquipCommTL_VISA(strVisaAddress);
#endif
        mnIEquipCommTLType = 1;
        return m_pEquipInter;
    }

    IEquipCommTL* CCJGW_TSEIntrument::OpenIEquipCommTLToGPIB(BYTE bGPIBType /* = NI_GPIB_TYPE */, DWORD dwBoardIndex /* = 0 */, DWORD dwPrimaryAddress /* = 20 */,DWORD secAddress /* = 96 */)
    {
        if (m_pEquipInter) return m_pEquipInter;
        CloseIEquipCommTL();
#ifdef DEBUG_TEST
        m_pEquipInter = new CDebugEquipComm();
#else
        if (AGILENT_GPIB_TYPE == bGPIBType)
        {
            m_pEquipInter = new CEquipCommTL_GPIB_Agilent( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
        else
        {       
            m_pEquipInter = new CEquipCommTLV1_0_HIVE( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
#endif
        mnIEquipCommTLType = 2;
        return m_pEquipInter;
    }

	IEquipCommTL* CCJGW_TSEIntrument::OpenIEquipCommTL(EquipmentDefinition* pEquipmentDefinition)
	{
		switch(pEquipmentDefinition->comm_link_type)
		{
		case EQUIP_COMM_TYPE_GPIB:
#if 0
			gpib_address
			gpib_sec_address
			gpib_boardid
#endif
			if (pEquipmentDefinition->isNiGpibType)
			{
				m_pEquipInter = new CEquipCommTLV1_0_HIVE( pEquipmentDefinition->address,pEquipmentDefinition->secAddress,pEquipmentDefinition->boardId );	
			}
			else
			{
				m_pEquipInter = new CEquipCommTL_GPIB_Agilent( pEquipmentDefinition->address,pEquipmentDefinition->secAddress,pEquipmentDefinition->boardId );
			}
			//if(mDllExported.instrument_gpib_create_session)
			//{
			//	inst = mDllExported.instrument_gpib_create_session(def.identifier.c_str(),def.address,def.secAddress,def.boardId);
			//}
			return m_pEquipInter;
		case EQUIP_COMM_TYPE_TCP_IP:
#if 0
			ip_address
			ip_inst_number
			ip_socket_port
			ip_source_address
#endif
			if (NetworkInstrumentProtocol == pEquipmentDefinition->eTcpIpClient)
			{
				m_pEquipInter = new CEquipCommTL_TCP_IP(pEquipmentDefinition->ip_address.c_str(),pEquipmentDefinition->inst_number,pEquipmentDefinition->source_address);
			}
			else
			{
				m_pEquipInter = new CEquipCommtl_TcpClient(pEquipmentDefinition->ip_address,pEquipmentDefinition->socketPort,pEquipmentDefinition->source_address);
			}
			
			//!  Network Instrument Protocol

				/*if(mDllExported.instrument_tcpip_create_session)
				{
				inst = mDllExported.instrument_tcpip_create_session(def.identifier.c_str(),def.ip_address.c_str(),def.inst_number);
				}*/
			return m_pEquipInter;
		case EQUIP_COMM_TYPE_SERIAL:
#if 0
			serial_com_port
#endif
			//if(mDllExported.instrument_rs232_create_session)
			//{
			//	inst = mDllExported.instrument_rs232_create_session(def.identifier.c_str(),def.comPortNumber);
			//}
			m_pEquipInter = new CEquipCommTL_Serial(pEquipmentDefinition->comPortNumber);
			return m_pEquipInter;
		case EQUIP_COMM_TYPE_VISA:
#if 0
			visa_address
#endif
			/*if(mDllExported.instrument_visa_create_session)
			{
			inst = mDllExported.instrument_visa_create_session(def.identifier.c_str(),def.visaAddress.c_str());
			}*/
			m_pEquipInter = new CEquipCommTL_VISA(pEquipmentDefinition->visaAddress);
			return m_pEquipInter;
		case EQUIP_COMM_TYPE_SIMULATED:
			m_pEquipInter = new CEquipCommTL_UsrDll(*pEquipmentDefinition);
			return m_pEquipInter;
		default:
			break;
		}

		return m_pEquipInter;
	}
}