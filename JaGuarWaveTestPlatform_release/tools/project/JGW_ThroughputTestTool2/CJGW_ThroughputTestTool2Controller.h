#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "JGW_ThroughputTestTool2Define.h"
#include <JGW_WindowsFuncPlugin/CJGW_AdapterCommonInfo.h>
namespace JGW
{
    class CCJGW_PhoneSocketThread;
    class CCJGW_ThroughputTestThread;
    class CCJGW_ThroughputTestToolDlg2;

    class CCJGW_ThroughputTestTool2Controller
    {
    public:
        CCJGW_ThroughputTestTool2Controller(CCJGW_ThroughputTestToolDlg2* pView);
        ~CCJGW_ThroughputTestTool2Controller(void);
    public:
        //! 响应WINDOWS 消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 响应DUILIB事件通知
        void OnViewNotify(TNotifyUI& msg);
        //! 正在关闭当前窗口
        void OnCloseWindow();
        //! 开始初始化请求
        bool OnInit();
        //!
        const S_TestParams& GetTestParams() {return msTestParams;}
    private:
        //! 点击更新配置按钮
        void OnClickUpdateConfigOptionOption();
        //! 点击开始按钮
        void OnClickStartBtn();
        //! 点击停止测试按钮
        void OnClickStopBtn();
    private:
        //! 检查IP地址有效性
        bool CheckIPAddressValid(const std::wstring& strIPAddress);
    private:
        //! UI RichEdit插件添加测试消息
        void OnAppendRichEditTestMsg(const wchar_t* strMsg);
        //! UI 定时器更新消息，每隔一秒更新一次
        void OnUpdateUiTimerMsg();
        //! 测试完成消息
        void OnTestCompletedMsg();
        //! 更新网卡信息
        void UpdateNetCardInfoComBo();
        //! 更新手机热点IP信息
        void UpdatePhoneIpConfig();
        //! 保存配置
        void SaveConfig();
        //! 更新测试结果
        void UpdateTestResult();
    private:
        //! 响应接收GPS经纬度以及速率信息
        void OnRecvGPSSpeedMsg(const wchar_t* strMsg);
        //! 响应更新吞吐率信息
        void OnThroughputRateMsg(const wchar_t* strMsg);
        //! 响应时延消息
        void OnTimeDelayMsg(const wchar_t* strMsg);
        //! 写入测试日志
        void WriteTestLog(const std::wstring& strTestLog);
    private:
        bool mbUpdateTimerIsSetTimer;
        std::wstring mstrTestLogFilePath;
        std::wstring mstrConfigFilePath;
        S_LogInfo msLogInfo;
        S_TestParams msTestParams;
        CCJGW_ThroughputTestToolDlg2* mpView;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        CCJGW_ThroughputTestThread* mpThroughputTestThread;
        CCJGW_AdapterCommonInfo mcAdapterCommonInfo;
    };
}
