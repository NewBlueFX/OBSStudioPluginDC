/*	PlatformMac.h
/*
MIT License

Copyright (c) 2016,2018 NewBlue, Inc. <https://github.com/NewBlueFX>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <Carbon/Carbon.h>

//#include <sys/time.h> // for gettimeofday()


#define UNREFERENCED_PARAMETER(P)          (P)

// Generic types

#ifndef __NB_PLATFORM_MAC_GENERIC__
#define __NB_PLATFORM_MAC_GENERIC__

typedef uint8_t         BYTE;
typedef BYTE            byte;
typedef char            CHAR;
typedef unsigned char   UCHAR;

#ifndef OBJC_BOOL_DEFINED
typedef int             BOOL;
#endif

typedef short           SHORT;
typedef unsigned short  USHORT;
typedef uint16_t        WORD;

#if __LP64__
typedef int32_t         LONG;
typedef uint32_t        ULONG;
#else
typedef long            LONG;
typedef unsigned long   ULONG;
#endif

typedef uint32_t        DWORD;
typedef int             INT;
typedef unsigned int    UINT;
typedef float           FLOAT;

#ifndef __INTEL_COMPILER
typedef int8_t          __int8;
typedef int16_t         __int16;
typedef int32_t         __int32;
typedef int64_t         __int64;
#endif

typedef int64_t         LONGLONG;
typedef uint64_t        ULONGLONG;

typedef CHAR            TCHAR;
typedef wchar_t         WCHAR;

typedef BYTE *          PBYTE;
typedef FLOAT *         PFLOAT;
typedef LONG *          PLONG;
typedef void *          PVOID;
typedef void *          LPVOID;
typedef USHORT *        PUSHORT;
typedef const char *    LPCSTR;

#ifdef __LP64__
typedef ULONGLONG       DWORD_PTR;
#else
typedef DWORD           DWORD_PTR;
#endif

#if !defined(TRUE)
    #define TRUE	1
#endif

#if !defined(FALSE)
    #define FALSE	0
#endif

#define TEXT(str) (str)

#endif // __NB_PLATFORM_MAC_GENERIC__



// GUID

#ifndef __NB_PLATFORM_MAC_GUID__
#define __NB_PLATFORM_MAC_GUID__

#pragma pack(push, 1)

typedef struct _GUID {
    unsigned long   Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char   Data4[8];
} GUID;

#pragma pack(pop)

typedef GUID *          LPGUID;
typedef const GUID *    LPCGUID;
typedef GUID            IID;
typedef IID *           LPIID;
typedef GUID            CLSID;
typedef CLSID *         LPCLSID;

#ifdef __cplusplus
typedef const GUID &    REFGUID;
typedef const IID &     REFIID;
typedef const IID &     REFCLSID;

#else
typedef const GUID * const REFGUID;
typedef const IID * const  REFIID;
typedef const IID * const  REFCLSID;

#endif



__inline int InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    return (
            ((unsigned long *) &rguid1)[0] == ((unsigned long *) &rguid2)[0] &&
            ((unsigned long *) &rguid1)[1] == ((unsigned long *) &rguid2)[1] &&
            ((unsigned long *) &rguid1)[2] == ((unsigned long *) &rguid2)[2] &&
            ((unsigned long *) &rguid1)[3] == ((unsigned long *) &rguid2)[3]);
}

__inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    //return !memcmp(&rguid1, &rguid2, sizeof(GUID));
    // Can't compare memory. In x64 GUID = 20 bytes and bytes 4-7 are not initialized.
    return
#ifdef __cplusplus
    *(int*)&rguid1.Data1 == *(int*)&rguid2.Data1 &&
    rguid1.Data2 == rguid2.Data2 &&
    rguid1.Data3 == rguid2.Data3 &&
    *((int*)&rguid1.Data4) == *((int*)&rguid2.Data4) &&
    *(((int*)&rguid1.Data4) + 1) == *(((int*)&rguid2.Data4) + 1);
#else
    *(int*)&rguid1->Data1 == *(int*)&rguid2->Data1 &&
    rguid1->Data2 == rguid2->Data2 &&
    rguid1->Data3 == rguid2->Data3 &&
    *((int*)&rguid1->Data4) == *((int*)&rguid2->Data4) &&
    *(((int*)&rguid1->Data4) + 1) == *(((int*)&rguid2->Data4) + 1);
#endif
}

#ifdef __cplusplus

__inline int operator==(REFGUID guidOne, REFGUID guidOther)
{
    return IsEqualGUID(guidOne,guidOther);
}

__inline int operator!=(REFGUID guidOne, REFGUID guidOther)
{
    return !(guidOne == guidOther);
}

#endif


#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)


#ifdef INITGUID

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
EXTERN_C const GUID name \
= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#else

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
EXTERN_C const GUID name

#endif // INITGUID

__inline GUID GetNullGuid()
{
    GUID result = {0};
    return result;
}

#define GUID_NULL (GetNullGuid())

#endif // __NB_PLATFORM_MAC_GUID__




// Common Windows types
// Todo: copy correct structures for SIZE, POINT and RECT here

#ifndef __NB_PLATFORM_MAC_COMMON_WINDOWS__
#define __NB_PLATFORM_MAC_COMMON_WINDOWS__

typedef struct tagSIZE
{
    LONG    cx;
    LONG    cy;
} SIZE;

typedef struct tagPOINT
{
    LONG    x;
    LONG    y;
} POINT;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT;


typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG  HighPart;
	} ;
	struct {
		DWORD LowPart;
		LONG  HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} ;
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
} ULARGE_INTEGER, *UPLARGE_INTEGER;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;


/*
 * extended waveform format structure used for all non-PCM formats. this
 * structure is common to all non-PCM formats.
 */
typedef struct tWAVEFORMATEX
{
    WORD            wFormatTag;         /* format type */
    WORD            nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD           nSamplesPerSec;     /* sample rate */
    DWORD           nAvgBytesPerSec;    /* for buffer estimation */
    WORD            nBlockAlign;        /* block size of data */
    WORD            wBitsPerSample;     /* number of bits per sample of mono data */
    WORD            cbSize;             /* the count in bytes of the size of */
                                        /* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX;

// flags for wFormatTag field of WAVEFORMAT
#define WAVE_FORMAT_PCM     1


// TODO: to understand which the variant of STATSTG we must use
typedef struct tagSTATSTG
{
    /*LPOLESTR*/void * pwcsName;  // TODO: need use the right characters type instead of LPOLESTR
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
} 	STATSTG;


typedef void *          HANDLE;
typedef WindowRef       HWND;
typedef CFBundleRef     HINSTANCE;
typedef LONG            HRESULT;
typedef HANDLE          HCURSOR;
typedef HINSTANCE       HMODULE;
typedef HANDLE          HDC;


#define INVALID_HANDLE_VALUE (HANDLE)((DWORD)-1)
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)


#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)



#pragma pack( push, 1 )

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

#pragma pack( pop )


#ifndef BI_RGB
	#define	BI_RGB	0L
#endif


#define STDMETHODCALLTYPE
#define STDAPICALLTYPE
#define CALLBACK
#define WINAPI
#define APIENTRY

#define FAR
#define NEAR

#define __cdecl
#define __fastcall

// [out] **
#define __RPC__out

// [out] **
#define __RPC__deref_out_opt

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD)(a) & 0xff)) | ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD)(w) >> 8))


#define MAX_PATH 260


#ifndef DWORD_MAX
	#define DWORD_MAX 0xffffffffUL
#endif


typedef DWORD COLORREF;


#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#define GetAValue(rgb)      (LOBYTE((rgb)>>24))

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))


#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif


// Windows COM stuff

#define STDAPI                      EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type)               EXTERN_C type STDAPICALLTYPE
#define STDMETHODVCALLTYPE
#define STDAPIVCALLTYPE       
#define STDMETHOD(method)           virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type, method)    virtual type STDMETHODCALLTYPE method
#define STDMETHODIMP                HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)         type STDMETHODCALLTYPE

#define PURE						= 0


#define S_OK                                   ((HRESULT)0x00000000L)
#define S_FALSE                                ((HRESULT)0x00000001L)

#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

#define NOERROR							 0
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)
#define E_INVALIDARG					 _HRESULT_TYPEDEF_(0x80070057L)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFFL) // Catastrophic failure


#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr)    ((HRESULT)(hr) < 0)

#endif // __NB_PLATFORM_MAC_COMMON_WINDOWS__


#ifndef __NB_PLATFORM_MAC_ATOMIC_OPERATIONS__
#define __NB_PLATFORM_MAC_ATOMIC_OPERATIONS__

#ifdef __cplusplus

__inline long InterlockedDecrement(LONG * aVal)
{
    // DecrementAtomic returns the original value, not the result
    // return ::OSAtomicIncrement32(aVal); - can be useful on iOS
    return (::DecrementAtomic(aVal) - 1);
}

__inline long InterlockedIncrement(LONG * aVal)
{
    // AddAtomic returns the original value, not the result
    //return ::OSAtomicDecrement32(aVal); - can be useful on iOS
    return (::IncrementAtomic(aVal) + 1);
}

#endif // __cplusplus

#endif // __NB_PLATFORM_MAC_ATOMIC_OPERATIONS__



// IUnknown interface

#ifndef __NB_PLATFORM_MAC_IUNKNOWN__
#define __NB_PLATFORM_MAC_IUNKNOWN__

#ifdef __cplusplus

#ifndef __interface
#   define __interface struct
#endif

__interface IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE QueryInterface (const IID & iid, void ** ppOut) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef (void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release (void) = 0;
	
	virtual ~IUnknown() {}
};

DEFINE_GUID(IID_IUnknown, 0x00000000, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

#endif // __cplusplus

#endif // __NB_PLATFORM_MAC_IUNKNOWN__


__inline HRESULT CoCreateGuid(LPGUID pguid)
{
    if (pguid == NULL)
    {
        return E_INVALIDARG;
    }
    
    CFUUIDRef newGuid = CFUUIDCreate(NULL);
    CFUUIDBytes guidBytes = CFUUIDGetUUIDBytes(newGuid);
    
    // if we use direct memory copy - guid comparison fails!
    // memcpy(pguid, &guidBytes, sizeof(CFUUIDBytes));
    
    unsigned char* pData1Bytes = (unsigned char*)&(pguid->Data1);
    
    pData1Bytes[0] = guidBytes.byte0;
    pData1Bytes[1] = guidBytes.byte1;
    pData1Bytes[2] = guidBytes.byte2;
    pData1Bytes[3] = guidBytes.byte3;
    
    
    unsigned char* pData2Bytes = (unsigned char*)&(pguid->Data2);
    
    pData2Bytes[0] = guidBytes.byte4;
    pData2Bytes[1] = guidBytes.byte5;
    
    
    unsigned char* pData3Bytes = (unsigned char*)&(pguid->Data3);
    
    pData3Bytes[0] = guidBytes.byte6;
    pData3Bytes[1] = guidBytes.byte7;
    
    (pguid->Data4)[0] = guidBytes.byte8;
    (pguid->Data4)[1] = guidBytes.byte9;
    (pguid->Data4)[2] = guidBytes.byte10;
    (pguid->Data4)[3] = guidBytes.byte11;
    (pguid->Data4)[4] = guidBytes.byte12;
    (pguid->Data4)[5] = guidBytes.byte13;
    (pguid->Data4)[6] = guidBytes.byte14;
    (pguid->Data4)[7] = guidBytes.byte15;
    
    CFRelease(newGuid);
    
    return S_OK;
}


#define strcpy_s  	strcpy
#define strncpy_s  	strncpy
#define sscanf_s  	sscanf
#define swscanf_s 	swscanf
#define vswprintf_s vswprintf
#define vsprintf_s 	vsprintf
#define wcscat_s 	wcscat

#define _strnicmp strncasecmp

#define _tcsncpy  strncpy
#define _tcslen   strlen
#define _tcscmp   strcmp
#define _ttoi     atoi
#define _ttoi64   atoll
#define _tcsstr   strstr
#define _tfopen   fopen
#define _ftprintf fprintf
#define _tcsncat  strncat

#define _snprintf_s  snprintf
#define _snprintf    snprintf


#define _T(x) (x)
#define _TEXT(x) _T(x)



// Stub-out Win debug macros
#ifdef NDEBUG
#define _RPT0(x, y) void(0)
#define _RPT1(x, y, z1) void(0)
#define _RPT2(x, y, z1, z2) void(0)
#define _RPT3(x, y, z1, z2, z3) void(0)
#define _RPT4(x, y, z1, z2, z3, z4) void(0)
#define _RPT5(x, y, z1, z2, z3, z4, z5) void(0)
#define _RPT6(x, y, z1, z2, z3, z4, z5, z6) void(0)
#else
#define _RPT0(x, y) {::printf(y); ::fflush(stdout);}
#define _RPT1(x, y, z1) {::printf(y, z1); ::fflush(stdout);}
#define _RPT2(x, y, z1, z2) {::printf(y, z1, z2); ::fflush(stdout);}
#define _RPT3(x, y, z1, z2, z3) {::printf(y, z1, z2, z3); ::fflush(stdout);}
#define _RPT4(x, y, z1, z2, z3, z4) {::printf(y, z1, z2, z3, z4); ::fflush(stdout);}
#define _RPT5(x, y, z1, z2, z3, z4, z5) {::printf(y, z1, z2, z3, z4, z5); ::fflush(stdout);}
#define _RPT6(x, y, z1, z2, z3, z4, z5, z6) {::printf(y, z1, z2, z3, z4, z5, z6); ::fflush(stdout);}
#endif

// TODO: support debug versions of RPTW
#define _RPTW0(x, y) void(0)
#define _RPTW1(x, y, z1) void(0)
#define _RPTW2(x, y, z1, z2) void(0)
#define _RPTW3(x, y, z1, z2, z3) void(0)
#define _RPTW4(x, y, z1, z2, z3, z4) void(0)
#define _RPTW5(x, y, z1, z2, z3, z4, z5) void(0)

#define _CRT_WARN 0
#define _CRT_ERROR 0
#define _CRT_ASSERT 0


#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))


#include <mach/mach_time.h>

__inline DWORD GetTickCount()
{
	//uint64_t time = mach_absolute_time();
	//Nanoseconds nanosec = AbsoluteToNanoseconds(*(AbsoluteTime *)&time);
	//return ((*(uint64_t *)&nanosec) / 1000000);
	
	mach_timebase_info_data_t timebase;
	mach_timebase_info(&timebase);
	
	uint64_t time = mach_absolute_time();
	time *= timebase.numer;
	time /= timebase.denom;
	
	return (DWORD)(time / 1000000); // From nanoseconds to milliseconds
}


#define NBMax(a,b)		(((a) > (b)) ? (a) : (b))
#define NBMin(a,b)		(((a) < (b)) ? (a) : (b))
#define NBAbs(N)		( (N) < 0    ? (-(N)) : (N))

// useful define
#ifndef CheckPointer
	#define CheckPointer(p,ret)		{if((p)==NULL) return (ret);}
#endif

#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(ar)        (sizeof(ar)/sizeof((ar)[0]))
#endif // !defined(SIZEOF_ARRAY)


#ifndef NDEBUG
#define DebugBreak()	Debugger()
#else
#define DebugBreak()	void(0)
#endif

#ifdef __cplusplus

#include <stdarg.h>

struct __NBDebug
{
	static int PrintAndBreak(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		int ret = vprintf(format, args);
		fflush(stdout);
		va_end(args);
		DebugBreak();
		return ret;
	}

	static int AssertBox(const char* condition, const char* function, const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char message[512] = {0};
		vsnprintf(message, 511, format, args);
		va_end(args);

		printf("Assertion failed: %s; in %s (%s#%d) %s \n", condition, function, file, line, message);
		fflush(stdout);

		CFOptionFlags responce = kCFUserNotificationDefaultResponse;
		CFStringRef boxText = CFStringCreateWithFormat(NULL, NULL, CFSTR("Condition: %s\n\nIn %s\n%s#%d\n\n%s"), condition, function, file, line, message);
		CFUserNotificationDisplayAlert(0, 0, NULL, NULL, NULL, CFSTR("Assertion failed"), boxText, CFSTR("Ignore"), CFSTR("Debug"), CFSTR("Abort"), &responce);
		CFRelease(boxText);
		if (responce == kCFUserNotificationAlternateResponse)
		{
			DebugBreak();
		}
		else if (responce == kCFUserNotificationOtherResponse)
		{
			exit(1);
		}

		return 0;
	}
	
	static int ErrorBox(const char* condition, const char* function, const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char message[512] = {0};
		vsnprintf(message, 511, format, args);
		va_end(args);
		
		printf("Assertion failed: %s; in %s (%s#%d) %s \n", condition, function, file, line, message);
		fflush(stdout);
		
		CFStringRef boxText = CFStringCreateWithFormat(NULL, NULL, CFSTR("Condition: %s\n\nIn %s\n%s#%d\n\n%s\n\nPlease send text or screenshot of this error to the developers."), condition, function, file, line, message);
		CFUserNotificationDisplayAlert(0, 0, NULL, NULL, NULL, CFSTR("NewBlue Assertion failed"), boxText, CFSTR("Continue"), NULL, NULL, NULL);
		CFRelease(boxText);

		return 0;
	}
};

#endif // __cplusplus


// This assert works even in release.
#define NBForceAssert(x)			(!(x) ? __NBDebug::ErrorBox(#x, __PRETTY_FUNCTION__, __FILE__, __LINE__, "") : 0)

// In debug build, if the condition is false, show a dialog with the option to break into the code.
#ifndef NDEBUG
#define NBAssert(x)		 			(!(x) ? __NBDebug::AssertBox(#x, __PRETTY_FUNCTION__, __FILE__, __LINE__, "") : 0)
#define NBAssertStr(x, msg, ...) 	if (!(x)) __NBDebug::AssertBox(#x, __PRETTY_FUNCTION__, __FILE__, __LINE__, msg, ##__VA_ARGS__);

#else
#define NBAssert(x)		 			void(0)
#define NBAssertStr(x, msg, ...) 	void(0)

#endif


//#define GL_GLEXT_FUNCTION_POINTERS
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>


typedef long long GLint64;
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;


//typedef void *GLhandleARB;

#if __LP64__
//typedef unsigned int GLuint;
#else
//typedef unsigned long GLuint;
#endif


typedef int (WINAPI *PROC)();
