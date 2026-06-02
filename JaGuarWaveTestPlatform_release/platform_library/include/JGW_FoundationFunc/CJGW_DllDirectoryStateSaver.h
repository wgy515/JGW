#pragma once

namespace JGW
{
    class CCJGW_DllDirectoryStateSaver
    {
    public:
        CCJGW_DllDirectoryStateSaver(const wchar_t* strDllDriectory)
        {
            GetDllDirectory(512,mstrOldDllDirectory);
            SetDllDirectory(strDllDriectory);
        }

        ~CCJGW_DllDirectoryStateSaver()
        {
            SetDllDirectory(mstrOldDllDirectory);
        }
    private:
        wchar_t mstrOldDllDirectory[512];
    };
}
