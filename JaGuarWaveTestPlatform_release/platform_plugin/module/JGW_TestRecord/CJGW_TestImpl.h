#pragma once
#if 0
/*
测试线程接口
*/
namespace JGW
{
    class CCJGW_TestImpl
    {
    public:
        virtual bool OnInit(WPARAM wParam,LPARAM lParam) = 0;

        virtual bool OnRun(WPARAM wParam,LPARAM lParam) = 0;

        virtual bool OnExit(WPARAM wParam,LPARAM lParam) = 0;
    };
}
#endif