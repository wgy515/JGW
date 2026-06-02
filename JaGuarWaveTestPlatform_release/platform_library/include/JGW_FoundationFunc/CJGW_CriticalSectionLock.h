#pragma once
namespace JGW
{
    class __declspec(dllexport) CCJGW_CriticalSectionLock
    {
    public:
        CCJGW_CriticalSectionLock()
        {
            InitializeCriticalSection(&mcs);
        }

        ~CCJGW_CriticalSectionLock()
        {
            DeleteCriticalSection(&mcs);
        }

        CRITICAL_SECTION& GetCriticalSection()
        {
            return mcs;
        }
    private:
        CRITICAL_SECTION mcs;
    };


	class __declspec(dllexport) CCJGW_CriticalSectionAutoLock
	{
	public:
		CCJGW_CriticalSectionAutoLock(CCJGW_CriticalSectionLock& cs):mcs(cs)
		{
			Lock();
		}

		virtual ~CCJGW_CriticalSectionAutoLock(void)
		{
			UnLock();
		}

		void Lock()
		{
			EnterCriticalSection(&mcs.GetCriticalSection());
		}

		void UnLock()
		{
			return LeaveCriticalSection(&mcs.GetCriticalSection());
		}
	private:
		CCJGW_CriticalSectionLock& mcs;
	};
}
