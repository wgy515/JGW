#pragma once
#include <JGW_NIGPIBControllImpl/iequipcommtl.h>
namespace JGW
{
    class CCJGW_TSEIntrument
    {
    public:
        CCJGW_TSEIntrument(void);
        ~CCJGW_TSEIntrument(void);
    public:
        /*static*/ void CloseIEquipCommTL();
        //!
        /*static*/ IEquipCommTL* GetIEquipCommTL();
        //!0:未连接 1: Visa 2 : GPIB
        /*static*/ int GetIEquipCommTLType();
        //! 根据VISA获取仪器接口
        /*static*/ IEquipCommTL* OpenIEquipCommTLToVisa(const std::string& strVisaAddress);
        //! 根据GPIB获取仪器接口
        /*static*/ IEquipCommTL* OpenIEquipCommTLToGPIB(BYTE bGPIBType = NI_GPIB_TYPE, DWORD dwBoardIndex = 0, DWORD dwPrimaryAddress = 20,DWORD secAddress = 96);
	public:
		IEquipCommTL* OpenIEquipCommTL(EquipmentDefinition* pEquipmentDefinition);
    private:
       /*static*/ IEquipCommTL*	m_pEquipInter;
       /*static*/ int mnIEquipCommTLType;
    };
}