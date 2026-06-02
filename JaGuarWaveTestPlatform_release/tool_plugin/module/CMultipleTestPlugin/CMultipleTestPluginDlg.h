#pragma once
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <UiLib/UIlib.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "../../../platform_include/CJGWUiLibViewResolver_Define.h"
#include "CMultipleTestPluginDefine.h"
#include "CJGW_MultipleTestConfig.h"
#include <vector>
#include "CMultipleTestManageThread.h"

namespace JGW
{
    typedef struct
    {
        int mnViewIndex;
        //! 进度条位置
        int mnProcessPos;
        //! 测试进度条
        CProgressUI* mpDownloadProcess;
        //! 索引标签控件
        CLabelUI* mpIndexLabel;
        //! 测试状态标签控件
        CLabelUI* mpStatusLabel;
        //! 手动开始按钮或者设备描述
        CButtonUI* mpStartButton;
        //CTextUI* mpDeviceDesTextUI;
        //! 测试状态水平控件
        CHorizontalLayoutUI* mpStatusHorizontal;
        //! log内容显示控件
        CRichEditUI* mpLogRickEdit;
    }S_MULTIPLE_VIEW_INFO,*PS_MULTIPLE_VIEW_INFO;

    class CCMultipleTestPluginDlg  : public CCJGWUiLibViewResolverImpl
    {
    public:
        CCMultipleTestPluginDlg(void);
        ~CCMultipleTestPluginDlg(void);
    protected://! 继承CCJGWUiLibViewResolverImpl 接口类
        //! 初始化
        bool InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam);
        // 接收消息
        void OnNotifySubWndPlugin(LPVOID pMsg);

        void CloseSubWndPlugin();
    private:
        //! 响应网卡状态改变消息
        void OnResponseNetCardChangeMSG(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应设备插入消息
        void OnResponseDeviceMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应定时器消息
        void OnResponseTimer(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应设备排序消息
        static void OnResponseDeviceQueueMsg(LPS_DEVICE_QUEUE_INFO psDeviceQueueInfo,LPVOID pContext);
        //! 响应点击开始测试事件
        bool OnStartMsgBtnClick(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
        //! 响应点击CONFIG按钮消息
        void OnClickConfigBtn();
        //! 响应选择按钮消息
        void OnChangeDownloadPathChooseBtn();
    private:
        //! 初始化多路测试视图
        void InitMultipleTestView();
        //! 检查多路测试初始化是否成功,并进行提示
        void CheckInitMultipleTestIsOk();
        //! 检查当前配置是否有效
        void CheckMultipleTestConfigIsVaild();
        //! 卸载多路测试视图
        void UnInitMultipleTestView();
    private:
        // 更新视图
        void UpdateMultipleSingleView(S_TSE_MULTIPLE_TEST_THREAD_PARAM& sThreadParam,S_MULTIPLE_VIEW_INFO& sMultipleViewInfo);
    private:
        LPS_VIEW_RESOLVER_PARAM mpsViewResolverParam;
        CPaintManagerUI* mpPluginPaintManagerUI;
        CVerticalLayoutUI* mpMainMultipleVerticalLayout;
        CButtonUI* mpChooseBtn;
        CButtonUI* mpConfigBtn;
        CLabelUI* mpBinaryImagePathLabel;
    private:
        bool mbInitMultipleTestOk;
        int mnThreadIndex;
        S_MULTIPLE_CONFIG msMultipleConfig;
        CCJGW_MultipleTestConfig mcMultipleTestConfig;
        std::vector<S_MULTIPLE_VIEW_INFO> mvsMultipleViewInfos;
        std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM> mvsMultipleTestThreadParam;
        CCMultipleTestManageThread mcMultipleTestManageThread;
    };
}
