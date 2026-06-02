//
// UDPEchoServer.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/UDPEchoServer.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "stdafx.h"
#include "UDPEchoServer.h"
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>
#include <poco/InflatingStream.h>
#include <poco/StreamCopier.h>
#include <poco/MemoryStream.h>
#include <iostream>
#include <sstream>


using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;


UDPEchoServer::UDPEchoServer():
	_thread("UDPEchoServer"),
	_stop(false)
{
	_socket.bind(SocketAddress(), true);
	_thread.start(*this);
	_ready.wait();
}


UDPEchoServer::UDPEchoServer(const SocketAddress& sa):
	_thread("UDPEchoServer"),
	_stop(false)
{
	_socket.bind(sa, true);
	_thread.start(*this);
	_ready.wait();
}


UDPEchoServer::~UDPEchoServer()
{
	_stop = true;
	_thread.join();
}


Poco::UInt16 UDPEchoServer::port() const
{
	return _socket.address().port();
}


void UDPEchoServer::run()
{
	Poco::Timespan span(250000);
	while (!_stop)
	{
		_ready.set();
		if (_socket.poll(span, Socket::SELECT_READ))
		{
			try
			{
				char buffer[256];
				SocketAddress sender;
				int n = _socket.receiveFrom(buffer, sizeof(buffer), sender);
                if (n > 0)
                {
                    //buffer[n] = '\0';
#if 1
                    Poco::MemoryInputStream istr(buffer,n);
                    Poco::InflatingInputStream inflater(istr);
                    //! StreamCopier::copyStream(istr, deflater);

                    std::ostringstream buffer1;
                    Poco::StreamCopier::copyStream(inflater, buffer1);

                     std::cout << buffer1.str() << std::endl;

#endif

                   
                }
                
				//n = _socket.sendTo(buffer, n, sender);
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "UDPEchoServer: " << exc.displayText() << std::endl;
			}
		}
	}
}


SocketAddress UDPEchoServer::address() const
{
	return _socket.address();
}
