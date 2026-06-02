#include "StdAfx.h"
#include "TSE_SaveWiFiTestRecord.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_WlanExcelTemplateConfig.h"
#include "CJGW_WiFiTestResultRecord.h"
namespace JGW
{
    CTSE_SaveWiFiTestRecord::CTSE_SaveWiFiTestRecord(void) : mstrWiFiTestResultRecordEnvironment(L"TSE_WIFI_TEST_RESULT_RECORD")
    {
    }


    CTSE_SaveWiFiTestRecord::~CTSE_SaveWiFiTestRecord(void)
    {
    }

    const wchar_t* CTSE_SaveWiFiTestRecord::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_SaveWiFiTestRecord::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"WiFiTestResultRecordEnvironment",strParamName))
        {
            mstrWiFiTestResultRecordEnvironment =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ExcelParseIniFilePath",strParamName))
        {
            mstrExcelParseIniFilePath =  JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ExcelTemplateFilePath",strParamName))
        {
            mstrExcelTemplateFilePath =  JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveExcelFilePathCommand",strParamName))
        {
            mstrSaveExcelFilePathCommand =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TemperatureStatusEnvironment",strParamName))
        {
            mstrTemperatureStatusEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaveWiFiTestRecord::TSE_Init()
    {
        mvsExcelWiFiPos.clear();
        if (!JGW_FileExistsToFilePath(mstrExcelParseIniFilePath.c_str()))
        {
            LogE(L"Excel parse ini config file does not exist");
            return false;
        }
        if (!JGW_FileExistsToFilePath(mstrExcelTemplateFilePath.c_str()))
        {
            LogE(L"Excel Template file does not exist");
            return false;
        }

        CCJGW_WlanExcelTemplateConfig wlanExcelTemplateConfig;
        if (!wlanExcelTemplateConfig.LoadWlanExcelTemplateConfig(mstrExcelParseIniFilePath,mstrExcelTemplateFilePath,mvsExcelWiFiPos))
        {
            LogE_F(L"Load Wlan Excel Template Config Fail :%s",wlanExcelTemplateConfig.GetErrorMessage().c_str());
            mvsExcelWiFiPos.clear();
            return false;
        }
        return true;
    }

    bool CTSE_SaveWiFiTestRecord::TSE_Run()
    {
        std::wstring strSaveExcelFilePath (mstrSaveExcelFilePathCommand),strTemp;
        CCJGW_WiFiTestResultRecord* pWifiTestResultRecord = (CCJGW_WiFiTestResultRecord*)GetGlobalEnvironment()->GetInt(mstrWiFiTestResultRecordEnvironment);
        if (NULL == pWifiTestResultRecord || 0 == pWifiTestResultRecord->GetTRxTestData()->size())
        {
            LogE("Get WiFi Test Result Record Fail");
            return false;
        }

        GetEnvironmentCommandString(strSaveExcelFilePath);
        strSaveExcelFilePath = JGW_RealativePathToAbsPath(strSaveExcelFilePath.c_str());
        JGW_CreateDirectory(strSaveExcelFilePath.c_str(),false);
        if (!mcExcelLibXL.OpenExcel(mstrExcelTemplateFilePath))
        {
            LogE(L"Can not open template excel file");
            return false;
        }

        E_TEMP_STATUS eTempStatu = (E_TEMP_STATUS)GetGlobalEnvironment()->GetInt(mstrTemperatureStatusEnvironment);
        std::vector<S_TRX_TEST_DATA>* pvsTRxTestData = pWifiTestResultRecord->GetTRxTestData();
        for (std::vector<S_TRX_TEST_DATA>::iterator it = pvsTRxTestData->begin();
            it != pvsTRxTestData->end();
            ++ it)
        {
            for (std::vector<S_EXCEL_WIFI_POS>::iterator itPos = mvsExcelWiFiPos.begin();
                itPos != mvsExcelWiFiPos.end();
                ++itPos)
            {
                if (it->meChainMask == itPos->meChainMask && it->meDataRate == itPos->meDataRate
                    && it->muChannel == itPos->muChannel && it->meRateBw == itPos->meRateBw
                    && it->mePhyID == itPos->mePhyID)
                {
                    mcExcelLibXL.SetExcelSheet(JGW_A2W(itPos->mstrSheetName));
                    if (eTempStatu == E_TEMP_NORMAL_STATUS)
                    {
                        if (it->mfAvgTxPower != INVAILD_WIFI_RESULT)
                        {
                            JGW_FormatWString(strTemp,L"%.2lf",it->mfAvgTxPower);
                            mcExcelLibXL.WriteExcelContent(itPos->muTxPowerPos[E_EXCEL_ROW],itPos->muTxPowerPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                            JGW_FormatWString(strTemp,L"%s",it->mbSemResult ? "pass" : "fail");
                            mcExcelLibXL.WriteExcelContent(itPos->muTxSemPos[E_EXCEL_ROW],itPos->muTxSemPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                            JGW_FormatWString(strTemp,L"%.2lf",it->mfFreqErr);
                            mcExcelLibXL.WriteExcelContent(itPos->muTxFreqErrorPos[E_EXCEL_ROW],itPos->muTxFreqErrorPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                            JGW_FormatWString(strTemp,L"%.2lf",it->mfEvm);
                            mcExcelLibXL.WriteExcelContent(itPos->muTxEvmPos[E_EXCEL_ROW],itPos->muTxEvmPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                        }
                        if (it->mnPERPower != INVAILD_WIFI_RESULT)
                        {
                            JGW_FormatWString(strTemp,L"%d",it->mnPERPower);
                            mcExcelLibXL.WriteExcelContent(itPos->muRxPerPos[E_EXCEL_ROW],itPos->muRxPerPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                        }
                    }
                    else if (eTempStatu == E_TEMP_HIGH_STATUS)
                    {
                        if (it->mfAvgTxPower != INVAILD_WIFI_RESULT)
                        {
                            JGW_FormatWString(strTemp,L"%.2lf",it->mfAvgTxPower);
                            mcExcelLibXL.WriteExcelContent(itPos->muTxHighTmpPowerPos[E_EXCEL_ROW],itPos->muTxHighTmpPowerPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());

                            JGW_FormatWString(strTemp,L"%.2lf",it->mfFreqErr);
                            mcExcelLibXL.WriteExcelContent(itPos->muTxHighTmpFreqErrorPos[E_EXCEL_ROW],itPos->muTxHighTmpFreqErrorPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                        }
                    }
                    else if (eTempStatu == E_TEMP_LOW_STATUS)
                    {
                        JGW_FormatWString(strTemp,L"%.2lf",it->mfAvgTxPower);
                        mcExcelLibXL.WriteExcelContent(itPos->muTxLowTmpPowerPos[E_EXCEL_ROW],itPos->muTxLowTmpPowerPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());

                        JGW_FormatWString(strTemp,L"%.2lf",it->mfFreqErr);
                        mcExcelLibXL.WriteExcelContent(itPos->muTxLowTmpFreqErrorPos[E_EXCEL_ROW],itPos->muTxLowTmpFreqErrorPos[E_EXCEL_COL],libxl::CELLTYPE_STRING,(void*)strTemp.c_str());
                    }   
                }
            }
        }

        if (!mcExcelLibXL.SaveExcel(strSaveExcelFilePath))
        {
            LogE(L"Save wifi test result excel file fail");
            return false;
        }

        return true;
    }

    bool CTSE_SaveWiFiTestRecord::TSE_Exit()
    {
        if (mcExcelLibXL.CheckExcelIsOpen())
        {
            mcExcelLibXL.CloseExcel();
        }
        return true;
    }
}
