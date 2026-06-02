/******************************************************************************
* $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/NetworkInstrumentProtocol.cpp#12 $
* $DateTime: 2016/04/04 18:48:45 $
*
*
******************************************************************************
*
* Copyright (c) 2008-2016 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
******************************************************************************
*/

#include "stdafx.h"
#include <JGW_NIGPIBControllImpl/NetworkInstrumentProtocol.h>
#include <sstream>
/*#include "Utilities/UI/uimanagerfactory.h"*/

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

#ifndef _VISA32_SUPPORT_

CNetworkInstrumentProtocol::CNetworkInstrumentProtocol():m_lid(0),
    m_xid(0),
    m_maxRecvSize(maxBufferSize)
{
    /* m_pUIError=CUIManagerFactory::GetUIManager();*/

    // Set the fixed Abort Channel parameters
    m_abortChannel.prog=abortChannel_prog;
    m_abortChannel.prot=abortChannel_prot;
    m_abortChannel.vers=abortChannel_vers;

    // Set the fixed Core Channel parameters
    m_coreChannel.prog=coreChannel_prog;
    m_coreChannel.vers=coreChannel_vers;
    m_coreChannel.prot=coreChannel_prot;

    // Set the fixed Portmapper parameters
    m_portmapper.port=portmapper_port;
    m_portmapper.prog=portmapper_prog;
    m_portmapper.prot=portmapper_prot;
    m_portmapper.vers=portmapper_vers;
}

CNetworkInstrumentProtocol::~CNetworkInstrumentProtocol()
{
    // If the link is still active then try to destroy it
    if(0!=m_lid)
    {
        destroy_link(m_lid);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Network Instrument Protocol Layer Functions
///////////////////////////////////////////////////////////////////////////////
Create_LinkResp CNetworkInstrumentProtocol::create_link(const Create_LinkParms parms)
{
   char device[6];
   Create_LinkParms createLinkParms=parms;
   Create_LinkResp resp;
   XDR_createLinkParms data;
   unsigned long dataSize;

   // "INST0" should always be accepted (VXI-11.3 Rule B.1.2)
   // inst is defaulted to 0
   sprintf_s(device,"INST%d",createLinkParms.inst);
   // Fill out the response structure with default values
   resp.error=VXI11Error_NoError;
   resp.lid=0;
   resp.abortPort=0;
   resp.maxRecvSize=maxBufferSize;

    // Check that this is the first time create_link has been called
    if(0!=m_lid)
    {
        Log4WE(L"Error attempting to create NIP link: Link already established");
        resp.lid=m_lid;
        resp.abortPort=m_abortChannel.port;
        resp.error=VXI11Error_DeviceNotAccessible;
        return resp;
    }

    // Initialise the TCP layer
    if(0!=TCP_Init())
    {
        resp.error=VXI11Error_DeviceNotAccessible;
        return resp;
    }

    // Use portmapper to obtain the remote Core Channel port
    if(0==(m_coreChannel.port=RPC_PMAPPROC_GETPORT(parms.device,m_coreChannel.prog,m_coreChannel.vers,m_coreChannel.prot,0,parms.sourceAddress.empty() ? NULL : parms.sourceAddress.c_str())))
    {
        resp.error=VXI11Error_DeviceNotAccessible;
        return resp;
    }

    // Open the Core Channel
    if(INVALID_SOCKET==(m_coreChannel.socket=TCP_OpenSocket(parms.device,m_coreChannel.port,parms.sourceAddress.empty() ? NULL : parms.sourceAddress.c_str())))
    {
        resp.error=VXI11Error_DeviceNotAccessible;
        return resp;
    }

    // Prepare to call the instrument's create_link function
    m_coreChannel.proc=VXI11Procedure_create_link;

    // Fill out the data structure with the create_link paramaters
    data.clientID=static_cast<unsigned long>(parms.clientId);
    data.lockDevice=static_cast<unsigned long>(parms.lockDevice);
    data.lockTimeout=parms.lock_timeout;
    if(0!=XDR_formatString(device,sizeof(data.device),reinterpret_cast<char*>(data.device),&data.stringLength,&dataSize))
    {
        resp.error=VXI11Error_ParameterError;
        return resp;
    }

    // Call the instrument's create_link function
    if(0!=RPC_Call(m_coreChannel,sizeof(data),reinterpret_cast<unsigned char*>(&data)))
    {
        resp.error=VXI11Error_IOError;
        return resp;
    }

    // Fetch the response from the instrument's create_link function
    if(0!=RPC_Reply(m_coreChannel,sizeof(resp),reinterpret_cast<unsigned char*>(&resp)))
    {
        resp.error=VXI11Error_IOError;
        return resp;
    }

    // There is no need to call portmapper to get the Abort Channel port
    // as this is returned from the Core Channel creation process
    m_abortChannel.port=resp.abortPort;
    m_lid=resp.lid;
    // Determine the limiting factor - our buffer size or the instruments
    m_maxRecvSize=resp.maxRecvSize>maxBufferSize?maxBufferSize:resp.maxRecvSize;

    // Open the Abort channel
    if(INVALID_SOCKET==(m_abortChannel.socket=TCP_OpenSocket(parms.device,m_abortChannel.port,parms.sourceAddress.empty() ? NULL : parms.sourceAddress.c_str())))
    {
        resp.error=VXI11Error_DeviceNotAccessible;
        return resp;
    }

    // TO-DO Establish the Interrupt Channel

    return resp;
}

Device_WriteResp CNetworkInstrumentProtocol::device_write(const Device_WriteParms parms)
{
    Device_WriteResp resp;
    resp.error=0;
    resp.size=0;
    XDR_deviceWriteParms* data=new XDR_deviceWriteParms;
    unsigned long dataSize;

    // Check the lid (VXI 11 Rule B.6.15)
    if(m_lid!=parms.lid)
    {
        resp.error=VXI11Error_InvalidLinkID;
        delete data;
        return resp;
    }

    // Check that the data is small enough to transmit (VXI 11 Rule B.6.16)
    if(m_maxRecvSize<static_cast<unsigned long>(*parms.data))
    {
        resp.error=VXI11Error_ParameterError;
        delete data;
        return resp;
    }

    // Fill out the data structure with the parameters for the call
    data->lid=parms.lid;
    data->io_timeout=parms.io_timeout;
    data->lock_timeout=parms.lock_timeout;
    data->flags=parms.flags;
    if(0!=XDR_formatString(reinterpret_cast<char*>(parms.data+sizeof(long)),sizeof(data->command),reinterpret_cast<char*>(data->command),&data->stringLength,&dataSize))
    {
        resp.error=VXI11Error_ParameterError;
        delete data;
        return resp;
    }

    // Add the size of the command string padded to the 32 bit boundary
    // to the size of 5 longs (lid,io_timeout,lock_timeout,flags,stringLength)
    // to get the total packet size.
    dataSize+=5*sizeof(long);

    // Write the data to the instrument
    m_coreChannel.proc=VXI11Procedure_device_write;
    if(0!=RPC_Call(m_coreChannel,dataSize,reinterpret_cast<unsigned char*>(data)))
    {
        resp.error=VXI11Error_IOError;
        delete data;
        return resp;
    }

    // Fetch the response and check for errors
    if(0!=RPC_Reply(m_coreChannel,sizeof(resp),reinterpret_cast<unsigned char*>(&resp)))
    {
        resp.error=VXI11Error_IOError;
        delete data;
        return resp;
    }

    delete data;
    return resp;
}

Device_ReadResp CNetworkInstrumentProtocol::device_read(const Device_ReadParms parms)
{
    Device_ReadResp resp;
    resp.reason = 0;
    resp.data = nullptr;
    XDR_deviceReadParms data;
    XDR_deviceReadResp*reply=new XDR_deviceReadResp;

    resp.error=0;

    // Check the lid (VXI 11 Rule B.6.15)
    if(m_lid!=parms.lid)
    {
        resp.error=VXI11Error_InvalidLinkID;
        delete reply;
        return resp;
    }

    // Fill out the data structure with the parameters for the call
    data.lid=parms.lid;
    data.requestSize=parms.requestSize;
    data.io_timeout=parms.io_timeout;
    data.lock_timeout=parms.lock_timeout;
    data.flags=parms.flags;
    data.termChar=static_cast<unsigned long>(parms.termChar);

    // Call the instrument's read function
    m_coreChannel.proc=VXI11Procedure_device_read;
    if(0!=RPC_Call(m_coreChannel,sizeof(data),reinterpret_cast<unsigned char*>(&data)))
    {
        resp.error=VXI11Error_IOError;
        delete reply;
        return resp;
    }

    // Fetch the reply and check for errors
    if(0!=RPC_Reply(m_coreChannel,sizeof(XDR_deviceReadResp),reinterpret_cast<unsigned char*>(reply)))
    {
        resp.error=VXI11Error_IOError;
        delete reply;
        return resp;
    }

    // Check the returned error field for errors
    if(VXI11Error_NoError!=(resp.error=reply->error))
    {
        delete reply;
        return resp;
    }

    // Clear down the buffer prior to re-use for the return string
    memset(m_buffer,0,maxBufferSize);
    if(0!=XDR_getString(reinterpret_cast<char*>(reply->response),parms.requestSize,reinterpret_cast<char*>(m_buffer)))
    {
        resp.error=VXI11Error_ParameterError;
        delete reply;
        return resp;
    }

    // Point to the "read" string
    resp.data=reinterpret_cast<unsigned char*>(m_buffer);

    // TO-DO resp.reason has not been set at this point!!

    delete reply;
    return resp;
}

Device_Error CNetworkInstrumentProtocol::destroy_link(const Device_Link lid)
{
    Device_Error resp;
    Device_Link data;

    // Initialise the error flag, not that anybody will probably ever examine it
    resp.error=VXI11Error_NoError;

    // VXI 11 Rule B.6.11
    if(lid!=m_lid)
    {
        resp.error=VXI11Error_InvalidLinkID;
        return resp;
    }

    // destroy_link only has one parameter
    data=m_lid;

    // Call the instrument's destroy link function
    m_coreChannel.proc=VXI11Procedure_destroy_link;
    if(0!=RPC_Call(m_coreChannel,sizeof(data),reinterpret_cast<unsigned char*>(&data)))
    {
        resp.error=VXI11Error_IOError;
        return resp;
    }

    // Close down the Channel sockets
    TCP_CloseSocket(m_coreChannel.socket);
    TCP_CloseSocket(m_abortChannel.socket);
    m_lid=0;

    // With all channels closed, TCP can be cleaned up
    TCP_Cleanup();

    return resp;
}

///////////////////////////////////////////////////////////////////////////////
// XDR Layer Functions
///////////////////////////////////////////////////////////////////////////////

// Two types of data are currently transmitted - long integer and string
// RFC 4506 Sections 4.1 and 4.2 call for XDR to place all integers in big Endian format before passing to RPC
// However, the design is considerably simplied if the Endian conversion is performed just before the packet is sent to the TCP layer
// and just after a packet is received from the TCP layer.
// Given that the RPC layer performs the Endian conversion oblivious to the underlying data type then
// the only data type that needs to be handled by XDR is strings
unsigned int CNetworkInstrumentProtocol::XDR_formatString(const char*sourceString,const unsigned int maxStringLength,char*destinationBuffer,unsigned long*stringLength,unsigned long*dataSize)
{
    char*dPtr=destinationBuffer;
    char*sPtr=const_cast<char*>(sourceString);
    int blocks;

    // Ensure we have not been asked to do something silly
    if((maxStringLength<strlen(sourceString))||(0==strlen(sourceString)))
    {
        return static_cast<unsigned int>(-1);
    }

    // Calculate the number of 4 byte blocks required to hold the string and
    // the END character (RFC4506 4.11)
    *stringLength=strlen(sourceString);
    blocks=1+((*stringLength)>>2);

    // RPC expects XDR data as 4 byte little Endian integer blocks
    // and will perform a little Endian to big Endian conversion before passing the data to TCP
    for(int i=0; i<blocks; i++)
    {
        unsigned char temp[4];
        // Copy up to four characters from the supplied string
        for(unsigned int j=0; j<4; j++,sPtr++)
        {
            if(4*i+j<*stringLength)
            {
                // Copy a character from the supplied string
                temp[j]=*sPtr;
            }
            else if(4*i+j==*stringLength)
            {
                // Append the supplied string with an END character
                temp[j]=0x0A;
            }
            else
            {
                // Pad the rest of the final block, if any, with zeros (RFC4506 4.11)
                temp[j]=0;
            }
        }
        // Drop the copied characters in reverse order into the destination buffer
        for(int j=3; j>=0; j--,dPtr++)
        {
            *dPtr=temp[j];
        }
    }

    // Calculate the size of the data including the END character and any zero padding
    *dataSize=4*blocks;

    return 0;
}

unsigned int CNetworkInstrumentProtocol::XDR_getString(const char*sourceBuffer,const unsigned int maxStringLength,char*destinationBuffer)
{
    char*dPtr=destinationBuffer;
    char*sPtr=const_cast<char*>(sourceBuffer);
    int blocks;
    unsigned long stringLength=*reinterpret_cast<const unsigned long*>(sourceBuffer);

    // Ensure we have not been asked to do something silly
    if((maxStringLength<stringLength)||(0==stringLength))
    {
        return static_cast<unsigned int>(-1);
    }

    // The first block is the size in little Endian format
    for(int j=0; j<4; j++,sPtr++,dPtr++)
    {
        *dPtr=*sPtr;
    }

    // Calculate the number of 4 byte blocks used to hold the string
    blocks=1+(stringLength>>2);

    // RPC delivers XDR data as 4 byte little Endian integer blocks
    // which results in mixed-up strings.
    for(int i=0; i<blocks; i++)
    {
        unsigned char temp[4];
        // Copy a block of 4 characters
        for(int j=0; j<4; j++,sPtr++)
        {
            temp[j]=*sPtr;
        }
        // Write the block out in reverse order
        for(int j=3; j>=0; j--,dPtr++)
        {
            *dPtr=temp[j];
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// RPC Layer Functions
///////////////////////////////////////////////////////////////////////////////
unsigned int CNetworkInstrumentProtocol::RPC_Call(const channel chan,unsigned long dataSize,unsigned char*data)
{
    unsigned int i;
    unsigned long*ptrDest;
    unsigned long*ptrSrc;
    RPC_CallHeader header;

    // Clear the packet buffer
    memset(m_buffer,0,maxBufferSize);

    // Ensure we are not being asked to overflow the buffer
    if(m_maxRecvSize<sizeof(RPC_CallHeader)+dataSize)
    {
        Log4WE(L"TCP/IP Error: RPC send packet too large");
        return static_cast<unsigned int>(-1);
    }

    // Fill out the known header values;
    header.fragInfo.bits.lastFragment=1;
    header.fragInfo.bits.length=sizeof(header)-sizeof(header.fragInfo)+dataSize;
    header.xid=m_xid;
    header.callType=CALL;
    header.rpcVersion=RPC_Version;
    header.program=chan.prog;
    header.version=chan.vers;
    header.procedure=chan.proc;
    // Use null authentication
    for(i=0; i<4; header.authentication[i++]=0);

    // Move the header to the TCP buffer
    for(i=0,ptrDest=reinterpret_cast<unsigned long*>(m_buffer),ptrSrc=reinterpret_cast<unsigned long*>(&header); i<sizeof(header)/sizeof(long); i++,ptrDest++,ptrSrc++)
    {
        // Move the data into the buffer converting from little Endian to big Endian
        *ptrDest=htonl(*ptrSrc);
    }

    // Move the data to the TCP buffer
    for(i=0,ptrSrc=reinterpret_cast<unsigned long*>(data); i<dataSize/sizeof(long); i++,ptrDest++,ptrSrc++)
    {
        // Move the data into the buffer converting from little Endian to big Endian
        *ptrDest=htonl(*ptrSrc);
    }

    // Send the data
    return TCP_Send(chan.socket,reinterpret_cast<char*>(m_buffer),sizeof(header)+dataSize);
}

unsigned long CNetworkInstrumentProtocol::RPC_PMAPPROC_GETPORT(const std::string ipAddress,const unsigned int programNumber,const unsigned int version,const unsigned int protocol,const unsigned int port/* =0 */,const char* source_addr /* = NULL */)
{
    port; // warning fix: unreferenced formal parameter

    unsigned long getPort=0;
    portmapper_mapping mapping;

    // We want the GETPORT procedure
    m_portmapper.proc=PMAPPROC_GETPORT;

    // Fill out the parameters
    mapping.prog=programNumber;
    mapping.vers=version;
    mapping.prot=protocol;
    mapping.port=0;

    // Open up a new socket for the portmapper
    if(INVALID_SOCKET==(m_portmapper.socket=TCP_OpenSocket(ipAddress,m_portmapper.port,source_addr)))
    {
        return getPort;
    }

    // Send the request
    if(0!=RPC_Call(m_portmapper,sizeof(mapping),reinterpret_cast<unsigned char*>(&mapping)))
    {
        return getPort;
    }

    // Receive the reply
    if(0!=RPC_Reply(m_portmapper,sizeof(long),reinterpret_cast<unsigned char*>(&getPort)))
    {
        return 0L;
    }

    // Finished so close the socket down
    TCP_CloseSocket(m_portmapper.socket);
    return getPort;
}

unsigned int CNetworkInstrumentProtocol::RPC_Reply(const channel chan,unsigned long dataSize,unsigned char*data)
{
    int bytesReceived;
    unsigned int expectedMaxFragmentSize;
    int i;
    int TotalBytesReceived=0;
    unsigned long*ptrDataEnd=reinterpret_cast<unsigned long*>(&m_buffer);
    unsigned long*ptrDest=reinterpret_cast<unsigned long*>(data);
    unsigned long*ptrFragEnd;
    unsigned long*ptrReplyHeader;
    unsigned long*ptrSrc=reinterpret_cast<unsigned long*>(&m_buffer);
    std::ostringstream ostr;
    RPC_FragHeader fragHeader;
    RPC_ReplyHeader replyHeader;
    bool bFirstFragment=true;
    const int maxReceiveSize=sizeof(fragHeader)+sizeof(replyHeader)+dataSize;

    // Clear the buffers
    memset(m_buffer,0,maxBufferSize);
    fragHeader.fragInfo.word=0;

    // Ensure we are not being asked to overflow the buffer
    // Note this will not catch everything as there could be more than one fragment
    if(maxBufferSize<maxReceiveSize)
    {
        Log4WE(L"TCP/IP Error: RPC reply packet too large");
        return static_cast<unsigned int>(-1);
    }

    do
    {
        // Check that we are not about to overflow the buffer
        if(maxReceiveSize<TotalBytesReceived)
        {
            //ostr << "TCP/IP Error: Potential buffer overflow. Total bytes received " << TotalBytesReceived << " exceeds " << maxReceiveSize;
            Log4WE_F(L"TCP/IP Error: Potential buffer overflow. Total bytes received %d exceeds %d",TotalBytesReceived,maxReceiveSize);
            return static_cast<unsigned int>(-1);
        }

        // Get the reply
        bytesReceived=TCP_Receive(chan.socket,reinterpret_cast<char*>(ptrDataEnd),maxReceiveSize-TotalBytesReceived);
        if(SOCKET_ERROR==bytesReceived)
        {
            return static_cast<unsigned int>(-1);
        }
        TotalBytesReceived+=bytesReceived;

        //Set up a pointer to point to the end of the received data
        ptrDataEnd+=sizeof(char)*(bytesReceived/sizeof(long));

        do
        {
            // Move received data from the TCP buffer into the fragment header structure converting from big Endian to little Endian
            fragHeader.fragInfo.word=ntohl(*ptrSrc);
            // Set up a pointer to point to the end of the fragment
            ptrFragEnd=(ptrSrc+1)+(sizeof(char)*(fragHeader.fragInfo.bits.length/sizeof(long)));

            // Ensure we have a full fragment in memory before processing it
            if(ptrFragEnd<=ptrDataEnd)
            {
                // Move the source pointer to the first byte after the fragment header
                ++ptrSrc;

                // The RPC Header comes back in the first fragment only
                if(bFirstFragment)
                {
                    bFirstFragment=false;

                    // If there are any more fragments they should not be larger than this
                    expectedMaxFragmentSize=fragHeader.fragInfo.bits.length;

                    // Grab the RPC Header
                    for(i=0,ptrReplyHeader=reinterpret_cast<unsigned long*>(&replyHeader); i<sizeof(replyHeader)/sizeof(long); i++,ptrReplyHeader++,ptrSrc++)
                    {
                        // Move the data into the buffer converting from big Endian to little Endian
                        *ptrReplyHeader=ntohl(*ptrSrc);
                    }

                    // Check the header is what we expect
                    // Ensure that this is the expected response in the sequence
                    if(static_cast<unsigned long>(m_xid)!=replyHeader.xid)
                    {
                        //ostr << "TCP/IP Error: RPC xid recieved (" << replyHeader.xid << ") does not match expected xid (" << m_xid << ")";
                        Log4WE_F(L"TCP/IP Error: RPC xid recieved (%ud) does not match expected xid (%ud)",replyHeader.xid,m_xid);
                        return static_cast<unsigned int>(-1);
                    }

                    // Ensure that we have received a reply, not a call!
                    if(REPLY!=replyHeader.replyType)
                    {
                        //ostr << "TCP/IP Error: RPC message type recieved (" << replyHeader.replyType << ") does not match expected message type (" << REPLY << ")";
                        Log4WE_F(L"TCP/IP Error: RPC message type recieved (%ud) does not match expected message type (1)",replyHeader.replyType);
                        return static_cast<unsigned int>(-1);
                    }

                    // Ensure that the authentication protocol is null
                    for(i=0; i<4; i++)
                    {
                        if(0!=replyHeader.authentication[0])
                        {
                            Log4WE(L"TCP/IP Error: Unexpected RPC authentication protocol");
                            return static_cast<unsigned int>(-1);
                        }
                    }

                    // If we get here then the header is exactly what we expected
                    // Increment the xid for next time (overflow is acceptable)
                    m_xid++;
                }

                if(expectedMaxFragmentSize<fragHeader.fragInfo.bits.length)
                {
                    //ostr.str("");
                    //ostr << "TCP/IP Error: Larger than expected fragment detected. Expected maximum (" << expectedMaxFragmentSize << ") is less than current size (" << fragHeader.fragInfo.bits.length << ")";
                    Log4WE_F(L"TCP/IP Error: Larger than expected fragment detected. Expected maximum (%ud) is less than current size (%ud)",expectedMaxFragmentSize,fragHeader.fragInfo.bits.length);
                    return static_cast<unsigned int>(-1);
                }

                if(ptrFragEnd<=ptrDataEnd)
                {
                    // We have a full fragment in memory so process it
                    // Move received data from the TCP buffer into the data area
                    do
                    {
                        // Move the data into the buffer converting from big Endian to little Endian
                        *ptrDest=ntohl(*ptrSrc);
                        ptrDest++;
                        ptrSrc++;
                    }
                    while(ptrSrc<ptrFragEnd);
                }
            }
        }
        while((0==fragHeader.fragInfo.bits.lastFragment)&&(ptrFragEnd<ptrDataEnd));
    }
    while((0==fragHeader.fragInfo.bits.lastFragment)||(ptrFragEnd>ptrDataEnd));

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// TCP Layer Functions
///////////////////////////////////////////////////////////////////////////////
void CNetworkInstrumentProtocol::TCP_Cleanup(void)
{
    // WSACleanup must be the last call to Windows Sockets
    WSACleanup();
}

void CNetworkInstrumentProtocol::TCP_CloseSocket(const SOCKET socket)
{
    // Close the socket
    closesocket(socket);
}

long CNetworkInstrumentProtocol::TCP_Init(void)
{
    const WORD WSARequiredVers=MAKEWORD(MinWSAMajorVers,MinWSAMinorVers);
    int retval;
    std::ostringstream ostr;
    WSADATA wsaData;

    // WSAStartup must be the first function call to Windows Sockets
    // TO-DO Is there an issue in calling this for a second time for a second network instrument?
    if(0!=(retval=WSAStartup(WSARequiredVers,&wsaData)))
    {
        //ostr << "Error attempting to create TCP/IP link: WSAStartup failed with error " << retval;
        Log4WE_F(L"Error attempting to create TCP/IP link: WSAStartup failed with error %d",retval);
        TCP_Cleanup();
        return -1;
    }

    // The call was sucessful but is the requested version supported?
    if((MinWSAMinorVers!=LOBYTE(wsaData.wVersion))||MinWSAMajorVers!=HIBYTE(wsaData.wVersion))
    {
        ostr << "Error attempting to create TCP/IP link: Windows Sockets V" << MinWSAMajorVers << "." << MinWSAMinorVers << " not supported";
        Log4WE_F(L"Error attempting to create TCP/IP link: Windows Sockets V %u.%u not supported",MinWSAMajorVers,MinWSAMinorVers);
        TCP_Cleanup();
        return -1;
    }

    return 0;
}

SOCKET CNetworkInstrumentProtocol::TCP_OpenSocket(const std::string ipAddress,const long port,const char* source_addr /* = NULL */)
{
    char hoststr[NI_MAXHOST];
    char servstr[NI_MAXSERV];
    int retval;
    int socketCount = 0;
    std::ostringstream ostr;
    SOCKET connSocket=INVALID_SOCKET;
    struct addrinfo hints;
    struct addrinfo*results=NULL;
    struct addrinfo*addrptr=NULL;

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    hints.ai_flags=0;

    _itoa_s(port,servstr,10);
    if(0!=(retval=getaddrinfo(ipAddress.c_str(),servstr,&hints,&results)))
    {
        //ostr << "TCP/IP Error: getaddrinfo returned " << retval;
        Log4WE_F(L"TCP/IP Error: getaddrinfo returned %d",retval);
        freeaddrinfo(results);
        return connSocket;
    }

    // Make sure we got at least one address back
    if(NULL==results)
    {
        Log4WE(L"TCP/IP Error: Unable to resolve interface.");
        freeaddrinfo(results);
        return connSocket;
    }

    // Count how many addresses were returned
    addrptr=results;
    while(addrptr)
    {
        socketCount++;
        addrptr=addrptr->ai_next;
    }

    // Make sure that we only got one address back
    if(1<socketCount)
    {
        Log4WE(L"TCP/IP Error: More than one socket found.");
        freeaddrinfo(results);
        return connSocket;
    }

    // Create the socket
    addrptr=results;
    connSocket=socket(addrptr->ai_family,addrptr->ai_socktype,addrptr->ai_protocol);
    if(INVALID_SOCKET==connSocket)
    {
        Log4WE(L"TCP/IP Error: Socket Error.");
        freeaddrinfo(results);
        return connSocket;
    }

    if(0!=getnameinfo(addrptr->ai_addr,static_cast<socklen_t>(addrptr->ai_addrlen),hoststr,NI_MAXHOST,servstr,NI_MAXSERV,NI_NUMERICHOST|NI_NUMERICSERV))
    {
        Log4WE(L"TCP/IP Error: getnameinfo failed.");
        freeaddrinfo(results);
        return connSocket;
    }

	//°ó¶¨IPºÍ¶Ë¿Ú
	if (source_addr)
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = 0;
		sin.sin_addr.S_un.S_addr = inet_addr(source_addr); 
		if (bind(connSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			PELOG4WW(L"bind error !");
			return false;
		}
	}

    // Make the connection
    if(SOCKET_ERROR==connect(connSocket,addrptr->ai_addr,static_cast<int>(addrptr->ai_addrlen)))
    {
        Log4WE(L"TCP/IP Error: connect failed.");
        closesocket(connSocket);
        freeaddrinfo(results);
        connSocket=INVALID_SOCKET;
        return connSocket;
    }

    // Finished with results so clear down the dynamically allocated memory
    freeaddrinfo(results);

    return connSocket;
}

unsigned int CNetworkInstrumentProtocol::TCP_Receive(const SOCKET sock,char*buffer,const int maxSize)
{
    std::ostringstream ostr;
    int bytesReceived=recv(sock,buffer,maxSize,0);

    // Check for errors returned from recv
    if(SOCKET_ERROR==bytesReceived)
    {
        Log4WE(L"TCP/IP Error: receive failed.");
        return static_cast<unsigned int>(SOCKET_ERROR);
    }
    return bytesReceived;
}

unsigned int CNetworkInstrumentProtocol::TCP_Send(const SOCKET sock,const char*buffer,const int size)
{
    std::ostringstream ostr;
    int bytesSent=send(sock,buffer,size,0);

    // Check for errors returned from send and that all bytes were actually sent
    if(SOCKET_ERROR==bytesSent)
    {
        //Log4WE(L"TCP/IP Error: send failed.");
        return static_cast<unsigned int>(SOCKET_ERROR);
    }
    else if(bytesSent!=size)
    {
        //ostr << "TCP/IP Error: send failed - only sent " << bytesSent << " out of " << size << "bytes";
        Log4WE_F(L"TCP/IP Error: send failed - only sent %d out of %d bytes",bytesSent,size);
        return static_cast<unsigned int>(SOCKET_ERROR);
    }
    return 0;
}

#else // _VISA32_SUPPORT_
///////////////////////////////////////////////////////////////////////////////
// VISA32 Functions
///////////////////////////////////////////////////////////////////////////////
CNetworkInstrumentProtocol::CNetworkInstrumentProtocol()
{
}

CNetworkInstrumentProtocol::~CNetworkInstrumentProtocol()
{
}

Create_LinkResp CNetworkInstrumentProtocol::create_link(const Create_LinkParms parms)
{
    Create_LinkResp resp;
    resp.error=0;

    ViStatus status;

    status = viOpenDefaultRM(&m_defaultRM);
    if (status < VI_SUCCESS)
    {
        /* Error Initializing VISA...exiting */
        resp.error=-1;
        return resp;
    }
    string resName="TCPIP::"+parms.device;
    status = viOpen(m_defaultRM,const_cast<ViRsrc>(resName.c_str()),VI_NULL,VI_NULL,&m_instr);
    viSetAttribute(m_instr,VI_ATTR_TMO_VALUE,10000);

    resp.error=status;
    resp.lid=m_instr;

    return resp;
}

Device_WriteResp CNetworkInstrumentProtocol::device_write(const Device_WriteParms parms)
{
    Device_WriteResp resp;

    if(parms.lid!=m_instr)
    {
        // Incorrect lid for this instance
        resp.error=-1;
        return resp;
    }

    resp.error=viWrite(parms.lid,parms.data+sizeof(long),static_cast<long>(*parms.data),&resp.size);

    return resp;
}

Device_ReadResp CNetworkInstrumentProtocol::device_read(const Device_ReadParms parms)
{
    Device_ReadResp resp;

    if(parms.lid!=m_instr)
    {
        // Incorrect lid for this instance
        resp.error=-1;
        return resp;
    }

    memset(m_buffer,0,maxBufferSize);
    resp.error=viRead(parms.lid,reinterpret_cast<ViPBuf>(m_buffer)+sizeof(long),maxBufferSize,reinterpret_cast<ViPUInt32>(m_buffer));
    resp.data=reinterpret_cast<unsigned char*>(m_buffer);

    return resp;
}

Device_Error CNetworkInstrumentProtocol::destroy_link (const Device_Link lid)
{
    Device_Error devError;

    if(lid!=m_instr)
    {
        // Incorrect lid for this instance
        devError.error=-1;
        return devError;
    }

    viClose(m_instr);
    m_instr=NULL;
    viClose(m_defaultRM);
    m_defaultRM=NULL;
    devError.error=0;

    return devError;
}
#endif // _VISA32_SUPPORT_
