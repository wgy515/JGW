#ifndef JGW_LOG4DEFINE_H__
#define JGW_LOG4DEFINE_H__

// namespace JGW
// {
	//! logµÈ¼¶
	enum Log4Level
	{
		Log4Level_Verbose = 0,
		Log4Level_Debug,     //!< debug info
		Log4Level_Info,      //!< information
		Log4Level_Warning,   //!< warning info.
		Log4Level_Error,     //!< error info.
		Log4Level_Fatal      //!< fatal error info.
	};

    enum JGWPrintLogType
    {
        JGWPrintLog_Debug = 0,
        JGWPrintLog_Error,
        JGWPrintLog_Html
    };
/*}*/

#endif