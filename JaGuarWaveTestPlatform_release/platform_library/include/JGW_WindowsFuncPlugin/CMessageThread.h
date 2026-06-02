#pragma once
#include <Windows.h>
#include "JGW_WindowsFuncPlugin_Define.h"

namespace JGW
{

	class CCMessageThread;
	struct AFX_THREAD_MSGMAP;

	typedef void (CCMessageThread::*threadfunc)(WPARAM wParam,LPARAM lParam);
	typedef const AFX_THREAD_MSGMAP* (*pfnGetBaseMapFunc)();

	typedef struct
	{
		DWORD mssageID;
		threadfunc myFunc;
	}S_THREAD_MESSAGE,*LPS_THREAD_MESSAGE;

	typedef const AFX_THREAD_MSGMAP* (*pfnGetBaseThreadMapFunc)();

	struct AFX_THREAD_MSGMAP
	{
		pfnGetBaseThreadMapFunc pfnGetBaseMap;
		const S_THREAD_MESSAGE* lpEntries;
	};

#define BEGIN_MYTHREAD_MESSAGE_MAP(theClass, baseClass)	\
	const  AFX_THREAD_MSGMAP* theClass::GetMessageMap() const\
	{ return const_cast<theClass *>(this)->GetThisMessageMap(); }\
	void theClass::DispatchMessageFunc()\
	{ baseClass::DispatchMessageFunc();}\
	const AFX_THREAD_MSGMAP* theClass::GetThisMessageMap() \
	{ \
	typedef theClass ThisClass;						   \
	typedef baseClass TheBaseClass;					   \
	static  const S_THREAD_MESSAGE _messageEntries[] =  \
	{


#define ON_MYTHREAD_MESSGAE(Message_Id,memberFxn)\
	{(Message_Id),(static_cast< threadfunc >(memberFxn))},


#define END_MYTHREAD_MESSAGE_MAP()   \
	{0, (threadfunc)0 } \
	}; \
	static const AFX_THREAD_MSGMAP messageMap=\
	{&TheBaseClass::GetThisMessageMap,&_messageEntries[0]};\
	return &messageMap; \
	}

#define DECLARE_MYTHREAD_MESSAGE_MAP() \
public:\
	static const AFX_THREAD_MSGMAP* GetThisMessageMap();\
	virtual const  AFX_THREAD_MSGMAP* GetMessageMap() const;\
	virtual void	DispatchMessageFunc();

	enum E_MESSAGE_THREAD_STATUS
	{
		E_MESSAGE_THREAD_INIT,//! 准备运行
		E_MESSAGE_THREAD_RUNING, //! 正在运行
		E_MESSAGE_THREAD_STOP,
		E_MESSAGE_THREAD_EXIT //! 退出运行
	};
	class JGWWINDOWSFUNCPLUGIN_CLASS CCMessageThread
	{
	public:
		CCMessageThread(void);
		virtual ~CCMessageThread(void);
	public:
		HANDLE		m_hThread;
		unsigned int  m_dThreadID;

	public:
		//! 关闭当前消息线程
		void			TerminateMessageThread();
		//! 退出线程 等待线程自身退出
		void			ExitThread(bool bWaitExit = false,DWORD dwMilliseconds = INFINITE);
		//! 创建消息线程 CREATE_SUSPENDED TIMEOUT 1S
		bool			CreateMessageThread( unsigned stacksize = 0,unsigned createflag = 0,bool bWaitThreadRun = false );
		//! 向当前线程发送线程消息
		bool			PostThreadMessage(UINT Msg,WPARAM wParam = NULL,LPARAM lParam = NULL,int nCounts = 30);
		//! 判断当前线程是否正在运行
		bool			IsThreadRun();
		//! 设置线程读取消息时间间隔 ms
		void			SetThreadSleepTimeInterval( unsigned int nThreadSleepTimeInterval ){ m_nThreadSleepTimeInterval = nThreadSleepTimeInterval; }
		//！暂停当前线程运行
		void			SuspendMessageThread();
		//! 恢复当前线程运行
		void			ResumeMessageThread();
		//! 设置线程优先级
		/*
		1 idle （最低）THREAD_PRIORITY_IDLE 如果进程优先级为realtime则调整为16，其它情况为1
		2 LOWEST 低 THREAD_PRIORITY_LOWEST -2（在原有基础上-2）
		3 BELOW 低于标准 THREAD_PRIORITY_BELOW_NORMAL -1（在原有基础上-1）
		4 NORMAL（标准）THREAD_PRIORITY_NORMAL 不变(取进程优先级值) 
		5 ABOVE 高于标准 THREAD_PRIORITY_ABOVE_NORMAL +1（在原有基础上+1）
		6 HIGHEST （高）THREAD_PRIORITY_HIGHEST +2（在原有基础上+2）
		7 CRITICAL（最高) THREAD_PRIORITY_TIME_CRITICAL 如果进程优先级为realtime则调整为31，其它情况为15
		*/
		void			SetMessageThreadPriority(int nPriority = THREAD_PRIORITY_NORMAL);
		//! 获取线程优先级
		void			GetMessageThreadPriority(int& nPriority);
	protected:
		//! 线程自运行函数
		virtual void	RunThread(){}
		virtual int		GetThreadSleepTimeInterval();
		DECLARE_MYTHREAD_MESSAGE_MAP()
	private:
		static unsigned _stdcall	ThreadMessageFunc(void *lpParam);
		void						SetThreadStatus( E_MESSAGE_THREAD_STATUS eThreadStatus );
	private:
		E_MESSAGE_THREAD_STATUS		m_bThreadStatus;
    protected:
		unsigned int	m_nThreadSleepTimeInterval;
	};
}