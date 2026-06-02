#ifndef TSE_SequenceTestLib_H__
#define TSE_SequenceTestLib_H__

#if (defined TSE_SequenceTestLib_EXPORTS) || (defined TSE_SequenceTestLib_IMPORTS)
#else
#define TSE_SequenceTestLib_IMPORTS
#endif

#ifdef TSE_SequenceTestLib_IMPORTS
#	ifdef _DEBUG
#		pragma comment(lib,"TSE_SequenceTestLib_d.lib")
#	else
#		pragma comment(lib,"TSE_SequenceTestLib.lib")
#	endif
#endif

#endif