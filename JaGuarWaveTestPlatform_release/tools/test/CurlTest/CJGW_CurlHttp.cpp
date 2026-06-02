#include "StdAfx.h"
#include "CJGW_CurlHttp.h"
#include <curl/curl.h>
#include <sstream>
#include <sys/stat.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_CurlHttp::CCJGW_CurlHttp(void)
    {
    }


    CCJGW_CurlHttp::~CCJGW_CurlHttp(void)
    {
    }

    //回调函数
    size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
    {
        std::string data((const char*) ptr, (size_t) size * nmemb);

        *((std::stringstream*) stream) << data/* << std::endl*/;

        return size * nmemb;
    }

    bool CCJGW_CurlHttp::SendCurlHttpPostData(const std::wstring& strUrl,const std::wstring& strPostData,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strHeader /* = L"Content-Type:text/html;charset/* =UTF-8" */)
    {
        CURL *curl;
        CURLcode res;
        struct curl_slist* headers = NULL;
        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
        std::stringstream out;
        if(curl) 
        {
            std::string strUrlA = JGW_W2A(strUrl,CP_UTF8),strPostDataA = JGW_W2A(strPostData,CP_UTF8),strHeaderA = JGW_W2A(strHeader,CP_UTF8);
            /* First set the URL that is about to receive our POST. This URL can
            just as well be a https:// URL if that is what should receive the
            data. */
            curl_easy_setopt(curl, CURLOPT_URL, strUrlA.c_str());
            //! 
            //curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostDataA.c_str());
            //! 构建HTTP报文头 
            headers = curl_slist_append(headers, strHeaderA.c_str());
            //! 
            curl_easy_setopt(curl, CURLOPT_INTERFACE, "192.168.1.5");//设置回调函数
            //curl_easy_setopt(curl, CURLOPT_HTTPHEADER,strContentType.c_str()) ;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据
            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
            {
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8).c_str());
            }
            curl_slist_free_all(headers); /* free the list again */
            strResponce = JGW_A2W(out.str(),CP_UTF8);//返回请求值 
            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        return CURLE_OK == res;
    }

    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        size_t retcode;
        curl_off_t nread;

        /* in real-world cases, this would probably get this data differently
        as this fread() stuff is exactly what the library already would do
        by default internally */
        retcode = fread(ptr, size, nmemb, (FILE*)stream);

        nread = (curl_off_t)retcode;

        //fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);

        return retcode;
    }

    bool CCJGW_CurlHttp::UploadCurlHttpPostFile(const std::wstring& strUrl,const std::wstring& strLocalFilePath,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strFieldName /* = L"fileName" */)
    {
#if 0
        //! HTTP PUT operation. PUTs a file given as a command
        CURL *curl;
        CURLcode res;
        FILE *hd_src;
        struct _stat64i32 file_info;
        curl_off_t fsize;

        /* get the file size of the local file */
        if(_wstat(strLocalFilePath.c_str(), &file_info)) 
        {
            JGW_FormatWString(strErrorMsg,L"Couldn't open '%s': %s\n", strLocalFilePath.c_str(), JGW_A2W_A(strerror(errno),CP_UTF8));
            return false;
        }
        fsize = (curl_off_t)file_info.st_size;
        /* get a FILE * of the same file */
        hd_src = _wfopen(strLocalFilePath.c_str(), L"rb");
        if (NULL == hd_src)
        {
            JGW_FormatWString(strErrorMsg,L"Couldn't open '%s': %s\n", strLocalFilePath.c_str(), JGW_A2W_A(strerror(errno),CP_UTF8));
            return false;
        }

        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
        if(curl) 
        {
            /* we want to use our own read function */
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

            /* enable uploading */
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            /* HTTP PUT please */
            curl_easy_setopt(curl, CURLOPT_PUT, 1L);

            /* specify target URL, and note that this URL should include a file
            name, not only a directory */
            std::string strUrlA = JGW_W2A(strUrl,CP_UTF8);
            curl_easy_setopt(curl, CURLOPT_URL, strUrlA.c_str());

            /* now specify which file to upload */
            curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

            /* provide the size of the upload, we specicially typecast the value
            to curl_off_t since we must be sure to use the correct data size */
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                (curl_off_t)file_info.st_size);

            /* Now run off and do what you've been told! */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
            {
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8).c_str());
            }
            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        fclose(hd_src); /* close the local file */

        curl_global_cleanup();
        return CURLE_OK == res;
#else
        CURL *curl;
        CURLcode res;
        std::stringstream out;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if(curl != NULL)
        {
            struct curl_slist* pOptionList = NULL;
            //添加HTTP的包头 Expect：防止数据大于1024个字节 需要等待服务响应
            //也许有Expect: 100-continue，去掉它
            pOptionList = curl_slist_append(pOptionList, "Expect:");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pOptionList);

            struct curl_httppost* pFormPost = NULL;
            struct curl_httppost* pLastElem = NULL;
            std::string strLocalFilePathA = JGW_W2A(strLocalFilePath,CP_UTF8);
            std::string strFieldNameA = JGW_W2A(strFieldName,CP_UTF8);
            //上传文件，指定本地文件完整路径
            curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, strFieldNameA.c_str(), CURLFORM_FILE,strLocalFilePathA.c_str(), /*CURLFORM_CONTENTTYPE, "application/octet-stream",*/ CURLFORM_END);

            //不加一个结束的hfs服务端无法写入文件，一般不存在这种问题，这里加入只是为了测试.
            //curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "end", CURLFORM_COPYCONTENTS, "end", CURLFORM_END);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, pFormPost);
            std::string strUrlA = JGW_W2A(strUrl,CP_UTF8);
            curl_easy_setopt(curl, CURLOPT_URL, strUrlA.c_str());

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据

            res = curl_easy_perform(curl);
            if(res != CURLE_OK)
            {
                 JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8).c_str());
            }
            curl_formfree(pFormPost);
            strResponce = JGW_A2W(out.str(),CP_UTF8);//返回请求值 
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        return CURLE_OK == res;
#endif
    }
}
