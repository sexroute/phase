#ifndef __ERIC_DEBUG_HELPER
#define __ERIC_DEBUG_HELPER

void __cdecl zdlTrace(LPCTSTR lpszFormat, ...);

void __cdecl zdlTraceLine(LPCTSTR apszFormat, ...);

void __cdecl zdlTrace2(CString astrFileOrFunc,int anLine,LPCTSTR lpszFormat, ...);

void __cdecl zdlTraceLine2(CString astrFileOrFunc,int anLine,LPCTSTR apszFormat, ...);




inline int GetFormatLength(LPCTSTR apszFormat, va_list argList);


extern void NeedModify();



#ifdef UNICODE
#define _MyOutputTrace_(_DisplayFunName_,_dblExecuteTime_)\
    zdlTrace(_T("******************************* %s used Time : %fms %S %d*******************************\r\n"),_DisplayFunName_,_dblExecuteTime_,__FILE__,__LINE__)
#else
#define _MyOutputTrace_(_DisplayFunName_,_dblExecuteTime_)\
    zdlTrace(_T("******************************* %s used Time : %fms %s %d*******************************\r\n"),_DisplayFunName_,_dblExecuteTime_,__FILE__,__LINE__)
#endif


#define _DECLARE_PERF_MEASURE_TIME()							LARGE_INTEGER litmp;litmp;\
																double dfMinus, dfFreq, dfTim; dfMinus; dfFreq; dfTim;\
																double dblExecuteTime=0;\
																LONGLONG QPart1=0,QPart2=0;\
																QueryPerformanceFrequency(&litmp);\
																dfFreq = (double)litmp.QuadPart;\
																QueryPerformanceCounter(&litmp);\
																QPart1 = litmp.QuadPart;

#define _BEGIN_PERF_MEASURE_TIME()								QueryPerformanceCounter(&litmp);\
																QPart1 = litmp.QuadPart;
#define _END_PERF_MEASURE_TIME(DisplayFunName) 					QueryPerformanceCounter(&litmp); \
																QPart2 = litmp.QuadPart; \
																dfMinus = (double)(QPart2-QPart1); \
																dfTim = dfMinus / dfFreq;dblExecuteTime=dfTim*1000;\
                                                                _MyOutputTrace_(DisplayFunName,dblExecuteTime);\
																QPart1 = QPart2;

#define _END_PERF_MEASURE_TIME_LIMIT(DisplayFunName,TimeLimit)	QueryPerformanceCounter(&litmp); \
																QPart2 = litmp.QuadPart; \
																dfMinus = (double)(QPart2-QPart1); \
																dfTim = dfMinus / dfFreq;dblExecuteTime=dfTim*1000;\
																ASSERT(dfTim <= TimeLimit);\
																TRACE(_T("******************************* %s used Time : %fms %s %d*******************************\r\n"),DisplayFunName,dblExecuteTime,__FILE__,__LINE__);\
																QPart1 = QPart2;

#define  _NEEND_MODIFY											do{NeedModify();}while(FALSE);

#define _END_PERF_MEASURE_TIME_COMPUTE(nTimeUsed,DisplayFunName) QueryPerformanceCounter(&litmp); \
																 QPart2 = litmp.QuadPart; \
																 dfMinus = (double)(QPart2-QPart1); \
																 dfTim = dfMinus / dfFreq;dblExecuteTime=dfTim*1000;\
																 TRACE(_T("******************************* %s used Time : %fms %s %d*******************************\r\n"),DisplayFunName,dblExecuteTime,__FILE__,__LINE__);\
																 QPart1 = QPart2;\
																 nTimeUsed = dblExecuteTime;

									
#endif
