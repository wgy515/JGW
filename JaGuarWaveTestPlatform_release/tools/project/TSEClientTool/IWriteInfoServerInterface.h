#pragma once
#include <string>
#include <queue>

namespace JGW
{
    class IWriteInfoServerInterface
    {
    public:
        virtual void WriteInfoToTcpServer(std::queue<std::string>* pWriteMsgQueue) = 0;
    };

}