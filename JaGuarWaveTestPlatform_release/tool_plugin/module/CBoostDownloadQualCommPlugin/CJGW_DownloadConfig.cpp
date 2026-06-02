#include "StdAfx.h"
#include "download_define.h"
#include "CJGW_DownloadConfig.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

firehose_protocol_t fh;
SIZE_T_64 MaxBytesToReadFromUSB = MAX_READ_BUFFER_SIZE;
namespace JGW
{

    void InitAttributes (void)
    {
        fh.attrs.MaxPayloadSizeToTargetInBytes      = ONE_MEGABYTE; // Target will force this to real value after <configure> tag
        fh.attrs.MaxPayloadSizeToTargetInBytesSupported = ONE_MEGABYTE; //ONE_MEGABYTE;
        fh.attrs.MaxPayloadSizeFromTargetInBytes    = 8192; //ONE_MEGABYTE;

        MaxBytesToReadFromUSB             = fh.attrs.MaxPayloadSizeFromTargetInBytes;

        fh.attrs.Verbose                    = 0;
        fh.attrs.MaxDigestTableSizeInBytes  = 8192;
        fh.attrs.AckRawDataEveryNumPackets  = 0;
        fh.attrs.delayinseconds             = 1;
        fh.attrs.address64                  = 0;
        fh.attrs.value64                    = 0;
        fh.attrs.storagedrive               = 0;
        fh.attrs.physical_partition_number  = 0;
        fh.attrs.SECTOR_SIZE_IN_BYTES       = 512;
        fh.attrs.byte_offset                = 0;
        fh.attrs.physical_partition_number  = 0;
        fh.attrs.size_in_bytes              = 0;
        fh.attrs.num_partition_sectors      = 0;
        fh.attrs.file_sector_offset         = 0;

        fh.attrs.ZlpAwareHost               = ZLPAWAREHOST;
        fh.attrs.SkipWrite                  = 0;
        fh.attrs.BufferWrites               = 0;
        //fh.attrs.AckRawData               = 0;
        fh.attrs.AlwaysValidate             = 0;

        //fh.attrs.display                    = 0;
        //fh.attrs.read_back_verify           = 0;
        fh.attrs.TestWritePerformance       = 0;
        fh.attrs.TestReadPerformance        = 0;
        fh.attrs.TestDigestPerformance      = 0;
        fh.attrs.SkipStorageInit            = 0;
        fh.attrs.SkipSettingMinClocks       = 0;
        fh.attrs.SkipSettingMaxClocks       = 0;
        fh.attrs.actual_size_in_bytes       = 0;

        //fh.attrs.start_sector               = 0;
        strncpy(fh.attrs.start_sector,"0",sizeof("0"));
        fh.attrs.start_sector[1] = '\0';
        strncpy(fh.attrs.MemoryName,"emmc",sizeof("emmc"));
        fh.attrs.MemoryName[strlen("emmc")] = '\0';
    }

    CCJGW_DownloadConfig::CCJGW_DownloadConfig(void)
    {
        InitAttributes();
        msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf = NULL;
        msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_len = 0;
        msDownloadAllInfo.mnFileSizeCounts = 0;
    }

    CCJGW_DownloadConfig::~CCJGW_DownloadConfig(void)
    {
        CleanLoadDownloadFileInfo();
    }

    void CCJGW_DownloadConfig::CleanLoadDownloadFileInfo()
    {
        for (std::vector<firehose_xml_config>::iterator it = msDownloadAllInfo.ms_firehose_download_info.begin();
            it != msDownloadAllInfo.ms_firehose_download_info.end();
            ++ it)
        {
            if (it->mfile_buf)
            {
                free(it->mfile_buf);
                it->mfile_buf = NULL;
            }
        }

        if (msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf)
        {
            free(msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf);
            msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf = NULL;
        }
        msDownloadAllInfo.ms_firehose_download_info.clear();
        msDownloadAllInfo.mnFileSizeCounts = 0;
        mstrErrorMsg = "";
    }

    bool CCJGW_DownloadConfig::LoadDonwloadConfig(ps_download_config_info psDonwloadConfigInfo,const char* szMemoryName /* = "emmc" */)
    {
        mpsDonwloadConfigInfo = psDonwloadConfigInfo;
        if (!psDonwloadConfigInfo) return false;
        CleanLoadDownloadFileInfo();

        strncpy(fh.attrs.MemoryName,szMemoryName,strlen(szMemoryName));
        fh.attrs.MemoryName[strlen(szMemoryName)] = '\0';

        return LoadArmPrgFile() && BuildFireHoseXMLConfig();
    }

    bool CCJGW_DownloadConfig::LoadArmPrgFile()
    {
        msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf = JGW_GetFileBufToPath_C(mpsDonwloadConfigInfo->mstrArmPrgFilePath.c_str(),msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_len);
        return (msDownloadAllInfo.ms_sahara_arm_prg_file_info.file_buf != NULL);
    }


    bool CCJGW_DownloadConfig::BuildFireHoseXMLConfig()
    {
        if (!BuildFristXMLConfig() 
            || !BuildRawProgramXMLConfig(mpsDonwloadConfigInfo->mvstrRawProgram)
            || !BuildPatchXMLConfig(mpsDonwloadConfigInfo->mvstrPatch)
            || !BuildSetActivePartitionXMLConfig()
            || !BuildPowerResetXMLConfig(mpsDonwloadConfigInfo->mbReset))
        {
            return false;
        }

        return true;
    }

    bool CCJGW_DownloadConfig::BuildFristXMLConfig()
    {
        firehose_xml_config firehose_xml;
        firehose_xml.mfile_buf = NULL;

        JGW::JGW_FormatString(firehose_xml.mstrXMLConfig,"%s<data>\n<configure MemoryName=\"%s\" Verbose=\"%"SIZE_T_FORMAT"\" AlwaysValidate=\"%"SIZE_T_FORMAT"\" MaxDigestTableSizeInBytes=\"%"SIZE_T_FORMAT"\" MaxPayloadSizeToTargetInBytes=\"%d\" ZlpAwareHost=\"%"SIZE_T_FORMAT"\" SkipStorageInit=\"%"SIZE_T_FORMAT"\" />\n</data>\n",
            xml_header,
            fh.attrs.MemoryName,
            fh.attrs.Verbose,
            fh.attrs.AlwaysValidate,
            fh.attrs.MaxDigestTableSizeInBytes,
            fh.attrs.MaxPayloadSizeToTargetInBytes,
            fh.attrs.ZlpAwareHost,
            fh.attrs.SkipStorageInit);
#ifdef _DEBUG
        OutputDebugStringA(firehose_xml.mstrXMLConfig.c_str());
#endif
        msDownloadAllInfo.ms_firehose_download_info.push_back(firehose_xml);

        return true;
    }

    bool CCJGW_DownloadConfig::BuildSetActivePartitionXMLConfig()
    {
        firehose_xml_config firehose_xml;
        firehose_xml.mfile_buf = NULL;

        JGW::JGW_FormatString(firehose_xml.mstrXMLConfig,"%s<data>\n<setbootablestoragedrive value=\"%i\" />\n</data>\n",xml_header,setactivepartition);
#ifdef _DEBUG
        OutputDebugStringA(firehose_xml.mstrXMLConfig.c_str());
#endif
        msDownloadAllInfo.ms_firehose_download_info.push_back(firehose_xml);

        return true;
    }
    /*
    <!-- 重启进入fastboot模式 -->
    <?xml version="1.0" ?><data><power value="reset_to_edl" /></data> 
    <!-- 关机 -->
    <?xml version="1.0" ?><data><power value="off" /></data> 

    <?xml version="1.0" encoding="UTF-8" ?><data><power DelayInSeconds="5" value="reset" /></data>
    <!-- 直接重启 -->
    <?xml version="1.0" encoding="UTF-8" ?><data><power value="reset" /></data>
    */
    bool CCJGW_DownloadConfig::BuildPowerResetXMLConfig(bool bReset)
    {
        if (!bReset) return true;
        firehose_xml_config firehose_xml;
        firehose_xml.mfile_buf = NULL;

        JGW::JGW_FormatString(firehose_xml.mstrXMLConfig,"%s<data>\n<power DelayInSeconds=\"10\" value=\"reset\" />\n</data>\n",xml_header);
#ifdef _DEBUG
        OutputDebugStringA(firehose_xml.mstrXMLConfig.c_str());
#endif
        msDownloadAllInfo.ms_firehose_download_info.push_back(firehose_xml);

        return true;
    }

    bool CCJGW_DownloadConfig::BuildRawProgramXMLConfig(std::vector<std::string>& vstrRawProgram)
    {
        for (std::vector<std::string>::iterator it = vstrRawProgram.begin();
            it != vstrRawProgram.end();
            ++ it)
        {
            if (!PareseRawProgramXMLConfigToFile(mpsDonwloadConfigInfo->mstrDownloadPath + "\\" + it[0])) return false;
        }
        return true;
    }

    bool CCJGW_DownloadConfig::BuildPatchXMLConfig(std::vector<std::string>& vstrPatch)
    {
        for (std::vector<std::string>::iterator it = vstrPatch.begin();
            it != vstrPatch.end();
            ++ it)
        {
            if (!ParesePatchXMLConfigToFile(mpsDonwloadConfigInfo->mstrDownloadPath + "\\" + it[0])) return false;
        }
        return true;
    }

    bool CCJGW_DownloadConfig::ParesePatchXMLConfigToFile(std::string strPatch)
    {
        char *xml_buffer = GetFileBufferToFilePath(strPatch);
        if (!xml_buffer)
        {
            JGW::JGW_FormatString(mstrErrorMsg,"当前文件打开失败(%s)",strPatch.c_str());
            return false;
        }
        int nend;
        char single_xml_buffer[MAX_STRING_SIZE];
        char* pstart = xml_buffer,* pend;
        //! 分割program 节点
        while(1)
        {
            firehose_xml_config firehose_xml;
            firehose_xml.mfile_buf = NULL;
            pstart = strstr(pstart,"<patch ");
            if (!pstart) break;

            pend = strstr(pstart,"/>");
            //! 如果没找到XML结束符，说明XML文件有问题
            if (!pend) 
            {
                JGW_FormatString(mstrErrorMsg,"XML文件解析错误(%s)",strPatch.c_str());
                //! dbg(LOG_ERROR,"xml config(patch) parse fial!!!");
                free(xml_buffer);
                return false;
            }
            nend = pend + 2 - pstart;

            //! 复制单个program 节点
            CopyString(single_xml_buffer,pstart,0,0,nend ,MAX_STRING_SIZE,strlen(pstart));
            pstart = pend + 2;
            //! skip DISK
            {
                GetProgramNodeTag(single_xml_buffer,"filename",fh.attrs.filename);
                if ( strncmp (fh.attrs.filename, "DISK", MAX_NUMBER (strlen (fh.attrs.filename), strlen ("DISK") ) ) != 0 )
                    continue;;  // **unless it is filename="DISK", skip it
            }

            JGW::JGW_FormatString(firehose_xml.mstrXMLConfig,"%s<data>\n%s\n</data>\n",xml_header,single_xml_buffer);
#ifdef _DEBUG
            OutputDebugStringA(firehose_xml.mstrXMLConfig.c_str());
#endif
            msDownloadAllInfo.ms_firehose_download_info.push_back(firehose_xml);
        }

        free(xml_buffer);
        return true;
    }

    bool CCJGW_DownloadConfig::GetFileBufferToFilePath(std::string& strPath,void* file_buf)
    {
        size_t file_size = 0; 
        file_buf = JGW_GetFileBufToPath_C(strPath.c_str(),file_size);
        return (NULL != file_buf);
    }

    char* CCJGW_DownloadConfig::GetFileBufferToFilePath(std::string& strPath)
    {
        size_t file_size = 0; 
        char* file_buf = JGW_GetFileBufToPath_C(strPath.c_str(),file_size,"r");
        return file_buf;
    }

    bool CCJGW_DownloadConfig::PareseRawProgramXMLConfigToFile(std::string strPath)
    {
        char *xml_buffer = GetFileBufferToFilePath(strPath);
        if (!xml_buffer)
        {
            JGW::JGW_FormatString(mstrErrorMsg,"当前文件打开失败(%s)",strPath.c_str());
            return false;
        }
        int nend;
        char single_xml_buffer[MAX_STRING_SIZE];
        char* pstart = xml_buffer,* pend;

        //! 分割program 节点
        while(1)
        {
            pstart = strstr(pstart,"<program ");
            if (!pstart) break;

            pend = strstr(pstart,"/>");
            //! 如果没找到XML结束符，说明XML文件有问题
            if (!pend) 
            {
                JGW_FormatString(mstrErrorMsg,"XML文件解析错误(%s)",strPath.c_str());
                //mstrErrorMsg = "xml config parse fial!!!";
                free(xml_buffer);
                return false;
            }
            nend = pend + 2 - pstart;
            //! 复制单个program 节点
            CopyString(single_xml_buffer,pstart,0,0,nend ,MAX_STRING_SIZE,strlen(pstart));
            pstart = pend + 2;
            if (!PareseProgramNode(single_xml_buffer))
            {
                free(xml_buffer);
                return false;
            }
        }

        free(xml_buffer);
        return true;
    }

    void CCJGW_DownloadConfig::GetProgramNodeTag(char* szprogramNode,const char* szAttr,char* tag)
    {
        char* pfilename = strstr(szprogramNode,szAttr);
        tag[0] = '\0';
        if (!pfilename) return;

        pfilename = strstr(pfilename,"\"");
        if (!pfilename || '\0' == *pfilename) return;
        pfilename++;
        char* ptemp = strstr(pfilename,"\"");
        if (!ptemp) return;
        int length = ptemp - pfilename;

        CopyString(tag,pfilename,0,0,length,MAX_STRING_SIZE,strlen(ptemp));
    }

    //! <program SECTOR_SIZE_IN_BYTES="512" file_sector_offset="0" filename="" label="ssd" num_partition_sectors="16" physical_partition_number="0" size_in_KB="8.0" sparse="false" start_byte_hex="0x8a80400" start_sector="283650" />
    bool CCJGW_DownloadConfig::PareseProgramNode(char* szprogramNode)
    {
        char filename[MAX_STRING_SIZE] = {0};
        char* ptemp;
        firehose_xml_config firehose_xml;
        firehose_xml.mfile_buf = NULL;
        std::string strTemp;
        size_t file_size = 0, FileSizeNumSectors = 0,num_partition_sectors = 0;
        int start;
        //! 检查SECTOR_SIZE_IN_BYTES 和 num_partition_sectors 属性是否存在
        if (NULL == strstr(szprogramNode,"SECTOR_SIZE_IN_BYTES") || 
            NULL == strstr(szprogramNode,"num_partition_sectors") ||
            NULL == strstr(szprogramNode,"filename")
            )
        {
            mstrErrorMsg = "file 节点未找到SECTOR_SIZE_IN_BYTES或者num_partition_sectors或filename属性,请检查配置文件";
            return false;
        }

        //! 获取文件名称
        {
            GetProgramNodeTag(szprogramNode,"filename",filename);
            if ('\0' == filename[0]) return true;
            firehose_xml.mstrProgramFilePath = mpsDonwloadConfigInfo->mstrDownloadPath + "\\" + filename;
        }
        //! 获取文件大小
        {
            file_size = JGW_GetFileSize_C(firehose_xml.mstrProgramFilePath.c_str());
            if (file_size <= 0) 
            {
                JGW_FormatString(mstrErrorMsg,"下载文件大小不能为空(%s)",firehose_xml.mstrProgramFilePath.c_str());
                return false;
            }
        }

        GetProgramNodeTag(szprogramNode,"SECTOR_SIZE_IN_BYTES",filename);
        firehose_xml.SectorSizeInBytes  = atoi(filename);
        /*firehose_xml.SectorSizeInBytes = SectorSizeInBytes;*/
        GetProgramNodeTag(szprogramNode,"num_partition_sectors",filename);
        num_partition_sectors  = atoi(filename);


        FileSizeNumSectors = (file_size + firehose_xml.SectorSizeInBytes - 1) / firehose_xml.SectorSizeInBytes;
        if (FileSizeNumSectors > num_partition_sectors) 
        {
           //! ::MessageBoxW(FindWindow(NULL,NULL),L"文件段大于分区段表",L"警告",MB_ICONWARNING);
          //! FileSizeNumSectors = num_partition_sectors;
        }

        ptemp = strstr(szprogramNode,"num_partition_sectors");
        start = ptemp - szprogramNode;
        CopyString(filename,szprogramNode,0,0,start,MAX_STRING_SIZE,strlen(szprogramNode));
        strTemp = filename;
        //! 过滤掉\"
        ptemp = strstr(ptemp,"\"");
        if (!ptemp)
        {
            JGW_FormatString(mstrErrorMsg,"num_partition_sectors属性值有错误");
            return false;
        }
        ptemp ++;
        ptemp = strstr(ptemp,"\"");
        if (!ptemp)
        {
            JGW_FormatString(mstrErrorMsg,"num_partition_sectors属性值有错误");
            return false;
        }
        ptemp ++;
        CopyString(filename,ptemp,0,0,strlen(ptemp),MAX_STRING_SIZE,strlen(ptemp));

        firehose_xml.mfile_buf_len = FileSizeNumSectors * firehose_xml.SectorSizeInBytes;
        msDownloadAllInfo.mnFileSizeCounts += (size_t)firehose_xml.mfile_buf_len;

        //! 是否缓存当前下载文件
        {
            if (firehose_xml.mfile_buf_len <= MAX_FILE_CACHE_BUFFER && firehose_xml.mfile_buf_len > 0)
            {
                firehose_xml.mfile_buf = (char*)calloc((size_t)firehose_xml.mfile_buf_len,1);
                memset(firehose_xml.mfile_buf,0x00,(size_t)firehose_xml.mfile_buf_len);
                if (!firehose_xml.mfile_buf || !JGW_GetFileBufToPath_BufLen_C(firehose_xml.mstrProgramFilePath.c_str(),(char*)firehose_xml.mfile_buf,file_size)) 
                {
                    JGW_FormatString(mstrErrorMsg,"读取文件内容错误(%s)",firehose_xml.mstrProgramFilePath.c_str());
                    return false;
                }
            }

            if (0 == firehose_xml.mfile_buf_len)
            {
                JGW_FormatString(mstrErrorMsg,"读取文件内容错误(%s)",firehose_xml.mstrProgramFilePath.c_str());
                return false;
            }
        }

        JGW::JGW_FormatString(firehose_xml.mstrXMLConfig,"%s<data>\n%snum_partition_sectors=\"%d\"%s\n</data>\n",xml_header,strTemp.c_str(),FileSizeNumSectors,filename);
#ifdef _DEBUG
        OutputDebugStringA(firehose_xml.mstrXMLConfig.c_str());
#endif
        msDownloadAllInfo.ms_firehose_download_info.push_back(firehose_xml);

        return true;
    }
}
