#include "StdAfx.h"
#include "CJGW_WlanTestDevThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <CJGW_WiFiTestResultRecord.h>
#include "CJGW_ParseQsprWiFiTestResultCSVFile.h"
#include "CJGW_WlanExcelTemplateConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    typedef enum
    {
        E_TEMP_NORMAL_STATUS = 0,
        E_TEMP_HIGH_STATUS = 1,
        E_TEMP_LOW_STATUS = 2
    }E_TEMP_STATUS;

    CCJGW_WlanTestDevThread::CCJGW_WlanTestDevThread(void)
    {
    }


    CCJGW_WlanTestDevThread::~CCJGW_WlanTestDevThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_WlanTestDevThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CCJGW_WlanTestDevThread::OnStartExecuteQsprCsvThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_WlanTestDevThread::OnStartExecuteQsprCsvThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)lParam;
        mpsWiFiTestTemplateParam = (LPS_WIFI_TEST_TEMPLATE_PARAM)wParam;
        CCJGW_WiFiTestResultRecord cWiFiTestResultRecord;
        CCJGW_ParseQsprWiFiTestResultCSVFile cParseQsprWiFiTestResultCSVFile;

        if (!cParseQsprWiFiTestResultCSVFile.LoadQsprWiFiTestResultCSVFile(JGW_W2A(mpsWiFiTestTemplateParam->mstrWiFiTestLogPath),cWiFiTestResultRecord))
        {
            MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,cParseQsprWiFiTestResultCSVFile.GetErrorMessage().c_str(),L"Error",MB_ICONERROR);
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
            return;
        }

        if (0 == cWiFiTestResultRecord.GetTRxTestData()->size())
        {
            MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,L"Get WiFi Test Result Record Fail",L"Error",MB_ICONERROR);
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
            return;
        }


        std::vector<S_EXCEL_WIFI_POS> mvsExcelWiFiPos;
        {
            CCJGW_WlanExcelTemplateConfig wlanExcelTemplateConfig;
            if (!wlanExcelTemplateConfig.LoadWlanExcelTemplateConfig(mpsWiFiTestTemplateParam->mstrExcelParseIniFilePath,mpsWiFiTestTemplateParam->mstrExcelTemplateFilePath,mvsExcelWiFiPos))
            {
                std::wstring strErrorMsg;
                JGW_FormatWString(strErrorMsg,L"Load Wlan Excel Template Config Fail :%s",wlanExcelTemplateConfig.GetErrorMessage().c_str());
                MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,strErrorMsg.c_str(),L"Error",MB_ICONERROR);
                mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
                return;
            }
        }
        CCJGW_ExcelLibXL mcExcelLibXL;
        std::wstring strTemp;
        JGW_CreateDirectory(mpsWiFiTestTemplateParam->mstrSaveTemplatePath.c_str(),false);
        if (!mcExcelLibXL.OpenExcel(mpsWiFiTestTemplateParam->mstrExcelTemplateFilePath))
        {
            MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,L"Can not open template excel file",L"Error",MB_ICONERROR);
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
            return;
        }

        E_TEMP_STATUS eTempStatu = (E_TEMP_STATUS)mpsWiFiTestTemplateParam->mnTemperatureIndex;
        std::vector<S_TRX_TEST_DATA>* pvsTRxTestData = cWiFiTestResultRecord.GetTRxTestData();
        for (std::vector<S_TRX_TEST_DATA>::iterator it = pvsTRxTestData->begin();
            it != pvsTRxTestData->end();
            ++ it)
        {
            for (std::vector<S_EXCEL_WIFI_POS>::iterator itPos = mvsExcelWiFiPos.begin();
                itPos != mvsExcelWiFiPos.end();
                ++itPos)
            {
                if (it->meRateBw == RateBW_11N_HT20 && it->muChannel > 4900)
                {
                    JGW_FormatWString(strTemp,L"%d",it->mnPERPower);
                }
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
                            JGW_FormatWString(strTemp,L"%s",it->mbSemResult ? L"pass" : L"fail");
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

        if (!mcExcelLibXL.SaveExcel(mpsWiFiTestTemplateParam->mstrSaveTemplatePath))
        {
            MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,L"Save wifi test result excel file fail",L"Error",MB_ICONERROR);
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
            return;
        }
        std::wstring strMsg;
        JGW_FormatWString(strMsg,L"File(%s) saved Successfully",mpsWiFiTestTemplateParam->mstrSaveTemplatePath.c_str());
        MessageBox(mpsWiFiTestTemplateParam->mhMainWindow,strMsg.c_str(),L"Ok",MB_OK);
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }
}

