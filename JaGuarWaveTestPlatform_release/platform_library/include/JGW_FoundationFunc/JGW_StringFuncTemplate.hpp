// ×Ö·û´®²Ù×÷Ä£°åÀà
#ifndef JGW_COMMON_FUNCLIB_STRINGFUNC_TEMPLATE_H__
#define JGW_COMMON_FUNCLIB_STRINGFUNC_TEMPLATE_H__
#include <string>
#include <vector>

namespace JGW
{
	template<class strCls>
	class JGW_StringFunc
	{
	public:
		static void ReplaceString(strCls& str, const strCls& src, const strCls& des)
		{
			int pos = -1;
			int curPos = 0;

			while (-1 != (pos = str.find(src, curPos)))
			{
				str.replace(pos, src.size(), des);
				curPos = pos + des.size();
			}
		}

        static void RtrimChar(strCls& str,char eraseString)
        {
            int pos = -1;
            int curPos = str.size() - 1;

            while ( curPos >= 0 && eraseString == str.at(curPos) )
            {
                str.erase(curPos,1);
                curPos --;
            }
        }

		static void Rtrim(strCls& str)
		{
			int pos = -1;
			int curPos = str.size() - 1;

			while ( curPos >= 0 && ' ' == str.at(curPos) )
			{
				str.erase(curPos,1);
				curPos --;
			}
		}

		static void Ltrim(std::wstring& str)
		{
			int pos = -1;
			int curPos = 0;

			while ( !str.empty() &&  ' ' == str.at(curPos) )
			{
				str.erase(curPos,1);
			}
		}

		static std::wstring FormatWString(std::wstring& strFormat,const wchar_t *szFormat,...)
		{
			int nListCount = 0;

			{
				va_list pArgList;
				va_start(pArgList,szFormat);
				int nLength = _vscwprintf(szFormat,pArgList) + 1;
				strFormat.resize(nLength);
				nListCount +=_vsnwprintf_s(const_cast<wchar_t *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

				if( -1 == nListCount ) throw L"buffer too small";

				va_end(pArgList);
			}

			strFormat.reserve(nListCount + 1);

			return strFormat;
		}


		static std::string FormatString(std::string& strFormat,const char *szFormat,...)
		{
			int nListCount = 0;

			{
				va_list pArgList;
				va_start(pArgList,szFormat);
				int nLength = _vscprintf(szFormat,pArgList) + 1;
				strFormat.resize(nLength);
				nListCount +=_vsnprintf_s(const_cast<char *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

				if( -1 == nListCount ) return "buffer too small";

				va_end(pArgList);
			}

			strFormat.reserve(nListCount + 1);

			return strFormat;
		}

		static void StokString(const strCls ListStr,const strCls  delimiter,std::vector<strCls> &_vectorStr)
		{
			int nPos = -1;
			int curPos = 0;
			strCls strTemp ;

			if ( ListStr.size() && delimiter.size())
			{
				while (  -1 !=(nPos = ListStr.find(delimiter, curPos)) )
				{
					strTemp = ListStr.substr(curPos,nPos - curPos);
					_vectorStr.push_back(strTemp);
					curPos = nPos + delimiter.size();
				}

				_vectorStr.push_back(ListStr.substr(curPos/*,nPos*/));
			}
		}
	};

	//! Delete pointer object.
	/*!
	\ingroup _GROUP_UTILFUNC
	\param p pointer object created using 'new'.
	*/
	template<class T>
	void SafeDelete(T*& p)
	{
		if (p != NULL)
			delete p;
		p = NULL;
		*(&p) = NULL;
	}

	//! Delete pointer array object.
	/*!
	\ingroup _GROUP_UTILFUNC
	\param p pointer array object created using 'new []'.
	*/
	template<class T>
	void SafeDeleteArray(T*& p)
	{
		if (p != NULL)
			delete []p;
		p = NULL;
	}

	//! Delete all elements in a container.
	/*!
	\ingroup _GROUP_UTILFUNC
	\param container STL container variable (vector, list, map). eg: " vector<int*> arr; "
	*/
	template<class CONTAINER>
	void DeletePtrInContainer(CONTAINER& container)
	{
		typename CONTAINER::iterator it = container.begin();
		for(; it != container.end(); ++it)
			SafeDelete(*it);
		container.resize(0);
	}
}

#endif