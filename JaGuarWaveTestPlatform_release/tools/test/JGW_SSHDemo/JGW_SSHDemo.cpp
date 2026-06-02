/*
* Run it like this:
*
* $ ./ssh2_echo 127.0.0.1 user password
*
* The code sends a 'cat' command, and then writes a lot of data to it only to
* check that reading the returned data sums up to the same amount.
*
*/
#include "stdafx.h"
#include <iostream>
#include <string>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"
/*#include <JGW_Log4cplusPropFileImplPlugin/JGW_Log4cplus.h>*/

#include <JGW_SSHPlugin/CJGW_SecureShell.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
//#include "CJGW_SecureShell.h"

#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif
# ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#if 1
static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    return rc;
}
const char *keyfile1="~/.ssh/id_rsa.pub";
const char *keyfile2="~/.ssh/id_rsa";
const char *username="username";
const char *password="password";
#define BUFSIZE 32000
static void kbd_callback(const char *name, int name_len,
    const char *instruction, int instruction_len,
    int num_prompts,
    const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
    LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
    void **abstract)
{
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    if (num_prompts == 1) {
        responses[0].text = strdup(password);
        responses[0].length = strlen(password);
    }
    (void)prompts;
    (void)abstract;
} /* kbd_callback */



int main(int argc, char *argv[])
{
    //FILE* fp = NULL;
    ////fopen_s(&fp,"c:\\device_r_info.bin","rb");
    //fp = fopen("c:\\device_r_info.bin","rb");
    //char* bufssss = strerror(errno);
    unsigned long hostaddr;
    int rc, sock, i, auth_pw = 0;
    struct sockaddr_in sin;
    const char *fingerprint;
    char *userauthlist;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;



#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    if (argc > 1) {
        hostaddr = inet_addr(argv[1]);
    } else {
        hostaddr = htonl(0x7F000001);
    }

    if(argc > 2) {
        username = argv[2];
    }
    if(argc > 3) {
        password = argv[3];
    }

    rc = libssh2_init (0);
    if (rc != 0) {
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    /* Ultra basic "connect to port 22 on localhost".  Your code is
     * responsible for creating the socket establishing the connection
     */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) {
        fprintf(stderr, "failed to connect!\n");
        return -1;
    }

    /* Create a session instance and start it up. This will trade welcome
     * banners, exchange keys, and setup crypto, compression, and MAC layers
     */
    session = libssh2_session_init();
    if (libssh2_session_handshake(session, sock)) {
        fprintf(stderr, "Failure establishing SSH session\n");
        return -1;
    }

    /* At this point we havn't authenticated. The first thing to do is check
     * the hostkey's fingerprint against our known hosts Your app may have it
     * hard coded, may go to a file, may present it to the user, that's your
     * call
     */
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
    fprintf(stderr, "Fingerprint: ");
    for(i = 0; i < 20; i++) {
        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    }
    fprintf(stderr, "\n");

    /* check what authentication methods are available */
    userauthlist = libssh2_userauth_list(session, username, strlen(username));
    fprintf(stderr, "Authentication methods: %s\n", userauthlist);
    if (strstr(userauthlist, "password") != NULL) {
        auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
        auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey") != NULL) {
        auth_pw |= 4;
    }

    /* if we got an 4. argument we set this option if supported */
    if(argc > 4) {
        if ((auth_pw & 1) && !strcasecmp(argv[4], "-p")) {
            auth_pw = 1;
        }
        if ((auth_pw & 2) && !strcasecmp(argv[4], "-i")) {
            auth_pw = 2;
        }
        if ((auth_pw & 4) && !strcasecmp(argv[4], "-k")) {
            auth_pw = 4;
        }
    }

    if (auth_pw & 1) {
        /* We could authenticate via password */
        if (libssh2_userauth_password(session, username, password)) {
            fprintf(stderr, "\tAuthentication by password failed!\n");
            goto shutdown;
        } else {
            fprintf(stderr, "\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        /* Or via keyboard-interactive */
        if (libssh2_userauth_keyboard_interactive(session, username,
                                                  &kbd_callback) ) {
            fprintf(stderr,
                "\tAuthentication by keyboard-interactive failed!\n");
            goto shutdown;
        } else {
            fprintf(stderr,
                "\tAuthentication by keyboard-interactive succeeded.\n");
        }
    } else if (auth_pw & 4) {
        /* Or by public key */
        if (libssh2_userauth_publickey_fromfile(session, username, keyfile1,
                                                keyfile2, password)) {
            fprintf(stderr, "\tAuthentication by public key failed!\n");
            goto shutdown;
        } else {
            fprintf(stderr, "\tAuthentication by public key succeeded.\n");
        }
    } else {
        fprintf(stderr, "No supported authentication methods found!\n");
        goto shutdown;
    }

    /* Request a shell */
    if (!(channel = libssh2_channel_open_session(session))) {
        fprintf(stderr, "Unable to open a session\n");
        goto shutdown;
    }

    /* Some environment variables may be set,
     * It's up to the server which ones it'll allow though
     */
    libssh2_channel_setenv(channel, "FOO", "bar");

    /* Request a terminal with 'vanilla' terminal emulation
     * See /etc/termcap for more options
     */
    if (libssh2_channel_request_pty(channel, "vanilla")) {
        fprintf(stderr, "Failed requesting pty\n");
        goto skip_shell;
    }

    /* Open a SHELL on that pty */
    if (libssh2_channel_shell(channel)) {
        fprintf(stderr, "Unable to request shell on allocated pty\n");
        goto shutdown;
    }

    libssh2_channel_write(channel,"ifconfig",strlen("ifconfig"));
    char buf [1024] = {0};
    libssh2_channel_read(channel,buf,1024);
    /* At this point the shell can be interacted with using
     * libssh2_channel_read()
     * libssh2_channel_read_stderr()
     * libssh2_channel_write()
     * libssh2_channel_write_stderr()
     *
     * Blocking mode may be (en|dis)abled with: libssh2_channel_set_blocking()
     * If the server send EOF, libssh2_channel_eof() will return non-0
     * To send EOF to the server use: libssh2_channel_send_eof()
     * A channel can be closed with: libssh2_channel_close()
     * A channel can be freed with: libssh2_channel_free()
     */

  skip_shell:
    if (channel) {
        libssh2_channel_free(channel);
        channel = NULL;
    }

    /* Other channel types are supported via:
     * libssh2_scp_send()
     * libssh2_scp_recv2()
     * libssh2_channel_direct_tcpip()
     */

  shutdown:

    libssh2_session_disconnect(session,
                               "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);

#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    fprintf(stderr, "all done!\n");

    libssh2_exit();

    return 0;







#if 0
    const char *hostname = "192.168.1.1";
    const char *commandline = "ifconfig";
    const char *username    = "root";
    const char *password    = "20171015";
    unsigned long hostaddr;
    int sock;
    struct sockaddr_in sin;
    const char *fingerprint;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int rc;
    int exitcode = 0;
    char *exitsignal=(char *)"none";
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;
    int type;

#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    if (argc > 1)
        /* must be ip address only */
        hostname = argv[1];

    if (argc > 2) {
        username = argv[2];
    }
    if (argc > 3) {
        password = argv[3];
    }

    rc = libssh2_init (0);
    if (rc != 0) {
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    hostaddr = inet_addr(hostname);

    /* Ultra basic "connect to port 22 on localhost"
    * Your code is responsible for creating the socket establishing the
    * connection
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin),
        sizeof(struct sockaddr_in)) != 0) {
            fprintf(stderr, "failed to connect!\n");
            return -1;
    }

    /* Create a session instance */
    session = libssh2_session_init();
    if (!session)
        return -1;

    /* tell libssh2 we want it all done non-blocking */
    libssh2_session_set_blocking(session, 0);

    /* ... start it up. This will trade welcome banners, exchange keys,
    * and setup crypto, compression, and MAC layers
    */
    while ((rc = libssh2_session_handshake(session, sock)) ==
        LIBSSH2_ERROR_EAGAIN);
    if (rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return -1;
    }

    nh = libssh2_knownhost_init(session);
    if(!nh) {
        /* eeek, do cleanup here */
        return 2;
    }

    /* read all hosts from here */
    libssh2_knownhost_readfile(nh, "known_hosts",
        LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    /* store all known hosts to here */
    libssh2_knownhost_writefile(nh, "dumpfile",
        LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    fingerprint = libssh2_session_hostkey(session, &len, &type);
    if(fingerprint) {
        struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(nh, hostname, 22,
            fingerprint, len,
            LIBSSH2_KNOWNHOST_TYPE_PLAIN|
            LIBSSH2_KNOWNHOST_KEYENC_RAW,
            &host);

        fprintf(stderr, "Host check: %d, key: %s\n", check,
            (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
            host->key:"<none>");

        /*****
        * At this point, we could verify that 'check' tells us the key is
        * fine or bail out.
        *****/
    }
    else {
        /* eeek, do cleanup here */
        return 3;
    }
    libssh2_knownhost_free(nh);

    if ( strlen(password) != 0 ) {
        /* We could authenticate via password */
        while ((rc = libssh2_userauth_password(session, username, password)) ==
            LIBSSH2_ERROR_EAGAIN);
        if (rc) {
            fprintf(stderr, "Authentication by password failed.\n");
            exit(1);
        }
    }

    libssh2_trace(session, LIBSSH2_TRACE_SOCKET);

    /* Exec non-blocking on the remove host */
    while( (channel = libssh2_channel_open_session(session)) == NULL &&
        libssh2_session_last_error(session,NULL,NULL,0) ==
        LIBSSH2_ERROR_EAGAIN ) {
            waitsocket(sock, session);
    }
    if( channel == NULL ) {
        fprintf(stderr,"Error\n");
        exit( 1 );
    }
    while( (rc = libssh2_channel_exec(channel, "ifconfig")) ==
        LIBSSH2_ERROR_EAGAIN )
        waitsocket(sock, session);

    if( rc != 0 ) {
        fprintf(stderr, "exec error\n");
        exit( 1 );
    }
    else {
        LIBSSH2_POLLFD *fds = NULL;
        int running = 1;
        int bufsize = BUFSIZE;
        char buffer[BUFSIZE];
        int totsize = 1500000;
        int totwritten = 0;
        int totread = 0;
        int partials = 0;
        int rereads = 0;
        int rewrites = 0;
        int i;

        for (i = 0; i < BUFSIZE; i++)
            buffer[i] = 'A';

        if ((fds = (LIBSSH2_POLLFD*)malloc (sizeof (LIBSSH2_POLLFD))) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        fds[0].type = LIBSSH2_POLLFD_CHANNEL;
        fds[0].fd.channel = channel;
        fds[0].events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

        do {
            int rc = (libssh2_poll(fds, 1, 10));
            int act = 0;

            if (rc < 1)
                continue;

            if (fds[0].revents & LIBSSH2_POLLFD_POLLIN) {
                int n = libssh2_channel_read(channel, buffer, sizeof(buffer));
                act++;

                if (n == LIBSSH2_ERROR_EAGAIN) {
                    rereads++;
                    fprintf(stderr, "will read again\n");
                }
                else if (n < 0) {
                    fprintf(stderr, "read failed\n");
                    exit(1);
                }
                else {
                    totread += n;
                    fprintf(stderr, "read %d bytes (%d in total)\n",
                        n, totread);
                    fprintf(stderr,buffer);
                }
            }
#if 0
            if (fds[0].revents & LIBSSH2_POLLFD_POLLOUT) {
                act++;

                if (totwritten < totsize) {
                    /* we have not written all data yet */
                    int left = totsize - totwritten;
                    int size = (left < bufsize) ? left : bufsize;
                    int n = libssh2_channel_write_ex(channel, 0, buffer, size);

                    if (n == LIBSSH2_ERROR_EAGAIN) {
                        rewrites++;
                        fprintf(stderr, "will write again\n");
                    }
                    else if (n < 0) {
                        fprintf(stderr, "write failed\n");
                        exit(1);
                    }
                    else {
                        totwritten += n;
                        fprintf(stderr, "wrote %d bytes (%d in total)",
                            n, totwritten);
                        if (left >= bufsize && n != bufsize) {
                            partials++;
                            fprintf(stderr, " PARTIAL");
                        }
                        fprintf(stderr, "\n");
                    }
                } else {
                    /* all data written, send EOF */
                    rc = libssh2_channel_send_eof(channel);

                    if (rc == LIBSSH2_ERROR_EAGAIN) {
                        fprintf(stderr, "will send eof again\n");
                    }
                    else if (rc < 0) {
                        fprintf(stderr, "send eof failed\n");
                        exit(1);
                    }
                    else {
                        fprintf(stderr, "sent eof\n");
                        /* we're done writing, stop listening for OUT events */
                        fds[0].events &= ~LIBSSH2_POLLFD_POLLOUT;
                    }
                }
            }
#endif

            if (fds[0].revents & LIBSSH2_POLLFD_CHANNEL_CLOSED) {
                if (!act) /* don't leave loop until we have read all data */
                    running = 0;
            }
        } while(running);

        exitcode = 127;
        while( (rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN )
            waitsocket(sock, session);

        if( rc == 0 ) {
            exitcode = libssh2_channel_get_exit_status( channel );
            libssh2_channel_get_exit_signal(channel, &exitsignal,
                NULL, NULL, NULL, NULL, NULL);
        }

        if (exitsignal)
            fprintf(stderr, "\nGot signal: %s\n", exitsignal);

        libssh2_channel_free(channel);
        channel = NULL;

        fprintf(stderr, "\nrereads: %d rewrites: %d totwritten %d\n",
            rereads, rewrites, totwritten);

        if (totwritten != totread) {
            fprintf(stderr, "\n*** FAIL bytes written: %d bytes "
                "read: %d ***\n", totwritten, totread);
            exit(1);
        }
    }

    libssh2_session_disconnect(session,
        "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);

#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    fprintf(stderr, "all done\n");

    libssh2_exit();

    return exitcode;
#endif
}
#else

std::string gulp(std::istream &in)
{
    std::string ret;
    char buffer[4096];
    while (in.read(buffer, sizeof(buffer)))
        ret.append(buffer, sizeof(buffer));
    ret.append(buffer, in.gcount());
    return ret;
}
//! {"responseCode":200,"resultCode":1,"errorCode":0}
//! /userdata/bin/update.sh /root/%s
int _tmain(int argc, _TCHAR* argv[])
{
    int flags = 0;
    int fEncodingMask  = 0x3 ,fEncodingShift = 3;

    int i = flags & (fEncodingMask << fEncodingShift);
    /*    WritePrivateProfileString(L"",L"1313",L"5555",L"C:/log4cplus_default.properties");*/
    //JGW_Log4cplusPtr ptr = jgw_log4cplus_init(L"test");
    //jgw_log4cplus_write(ptr,Log4Level_Error,L"13131");
    Log4WE(L"²âÊÔ¹ý³Ì");
    // //     PDLOG4WW(L"12315555");
    // //    
    // 
    //     for (int i = 0;i < 30;i ++)
    //     {
    //         std::wcout << L"test" << std::endl;
    //         PDLOG4WW(L"123456789112345678912");
    //         //Sleep(1 * 1000);
    //     }
    // 
    //     const wchar_t* buf = GETPDLOGW();
    //     std::wcout << buf;

    //     void* ptr = jgw_logger_open(JGWPrintLog_Error);
    //     wchar_t szBuf[260] = {0};
    // 
    //     while(0 != jgw_logger_read(ptr,szBuf)) std::wcout << szBuf << std::endl;
    //     jgw_logger_windows(JGWPrintLog_Error,L"test demo4561");
    // 
    //     if(0 != jgw_logger_read(ptr,szBuf)) std::wcout << szBuf << std::endl;
    //     jgw_logger_windows(JGWPrintLog_Error,L"test demo4562");
    //     if(0 != jgw_logger_read(ptr,szBuf)) std::wcout << szBuf << std::endl;
    //     if(0 != jgw_logger_read(ptr,szBuf)) std::wcout << szBuf << std::endl;
    //     jgw_logger_close(ptr);
    return 0;
    //     jgw_log4cplus_write(L"test");
    //     jgw_log4cplus_write(L"test");
    //     jgw_log4cplus_write(L"test");
    //     jgw_log4cplus_write(L"test");

#if 0
    JGW::CCJGW_ICMPSocket icmpSocket;

    icmpSocket.Ping("192.168.213.117","192.168.213.3",1000);
    return 0;
    /*#if 0*/
    JGW::CCJGW_SecureShell css;
    std::string strRead;

    if (!css.ConnectSSHShell("192.168.213.252","root","lion","192.168.213.3"))
    {
        return -1;
    }

    css.ReadSSHShell(strRead);

    printf(strRead.c_str());
    printf("\n");

    if (!css.WriteReadSSHShell("ls -l\n",strRead))
    {
        return -1;
    }

    printf(strRead.c_str());
    printf("\n");

    if (!css.WriteReadSSHShell("cat /timestamp\n",strRead))
    {
        return -1;
    }
    printf(strRead.c_str());
    printf("\n");
#endif
    //     if (!css.ExecSSH("ls -l"))
    //     {
    //         return -1;
    //     }
    // 
    //      if (!css.ReadSSH(strRead))
    //      {
    //          return -1;
    //      }
    // 
    // 
    //     if (!css.SendSSH("cat /timestamp"))
    //     {
    // //         char *errmsg;
    // //         int errlen;
    // //         int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);
    // //         fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
    // 
    //         return -1;
    //     }
    // 
    //     if (!css.ReadSSH(strRead,10))
    //     {
    // //         char *errmsg;
    // //         int errlen;
    // //         int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);
    // //         fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
    // 
    //         return -1;
    //     }

    return 0;
#if 0
    if (!css.ConnectSFTP("192.168.1.202","root","lion","/root/update.zip"))
    {
        return -1;
    }

    if (!css.PutFileSFTP("F:/download/Lion_Update/update.zip"))
    {
        return -1;
    }
    css.DisConnectSFTP();
    //     if (!css.ExecSSH("/userdata/bin/update.sh /root/update.zip"))
    //     {
    //         return -1;
    //     }

    if (!css.SendSSH("cat /timestamp\n"))
    {
        return -1;
    }
    //     if (!css.ExecSSH("cat /timestamp"))
    //     {
    //         return -1;
    //     }

    if (!css.ReadSSH(strRead))
    {
        return -1;
    }
    printf(strRead.c_str());
    /*#else/*/
    try{
        std::string s = "{\"requestCode\":170,\"fileName\":\"update.zip\"}";
        Poco::URI url("http://192.168.1.202/cgi-bin/leopard.fcgi");
        Poco::Net::HTTPClientSession session(url.getHost(),url.getPort());
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,url.getPath(),Poco::Net::HTTPRequest::HTTP_1_1);


        request.setChunkedTransferEncoding(false);
        request.setContentType("application/json");
        request.setContentLength(s.length());

        session.sendRequest(request) << s;

        Poco::Net::HTTPResponse resp;
        std::istream & is = session.receiveResponse(resp);
        std::string ss = gulp(is);
        printf(ss.c_str());

        //         Poco::Net::HTMLForm form;
        // //         form.add("requestCode", "501");
        // //         form.add("fileName", "/root/update.zip");
        //         form.add("{\"requestCode\":501,\"fileName\":\"update.zip\"}","");
        //         form.prepareSubmit(request);
        //         form.write(session.sendRequest(request));
        //         Poco::Net::HTTPResponse res;
        //         std::istream & is = session.receiveResponse(res);
        //         Poco::StreamCopier::copyStream(is, std::cout);
        //         std::string s = gulp(is);
        //         printf(s.c_str());
    }
    catch(Poco::Net::NetException & ex){
        std::cout << ex.displayText();          
    }
#endif
    /*#endif */
    return 0;
}

#endif