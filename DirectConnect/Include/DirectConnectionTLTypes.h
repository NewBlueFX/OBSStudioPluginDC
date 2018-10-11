

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Oct 03 14:05:39 2018
 */
/* Compiler settings for DirectConnectionTLTypes.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __DirectConnectionTLTypes_h__
#define __DirectConnectionTLTypes_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DirectConnectionTLTypes_0000_0000 */
/* [local] */ 

#pragma pack( push, 4 )
typedef unsigned long long DCTLClientID;

typedef unsigned long long DCTLInstanceID;

typedef unsigned long DCTLProcessID;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_DirectConnectionTLTypes_0000_0000_0001
    {
        IDL_DCTLPF_BGRA8	= 0,
        IDL_DCTLPF_ARGB8	= ( IDL_DCTLPF_BGRA8 + 1 ) ,
        IDL_DCTLPF_YUVA8	= ( IDL_DCTLPF_ARGB8 + 1 ) ,
        IDL_DCTLPF_BGRA32	= ( IDL_DCTLPF_YUVA8 + 1 ) ,
        IDL_DCTLPF_ARGB32	= ( IDL_DCTLPF_BGRA32 + 1 ) ,
        IDL_DCTLPF_YUVA32	= ( IDL_DCTLPF_ARGB32 + 1 ) ,
        IDL_DCTLPF_GRAY8	= ( IDL_DCTLPF_YUVA32 + 1 ) ,
        IDL_DCTLPF_GRAY32	= ( IDL_DCTLPF_GRAY8 + 1 ) ,
        IDL_DCTLPF_BGRA16	= ( IDL_DCTLPF_GRAY32 + 1 ) ,
        IDL_DCTLPF_ARGB16	= ( IDL_DCTLPF_BGRA16 + 1 ) ,
        IDL_DCTLPF_GRAY16	= ( IDL_DCTLPF_ARGB16 + 1 ) ,
        IDL_DCTLPF_YUY2	= ( IDL_DCTLPF_GRAY16 + 1 ) ,
        IDL_DCTLPF_GRAY16U	= ( IDL_DCTLPF_YUY2 + 1 ) ,
        IDL_DCTLPF_YUVA_R408	= ( IDL_DCTLPF_GRAY16U + 1 ) ,
        IDL_DCTLPF_YUVA_R4FL	= ( IDL_DCTLPF_YUVA_R408 + 1 ) ,
        IDL_DCTLPF_RGBA8	= ( IDL_DCTLPF_YUVA_R4FL + 1 ) ,
        IDL_DCTLPF_RGBA32	= ( IDL_DCTLPF_RGBA8 + 1 ) 
    } 	DCTLVideoPixelFormatIDL;

typedef /* [version][uuid] */  DECLSPEC_UUID("50CE03C2-C328-49D2-A6D8-01548FC94505") struct DCTLVideoFrameParamsIDL
    {
    int m_nWidth;
    int m_nHeight;
    int m_nSpan;
    int m_nDepth;
    int m_nX;
    int m_nY;
    DCTLVideoPixelFormatIDL m_pfFormat;
    boolean m_bFlipped;
    boolean m_bPremultAlpha;
    } 	DCTLVideoFrameParamsIDL;

typedef /* [version][uuid] */  DECLSPEC_UUID("37DA9CA4-5739-4646-A624-D55C68D7644F") struct DCTLAudioFrameParamsIDL
    {
    int m_nSampleRate;
    int m_nChannels;
    int m_nSampleCount;
    } 	DCTLAudioFrameParamsIDL;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_DirectConnectionTLTypes_0000_0000_0002
    {
        DCTLSM_RENDER_VIDEO_FRAME	= 0,
        DCTLSM_RENDER_AUDIO_FRAME	= 1,
        DCTLSM_RENDER_HOST_FRAME1	= 2,
        DCTLSM_RENDER_HOST_FRAME2	= 3
    } 	DCTLSharedMemoryType;

typedef /* [version][uuid] */  DECLSPEC_UUID("C3867AC5-F335-4541-8AFF-1CF418753437") struct DCTLVideoRenderInfoIDL
    {
    long m_lValidFlags;
    double m_fProgress;
    double m_fTime;
    double m_fDuration;
    double m_fFramesPerSec;
    double m_fAspectRatio;
    long m_lFrame;
    long m_nField;
    double m_fScale;
    } 	DCTLVideoRenderInfoIDL;

#pragma pack( pop )


extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLTypes_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLTypes_0000_0000_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


