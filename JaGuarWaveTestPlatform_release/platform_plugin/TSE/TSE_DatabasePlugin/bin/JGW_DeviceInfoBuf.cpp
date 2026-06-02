#include "StdAfx.h"
#include "JGW_DeviceInfoBuf.h"
#include <tinyxml/tinyxml.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_Encrypt.h>
namespace JGW
{
    CJGW_DeviceInfoBuf::CJGW_DeviceInfoBuf(void) : mpDeviceInfoBuf(NULL),mnDeviceInfoBinFileSize(2048)
    {
    }


    CJGW_DeviceInfoBuf::~CJGW_DeviceInfoBuf(void)
    {
        if (NULL != mpDeviceInfoBuf)
        {
            free (mpDeviceInfoBuf);
            mpDeviceInfoBuf = NULL;
        }
    }

    bool CJGW_DeviceInfoBuf::LoadDeviceInfoBuf(const std::wstring& strDeviceInfoFilePath,const std::wstring& strBaseXmlFilePath,bool isEncryptFile /* = false */)
    {
        mvEntryInfo.clear();
        if (NULL != mpDeviceInfoBuf)
        {
            free (mpDeviceInfoBuf);
            mpDeviceInfoBuf = NULL;
        }

        if (!LoadDeviceInfoBaseXmlFilePath(strBaseXmlFilePath))
        {
            LogE_F(L"Load Device Info Base Xml File Path Fail(%s)",strBaseXmlFilePath.c_str());
            return false;
        }

        mpDeviceInfoBuf = (char*)calloc(mnDeviceInfoBinFileSize + 1,1);
        //! device_info.bin 文件目录为空，或者当前文件不存在
        if (!strDeviceInfoFilePath.empty() && JGW_FileExistsToFilePath(strDeviceInfoFilePath.c_str()))
        {
            size_t fileSize = 0;
            char* buf = JGW_GetFileBufToPath_C(JGW_W2A(strDeviceInfoFilePath).c_str(),fileSize);
            if (fileSize <= 0 || NULL == buf)
            {
                LogE_F(L"Read Bin File(%s) Fail",strDeviceInfoFilePath.c_str());
                return false;
            }
            //! bin文件大小不能大于定义的文件大小
            if (fileSize > mnDeviceInfoBinFileSize)
            {
                LogE_F(L"The bin file(%s) is too large.(bin:%d,base:%d)",strDeviceInfoFilePath.c_str(),fileSize,mnDeviceInfoBinFileSize);
                return false;
            }
            if (isEncryptFile)
            {
                JGW_Decrypt(buf,fileSize);
            }
            memcpy_s(mpDeviceInfoBuf,mnDeviceInfoBinFileSize,buf,fileSize);
            free (buf);
        }
        //! 
        char* posBuf = mpDeviceInfoBuf;
        for (std::vector<S_EntryInfo>::iterator it = mvEntryInfo.begin();it != mvEntryInfo.end();++it)
        {
            it->mszEntryBuf = posBuf;
            posBuf += it->mnSize;
        }

        return true;
    }

    bool CJGW_DeviceInfoBuf::LoadDeviceInfoBaseXmlFilePath(const std::wstring& strBaseXmlFilePath)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(JGW_W2A(strBaseXmlFilePath).c_str()))
        {
            return false;
        }

        if(!(xmlRoot = ptrXmlDoc->RootElement()))
        {
            return false;
        }	

        std::wstring value;
        GetElementAttributeValue(xmlRoot,"size",value,L"2048");
        mnDeviceInfoBinFileSize = _ttoi(value.c_str());

        const TiXmlElement* pTempXmlNode = xmlRoot->FirstChildElement("entry");
        size_t bufLen = 0;
        while (pTempXmlNode)
        {
            S_EntryInfo sEntryInfo = {0};
            GetElementAttributeValue(pTempXmlNode,"name",value,L"");
            sEntryInfo.mstrEntryName = JGW_W2A(value,CP_UTF8);
            GetElementAttributeValue(pTempXmlNode,"size",value,L"32");
            sEntryInfo.mnSize = _ttoi(value.c_str());
            GetElementAttributeValue(pTempXmlNode,"hexNumber",value,L"0");
            sEntryInfo.mbIsHexNumber = 1 == _ttoi(value.c_str());

            bufLen += sEntryInfo.mnSize;
            mvEntryInfo.push_back(sEntryInfo);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("entry");
        }
        if (bufLen > mnDeviceInfoBinFileSize)
        {
            LogE_F(L"device info base file buf is too large(device_info size:%d,buf size:%d)",mnDeviceInfoBinFileSize,bufLen);
            return false;
        }

        return true;
    }

    bool CJGW_DeviceInfoBuf::UpdateDeviceInfo(const std::string& strEntryName,const std::string& strEntryValue)
    {
        for (std::vector<S_EntryInfo>::iterator it = mvEntryInfo.begin();it != mvEntryInfo.end();++it)
        {
            if (NULL == JGW_StrComparenoCaseStr(strEntryName.c_str(),it->mstrEntryName.c_str()))
            {
                //! 
                if (it->mbIsHexNumber)
                {
                    std::string val(strEntryValue);
                    if (0 != strEntryValue.length() % 2)
                    {
                        val = ("0");
                        val += strEntryValue;
                    }
                    memset(it->mszEntryBuf,0x00,it->mnSize);
                    for (size_t i = 0,index = 0;i < val.length();i += 2,index++ )
                    {
                        std::string strVal = val.substr(i,2);
                        it->mszEntryBuf[index] = static_cast<char>(strtol(strVal.c_str(), NULL, 16));
                    }
                }
                else
                {
                    memset(it->mszEntryBuf,0x00,it->mnSize);
                    //! B4C0F5D7639E
                    memcpy_s(it->mszEntryBuf,it->mnSize,strEntryValue.c_str(),strEntryValue.length() < it->mnSize ?strEntryValue.length():it->mnSize);
                }
                LogI_F(JGW_A2W(JGW_GetFormatString("Set Number:%s,Value:%s",strEntryName.c_str(),strEntryValue.c_str())).c_str());
                return true;
            }
        }
        LogI_F(JGW_A2W(JGW_GetFormatString("No current segment information was found(%s)",strEntryName.c_str())).c_str());
        return false;
    }

    bool CJGW_DeviceInfoBuf::SaveDeviceInfoBuf(const std::string& strSaveFilePath,bool isEncrypt /* = false */)
    {
        if (mnDeviceInfoBinFileSize > 0 && NULL != mpDeviceInfoBuf)
        {
            if (isEncrypt)
            {
                char* buf= new char[mnDeviceInfoBinFileSize];
                memset(buf,0x00,mnDeviceInfoBinFileSize);
                memcpy_s(buf,mnDeviceInfoBinFileSize,mpDeviceInfoBuf,mnDeviceInfoBinFileSize);
                JGW_Encrypt(buf,mnDeviceInfoBinFileSize);
                bool result = JGW_WriteFileToBuf(strSaveFilePath.c_str(),buf,mnDeviceInfoBinFileSize);
                delete[] buf;
                return result;
            }
            return JGW_WriteFileToBuf(strSaveFilePath.c_str(),mpDeviceInfoBuf,mnDeviceInfoBinFileSize);
        }

        return false;
    }

    bool CJGW_DeviceInfoBuf::ReadDeviceInfo(const std::string& strEntryName,std::string& strEntryValue)
    {
        for (std::vector<S_EntryInfo>::iterator it = mvEntryInfo.begin();it != mvEntryInfo.end();++it)
        {
            if (NULL == JGW_StrComparenoCaseStr(strEntryName.c_str(),it->mstrEntryName.c_str()))
            {
                ///memset(it->mszEntryBuf,0x00,it->mnSize);
                ///memcpy_s(it->mszEntryBuf,it->mnSize,strEntryValue.c_str(),it->mnSize);
                if (it->mbIsHexNumber)
                {
                    std::string strTemp;
                    for (size_t i = 0;i < it->mnSize;i ++)
                    {
                        strTemp += JGW_GetFormatString("%02X",(BYTE)(it->mszEntryBuf[i]));
                    }
                    strEntryValue = strTemp;
                }
                else
                {
                    std::string strTemp;
                    for (size_t i = 0;i < it->mnSize;i ++)
                    {
                        if (it->mszEntryBuf[i] == '\0') break;
                        strTemp += (char)(it->mszEntryBuf[i]);
                    }
                    strEntryValue = strTemp/*it->mszEntryBuf*/;
                }
                LogI_F(JGW_A2W(JGW_GetFormatString("Read Number:%s,Value:%s",strEntryName.c_str(),strEntryValue.c_str())).c_str());
                return true;
            }
        }
        LogI_F(JGW_A2W(JGW_GetFormatString("No current segment information was found(%s)",strEntryName.c_str())).c_str());
        return false;
    }






}
