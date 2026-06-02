#include "stdafx.h"
#include <vector>
typedef bool (*LPTSE_RunSuiteToIndex) (size_t index);

struct S_TSE_IF_RULE;

struct S_TSE_SUITE_FUNC
{
    size_t mnIndex;
    LPTSE_RunSuiteToIndex mpTSE_RunSuiteToIndex;
    S_TSE_IF_RULE* mpTSEIFRule; //! 是否有IF规则
}; 

//! IF 条件
typedef struct
{
    bool mbResultIsFalse; //! 取反，结果是否为false ,true : 表示当前结果为false即可,否则结果必须为true
    bool mbTSESuiteResult; //! 取SUITE 结果还是运行suite函数
    int mnOperation; //! 运算符 0 : && 运算符  1 ： || 运算符 -1 : 表示后面没有运算符
    int mnTSESuiteResultIndex; //! suite 索引
    S_TSE_SUITE_FUNC mpsTSESuiteFunc;
}S_TSE_IF_CONDITION;

//! IF规则方案
typedef struct S_TSE_IF_RULE
{
    std::vector<S_TSE_IF_CONDITION> mvTSEIFCondition;
    std::vector<S_TSE_SUITE_FUNC> mvTSEIFSuiteFunc; //! 如果为true
    std::vector<S_TSE_SUITE_FUNC> mvTSEFALSESuiteFunc; //! 如果未false
}S_TSE_IF_RULE_H;

std::vector<S_TSE_SUITE_FUNC> gvTSESuiteFunc;
std::vector<bool> gvTSESuiteResult;

bool TSE_RunSuiteToIndex(size_t index)
{
    return true;
}
//! 默认情况下返回false
bool TSE_IFCondition(S_TSE_IF_CONDITION* pIFCondition)
{
    bool bResult = false/*pIFCondition->mbResultIsFalse*/;
    if (pIFCondition->mbTSESuiteResult)
    {
        bResult = gvTSESuiteResult[pIFCondition->mnTSESuiteResultIndex];
    }
    else
    {
       bResult = pIFCondition->mpsTSESuiteFunc.mpTSE_RunSuiteToIndex(pIFCondition->mpsTSESuiteFunc.mnIndex);
    }

    return bResult/*pIFCondition->mbResultIsFalse?!bResult:bResult*/;
}


bool TSE_IFFunc(S_TSE_SUITE_FUNC* psSuiteFunc)
{
    //! 如果允许为true 则直接返回
    bool bSuiteResult = gvTSESuiteFunc[i].mpTSE_RunSuiteToIndex(gvTSESuiteFunc[i].mnIndex);
    //! 测试判断调节
    for ()


    return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
    for (size_t i = 0;i < gvTSESuiteFunc.size();i ++)
    {
        //! 检查当前INDEX是否有IF规则，如果没有IF规则则直接判断当前测试序列
        if (NULL == gvTSESuiteFunc[i].mpTSEIFRule)
        {
            if (!gvTSESuiteFunc[i].mpTSE_RunSuiteToIndex(gvTSESuiteFunc[i].mnIndex)) return -1;
        }
        else
        {
            if (!TSE_IFFunc(&gvTSESuiteFunc[i])) return -1;
        }
    }

    return 0;
}