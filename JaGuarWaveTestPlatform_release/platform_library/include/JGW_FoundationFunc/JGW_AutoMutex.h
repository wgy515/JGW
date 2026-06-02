#ifndef JGW_AUTO_MUTEX_H__
#define JGW_AUTO_MUTEX_H__

namespace JGW
{
	class JGW_Mutex
	{
	public:
		JGW_Mutex( const wchar_t* strMutexName = NULL )
		{
			mMutex_Handle = CreateMutex( NULL,FALSE,strMutexName );
		}

		~JGW_Mutex()
		{
			CloseHandle(mMutex_Handle);
		}

		bool Lock()
		{
			return (WAIT_OBJECT_0 == WaitForSingleObject(mMutex_Handle,INFINITE));
		}

		bool UnLock()
		{
			return (0 != ReleaseMutex(mMutex_Handle));
		}

	private:
		HANDLE mMutex_Handle;
	};

	class JGW_AutoMutex
	{
	public:
		JGW_AutoMutex()
		{
			mMutex_Handle = CreateMutex(NULL,FALSE,NULL);
			Lock();
		}
		~JGW_AutoMutex()
		{
			UnLock();
			CloseHandle(mMutex_Handle);
		}

		bool Lock()
		{
			return (WAIT_OBJECT_0 == WaitForSingleObject(mMutex_Handle,INFINITE));
		}

		bool UnLock()
		{
			return (0 != ReleaseMutex(mMutex_Handle));
		}

	private:
		HANDLE mMutex_Handle;
	};

}


#endif