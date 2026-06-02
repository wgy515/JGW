#include "StdAfx.h"
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <curl/curl.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
namespace JGW
{
    CCJGW_CurlFtp::CCJGW_CurlFtp(const std::string& host, const std::string& username /* = "" */, const std::string& password /* = "" */,size_t nFtpPort) : mnFtpPort(nFtpPort)
    {
        strcpy_s(mstrHost,128,host.c_str());
        strcpy_s(mstrUserName,128,username.c_str());
        strcpy_s(mstrPassword,128,password.c_str());
    }


    CCJGW_CurlFtp::~CCJGW_CurlFtp(void)
    {
    }

    void CCJGW_CurlFtp::UpdateHostUserNamePassword(const std::string& host,const std::string& username /* = "" */,const std::string& password /* = "" */,size_t nFtpPort /* = 22 */)
    {
        strcpy_s(mstrHost,128,host.c_str());
        strcpy_s(mstrUserName,128,username.c_str());
        strcpy_s(mstrPassword,128,password.c_str());
        mnFtpPort = nFtpPort;
    }

    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        curl_off_t nread;
        /* in real-world cases, this would probably get this data differently
        as this fread() stuff is exactly what the library already would do
        by default internally */
        size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);
        nread = (curl_off_t)retcode;
        //fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
        return retcode;
    }


    bool CCJGW_CurlFtp::UploadCurlFtpFile(const std::wstring& strFTPFilePath,const std::wstring& strUploadFilePath,std::wstring& strErrorMsg,size_t mnTimeOut /* = 10 * 60 */)
    {
        CURL *curl;
        CURLcode res;
        FILE *hd_src;
        struct _stat64i32 file_info;
        curl_off_t fsize;

        struct curl_slist *headerlist = NULL;
        //static const char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
        //static const char buf_2 [] = "RNTO " RENAME_FILE_TO;

        /* get the file size of the local file */
        if(_wstat(strUploadFilePath.c_str(), &file_info)) 
        {
            JGW_FormatWString(strErrorMsg,L"Couldn't open '%s': %s\n", strUploadFilePath.c_str(), JGW_A2W_A(strerror(errno),CP_UTF8));
            return false;
        }
        fsize = (curl_off_t)file_info.st_size;

        //! printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

        /* get a FILE * of the same file */
        hd_src = _wfopen(strUploadFilePath.c_str(), L"rb");

        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
        if(curl) 
        {
            std::string strUserPwd,strFtpUrl;
            JGW_FormatString(strUserPwd,"%s:%s",mstrUserName,mstrPassword);
            JGW_FormatString(strFtpUrl,"ftp://%s:%d%s",mstrHost,mnFtpPort,JGW_W2A(strFTPFilePath,CP_UTF8).c_str());
            /* build a list of commands to pass to libcurl */
            //! headerlist = curl_slist_append(headerlist, buf_1);
            //! headerlist = curl_slist_append(headerlist, buf_2);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, mnTimeOut); // times out after 300s

            curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPwd.c_str());//FTP用户名：密码

            /* we want to use our own read function */
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

            /* enable uploading */
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            /* specify target */
            curl_easy_setopt(curl, CURLOPT_URL, strFtpUrl.c_str());

            /* pass in that last of FTP commands to run after the transfer */
            //curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

            /* now specify which file to upload */
            curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

            /* Set the size of the file to upload (optional).  If you give a *_LARGE
            option you MUST make sure that the type of the passed-in argument is a
            curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
            make sure that to pass in a type 'long' argument. */
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                (curl_off_t)fsize);

            /* Now run off and do what you've been told! */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
            {
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8).c_str());
            }
            /* clean up the FTP commands list */
            //curl_slist_free_all(headerlist);

            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        fclose(hd_src); /* close the local file */

        curl_global_cleanup();
        return res == CURLE_OK;
    }

    struct FtpFile {
        const wchar_t *filename;
        FILE *stream;
    };

    static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
    {
        struct FtpFile *out = (struct FtpFile *)stream;
        if(!out->stream) {
            /* open file for writing */
            JGW_CreateDirectory(out->filename,false);
            out->stream = _wfopen(out->filename, L"wb");
            if(!out->stream)
                return -1; /* failure, can't open file to write */
        }
        return fwrite(buffer, size, nmemb, out->stream);
    }

    bool CCJGW_CurlFtp::DownloadCurlFtpFile(const std::wstring& strFTPFilePath,const std::wstring& strDownloadFilePath,std::wstring& strErrorMsg,size_t mnTimeOut /* = 10 * 60 */)
    {
        CURL *curl;
        CURLcode res;
        struct FtpFile ftpfile = {
            strDownloadFilePath.c_str(), /* name to store the file as if successful */
            NULL
        };

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if(curl) 
        {
            std::string strUserPwd,strFtpUrl;
            JGW_FormatString(strUserPwd,"%s:%s",mstrUserName,mstrPassword);
            JGW_FormatString(strFtpUrl,"ftp://%s:%d%s",mstrHost,mnFtpPort,JGW_W2A(strFTPFilePath,CP_UTF8).c_str());
            /*
            * You better replace the URL with one that works!
            */
            curl_easy_setopt(curl, CURLOPT_URL,strFtpUrl.c_str());
            ///
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, mnTimeOut); // times out after 300s
            ///
            curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPwd.c_str());//FTP用户名：密码
            /* Define our callback to get called when there's data to be written */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
            /* Set a pointer to our struct to pass to the callback */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

            /* Switch on full protocol/debug output */
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            res = curl_easy_perform(curl);

            /* always cleanup */
            curl_easy_cleanup(curl);

            if(CURLE_OK != res) 
            {
                /* we failed */
                JGW_FormatWString(strErrorMsg,L"curl told us %d\n", res);
            }
        }

        if(ftpfile.stream)
            fclose(ftpfile.stream); /* close the local file */

        curl_global_cleanup();

        return res == CURLE_OK;
    }

    bool CCJGW_CurlFtp::TraverseCreateFTPDirectory(const std::wstring& strFTPDirectory,std::wstring& strErrorMsg,size_t mnTimeOut /* = 60 */)
    {
        wchar_t path[MAX_PATH] = {0};
        wchar_t cSaveChar;
        wcscpy_s(path, MAX_PATH, strFTPDirectory.c_str());
        JGW_PathAddBackslash(path);
        bool bResult = true;
        size_t i, nLen = wcslen(path);

        for (i = 2; i < nLen; i++)
        {
            if (JGW_IsPathSlash(path[i]))
            {
                cSaveChar = path[i];
                path[i] = 0;
                strErrorMsg = L"" ;
                bResult = CreateDirectoryCurlFtpFile(path,strErrorMsg,mnTimeOut);
                path[i] = cSaveChar;
            }
        }
        return bResult;
    }

    bool CCJGW_CurlFtp::CreateDirectoryCurlFtpFile(const std::wstring& strFTPDirectory,std::wstring& strErrorMsg,size_t mnTimeOut /* = 60 */)
    {
        CURL *curl;
        CURLcode res;

        struct curl_slist *headerlist = NULL;
        //static const char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
        //static const char buf_2 [] = "RNTO " RENAME_FILE_TO;

        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
        if(curl) 
        {
            std::string strUserPwd,strFtpUrl,strFtpDirCommand;
            JGW_FormatString(strUserPwd,"%s:%s",mstrUserName,mstrPassword);
            JGW_FormatString(strFtpUrl,"ftp://%s:%d%s",mstrHost,mnFtpPort,"/");
            JGW_FormatString(strFtpDirCommand,"MKD %s",JGW_W2A(strFTPDirectory,CP_UTF8).c_str());
            /* build a list of commands to pass to libcurl */
            headerlist = curl_slist_append(headerlist, strFtpDirCommand.c_str());
            //! headerlist = curl_slist_append(headerlist, buf_2);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, mnTimeOut); // times out after 300s

            curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPwd.c_str());//FTP用户名：密码
            /* pass in that last of FTP commands to run after the transfer */
            curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
            /* specify target */
            curl_easy_setopt(curl, CURLOPT_URL, strFtpUrl.c_str());

            /* Now run off and do what you've been told! */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
            {
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8).c_str());
            }
            /* clean up the FTP commands list */
            curl_slist_free_all(headerlist);

            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        return res == CURLE_OK;
    }

     static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
    {
        (void)ptr;
        (void)data;
        /* we are not interested in the headers itself,
        so we only return the size we would have saved ... */
        return (size_t)(size * nmemb);
    }

    bool CCJGW_CurlFtp::CheckPathCurlFtpExist(const std::wstring& strFTPPath,std::wstring& strErrorMsg,size_t mnTimeOut /* = 60 */)
    {
        //char ftpurl[] = "ftp://ftp.example.com/gnu/binutils/binutils-2.19.1.tar.bz2";
        CURL *curl;
        CURLcode res;
        //long filetime = -1;
        double filesize = 0.0;
        //const char *filename = strrchr(ftpurl, '/') + 1;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if(curl) 
        {
            std::string strUserPwd,strFtpUrl,strFtpDirCommand;
            JGW_FormatString(strUserPwd,"%s:%s",mstrUserName,mstrPassword);
            JGW_FormatString(strFtpUrl,"ftp://%s:%d%s",mstrHost,mnFtpPort,JGW_W2A(strFTPPath,CP_UTF8).c_str());

            curl_easy_setopt(curl, CURLOPT_TIMEOUT, mnTimeOut); // times out after 300s

            curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPwd.c_str());//FTP用户名：密码

            curl_easy_setopt(curl, CURLOPT_URL, strFtpUrl.c_str());
            /* No download if the file */
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            /* Ask for filetime */
            curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
            /* No header output: TODO 14.1 http-style HEAD output for ftp */
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
            curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
            /* Switch on full protocol/debug output */
            /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */



            res = curl_easy_perform(curl);

            if(CURLE_OK == res) 
            {
                /* https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
                //res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
                //if((CURLE_OK == res) && (filetime >= 0))
                //{
                //    time_t file_time = (time_t)filetime;
                //    printf("filetime %s: %s", strFTPPath.c_str(), ctime(&file_time));
                //}
                res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                    &filesize);
                //if((CURLE_OK == res) && (filesize>0.0))
                //    printf("filesize %s: %0.0f bytes\n", strFTPPath.c_str(), filesize);
            }
            else 
            {
                /* we failed */
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8));
                //JGW_FormatString(strErrorMsg,"curl told us %s\n",curl_easy_strerror(res));
            }

            /* always cleanup */
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();

        return CURLE_OK == res && filesize >= 0;
    }

    bool CCJGW_CurlFtp::GetCurlFtpFileTimeAndFileSize(const std::wstring& strFTPPath,std::wstring& strErrorMsg,long& lFileTime,double& dFileSize,size_t mnTimeOut /* = 60 */)
    {
        //char ftpurl[] = "ftp://ftp.example.com/gnu/binutils/binutils-2.19.1.tar.bz2";
        CURL *curl;
        CURLcode res;
        long filetime = -1;
        //double filesize = 0.0;
        //const char *filename = strrchr(ftpurl, '/') + 1;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if(curl) 
        {
            std::string strUserPwd,strFtpUrl,strFtpDirCommand;
            JGW_FormatString(strUserPwd,"%s:%s",mstrUserName,mstrPassword);
            JGW_FormatString(strFtpUrl,"ftp://%s:%d%s",mstrHost,mnFtpPort,JGW_W2A(strFTPPath,CP_UTF8).c_str());

            curl_easy_setopt(curl, CURLOPT_TIMEOUT, mnTimeOut); // times out after 300s

            curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPwd.c_str());//FTP用户名：密码

            curl_easy_setopt(curl, CURLOPT_URL, strFtpUrl.c_str());
            /* No download if the file */
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            /* Ask for filetime */
            curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
            /* No header output: TODO 14.1 http-style HEAD output for ftp */
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
            curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
            /* Switch on full protocol/debug output */
            /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */



            res = curl_easy_perform(curl);

            if(CURLE_OK == res) 
            {
                /* https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
                res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
                if((CURLE_OK == res) && (filetime >= 0))
                {
                    lFileTime = filetime;
                    //time_t file_time = (time_t)filetime;
                    //printf("filetime %s: %s", strFTPPath.c_str(), ctime(&lFileTime));
                }
                res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                    &dFileSize);
                //if((CURLE_OK == res) && (filesize>0.0))
                //    printf("filesize %s: %0.0f bytes\n", strFTPPath.c_str(), filesize);
            }
            else 
            {
                /* we failed */
                JGW_FormatWString(strErrorMsg,L"curl_easy_perform() failed: %s\n",JGW_A2W_A(curl_easy_strerror(res),CP_UTF8));
                //JGW_FormatString(strErrorMsg,"curl told us %s\n",curl_easy_strerror(res));
            }

            /* always cleanup */
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();

        return CURLE_OK == res && dFileSize >= 0;
    }
}
