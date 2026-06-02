#include "stdafx.h"
#include "UIBarcode.h"

namespace UiLib
{
    CBarcodeUI::CBarcodeUI(void) : meBarcodeType(BARCODE_TYPE_39),mnBarcodeWidth(1),mfBarcodeRatio(0),mbAutoAdjustWidthHeight(true),mnQRecLevel(0),mnQRencodeMode(2),mnQRecVersion(0),mnQRecMicro(0)
    {
    }


    CBarcodeUI::~CBarcodeUI(void)
    {
    }

    LPCTSTR CBarcodeUI::GetClass() const
    {
        return _T("BarcodeUI");
    }

    LPVOID CBarcodeUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, DUI_CTR_BARCODE) == 0 ) return static_cast<CBarcodeUI*>(this);
        return CContainerUI::GetInterface(pstrName);
    }

    void CBarcodeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcscmp(pstrName, _T("BarcodeType")) == 0 )
        {
            SetBarcodeType((E_BARCODE_TYPE)_ttoi(pstrValue));
        }
        else if (  _tcscmp(pstrName, _T("BarcodeRatio")) == 0 )
        {
            SetBarcodeRatio(static_cast<float>(_ttof(pstrValue)));
        }
        else if(_tcscmp(pstrName, _T("QRecLevel")) == 0 )
        {
            mnQRecLevel = _ttoi(pstrValue);
        }
        else if(_tcscmp(pstrName, _T("QRencodeMode")) == 0 )
        {
            mnQRencodeMode = _ttoi(pstrValue);
        }
        else if(_tcscmp(pstrName, _T("QRecVersion")) == 0 )
        {
            mnQRecVersion = _ttoi(pstrValue);
        }
        else if(_tcscmp(pstrName, _T("QRecMicro")) == 0 )
        {
            mnQRecMicro = _ttoi(pstrValue);
        }
        else if (  _tcscmp(pstrName, _T("BarcodeWidth")) == 0 )
        {
            SetBarcodeWidth(_ttoi(pstrValue));
        }
        else if (  _tcscmp(pstrName, _T("AutoAdjustWidthHeight")) == 0 )
        {
            SetAutoAdjustWidthHeight(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else if (  _tcscmp(pstrName, _T("text")) == 0 )
        {
            SetText(pstrValue);
        }
        else
            CContainerUI::SetAttribute(pstrName,pstrValue);
    }

    void CBarcodeUI::SetAutoAdjustWidthHeight(bool bAutoAdjustWidthHeight)
    {
        mbAutoAdjustWidthHeight = bAutoAdjustWidthHeight;
        //Invalidate();
    }

    bool CBarcodeUI::GetAutoAdjustWidthHeight() const
    {
        return mbAutoAdjustWidthHeight;
    }

    void CBarcodeUI::AutoAdjustWidthHeight()
    {
        if (!mbAutoAdjustWidthHeight) return ;
        RECT rc = GetPos();
        int width = 0,heigth = 0;
        switch (meBarcodeType)
        {
        case BARCODE_TYPE_93:
            width = mBarcode93.GetEncodeWidth(mnBarcodeWidth);
            heigth = mBarcode93.GetEncodeHeight(mnBarcodeWidth);
            break;
        case BARCODE_TYPE_128A:
        case BARCODE_TYPE_128C:
        case BARCODE_TYPE_128B:
            width = mBarcode128.GetEncodeWidth(mnBarcodeWidth);
            heigth = mBarcode128.GetEncodeHeight(mnBarcodeWidth);
            break;
        case BARCODE_TYPE_I2of5:
            width = mBarcodeI2of5.GetEncodeWidth(mnBarcodeWidth);
            heigth = mBarcodeI2of5.GetEncodeHeight(mnBarcodeWidth);
            break;
        case BARCODE_TYPE_QR:
            width = mBarcodeQR.GetEncodeWidth(mnBarcodeWidth);
            heigth = mBarcodeQR.GetEncodeHeight(mnBarcodeWidth);
            break;
        default:
            width = mBarcode39.GetEncodeWidth(mnBarcodeWidth);
            heigth = mBarcode39.GetEncodeHeight(mnBarcodeWidth);
            //mBarcode39.Draw39(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
            break;
        }
        if (heigth > 0)
        {
            SetFixedHeight(heigth + (m_rcPadding.bottom - m_rcPadding.top));
        }
        if (width > 0)
        {
            SetFixedWidth(width + (m_rcPadding.right - m_rcPadding.left));
        }
    }


    void CBarcodeUI::SetText(LPCTSTR pstrText)
    {
        mstrBarcode = pstrText;
        switch (meBarcodeType)
        {
        case BARCODE_TYPE_93:
            mBarcode93.Encode93(mstrBarcode.GetStringA().c_str());
            break;
        case BARCODE_TYPE_128A:
            mBarcode128.Encode128A(mstrBarcode.GetStringA().c_str());
            break;
        case BARCODE_TYPE_128B:
            mBarcode128.Encode128B(mstrBarcode.GetStringA().c_str());
            break;
        case BARCODE_TYPE_128C:
            mBarcode128.Encode128C(mstrBarcode.GetStringA().c_str());
            break;
        case BARCODE_TYPE_I2of5:
            mBarcodeI2of5.EncodeI2of5(mstrBarcode.GetStringA().c_str());
            break;
        case BARCODE_TYPE_QR:
            mBarcodeQR.QRCodeSetting(mnQRecLevel,mnQRencodeMode,mnQRecVersion,mnQRecMicro);
            mBarcodeQR.Encode(mstrBarcode.GetStringA().c_str(),GetBarcodeWidth());
            break;
        default:
            mBarcode39.Encode39(mstrBarcode.GetStringA().c_str());
            break;
        }
        Invalidate();
    }

    float CBarcodeUI::GetBarcodeRatio()
    {
        return mfBarcodeRatio;
    }

    void CBarcodeUI::SetBarcodeRatio(float fBarcodeRatio)
    {
        mfBarcodeRatio = fBarcodeRatio;
        //Invalidate();
    }

    CDuiString CBarcodeUI::GetText() const
    {
        return mstrBarcode;
    }

    void CBarcodeUI::SetBarcodeType(E_BARCODE_TYPE eBarcodeType)
    {
        meBarcodeType = eBarcodeType;
        //Invalidate();
    }

    E_BARCODE_TYPE CBarcodeUI::GetBarcodeType() const
    {
        return meBarcodeType;
    }

    void CBarcodeUI::SetBarcodeWidth(int nBarcodeWidth)
    {
        mnBarcodeWidth = nBarcodeWidth;
        if (meBarcodeType == BARCODE_TYPE_QR)
        {
            mBarcodeQR.Encode(mstrBarcode.GetStringA().c_str(),GetBarcodeWidth());
        }
        //Invalidate();
    }

    int CBarcodeUI::GetBarcodeWidth() const
    {
        return mnBarcodeWidth;
    }

    void CBarcodeUI::PaintBkImage(HDC hDC)
    {
        if (!mstrBarcode.IsEmpty())
        {
            RECT rcPaint = m_rcPaint;
            COLORREF clrBar(0x00),clrSpace(0xFFFFFF);
            switch (meBarcodeType)
            {
            case BARCODE_TYPE_93:
                //mBarcode93.Encode93(mstrBarcode.GetStringA().c_str());
                mBarcode93.SetBarcodeRatio(mfBarcodeRatio);
                mBarcode93.Draw93(hDC,m_rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;
            case BARCODE_TYPE_128A:
                mBarcode128.SetBarcodeRatio(mfBarcodeRatio);
                //mBarcode128.Encode128A(mstrBarcode.GetStringA().c_str());
                mBarcode128.Draw128(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                
                break;
            case BARCODE_TYPE_128B:
                mBarcode128.SetBarcodeRatio(mfBarcodeRatio);
                //mBarcode128.Encode128B(mstrBarcode.GetStringA().c_str());
                mBarcode128.Draw128(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;
            case BARCODE_TYPE_128C:
                mBarcode128.SetBarcodeRatio(mfBarcodeRatio);
                //mBarcode128.Encode128C(mstrBarcode.GetStringA().c_str());
                mBarcode128.Draw128(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;
            case BARCODE_TYPE_I2of5:
                mBarcodeI2of5.SetBarcodeRatio(mfBarcodeRatio);
                mBarcodeI2of5.DrawI2of5(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;
            case BARCODE_TYPE_QR:
                mBarcodeQR.Draw(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;;
            default:
                mBarcode39.SetBarcodeRatio(mfBarcodeRatio);
                //mBarcode39.Encode39(mstrBarcode.GetStringA().c_str());
                mBarcode39.Draw39(hDC,rcPaint.left + m_rcPadding.left,rcPaint.top + m_rcPadding.top,rcPaint.bottom - m_rcPadding.bottom,clrBar,clrSpace,mnBarcodeWidth);
                break;
            }
            AutoAdjustWidthHeight();
        }
    }
}
