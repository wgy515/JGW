#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_Clipboard.h>

namespace JGW
{
    CCJGW_Clipboard::CCJGW_Clipboard(void)
    {
    }


    CCJGW_Clipboard::~CCJGW_Clipboard(void)
    {
    }

    std::wstring CCJGW_Clipboard::GetClipBoardText(HWND hWnd,std::wstring& strClipBoardText)
    {
        _ASSERT(hWnd);
        //判断剪贴板的数据格式是否可以处理。
        if (!IsClipboardFormatAvailable(MY_CLIP_FORMAT)) 
            return strClipBoardText; 

        //打开剪贴板。        
        if (!OpenClipboard(hWnd))
            return strClipBoardText; 

        //获取数据
        HGLOBAL hMem = GetClipboardData(MY_CLIP_FORMAT); 
        if (hMem != NULL)         
        { 
            //获取字符串。
            LPTSTR lpStr = (LPTSTR)GlobalLock(hMem); 
            if (lpStr != NULL)      
            { 
                strClipBoardText = lpStr;
                //释放锁内存
                GlobalUnlock(hMem);     
            }          
        }  

        //关闭剪贴板      
        CloseClipboard();  
        return strClipBoardText;
    }

    bool CCJGW_Clipboard::SetClipBoardText(const wchar_t* text,HWND hWnd)
    {
        _ASSERT(hWnd);
        //打开剪贴板
        if ( !OpenClipboard(hWnd) )
            return false;
        //empties the clipboard and frees handles to data in the clipboard
        if ( !EmptyClipboard() )
        {
            CloseClipboard();
            return false;
        }
        //get text length
        int len = wcslen(text);
        //After SetClipboardData is called, the system owns the object identified by the hMem parameter. 
        //The application can read the data, but must not free the handle or leave it locked. If the 
        //hMem parameter identifies a memory object, the object must have been allocated using the 
        //GlobalAlloc function with the GMEM_MOVEABLE and GMEM_DDESHARE flags. 
        HANDLE hClip=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,(len + 1) * sizeof(TCHAR));
        if (hClip==NULL)
        {
            CloseClipboard();
            return false;
        }
        //locks a global memory object and returns a pointer to the first byte of the object's memory block
        TCHAR* pBuf=(TCHAR*)GlobalLock(hClip);
        if (pBuf==NULL)
        {
            GlobalFree(hClip);  
            CloseClipboard();
            return false;
        }

        memcpy(pBuf,text,len * sizeof(TCHAR));
        pBuf[len] = L'\0';
        GlobalUnlock(hClip);

        if (NULL == SetClipboardData(MY_CLIP_FORMAT,hClip))
        {
            GlobalFree(hClip);  
            CloseClipboard();
            return false;
        }
        CloseClipboard();
        return true;
    }
}