#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "PackageUpdateThread.h"

namespace JGW
{
    class CCJaGuarWavePackageUpdateToolDlg : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarWavePackageUpdateToolDlg(void);
        ~CCJaGuarWavePackageUpdateToolDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJaGuarWavePackageUpdateToolDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
    private:
        //! 选择安装目录
        void OnClickSelectProgramFilesBtnMsg();
        //! 添加打包文件
        void OnClickAddPackageFileBtnMsg();
        //! 添加打包目录
        void OnClickAddPackageFolderBtnMsg();
        //! 上传打包相关信息
        void OnClickPackageBtnMsg();
        //!
        bool CheckFileIsAddUpdateList(int& index,const std::wstring& strFilePath);
        //! 
        void OnListMenuRightClick(TNotifyUI& msg);
        //!
        void OnMenuElementClick(TEventUI* pTEventUI);  
    private:
        void UpdateUiShow();
    private:
        CLabelUI* mpPackageFilesTotal;
        CEditUI* mpProgramFilesEdt;
        CButtonUI* mpSelectProgramFilesEdtBtn;
        CEditUI* mpTaskNumberEdt;
        CEditUI* mpUpdateVersionEdt;
        CListTestCtrlUI* mpPackageFileList;
        CButtonUI* mpAddPackageFileBtn;
        CButtonUI* mpAddPackageFolderBtn;
        CButtonUI* mpPackageBtn;
        CVerticalLayoutUI* mpHomeVerticalLayout;
        CPackageUpdateThread* mpPackageUpdateThread;
        S_PACKAGEUPDATE_INFO msPackageUpdateInfo;
        //! 菜单栏点击状态
        map<CDuiString,bool> mpMenuCheckInfo;
    };
}

