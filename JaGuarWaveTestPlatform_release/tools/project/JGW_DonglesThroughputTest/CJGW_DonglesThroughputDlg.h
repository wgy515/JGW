#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "UIWkeWindowlessBrowser.h"
#include "JGW_DonglesThroughputTest_Define.h"
#include "CJGW_TcpStatsImplThread.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <map>
#include <fstream>
#include "CJGW_IfTableTrafficMonitor.h"
#include "SCLLibHelper.h"
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include "CJGW_UIDelegateThread.h"
namespace JGW
{
    class CCJGW_DonglesThroughputDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_DonglesThroughputDlg(void);
        ~CCJGW_DonglesThroughputDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_DonglesThroughputDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 获取UI控件指针
        bool GetUIControl();
        //! 切换终端类型UI视图 - 服务端 or 客户端
        void SwitchTerminalTypeUI(int selPos);
        //! 切换数据传输类型UI
        void SwitchDataTransceiversTypeUI(int selPos);
        //! 响应开始按钮点击
        void OnClickStartBtn();
        //! 响应停止按钮点击
        void OnClickStopBtn();
        //! 设置控件状态
        void SetControlEnable(bool enable = true);
        //! 
        void OnStartDonglesthroughputTest(WPARAM& wParam,LPARAM& lParam);
    private:
        //! 获取bps 字符串
        void GetBpsString(const ULONGLONG& uBps,std::wstring& strBps);
        //! 创建外部控件
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
        //! 响应客户消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 响应Log
        void OnRspAppendRichedt(const wchar_t* strParam);

        //! 从UI中获取配置文件
        void GetThroughputConfigToControl();
        //! 更新速率测试配置
        void UpdateThroughputConfigToXMLConfig();
        //! 响应测试状态消息
        void OnTestStatusMsg(E_TEST_STATUS_TYPE eTestStatusType);
        //! 响应WM_TIME消息
        void OnTimerMsg(const WPARAM wParam,LPARAM lParam);
        //!
        void UpdateBpsDataInfo();
        //!
        void SaveXmlConfig();
        //! 响应
        void OnDataCollectionModeMenuClick(TNotifyUI& msg);
        //!
        void OnEditMenuClick(TNotifyUI& msg);
        //!
        void OnResponMenuClick(const std::wstring& strClickName);
        //!
        bool OpenTurnTableComPort();
        //!
        void OnEnableTurnTable();
        //!
        void OnStartTurnTableTestQueque();
        //!
        void CreateLogFile();
        //!
        bool ReadDonglesSerialNumber();
        //!
        bool OpenPerasoComPortView();
    private:
        //! wke 电流测试显示
        CWkeWindowlessUI* mpWkeBrowser;
        //! 终端类型 是服务端还是客户端
        CComboBoxUI* mpTerminalType;
        //! IP 地址
        CEditUI* mpIPAddressEdt;
        //! socket端口
        CEditUI* mpSocketPortEdt;
        //! 传输类型是发送端还是接收端
        CComboBoxUI* mpDataTransceivers;
        //! 发送端的测试时间间隔
        CLabelUI* mpTestIntervTimeLab;
        CEditUI* mpTestIntervTimeEdt;
        //! client config
        CHorizontalLayoutUI* mpClientConfigHor;
        //! 测试时间
        CEditUI* mpTestTimeEdt;
        //! socket数量
        CEditUI* mpSocketCount;
        //! socket发送的数据长度
        CEditUI* mpDataLengthEdt;
        //! services config
        CHorizontalLayoutUI* mpServicesConfigHor;
        //! 数据打包方式 只针对于服务端的发送
        CComboBoxUI* mpSendPolicy;
        //! 线程数量 默认：处理器核心*2 + 2
        CEditUI* mpThreadCountEdt;
        //! 服务端最大连接的Socket数量
        CEditUI* mpMaxSocketCount;
        //! log显示
        CRichEditUI* mpLogEdt;
        //! 开始测试按钮
        CButtonUI* mpStartBtn;
        //! 停止测试按钮
        CButtonUI* mpStopBtn;
        //!
        CButtonUI* mpDataCollectionModeMenu;
        //!
        CButtonUI* mpEditFuncMenu;
        //!
        CControlUI* mpSelMenuBtn;
        //! turntable
        CCheckBoxUI* mpEnableTurnTable;
        //!
        CEditUI* mpTurnTableStep;
        //! 起始角度
        CEditUI* mpTurntableStartAngle;
        //! 结束角度
        CEditUI* mpTurntableEndAngle;
        //! 保存配置
        //! CButtonUI* mpSaveXmlConfigBtn;
        //! 转台布局视图
        CHorizontalLayoutUI* mpStsclHorizontalLayoutUI;
        //! TCP 客户端线程
        CCJGW_TcpStatsImplThread* mpTcpStatsImplThread;
        //!
        bool mbSelectIfTableTrafficMonitor;
        //! 当前选择网络的名称
        std::string mstrConnectionName;	
        //! 
        CCJGW_IfTableTrafficMonitor mIfTableTrafficMonitor;
        //! 测试时间
        size_t mnTestTimeIndex;
        //! 计数
        jgw_timer mtm;
        //! 当前转台角度 默认从0度开始
        int mnCurrentTurnTableAngle;
        //! 吞吐率测试配置
        S_ThroughputTest_Confige msThroughputTestConfig;
        //!
        map<CDuiString,bool> m_MenuCheckInfo;
        //!
        SCLLibHelper mSCLLibHelper;
        //! 转台的角度队列
        std::queue<int> mvTurnTableAngleQueue;
        //! 
        std::wofstream mFileStream;
        //! 
        std::wstring mstrDonglesSerialNumber;
        //!
        CCJGW_CSerialComPort mcSerialComPort;
        CCJGW_DonglesComPort mDonglesComPort;
        CCJGW_UIDelegateThread mUIDelegateThread;
    };
}

