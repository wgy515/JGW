#pragma once
#include "HelpPackageUIInterface.h"
#include "JGWPackageUpdateDefine.h"
#include <poco/Zip/Decompress.h>
#include <poco/Zip/ZipLocalFileHeader.h>


namespace JGW
{
    typedef struct
    {
        int mnProjectID;
        std::wstring mstrProjectName;
        std::wstring mstrConfigFolder;
        std::wstring mstrVersion;
        std::wstring mstrConfigName;
        std::wstring mstrConfigDes;
        std::wstring mstrLocalConfigPackageZipPath;
        //! 更新包的FTP路径
        std::wstring mstrUploadFtpUrl;
    } S_ADD_CONFIG_UI_INFO,*LPS_ADD_CONFIG_UI_INFO;

    typedef struct  
    {
        int mnTaskID;
        int mnConfigID;
        int mnProjectID;

        std::wstring mstrTaskName;
        std::wstring mstrProjectName;
        std::wstring mstrConfigName;
        std::wstring mstrConfigDes;
        //! 配置下载保存目录
        std::wstring mstrConfigFolder;
        //! 更新包的版本
        std::wstring mstrUpdateConfigVersion;
        //! 本地更新包保存路径
        std::wstring mstrLocalUpdateConfigPackageZipPath;
        //! 更新包的FTP路径
        std::wstring mstrUpdateFtpUrl;
    }S_UPDATE_CONFIG_UI_INFO;

    typedef enum
    {
        //! 选择TASK
        E_SELECT_TASK_ASYNC_ACTION_ID = 0,
        //! 下载配置
        E_DOWNLOAD_CONFIG_ASYNC_ACTION_ID = 1,
        //! 更新配置
        E_UPDATE_CONFIG_ASYNC_ACTION_ID = 2,
        //! 新增配置
        E_ADD_CONFIG_ASYNC_ACTION_ID = 3,
    }E_CONFIG_ASYNC_ACTION_ID;

    /*
    一、修改指定订单对应的配置文件
    1、选择订单号
    2、显示配置目录路径(规则：程序目录\PackageConfig\订单\版本(此版本为根据当前时间信息生成的版本比如:Config20220608185138)\)、当前配置版本、配置名称、配置描述
    3、点击下载配置按钮，将当前配置下载并解压到配置目录路径中
    4、下载以及解压成功，更新配置按钮enable状态
    5、点击更新配置按钮
    6、打包配置目录路径下的所有文件按照当前规则打包，比对MD5等信息，如果MD5信息一致则不进行修改，并提示当前配置与服务器配置一致
    7、上传配置路径定义按照（FTP目录定义: JGWPTP_Help\Project\项目信息\订单\配置版本.zip）
    8、首先检查当前目录是否已经创建如果未创建则先创建目录然后在进行上传操作


    二、添加配置文件
    1、选择项目信息
    2、配置名称以及描述信息
    3、选择项目后自动构造版本信息格式如下YYYYMMDDHHMMMSS
    4、选择配置目录（目录结构如下：选择目录(包含exec或者help目录)）或者选择已经打包好的ZIP文件
    5、点击新增按钮即可上传配置信息(FTP目录定义: JGWPTP_Help\Project\项目信息\订单\配置版本.zip)
    */
    class CCJGWHelpConfigDlg : public CHelpPackageUIInterface
    {
    public:
        CCJGWHelpConfigDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig);
        ~CCJGWHelpConfigDlg(void);
    private:
        //! 初始化UI
        bool InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI);
        //! 响应UI点击事件通知
        void OnNotifyHelpPackageUI(TNotifyUI& msg);
        //! 关闭UI事件回调
        void CloseHelpPackageUI();
        //! 处理客户事件消息
        void OnHandleEventCustomMessage(TEventUI* pTEventUI);
        //! 处理异步函数调用消息
        bool OnHandleAsyncFuncMsg(int nActionIndex);
    private:
        //! 响应订单下拉栏选中事件
        bool OnRspConfigTaskComboItemSelectMsg();
        //! 响应点击下载配置消息
        bool OnRspDownloadConfigClickMsg();
        //! 响应更新配置按键消息
        bool OnRspUpdateConfigClickMsg();
        //! 响应选择配置包目录消息
        bool OnRspSelectConfigPackageFolderClickMsg();
        //! 响应上传新增配置消息
        bool OnRspUploadAddConfigPackageClickMsg();


    private:
        //! WM_HWMD_OPERATION_COMPLETE_MSG
        void OnRspOperationCompleteMsg(E_CONFIG_ASYNC_ACTION_ID eActionIndex,bool result);
    private:
        //！上传FTP配置
        bool UploadFTPPackageConfig(const std::wstring& strUploadFtpUrl,const std::wstring& strLocalFilePath);
        //! 修改更新FTP文件信息
        bool UpdatePackageConfigSQLite();
        //!
        bool AddPackageConfigToSQLite();
    private:
        //! change config ui
        MEMBER_VARIABLE_UI(CComboBoxUI,ConfigTaskCombo);
        MEMBER_VARIABLE_UI(CButtonUI,DownloadConfigBtn);
        MEMBER_VARIABLE_UI(CEditUI,ConfigFolderLabel);
        MEMBER_VARIABLE_UI(CEditUI,ConfigNameEdit);
        MEMBER_VARIABLE_UI(CEditUI,ConfigDesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,ConfigUpdateBtn);
        MEMBER_VARIABLE_UI(CEditUI,ConfigVersionEdit);
        //! add config ui
        MEMBER_VARIABLE_UI(CComboBoxUI,AddConfigProjectCombo);
        MEMBER_VARIABLE_UI(CEditUI,AddConfigFolderLabel);
        MEMBER_VARIABLE_UI(CButtonUI,AddConfigPackageFolderBtn);
        MEMBER_VARIABLE_UI(CEditUI,AddConfigVersionEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddConfigNameEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddConfigDesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,UploadAddConfigPackageBtn);
    private:
        S_ADD_CONFIG_UI_INFO msAddConfigUiInfo;
        S_UPDATE_CONFIG_UI_INFO msUpdateConfigUiInfo;
    private:
        CPaintManagerUI* mpPaintManagerUI;
        DWORD _errCnt;
        S_CONFIG msConfig;
        //! S_PROJECT msProject;
        ////! 打包临时升级包配置ZIP文件路径
        //std::wstring mstrTempPackageConfigZipPath;
        ////! 临时升级包配置版本
        //std::wstring mstrTempPackageConfigVersion;
        ////! mpConfigFolderLabel
        //std::wstring mstrConfigFolder;
        ////!
        //std::wstring mstrRemoteFtpPath;
        ////!
        //std::wstring mstrMD5;
        //CComboBoxUI* mpConfigTaskCombo;
        //CButtonUI* mpDownloadConfigBtn;
        //CEditUI* mpConfigFolderLabel;
        //CEditUI* mpConfigNameEdit;
        //CEditUI* mpConfigDesEdit;
        //CButtonUI* mpConfigUpdateBtn;
    };
}
