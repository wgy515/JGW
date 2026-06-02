#include "StdAfx.h"
#include "TSE_WiFiCalVFSTest.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CTSE_WiFiCalVFSTest::CTSE_WiFiCalVFSTest(void)
    {
    }


    CTSE_WiFiCalVFSTest::~CTSE_WiFiCalVFSTest(void)
    {
    }

    const wchar_t* CTSE_WiFiCalVFSTest::TSE_GetParamDescription()
    {
        return L"";
    }
    //<SuiteProject>
    //    <ModuleName paramVal="TSE_DevicesPlugin.dll" description="模块名称或路径，默认路径：EXE执行路径" />
    //    <RealName paramVal="CTSE_WiFiCalVFSTest" description="高通RF测试或者校准" />
    //    <TestName paramVal="WIFI校准综测" description="测试名称描述" />
    //    <Library paramVal="JGW_WiFiTestPlugin.dll" description="Qualcomm server DLL名称 默认路径：EXE执行路径" />
    //    <Config paramVal="cache\QSEQ\QSEQ_LYNX_H.xml" description="Qualcomm xtt file or xml file 默认路径：EXE执行路径" />
    //    <EquipConfig paramVal="" description="equip config"/>
    //    <LossConfig paramVal="" description="loss config" />
    //    <ErrorReTest paramVal="3" description="出现错误，重测次数" />
    //    <ErrorStop paramVal="1" description="1：出现错误立即停止 0：出现错误继续执行 直到此测试序列测试失败" />
    //    <RetryCount paramVal="1" description="错误重测次数"/>
    //    <SNEnvironment paramVal="TSE_READER_SN_NUMBER" description="SN环境变量名称" />
    //    <SaveTestLogging paramVal="1" description="保存测试记录" />
    //    <RecordCPKData paramVal="1" description="保存CPK数据"/>
    //    <CPKFilePathEnvironment paramVal="TSE_CPK_FILE_PATH" description="环境变量名称" />
    //    </SuiteProject>

    bool CTSE_WiFiCalVFSTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Library",strParamName))
        {
            mstrLibraryFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Config",strParamName))
        {
            mstrConfigFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"EquipConfig",strParamName))
        {
            mstrEquipConfig = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"LossConfig",strParamName))
        {
            mstrLossConfig = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ErrorReTest",strParamName))
        {
            mnErrorReTest = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ErrorStop",strParamName))
        {
            mbErrorStop = (0 == _ttoi(strParamValue)) ? false:true;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WiFiCalVFSTest::TSE_Run()
    {
        return false;
    }
}


