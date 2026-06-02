#include "StdAfx.h"
#include "TSE_WlanMemCommit.h"

namespace JGW
{
    CTSE_WlanMemCommit::CTSE_WlanMemCommit(void) : meTemplate(templateNone),meWriteOption(DataFile),meMemSize(MEM_AUTO),mbCompress(true),mbOverwrite(true),mstrSection("2,3,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1")
    {
        JGW_ParserIntA(mstrSection.c_str(),",",mvSectionId);
    }


    CTSE_WlanMemCommit::~CTSE_WlanMemCommit(void)
    {
    }

    const wchar_t* CTSE_WlanMemCommit::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanMemCommit\",\
                \"TestName\": \"Wlan Mem Commit\",\
                \"Template\": \"template number-> 0:no template; 20:qc98xx; 21:cus220; 22:cus223; 23:wb342; 24:xb340; 25:cus226; 26:cus226_030; 27:xb141\",\
                \"WriteOption\": \"NV memory type -> 0:auto; 1:flash; 2:eeprom; 3:otp; 4:none; 5:file;\",\
                \"MemSize\": \"memory size used for NV memory commit-> 0:auto; 1:1024(1k); 2:2048(2k); 3:4096(4k); 4:8192(8k);\",\
                \"Compress\": \"compressionthe NV data?\",\
                \"Overwrite\": \"overwrite existing data?\",\
                \"Section\": \"NV memory section that user wants to commit. SECTION_ALL=0;SECTION_ID=1;SECTION_MAC=2;SECTION_CAL2G_OLPC=3;SECTION_CAL5G_OLPC=4;SECTION_CTL2G=5;SECTION_CTL5G=6;SECTION_CONFIG=7;SECTION_CUSTOM=8;SECTION_VIDPID=9;SECTION_SDIOPID=10;SECTION_XTAL=11;SECTION_RXCAL5G=12;SECTION_RXCAL2G=13;SECTION_CAL2G_CLPC=14;SECTION_CAL5G_CLPC=15;SECTION_NONE=-1\"\
                }";
    }

    bool CTSE_WlanMemCommit::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Template",strParamName))
        {
            meTemplate = (NVMEM_TEMPLATE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WriteOption",strParamName))
        {
            meWriteOption = (NVMEM_OPTION)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"MemSize",strParamName))
        {
            meMemSize = (NVMEM_SIZE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Compress",strParamName))
        {
            mbCompress = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Overwrite",strParamName))
        {
            mbOverwrite = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Section",strParamName))
        {
            mvSectionId.clear();
            mstrSection = JGW_W2A(strParamValue);
            JGW_ParserIntA(mstrSection.c_str(),",",mvSectionId);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanMemCommit::TSE_Run()
    {
        byte iCompress2 = mbCompress ? 1 : 0;
        byte iOverwrite2 = mbOverwrite ? 1 : 0;

        byte array[33] = {0};
        for (size_t i = 0; i < 16 && i < mvSectionId.size(); i++)
        {
            if (mvSectionId[i] == SECTION_NONE)
            {
                array[i] = 0;
                break;
            }
            array[i] = (byte)mvSectionId[i];
        }

        return PHONE->TxCalMemCommit(meTemplate,meWriteOption,meMemSize,iCompress2,iOverwrite2,array);
    }
}

