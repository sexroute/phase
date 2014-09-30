#include "StdAfx.h"
#include "DebugHelper.h"
#include <stdio.h>
#include <stdarg.h>


struct _AFX_DOUBLE  { BYTE doubleBits[sizeof(double)]; };

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000
#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char
#ifdef _X86_
#define DOUBLE_ARG  _AFX_DOUBLE
#else
#define DOUBLE_ARG  double
#endif

void NeedModify()
{
	TRACE(_T("******************************* NeedModify: %s %d *******************************\r\n"),__FILE__,__LINE__);
}	

extern inline int GetFormatLength(LPCTSTR apszFormat, va_list argList);

void __cdecl zdlTraceLine(LPCTSTR apszFormat, ...)
{
	
	va_list  args;

	va_start(args, apszFormat);

	va_list argListSave = args;

	int lnDataLength = GetFormatLength(apszFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);

	_vstprintf((TCHAR *)lpszBuffer, apszFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

#ifdef _UNICODE

	::OutputDebugStringW(lstrTempData.GetBuffer(0));

	::OutputDebugStringW(_T("\r\n"));

#else

	::OutputDebugStringA(lstrTempData.GetBuffer(0));

	::OutputDebugStringA(_T("\r\n"));

#endif

	lstrTempData.ReleaseBuffer();

	va_end(argListSave);

	va_end(args);

	
}


void __cdecl zdlTraceLine2(CString astrFileOrFunc,int anLine,LPCTSTR apszFormat, ...)
{
	va_list  args;

	va_start(args, apszFormat);

	va_list argListSave = args;

	int lnDataLength = GetFormatLength(apszFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);

	_vstprintf((TCHAR *)lpszBuffer, apszFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

	CString lstrTemp;

#ifdef _UNICODE
	lstrTemp.Format(_T("[Module: %S@%n ] :%S"),astrFileOrFunc,anLine,lstrTempData);
#else
	lstrTemp.Format(_T("[Module: %S@%n ] :%s"),astrFileOrFunc,anLine,lstrTempData);
#endif
	

#ifdef _UNICODE

	::OutputDebugStringW(lstrTemp.GetBuffer(0));

	::OutputDebugStringW(_T("\r\n"));

#else

	::OutputDebugStringA(lstrTemp.GetBuffer(0));

	::OutputDebugStringA(_T("\r\n"));

#endif

	lstrTempData.ReleaseBuffer();

	va_end(argListSave);

	va_end(args);


}

void  __cdecl  zdlTrace(LPCTSTR apszFormat, ...)
{
	va_list  args;

	va_start(args, apszFormat);

	va_list argListSave = args;
	
	int lnDataLength = GetFormatLength(apszFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);
	
	_vstprintf((TCHAR *)lpszBuffer, apszFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

#ifdef _UNICODE

	::OutputDebugStringW(lstrTempData.GetBuffer(0));

#else

	::OutputDebugStringA(lstrTempData.GetBuffer(0));

#endif

	lstrTempData.ReleaseBuffer();

	va_end(argListSave);

	va_end(args);

}



void  __cdecl  zdlTrace2(CString astrFileOrFunc,int anLine,LPCTSTR apszFormat, ...)
{
	va_list  args;

	va_start(args, apszFormat);

	va_list argListSave = args;

	int lnDataLength = GetFormatLength(apszFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);

	_vstprintf((TCHAR *)lpszBuffer, apszFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

	CString lstrTemp;

#ifdef _UNICODE
	lstrTemp.Format(_T("[Module: %S@%n ] :%S"),astrFileOrFunc,anLine,lstrTempData);
#else
	lstrTemp.Format(_T("[Module: %S@%n ] :%s"),astrFileOrFunc,anLine,lstrTempData);
#endif

#ifdef _UNICODE

	::OutputDebugStringW(lstrTemp.GetBuffer(0));

#else

	::OutputDebugStringA(lstrTemp.GetBuffer(0));

#endif

	lstrTempData.ReleaseBuffer();

	va_end(argListSave);

	va_end(args);

}

inline int GetFormatLength(LPCTSTR apszFormat, va_list argList)
{

	int nMaxLen = 0;
	for (LPCTSTR lpsz = apszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
		{
			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			ASSERT(FALSE);
#endif
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

			// strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
					nItemLen = 6;  // "(null)"
				else
				{
					nItemLen = lstrlen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
					nItemLen = 6;  // "(null)"
				else
				{
					nItemLen = wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
#else
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
					nItemLen = 6; // "(null)"
				else
				{
					nItemLen = lstrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
					nItemLen = 6; // "(null)"
				else
				{
					nItemLen = lstrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
					nItemLen = 6; // "(null)"
				else
				{
					nItemLen = wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312+nPrecision+6));

					f = va_arg(argList, double);
					_stprintf( pszTemp, _T( "%*.*f" ), nWidth, nPrecision+6, f );
					nItemLen = _tcslen(pszTemp);
				}
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

				// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				ASSERT(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	return nMaxLen;
}


