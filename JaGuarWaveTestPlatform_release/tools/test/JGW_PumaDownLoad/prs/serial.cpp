/*
 * Copyright (c) 2014 Peraso Technologies Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * File contents: Serial CLI support
 */
#include "stdafx.h"
#include "serial.h"
#include <Windows.h>
//#include "log.h"

//#ifdef SERIAL_LINUX
#define RESP_TERMINATOR '>'

#ifdef _MSC_VER // i.e. if compiling under Windows
HANDLE did = INVALID_HANDLE_VALUE;
#else
int did = -1;
#endif

int open_serial_port(char *device);

// Only one client can interact with serial at
// any given time. So have to lock access to
// the port
/*pthread_mutex_t g_lock;*/
void init_ser(void)
{
    //pthread_mutex_init(&g_lock, NULL);
}

bool send_ser(
    char 	*cmd,
	int	    cmd_len,
	char	*resp,
	int	    resp_max_buff_len,
	int	    *actual_resp_len)
{
#ifdef _MSC_VER
    DWORD dwWriten = 0,dwRead = 0;;

    BOOL bResult = WriteFile(did,cmd,cmd_len,&dwWriten,NULL);

    if (!bResult || dwWriten != cmd_len)
    {
        return 1;
    }
    char 	*p;
    *actual_resp_len = 0;
    p = resp;

    while ((bResult = ReadFile(did, p, 1,&dwRead,0)) != 0 && *actual_resp_len < resp_max_buff_len)
    {
        *actual_resp_len = *actual_resp_len + 1;
        if (p[0] == RESP_TERMINATOR)
        {
            break;
        }
        //! printf("%c ",p[0]);
#if 0		
        // For testing to display the return message
        if (*p == '\n')
        {
            *p = 'n';
        }
        if (*p == '\r')
        {
            *p = 'r';
        }
#endif		    
        p++;
    }
    //! printf("\n");
#else
	int 	r = 0;
	char 	*p;
	int 	w = 0;

    // Write out to the port
	w = write (did, cmd, cmd_len);
	
	if (w != cmd_len)
	{
//		write_log (LL_W, "To uart len %d incompletely written %d\n", cmd_len, w);
		return 1;
	}
	

	// Read until serial prints next command prompt!!
	*actual_resp_len = 0;
	p = resp;
	while ((r = read(did, p, 1)) != 0 && *actual_resp_len < resp_max_buff_len)
	{
		*actual_resp_len = *actual_resp_len + 1;
		if (p[0] == RESP_TERMINATOR)
		{
			break;
		}
#if 0		
        // For testing to display the return message
		if (*p == '\n')
		{
		    *p = 'n';
		}
		if (*p == '\r')
		{
		    *p = 'r';
		}
#endif		    
		p++;
	}
	/*if (*actual_resp_len == resp_max_buff_len)
	{
		write_log(LL_W, "Possibly truncated data actual_resp_len %d\n", *actual_resp_len);
	}
	*/
#endif
    //pthread_mutex_unlock (&g_lock);
	return 0;
}

int open_ser(const char *device)
{
#ifdef _MSC_VER
    did = //CreateFileA(  (LPCSTR)pData,
        CreateFileA (  (LPCSTR) device,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,//OPEN_EXISTING,
        0,              // FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
        NULL);

    if (did == INVALID_HANDLE_VALUE)
    {
        //!dbg (LOG_ERROR, "Failed to open com port '%s'", pData);
        return -1;
    }
    return 1;
#else
	struct termios	current;

    did = open (device, O_RDWR | O_NOCTTY );
    if( did < 0 ) {
//		write_log(LL_E, "Unable to open device %s\n", device);
        return did;
	}

	// Set port characteristics
	memset (&current, 0, sizeof(current));
    if (tcgetattr(did, &current) != 0)
	{
//		write_log (LL_E, "Unable to get termios\n");
        close_ser();
		return -1;
	}

	// Baud rate
	cfsetispeed (&current, B115200);
	cfsetospeed (&current, B115200);
    if (tcsetattr(did, TCSANOW, &current))
	{
//		write_log (LL_E, "Unable to set baud rate\n");
        close_ser();
        return -1;
	}

	// No parity (8N1)
	current.c_cflag &= ~CSIZE;
	current.c_cflag &= ~(PARENB | PARODD);
	current.c_cflag &= ~CSTOPB;
	current.c_cflag |= CS8;
    // no flow control
    current.c_cflag &= ~CRTSCTS;
	current.c_iflag &= ~(IXON|IXOFF|IXANY);
    // block until 1 character recevied
	current.c_cc[VMIN] = 1;
	// read time out
	current.c_cc[VTIME] = 5;
	current.c_cflag |= CREAD | CLOCAL;
	// Make raw
	cfmakeraw(&current);

	// Set the attributes
    tcflush(did, TCIFLUSH);
    if (tcsetattr(did, TCSANOW, &current))
	{
//		write_log (LL_E, "Unable to set parity\n");
        close_ser();
		return -1;
	}

    return did;
#endif   
}

void close_ser(void)
{
#ifdef _MSC_VER
    CloseHandle(did);
    did = INVALID_HANDLE_VALUE;
#else
    close(did);
    did = -1;
#endif 
}

//#endif // SERIAL_LINUX
