#include "StdAfx.h"
#include <qrencode/CJGW_QRenCode.h>

BarcodeBase::BarcodeBase() : mfBarcodeRatio(0)
{
    Clear();
    i_Ratio = 3;
}

void BarcodeBase::operator=(const BarcodeBase&bc) {
    i_LenBuf = bc.i_LenBuf;
    i_Ratio = bc.i_Ratio;
    memcpy(ia_Buf, bc.ia_Buf, sizeof(ia_Buf));
}

void BarcodeBase::Clear() {
    memset(ia_Buf, 0, sizeof(ia_Buf));
    i_LenBuf = 0;
}

void BarcodeBase::SetBarcodeRatio(float fBarcodeRatio)
{
    mfBarcodeRatio = fBarcodeRatio;
}

int BarcodeBase::GetEncodeLength()	const {
    BYTE*pb = (BYTE*)ia_Buf;
    int i, iLen = 0;
    for (i = 0; i < i_LenBuf; i++) {
        //wide is 3
        if (*pb & 2)	iLen += (i_Ratio - 1);
        pb++;
    }
    return iLen + i_LenBuf;
}
int BarcodeBase::GetBufferLength()	const {
    return i_LenBuf;
}
const BYTE& BarcodeBase::GetAt(int i)	const {
    return ia_Buf[i];
}
int BarcodeBase::GetRatio()	const {
    return i_Ratio;
}
int BarcodeBase::SetRatio(int iRatio) {
    i_Ratio = iRatio;
    if (i_Ratio <= 0)	i_Ratio = 1;
    return i_Ratio;
}

int BarcodeBase::GetEncodeWidth(int iPenW)
{
    int width = 0;
    BYTE*pb = ia_Buf;
    int i0, iNum0 = i_LenBuf,iNum1,i1;
    for (i0 = 0; i0 < iNum0; i0++) 
    {
        iNum1 = (*pb & 0x02) ? i_Ratio : 1;
        for (i1 = 0; i1 < iNum1; i1++) 
        {
            width += iPenW;
        }
    }
    return width;
}

int BarcodeBase::GetEncodeHeight(int iPenW)
{
    if (mfBarcodeRatio > 0)
    {
        int height = static_cast<int>(GetEncodeWidth(iPenW) * mfBarcodeRatio);
        return height;
    } 
    return -1;
}

void BarcodeBase::DrawBarcode(HDC hDC, int iX, int iY0, int iY10, int iY11, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
    HPEN hPenBar = ::CreatePen(PS_SOLID, iPenW, clrBar);
    HPEN hPenSpace = ::CreatePen(PS_SOLID, iPenW, clrSpace);
    HPEN hPenOld = (HPEN)::SelectObject(hDC, hPenBar);

    BYTE*pb = ia_Buf;
    int i0, iNum0 = i_LenBuf;

    char szLog[20] = {0};
    BYTE bBar;
    int i1, iNum1;
    int iY;

    if (mfBarcodeRatio > 0)
    {
        int width = iX;
        for (i0 = 0; i0 < iNum0; i0++) 
        {
            iNum1 = (*pb & 0x02) ? i_Ratio : 1;
            for (i1 = 0; i1 < iNum1; i1++) 
            {
                width += iPenW;
            }
        }
        int height = static_cast<int>((width - iX) * mfBarcodeRatio);
        if (height > 0)
        {
            iY10 = iY0 + height;
            iY11 = iY10;
        }
    }
    

    for (i0 = 0; i0 < iNum0; i0++) {
        bBar = *pb & 0x01;
        iNum1 = (*pb & 0x02) ? i_Ratio : 1;
        //sprintf_s(szLog,20,"%d",iNum1);
        //OutputDebugStringA(szLog);

        iY = (*pb & 0x04) ? iY11 : iY10;
        for (i1 = 0; i1 < iNum1; i1++) {
            if (bBar)	::SelectObject(hDC, hPenBar);
            else		::SelectObject(hDC, hPenSpace);

            ::MoveToEx(hDC, iX, iY0, 0);
            ::LineTo(hDC, iX, iY);
            iX += iPenW;
        }
        pb++;
    }
    //OutputDebugStringA("\n");
    ::SelectObject(hDC, hPenOld);

    ::DeleteObject(hPenBar);
    ::DeleteObject(hPenSpace);
}



BarcodeQR::BarcodeQR() : mpQRcode(NULL)
{

}

BarcodeQR::~BarcodeQR()
{
    if (mpQRcode)
    {
        FreeQRCode(((QRcode*)mpQRcode));
        mpQRcode = NULL;
    }
}


BOOL BarcodeQR::Encode(const char*pszCodeIn,int size/* = 3*/) 
{
    if (mpQRcode)
    {
        FreeQRCode(((QRcode*)mpQRcode));
        mpQRcode = NULL;
    }
    mpQRcode = EncodeQRCode((const unsigned char*)pszCodeIn,strlen(pszCodeIn),1/*size*/);
    return mpQRcode != NULL;
}

void BarcodeQR::QRCodeSetting(int level /* = 0 */,int hint /* = 2 */,int version /* = 0 */,int micro /* = 0 */)
{
    InitQRCode(level,hint,version,micro);
}

int BarcodeQR::GetEncodeWidth(int nPenW)
{
    if (NULL == mpQRcode) return -1;
    return ((QRcode*)mpQRcode)->width * nPenW + (nPenW/4 >= 1 ?nPenW/4:1) + nPenW;
}

int BarcodeQR::GetEncodeHeight(int nPenW)
{
    if (NULL == mpQRcode) return -1;
    return ((QRcode*)mpQRcode)->width * nPenW + (nPenW/4 >= 1 ?nPenW/4:1) + nPenW;
}

void BarcodeQR::Draw(HDC hDC, int iX, int iY0, int iY1, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW)
{
    if (!mpQRcode) return;
    int nPenW = iPenW;
    HPEN hPenBar = ::CreatePen(PS_SOLID, nPenW, clrBar);
    HBRUSH hBrush = ::CreateSolidBrush(clrSpace);
    //HPEN hPenSpace = ::CreatePen(PS_SOLID, (nPenW/4 >= 1 ?nPenW/4:1), clrSpace);

    HPEN hPenOld = (HPEN)::SelectObject(hDC, hPenBar);
    unsigned char *row, *p;
    int rle = 0;
    RECT rc;
    p = ((QRcode*)mpQRcode)->data;

    rc.left = iX - nPenW;
    rc.right = iX + ((QRcode*)mpQRcode)->width * nPenW + (nPenW/3 >= 1 ?nPenW/3:1);
    rc.top = iY0 - nPenW;
    rc.bottom = iY0 + ((QRcode*)mpQRcode)->width * nPenW + (nPenW/3 >= 1 ?nPenW/3:1);
    ::FillRect(hDC, &rc, hBrush);
    ::DeleteObject(hBrush);
  
    for(int y = 0; y < ((QRcode*)mpQRcode)->width; y++) 
    {
        row = (p+(y*((QRcode*)mpQRcode)->width));
        if( !rle ) 
        {
            /* no RLE */
            for(int x = 0; x < ((QRcode*)mpQRcode)->width; x++) 
            {
                if(*(row+x)&0x1) 
                {
                    rc.left = iX + x * nPenW;
                    rc.right = rc.left + (nPenW/3 >= 1 ?nPenW/3:1);
                    rc.top = iY0 + y * nPenW;
                    rc.bottom = rc.top + (nPenW/3 >= 1 ?nPenW/3:1);

                    ::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
                }
                /*else
                {
                    ::SelectObject(hDC, hPenSpace);
                }*/
            }
        }
    }
   
    /*BYTE*pb = ia_Buf;
    int i0, iNum0 = i_LenBuf;

    BYTE bBar;
    int i1, iNum1;
    int iY;
    for (i0 = 0; i0 < iNum0; i0++) {
    bBar = *pb & 0x01;
    iNum1 = (*pb & 0x02) ? i_Ratio : 1;
    iY = (*pb & 0x04) ? iY11 : iY10;
    for (i1 = 0; i1 < iNum1; i1++) {
    if (bBar)	::SelectObject(hDC, hPenBar);
    else		::SelectObject(hDC, hPenSpace);

    ::MoveToEx(hDC, iX, iY0, 0);
    ::LineTo(hDC, iX, iY);
    iX += iPenW;
    }
    pb++;
    }*/

    ::SelectObject(hDC, hPenOld);

    ::DeleteObject(hPenBar);
    //::DeleteObject(hPenSpace);
}