#pragma once
#include <string>
#include <vector>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
//! 调试模式，不下载或者上传文件到HELP FTP
//#define DEBUG_HELP

#define MEMBER_VARIABLE_UI(cls,name) cls* mp##name
#define FIND_UI(cls,name,PaintManagerUIPtr) mp##name = static_cast<cls*>(PaintManagerUIPtr->FindControl(_T(#name)))
#define CHECK_UI(name) mp##name != NULL

namespace JGW
{
    /*
    CREATE TABLE [PROJECT] (
    [ID] integer  PRIMARY KEY AUTOINCREMENT NULL,
    [NAME] TEXT  UNIQUE NOT NULL,
    [DES] TEXT  NULL,
    [CreateDate] datetime default (datetime('now', 'localtime'))    -- 鏃堕棿
    )
    */
    struct S_PROJECT
    {
        //! 
        int mnID;
        //! 名称
        std::wstring mstrName;
        //! 描述
        std::wstring mstrDes;
        //! 创建时间
        std::wstring mstrCreateData;
    };
    /*
    CREATE TABLE [TASK](
    [ID] integer primary key autoincrement,
    [NAME] TEXT UNIQUE NOT NULL, -- Task Number
    [DES]  TEXT, -- Task Description
    [InstallVersionID] INT NOT NULL, -- InstallVersion ID
    [OTAID] INT default -1 NOT NULL,-- UpdateVersion ID
    [ConfigID] INT NOT NULL, -- Task Config ID
    [TaskConfigID] INT NOT NULL, -- TaskConfig ID
    [CreateDate] datetime default (datetime('now', 'localtime')),    -- 鏃堕棿
    foreign key(InstallVersionID) references INSTALLVERSION(ID) on delete cascade,
    --foreign key(OTAID) references OTA(ID) on delete cascade,
    foreign key(ConfigID) references CONFIG(ID) on delete cascade,
    foreign key(TaskConfigID) references TASKCONFIG(ID) on delete cascade
    )
    */
    struct S_TASK 
    {
        //! 订单主键
        int mnID;
        //! 安装版本主键ID
        int mnInstallVersionID;
        //! OTA主键ID
        int mnOTAID;
        //! CONFIG主键ID
        int mnConfigID;
        //! TaskConfigID主键ID
        int mnTaskConfigID;
        //! 名称
        std::string mstrName;
        //! 描述
        std::string mstrDes;
        //! 版本创建时间
        std::string mstrCreateDate;
    };

    /*
    CREATE TABLE [CONFIG] (
    [ID] integer  PRIMARY KEY AUTOINCREMENT NULL,
    [Version] VARCHAR(64)  UNIQUE NOT NULL,
    [NAME] TEXT  UNIQUE NOT NULL,
    [DES] TEXT  NULL,
    [MD5] varchar(32)  NULL,
    [FTPUrl] varchar(512)  NULL,
    [ProjectID] INT  NULL,
    [CreateDate] datetime DEFAULT '''''''datetime(''''''''now'''''''', ''''''''localtime'''''''')''''''' NULL
    )
    */
    struct S_CONFIG
    {
        //! 
        int mnID;
        //! 关联PROJECT信息
        int mnProjectID;
        //! 名称
        std::wstring mstrName;
        //! 描述
        std::wstring mstrDes;
        //! 版本信息
        std::wstring mstrVersion;
        //! 版本的MD5值
        std::wstring mstrMD5;
        //! 版本基于FTP根目录的路径
        std::wstring mstrFTPUrl;
        //! 版本创建时间
        std::wstring mstrCreateDate;
    };
    /*
    CREATE TABLE INSTALLVERSION(
    [ID] integer primary key autoincrement,
    [NAME] TEXT UNIQUE NOT NULL,
    [DES]  TEXT,
    [Version] varchar(64),
    [MD5] varchar(32),
    [FTPUrl] varchar(512),
    [CreateDate] datetime default (datetime('now', 'localtime'))    -- 鏃堕棿
    )
    */
    struct S_INTALLVERSION
    {
        //! 
        int mnID;
        //! 安装版本名称
        std::wstring mstrName;
        //! 安装版本描述
        std::wstring mstrDes;
        //! 安装版本信息
        std::wstring mstrVersion;
        //! 安装版本的MD5值
        std::wstring mstrMD5;
        //! 安装版本基于FTP根目录的路径
        std::wstring mstrFTPUrl;
        //! 安装版本创建时间
        std::wstring mstrCreateDate;
    };
    /*
    CREATE TABLE [OTA] (
    [ID] integer  PRIMARY KEY AUTOINCREMENT NULL,
    [NAME] TEXT UNIQUE NOT NULL,
    [DES] TEXT  NULL,
    [Version] varchar(64)  NULL,
    [MD5] varchar(32)  NULL,
    [FTPUrl] varchar(512)  NULL,
    [IsLatestVersion] BOOLEAN DEFAULT '0' NULL,
    [InstallVersionID] INT  NULL,
    [CreateDate] datetime default (datetime('now', 'localtime')),    -- 鏃堕棿
    foreign key(InstallVersionID) references INSTALLVERSION(ID) on delete cascade
    )
    */
    struct S_OTA
    {
        //! 是否是最后更新版本
        bool mbIsLatestVersion;
        int mnID;
        int mnInstallVersionID;
        std::wstring mstrName;
        std::wstring mstrDes;
        std::wstring mstrVersion;
        std::wstring mstrMD5;
        std::wstring mstrFTPUrl;
        std::wstring mstrCreateDate;
    };

    typedef struct
    {
        size_t mnThreadID;
        //! FTP端口
        size_t mnFTPPort;
        //! FTP HOST
        std::string mstrFTPHost;
        std::string mstrUserName;
        std::string mstrUserPassword;

        std::wstring mstrFTPRootPath;
        std::wstring mstrFTPLogRootPath;  

        std::wstring mstrHelpDBPath;

        std::vector<S_TASK> mvsTask;
        std::vector<S_PROJECT> mvsProject;
        std::vector<S_INTALLVERSION> mvsInstallVersion;
        CCJGW_SQLite mcSQLite;
    }S_PACKAGE_UPDATE_CONFIG,*LPS_PACKAGE_UPDATE_CONFIG;
}