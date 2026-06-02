#include "StdAfx.h"
#include "TSE_UpdateExcelDatabase.h"
#include "CJGW_ExcelLibXL.h"

namespace JGW
{
    CTSE_UpdateExcelDatabase::CTSE_UpdateExcelDatabase(void) : mpsExcelContent(NULL),mnExcelHeaderPos(-1)
    {
    }


    CTSE_UpdateExcelDatabase::~CTSE_UpdateExcelDatabase(void)
    {
    }

    const wchar_t* CTSE_UpdateExcelDatabase::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_UpdateExcelDatabase\",\
                \"TestName\": \"加载EXCEL文件到内存中\",\
                \"EnvironmentExcelSource\": \"excel数据解析后保存的环境变量\",\
                \"ExcelHeader\": \"表头名称，在哪一列写入相关数据\",\
                \"EnvironmentValue\": \"写入数据对应的环境变量，写入位置是主KEY对应的行,必须与TSE_GetExcelDataToPrimaryKey搭配使用\"\
                }";
    }

    bool CTSE_UpdateExcelDatabase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentExcelSource"))
        {
            mstrEnvironmentExcelSource = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ExcelHeader"))
        {
            mstrExcelHeader = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentValue"))
        {
            mstrEnvironmentValue = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    int CTSE_UpdateExcelDatabase::GetHeaderPosToExcel()
    {
        for (size_t i = 0;i < mpsExcelContent->msvvExcelContent[0].size();i ++)
        {
            if (NULL == JGW_WStrComparenoCaseWStr(mpsExcelContent->msvvExcelContent[0][i].c_str(),mstrExcelHeader.c_str()))
            {
                return i;
            }
        }
        LogE_F(L"Excel 表头未发现当前字符串(%s)",mstrExcelHeader.c_str());
        return -1;
    }

    bool CTSE_UpdateExcelDatabase::TSE_Run()
    {
        mpsExcelContent = (PS_EXCEL_CONTENT)GetGlobalEnvironment()->GetInt(mstrEnvironmentExcelSource);
        if (!mpsExcelContent)
        {
            LogE(L"当前EXCEL数据源并不存在，请检查EXCEL文件是否加载成功");
            return false;
        }
        //! OPEN Excel
        CCJGW_ExcelLibXL excelLibXL;
        if (!excelLibXL.OpenExcel(mpsExcelContent->mstrExcelFilePath))
        {
            LogE_F(L"open excel fail(%s)(%s)",mpsExcelContent->mstrExcelFilePath.c_str(),JGW_A2W_A(excelLibXL.GetErrorMsg()));
            return false;
        }

        //! 如果主KEY对应的列为-1，则重新定位需要更新KEY
        if (-1 == mnExcelHeaderPos)
        {
            mnExcelHeaderPos = GetHeaderPosToExcel();
            if (-1 == mnExcelHeaderPos) return false;
        }

        if (!excelLibXL.WriteExcelContent(mpsExcelContent->mnPrimaryKeyPos,mnExcelHeaderPos,libxl::CELLTYPE_STRING,(void*)(GetGlobalEnvironment()->GetString(mstrEnvironmentValue).c_str())))
        {
            LogE_F(L"写入%s第%行，第%d列数据失败",mstrExcelHeader.c_str(),mpsExcelContent->mnPrimaryKeyPos,mnExcelHeaderPos);
            return false;
        }

        if (!excelLibXL.SaveExcel())
        {
            LogE_F(L"save excel fail(%s)(%s)",mpsExcelContent->mstrExcelFilePath.c_str(),JGW_A2W_A(excelLibXL.GetErrorMsg()));
            return false;
        }

        return true;
    }
}