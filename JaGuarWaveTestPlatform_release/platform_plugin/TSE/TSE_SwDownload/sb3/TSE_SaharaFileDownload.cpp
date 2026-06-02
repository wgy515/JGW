#include "StdAfx.h"
#include "TSE_SaharaFileDownload.h"
#include "sw_sb3_download_define.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

#define MAX_TO_READ 1048576
#define MAX_TO_WRITE 1048576
namespace JGW
{
    int JGW_GetPageSize(void) 
    {
        static DWORD page_size = 0;
        if (0 == page_size) 
        {
            SYSTEM_INFO system_info;
            GetSystemInfo (&system_info);
            page_size = system_info.dwPageSize;
        }
        return page_size;
    }

    CTSE_SaharaFileDownload::CTSE_SaharaFileDownload(void) : mnNumberOfSaharaProtocolFileMapping(1),mpCSerialPort(NULL),mRxBuf(NULL),mTxBuf(NULL),mpDownloadFile(NULL)
    {
        mRxBuf = (char*)memalign (JGW_GetPageSize (), SAHARA_RAW_BUFFER_SIZE);
        mTxBuf = (char*)memalign (JGW_GetPageSize (), SAHARA_RAW_BUFFER_SIZE);
        mpsahara_header = (sahara_packet_header*)mRxBuf;
        mpsahara_hello = (sahara_packet_hello *)mRxBuf;
        mpsahara_hello_resp = (sahara_packet_hello_resp *)mTxBuf;
        mpsahara_end_image_tx = (sahara_packet_end_image_tx *)mRxBuf;
        mpsahara_read_data = (sahara_packet_read_data *)mRxBuf;

        //sahara_packet_read_data_64bit *sahara_read_data_64bit = (sahara_packet_read_data_64bit *)mRxBuf;
        //sahara_packet_read_data_64bit_better *sahara_read_data_64bit_better = (sahara_packet_read_data_64bit_better *)mRxBuf;
        /*sahara_packet_memory_debug *sahara_memory_debug = (sahara_packet_memory_debug *)mRxBuf;
        sahara_packet_memory_debug_64bit *sahara_memory_debug_64bit = (sahara_packet_memory_debug_64bit *)mRxBuf;
        dload_debug_type *sahara_memory_table_rx = (dload_debug_type *)mRxBuf;
        dload_debug_type_64bit *sahara_memory_table = (dload_debug_type_64bit *)sahara_data.misc_buffer;*/
        mpsahara_cmd_switch_mode = (sahara_packet_cmd_switch_mode *)mTxBuf;
        mpsahara_done_resp = (sahara_packet_done_resp *)mRxBuf;
        mpsahara_done = (sahara_packet_done *)mTxBuf;
        mpsahara_reset_resp = (sahara_packet_reset_resp *)mRxBuf;
    }


    CTSE_SaharaFileDownload::~CTSE_SaharaFileDownload(void)
    {
        if (mRxBuf)
        {
            ALIGNED_FREE(mRxBuf);
        }
        if (mTxBuf)
        {
            ALIGNED_FREE(mTxBuf);
        }
        CloseDownloadFile();
    }


    const wchar_t* CTSE_SaharaFileDownload::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaharaFileDownload\",\
                \"TestName\": \"Sahara File Download\",\
                \"SaharaSerialClassEnv\": \"get sahara serial class environment\",\
                \"DownloadFolderPathEnv\": \"Download Folder Path environment\",\
                \"NumberOfSaharaProtocolFileMapping\": \"ensure to provide the correct number of files listed. range: 1 ~ 15\",\
                \"img_id1\": \"index 1 image id\",\
                \"file_name1\": \"index 1 image file name\"\
                }";
    }

    bool CTSE_SaharaFileDownload::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaharaSerialClassEnv"))
        {
            mstrSaharaSerialClassEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadFolderPathEnv"))
        {
            mstrDownloadFolderPathEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"NumberOfSaharaProtocolFileMapping"))
        {
            mnNumberOfSaharaProtocolFileMapping = _ttoi(strParamValue);
        }
        else
        {
            mmapParameters[JGW_W2A(strParamName)] = JGW_W2A(strParamValue);
        }
        return true;
    }

    bool CTSE_SaharaFileDownload::TSE_Init()
    {
        mmapImageIdFileName.clear();
        std::string strImageIDKey,strImageFileNameKey,strImageIDValue,strImageFileNameValue;
        for (int i = 1;i <= mnNumberOfSaharaProtocolFileMapping;i ++)
        {
            JGW_FormatString(strImageIDKey,"img_id%d",i);
            JGW_FormatString(strImageFileNameKey,"file_name%d",i);
            strImageIDValue = mmapParameters[strImageIDKey];
            strImageFileNameValue =  mmapParameters[strImageFileNameKey];
            if (strImageIDValue.empty() || strImageFileNameValue.empty())
            {
                LogE(L"Image ID And Image File config Is not empty");
                return false;
            }
            mmapImageIdFileName[atoi(strImageIDValue.c_str())] = strImageFileNameValue;
        }
        return true;
    }

    bool CTSE_SaharaFileDownload::TSE_Run()
    {
        if (mnNumberOfSaharaProtocolFileMapping <= 0)
        {
            LogE(L"Number Of Partition Files out of Range(NumberOfSaharaProtocolFileMapping must  > 0)");
            return false;
        }

        mpCSerialPort = (CCJGW_QcommDownloadSerialPort*)GetGlobalEnvironment()->GetInt(mstrSaharaSerialClassEnv);
        if (NULL == mpCSerialPort)
        {
            LogE_F(L"Could not connect to the Resource Context");
            return false;
        }
        msahara_state = SAHARA_WAIT_HELLO;
        CloseDownloadFile();
        mstrDownloadFolder = GetGlobalEnvironment()->GetAnsiString(mstrDownloadFolderPathEnv);
        bool result = ReadHandlePacket();
        CloseDownloadFile();
        return result;

        //! Target initiates protocol by sending HELLO packet with Image Transfer mode
        /*mpsahara_hello->version = 2;
        mpsahara_hello->mode = 0x00;
        memset(mTxBuf,0x00,sizeof(sahara_packet_hello));
        if (ReadPacket(0))
        {
        if (mpsahara_hello_resp->header.command != SAHARA_HELLO_RESP_ID)
        {
        LogE(L"read sahara hello resp id command fail");
        return false;
        }
        }
        else
        {
        LogI(L"warning:read sahara hello command fail");
        }
        mpsahara_hello_resp->header.command = SAHARA_HELLO_RESP_ID;
        mpsahara_hello_resp->header.length = sizeof(sahara_packet_hello_resp);
        mpsahara_hello_resp->version = mpsahara_hello->version;
        mpsahara_hello_resp->version_supported = mpsahara_hello->version;
        mpsahara_hello_resp->status = SAHARA_STATUS_SUCCESS;
        mpsahara_hello_resp->mode = mpsahara_hello->mode;

        bool result = SendHelloResp(SAHARA_MODE_IMAGE_TX_PENDING);
        CloseDownloadFile();
        return result;*/
    }

    void CTSE_SaharaFileDownload::CloseDownloadFile()
    {
        if (mpDownloadFile)
        {
            fclose(mpDownloadFile);
            mpDownloadFile = NULL;
        }
    }

    bool CTSE_SaharaFileDownload::SendHelloResp(boot_sahara_mode mode)
    {
        return WriteSerialPort(mTxBuf,sizeof(sahara_packet_hello_resp));
    }

    bool CTSE_SaharaFileDownload::WriteSerialPort(char* buffer,size_t bytes_to_send)
    {
        if (mpCSerialPort)
        {
            size_t temp_bytes_sent = 0;
            size_t bytes_sent = 0;

            while (bytes_sent < bytes_to_send)
            {
                if(!mpCSerialPort->WriteBufferToDownloadPort(buffer + bytes_sent, min(bytes_to_send - bytes_sent,MAX_TO_WRITE),temp_bytes_sent)) 
                {
                    LogE(L"Error occurred while writing to COM port");
                    return false;
                }
                else 
                {
                    bytes_sent += temp_bytes_sent;
                }

            }
            return true;
        }
        return false;
    }

    bool CTSE_SaharaFileDownload::ReadSerialPort(char* buffer, size_t bytes_to_read, size_t& bytes_read)
    {
        if (mpCSerialPort)
        {
            //! 1048576
            if(mpCSerialPort->ReadDownloadPort(buffer,min(bytes_to_read, MAX_TO_READ), bytes_read))
            {
                return true;
            }
        }
        return false;
    }

    bool CTSE_SaharaFileDownload::ReadPacket(size_t bytes_to_read)
    {
        //bool retval = false;
        sahara_packet_header* command_packet_header = NULL;
        size_t temp_bytes_read = 0, bytes_read = 0;

        // Treat byte_to_read 0 as asking for a command packet, since the caller anyway cannot predict the size
        // of the incoming command. We read 8 bytes below, which is the command followed by the length
        if (0 == bytes_to_read) 
        {
            if (false == ReadSerialPort(mRxBuf, sizeof(sahara_packet_header), temp_bytes_read)) 
            {	// sahara_packet_header is 4 byte command, 4 byte packet length
                return false;
            }
            else 
            {
                command_packet_header = (sahara_packet_header *) mRxBuf;
                LogI_F(L"Read %d bytes, Header indicates command %d and packet length %d bytes", temp_bytes_read,command_packet_header->command,command_packet_header->length);

                bytes_read += temp_bytes_read;
                if (bytes_read != sizeof(sahara_packet_header)) 
                {
                    LogE_F(L"Unable to read packet header. Only read %d bytes.", temp_bytes_read);
                    return false;
                }
                command_packet_header = (sahara_packet_header *) mRxBuf;

                LogI_F(L"Command packet length %d", command_packet_header->length);
                LogI_F(L"SAHARA_RAW_BUFFER_SIZE length %d", SAHARA_RAW_BUFFER_SIZE);

                if (command_packet_header->length > SAHARA_RAW_BUFFER_SIZE) 
                {
                    LogE_F("Command packet length %d too large to fit", command_packet_header->length);
                    return false;
                }
                bytes_to_read = command_packet_header->length;
            }
        }
        while (bytes_read < bytes_to_read) {
            if (false == ReadSerialPort(mRxBuf + bytes_read, bytes_to_read - bytes_read, temp_bytes_read))
                return false;
            else
                bytes_read += temp_bytes_read;
        }
        return true;
    }

    bool CTSE_SaharaFileDownload::ReadHandlePacket()
    {
        while (1)
        {
            switch (msahara_state)
            {
            case SAHARA_WAIT_HELLO:
                mpsahara_hello->version = 2;
                mpsahara_hello->mode = 0x00;
                memset(mTxBuf,0x00,sizeof(sahara_packet_hello));
                if (ReadPacket(0))
                {
                    if (mpsahara_hello->header.command != SAHARA_HELLO_ID)
                    {
                        LogE(L"read sahara hello resp id command fail");
                        return false;
                    }
                }
                else
                {
                    LogI(L"warning:read sahara hello command fail");
                }
                mpsahara_hello_resp->header.command = SAHARA_HELLO_RESP_ID;
                mpsahara_hello_resp->header.length = sizeof(sahara_packet_hello_resp);
                mpsahara_hello_resp->version = mpsahara_hello->version;
                mpsahara_hello_resp->version_supported = mpsahara_hello->version;
                mpsahara_hello_resp->status = SAHARA_STATUS_SUCCESS;
                mpsahara_hello_resp->mode = mpsahara_hello->mode;
                msahara_state = SAHARA_WAIT_COMMAND;
                SendHelloResp(SAHARA_MODE_IMAGE_TX_PENDING);
                break;
            case SAHARA_WAIT_COMMAND:
                if (false == ReadPacket(0))
                {
                    return false;
                }
                LogI(L"STATE <-- SAHARA_WAIT_COMMAND");
                if (SAHARA_END_IMAGE_TX_ID == mpsahara_header->command)
                {
                    CloseDownloadFile();
                    if (!is_ack_successful(mpsahara_end_image_tx->status))
                    {
                        //! LogE("SENDING --> SAHARA_RESET\n");
                        //send_sahara_reset();
                        LogE(L"SAHARA_END_IMAGE_TX Fail");
                        return false;
                    }
                    if (!SendDone())
                    {
                        LogE(L"Sending DONE packet failed");
                        return false;
                    }
                }
                else if (SAHARA_READ_DATA_ID == mpsahara_header->command)
                {
                    if (!SaharaStartImageTransfer())
                    {
                        return false;
                    }
                }
                break;
            case SAHARA_WAIT_DONE_RESP:
                if (false == ReadPacket(0))
                    return false;

                LogI_F(L"SAHARA_WAIT_DONE_RESP recieved with SAHARA_MODE_IMAGE_TX_PENDING=0x%.8X\n",SAHARA_MODE_IMAGE_TX_PENDING);
                LogI(L"STATE <-- SAHARA_WAIT_DONE_RESP\n");
                CloseDownloadFile();

                if(mnCurrentImageId==13)
                {
                    LogI(L"NOTE: Target requested image 13 which is DeviceProgrammer. Forcing QUIT. This is by design, ** All is well ** SUCCESS!!\n\n");
                    mpsahara_done_resp->image_tx_status = SAHARA_MODE_IMAGE_TX_COMPLETE;
                }

                if (SAHARA_MODE_IMAGE_TX_PENDING == mpsahara_done_resp->image_tx_status) 
                {
                    LogI(L"Still More images to be uploaded, entering Hello wait state");
                    msahara_state = SAHARA_WAIT_HELLO;
                }
                else if (SAHARA_MODE_IMAGE_TX_COMPLETE == mpsahara_done_resp->image_tx_status) 
                {
                    LogI(L"\n\nSuccessfully uploaded all images\n");
                    //delete_file_from_list(id_mapped_file); // taken care of in destroy_input_file_list()
                    return true;
                }
                else 
                {
                    LogE_F(L"Received unrecognized status %d at SAHARA_WAIT_DONE_RESP state",  mpsahara_done_resp->image_tx_status);
                    return false;
                }
                break;
            default:
                LogE_F(L"Unrecognized command %d",  mpsahara_header->command);
                return false;
            }
        }
        return true; 
    }

    bool CTSE_SaharaFileDownload::SendDone()
    {
        mpsahara_done->header.command = SAHARA_DONE_ID;
        mpsahara_done->header.length = sizeof(sahara_packet_done);
        msahara_state = SAHARA_WAIT_DONE_RESP;
        return WriteSerialPort(mTxBuf,sizeof(sahara_packet_done));
    }

    bool CTSE_SaharaFileDownload::SaharaStartImageTransfer()
    {
        if (NULL == mpDownloadFile)
        {
            std::string strFilePath (mstrDownloadFolder);
            std::map<int,std::string>::iterator it = mmapImageIdFileName.find(mpsahara_read_data->image_id);
            if (it == mmapImageIdFileName.end())
            {
                LogE_F(L"Matching input image for ID %d not found", mpsahara_read_data->image_id);
                return false;
            }
            mnCurrentImageId = mpsahara_read_data->image_id;
            strFilePath += "\\" ;
            strFilePath += it->second;
            mpDownloadFile = fopen(strFilePath.c_str(),"rb");
            if (!mpDownloadFile) 
            {
                LogE_F(L"Open File(%s) Fail",JGW_A2W(strFilePath).c_str());
                return false;
            }
        }

        size_t file_size = JGW_GetFilePtrFileSize_C(mpDownloadFile);
        uint32_t DataOffset = mpsahara_read_data->data_offset;
        uint32_t DataLength = mpsahara_read_data->data_length;
        if (0 == file_size || 0 == mpsahara_read_data->data_length || (DataOffset + DataLength) > file_size)
        {
            LogE_F(L"Invalid length %ud bytes request to be transmitted", DataLength);
            LogE_F(L"File offset %ud, file size %ud", DataOffset,file_size );
            return false;
        }

        fseek(mpDownloadFile, (long)DataOffset, SEEK_SET); // 1
        uint32_t bytes_read = 0, bytes_to_read_next;
        while (bytes_read < DataLength) 
        {
            bytes_to_read_next = min((uint32_t)DataLength - bytes_read, SAHARA_RAW_BUFFER_SIZE);
            size_t retval = fread(mTxBuf,1, bytes_to_read_next,mpDownloadFile);
            if (retval < 0) 
            {
                LogE_F(L"file read failed: %s", JGW_A2W(strerror(errno)).c_str());
                return false;
            }
            if ((uint32_t) retval != bytes_to_read_next) 
            {
                LogE_F(L"Read %d bytes, but was asked for %ud bytes", retval, DataLength);
                return false;
            }
            /*send the image data*/
            if (false == WriteSerialPort (mTxBuf,bytes_to_read_next)) 
            {
                LogE_F(L"Tx Sahara Image Failed");
                return false;
            }
            bytes_read += bytes_to_read_next;
        }

        return true;
    }

    bool CTSE_SaharaFileDownload::is_ack_successful (int status)
    {
        switch (status) {

            /*Success*/
        case SAHARA_STATUS_SUCCESS :
            LogI(L"SAHARA_STATUS_SUCCESS");
            return true;

            /*Invalid command received in current state*/
        case SAHARA_NAK_INVALID_CMD:
            LogE(L"SAHARA_NAK_INVALID_CMD");
            break;

            /*Protocol mismatch between host and target*/
        case SAHARA_NAK_PROTOCOL_MISMATCH:
            LogE(L"SAHARA_NAK_PROTOCOL_MISMATCH");
            break;

            /*Invalid target protocol version*/
        case SAHARA_NAK_INVALID_TARGET_PROTOCOL:
            LogE(L"SAHARA_NAK_INVALID_TARGET_PROTOCOL");
            break;

            /*Invalid host protocol version*/
        case SAHARA_NAK_INVALID_HOST_PROTOCOL:
            LogE(L"SAHARA_NAK_INVALID_HOST_PROTOCOL");
            break;

            /*Invalid packet size received*/
        case SAHARA_NAK_INVALID_PACKET_SIZE:
            LogE(L"SAHARA_NAK_INVALID_PACKET_SIZE");
            break;

            /*Unexpected image ID received*/
        case SAHARA_NAK_UNEXPECTED_IMAGE_ID:
            LogE(L"SAHARA_NAK_UNEXPECTED_IMAGE_ID");
            break;

            /*Invalid image header size received*/
        case SAHARA_NAK_INVALID_HEADER_SIZE:
            LogE(L"SAHARA_NAK_INVALID_HEADER_SIZE");
            break;

            /*Invalid image data size received*/
        case SAHARA_NAK_INVALID_DATA_SIZE:
            LogE(L"SAHARA_NAK_INVALID_DATA_SIZE");
            break;

            /*Unsupported image type received*/
        case SAHARA_NAK_INVALID_IMAGE_TYPE:
            LogE(L"SAHARA_NAK_INVALID_IMAGE_TYPE");
            break;

            /*Invalid tranmission length*/
        case SAHARA_NAK_INVALID_TX_LENGTH:
            LogE(L"SAHARA_NAK_INVALID_TX_LENGTH");
            break;

            /*Invalid reception length*/
        case SAHARA_NAK_INVALID_RX_LENGTH :
            LogE(L"SAHARA_NAK_INVALID_RX_LENGTH");
            break;

            /*General transmission or reception error*/
        case  SAHARA_NAK_GENERAL_TX_RX_ERROR:
            LogE(L"SAHARA_NAK_GENERAL_TX_RX_ERROR");
            break;

            /*Error while transmitting READ_DATA packet*/
        case SAHARA_NAK_READ_DATA_ERROR:
            LogE(L"SAHARA_NAK_READ_DATA_ERROR");
            break;

            /*Cannot receive specified number of program headers*/
        case SAHARA_NAK_UNSUPPORTED_NUM_PHDRS:
            LogE(L"SAHARA_NAK_UNSUPPORTED_NUM_PHDRS");
            break;

            /*Invalid data length received for program headers*/
        case SAHARA_NAK_INVALID_PDHR_SIZE:
            LogE(L"SAHARA_NAK_INVALID_PDHR_SIZE");
            break;

            /*Multiple shared segments found in ELF image*/
        case SAHARA_NAK_MULTIPLE_SHARED_SEG:
            LogE(L"SAHARA_NAK_MULTIPLE_SHARED_SEG");
            break;

            /*Uninitialized program header location*/
        case SAHARA_NAK_UNINIT_PHDR_LOC:
            LogE(L"SAHARA_NAK_UNINIT_PHDR_LOC");
            break;

            /* Invalid destination address*/
        case  SAHARA_NAK_INVALID_DEST_ADDR:
            LogE(L"SAHARA_NAK_INVALID_DEST_ADDR");
            break;

            /* Invalid data size receieved in image header*/
        case SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE:
            LogE(L"SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE");
            break;

            /* Invalid ELF header received*/
        case SAHARA_NAK_INVALID_ELF_HDR:
            LogE(L"SAHARA_NAK_INVALID_ELF_HDR");
            break;

            /* Unknown host error received in HELLO_RESP*/
        case SAHARA_NAK_UNKNOWN_HOST_ERROR:
            LogE(L"SAHARA_NAK_UNKNOWN_HOST_ERROR");
            break;

            // Timeout while receiving data
        case SAHARA_NAK_TIMEOUT_RX:
            LogE(L"SAHARA_NAK_TIMEOUT_RX");
            break;

            // Timeout while transmitting data
        case SAHARA_NAK_TIMEOUT_TX:
            LogE(L"SAHARA_NAK_TIMEOUT_TX");
            break;

            // Invalid mode received from host
        case SAHARA_NAK_INVALID_HOST_MODE:
            LogE(L"SAHARA_NAK_INVALID_HOST_MODE");
            break;

            // Invalid memory read access
        case SAHARA_NAK_INVALID_MEMORY_READ:
            LogE(L"SAHARA_NAK_INVALID_MEMORY_READ");
            break;

            // Host cannot handle read data size requested
        case SAHARA_NAK_INVALID_DATA_SIZE_REQUEST:
            LogE(L"SAHARA_NAK_INVALID_DATA_SIZE_REQUEST");
            break;

            // Memory debug not supported
        case SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED:
            LogE(L"SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED");
            break;

            // Invalid mode switch
        case SAHARA_NAK_INVALID_MODE_SWITCH:
            LogE(L"SAHARA_NAK_INVALID_MODE_SWITCH");
            break;

            // Failed to execute command
        case SAHARA_NAK_CMD_EXEC_FAILURE:
            LogE(L"SAHARA_NAK_CMD_EXEC_FAILURE");
            break;

            // Invalid parameter passed to command execution
        case SAHARA_NAK_EXEC_CMD_INVALID_PARAM:
            LogE(L"SAHARA_NAK_EXEC_CMD_INVALID_PARAM");
            break;

            // Unsupported client command received
        case SAHARA_NAK_EXEC_CMD_UNSUPPORTED:
            LogE(L"SAHARA_NAK_EXEC_CMD_UNSUPPORTED");
            break;

            // Invalid client command received for data response
        case SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD:
            LogE(L"SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD");
            break;

            // Failed to authenticate hash table
        case SAHARA_NAK_HASH_TABLE_AUTH_FAILURE:
            LogE(L"SAHARA_NAK_HASH_TABLE_AUTH_FAILURE");
            break;

            // Failed to verify hash for a given segment of ELF image
        case SAHARA_NAK_HASH_VERIFICATION_FAILURE:
            LogE(L"SAHARA_NAK_HASH_VERIFICATION_FAILURE");
            break;

            // Failed to find hash table in ELF image
        case SAHARA_NAK_HASH_TABLE_NOT_FOUND:
            LogE(L"SAHARA_NAK_HASH_TABLE_NOT_FOUND");
            break;

        case SAHARA_NAK_LAST_CODE:
            LogE(L"SAHARA_NAK_LAST_CODE");
            break;

        default:
            LogE_F(L"Invalid status field %d", status);
            break;
        }
        return false;
    }

}
