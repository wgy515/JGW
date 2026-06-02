// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_IPCPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include <string>
#include <algorithm>

//! Character array of all of the ASCII characters that are considered whitespace.
static const wchar_t WHITE_SPACE[] = L"\t\r\n \f\v";
/*!
CLASS
is_white_space

Function object for use with STL algorithms

USAGE
Example using with remove_if

// removes all whitespace from string
myString.erase( remove_if(theString.begin(), theString.end(), is_white_space()), theString.end() );
*/
class is_white_space
{
public:
    is_white_space() : len(wcslen(WHITE_SPACE)) {}
    bool operator()(const wchar_t c) const
    {
        const wchar_t* result = std::find(WHITE_SPACE, WHITE_SPACE + len, c);
        return (result != WHITE_SPACE + len);
    }
private:
    const int len;
};

std::wstring AllCAPSRemoveWhiteSpace( std::wstring theString )
{
    // Remove all whitespace
    theString.erase( std::remove_if(theString.begin(), theString.end(), is_white_space()), theString.end());
    // Make all uppercase
    std::transform(theString.begin(), theString.end(), theString.begin(), toupper);
    return theString;
}

