#pragma once

namespace JGW
{
    class CCJGW_TestSuiteImpl
    {
    public:
        CCJGW_TestSuiteImpl():mnTestCount(1){}
        virtual ~CCJGW_TestSuiteImpl(){}
        //!
        enum eTestState
        { 
            Idle, //the standard state for a test that hasn't been run yet
            Initing,	//Invoker() has been called, and has called the Init() method
            Running,	//the Run() method has been evoked via Start() or Loop()
            Looping,	//looping implies running.
            Exiting,	//Exit() method is called
            Finished,	//A test run has completed (current result is valid)
            Aborted,	//A brute-force stop-- could mean trouble
            Stopped,	//A normal stop, ie the test stopped cooperatively, Exit() will be called
            Paused,		//embedded mostly, but could be used for looping

            BreakPoint	//we've reached a breakpoint
        }; //others tbd
    public:
        //! create param
        virtual bool CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue) 
        {
            PDLOG4WW_F(_T("%s    %s"),strParamName,strParamValue);
            if( 0 == JGW_WStrComparenoCaseWStr(_T("RetryCount"),strParamName) )
            {
                mnTestCount = _ttoi(strParamValue);
                return true;
            }
            return false;
        }
        //!
        virtual bool UpdateEnviromentParam() { return true;}
        //! init test
        virtual bool Init(void) { return true;}
        //! run test
        virtual bool Run(void) { return true;}
        //! run exit
        virtual bool Exit(void) { return true;}

        virtual size_t GetErrorRetryTestCount(void){ return mnTestCount;}
    protected:
        size_t mnTestCount;
    };
}