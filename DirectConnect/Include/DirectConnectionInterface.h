/*  DirectConnectionInterface.h
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

#include <Platform.h>

#pragma pack( push, 4 )

// namespace DirectConnection Titler Live
namespace DCTL
{

enum VideoPixelFormat
{
    VPF_BGRA8 = 0,           // Standard Windows (and Intel) 32 bit pixel.

	// Following formats are not supported by DirectConnect yet.
	VPF_ARGB8,               // Inverted pixel (used by AfterEffects.)
	VPF_YUVA8,               // 8 bit YUV format.
	VPF_BGRA32,              // BGRA 32 bit float format (128 bits.)
	VPF_ARGB32,              // Inverted pixel (used by AfterEffects.)
	VPF_YUVA32,              // 32 bit float YUV format.
	VPF_GRAY8,               // 8 bit per pixel gray.
	VPF_GRAY32,              // 32 bit gray float
	VPF_BGRA16,				// BGRA 16 bit float format. Used in GPU.
	VPF_ARGB16,				// ARGB 16 bit float format. Used in GPU.
	VPF_GRAY16,				// 16 bit float gray. Reserved for using in GPU
	VPF_YUY2,				// Y, Cb, Y, Cr (1 component  8bit, 16bit/pixel) [Uncompress] EDIUS format,
	VPF_GRAY16U,				// Use for EDIUS for alpha channel.
	VPF_YUVA_R408,			// 8 bit YUVA Apple format (http://developer.apple.com/quicktime/icefloe/dispatch027.html).
	VPF_YUVA_R4FL,			// 32 bit YUVA Apple format (http://developer.apple.com/quicktime/icefloe/dispatch027.html).
	VPF_RGBA8,		        // RGBA is generic OpenFX format. Used in Resolve.
	VPF_RGBA32               // RGBA 32 bit float format (128 bits.)
};

// Struct used to communicate video frames with Titler Live.
struct VideoFrameParams
{
	VideoFrameParams()
	{
		m_pBuffer = 0;
		m_nWidth = m_nHeight = m_nSpan = m_nDepth = m_nX = m_nY = 0;
		m_pfFormat = VPF_BGRA8;
		m_bFlipped = false;
		m_bPremultAlpha = false;
	}
	VideoFrameParams(unsigned char * pBuffer, int nWidth, int nHeight, int nSpan, int nDepth, VideoPixelFormat pfFormat)
	{
		m_pBuffer = pBuffer;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nSpan = nSpan;
		m_nDepth = nDepth;
		m_pfFormat = pfFormat;
		m_nX = m_nY = 0;
		m_bFlipped = false;
		m_bPremultAlpha = false;
	}
	unsigned char * m_pBuffer;	// Pointer to data.
	int m_nWidth;				// Width in pixels of view area.
	int m_nHeight;				// Height in pixels of view area.
	int m_nSpan;				// Distance, in bytes, from line to line.
	int m_nDepth;				// Byte depth. 4 for standard BGRA color.
    int m_nX;					// X origin in bitmap of the view area.
    int m_nY;					// Y origin in bitmap of the view area.
	VideoPixelFormat m_pfFormat;// Color format of pixels.
    bool m_bFlipped;			// The image is flipped vertically.
	bool m_bPremultAlpha;		// The RGB components are alpha premultiplied.
};

// Struct used to communicate audio frames with Titler Live.
struct AudioFrameParams
{
	AudioFrameParams()
	{
		m_pBuffer = 0;
		m_nSampleRate = m_nChannels = m_nSampleCount = 0;
	};
	AudioFrameParams(float* pBuffer, int nSampleRate, int nChannels, int nSampleCount)
	{
		m_pBuffer = pBuffer;
		m_nSampleRate = nSampleRate;
		m_nChannels = nChannels;
		m_nSampleCount = nSampleCount;
	};

	float* m_pBuffer;	// Pointer to data.
	int m_nSampleRate;	// Sample rate.
	int m_nChannels;	// Number of channels.
	int m_nSampleCount;	// Number of samples.
};

// DirectConnection Titler Live Interfaces Definition.

enum DCResult
{
	DCERR_BufferIsNotEnough = -3,
	DCERR_ServerShutdown = -2,
	DCERR_Failed = -1,
	DCERR_OK = 0,
	DCERR_DataIsNotReady = 1,
	DCERR_OK_VideoSettingsNotMatch = 2, // Only GetVideoFrame() method can return this error.
};

// {C4D50457-6924-4F54-A494-560F9120C2FA}
DEFINE_GUID(IID_DirectConnection,
	0xc4d50457, 0x6924, 0x4f54, 0xa4, 0x94, 0x56, 0xf, 0x91, 0x20, 0xc2, 0xfa);

__interface IDirectConnection : public IUnknown
{
	/**
	 * InitializeDirectConnection() will initialize instance with requested parameters. Also it will open Main UI.
	 * '*pdStartTime' will be used as parameter 'dTime' for GetVideoFrame/GetAudioFrame functions.
	 * You should to increment 'dTime' beginning with this '*pdStartTime'. Increment it using 'dFrameRate'. */
	virtual DCResult InitializeDirectConnection(int nWidth, int nHeight, double dFrameRate, double dPixelAspectRatio,
		bool bInterlaced, int nSampleRate, int nNumChannels, double* pdStartTime) = 0;

	/**
	 * Return actual video settings. */
    virtual DCResult GetVideoSettings(int* pnWidth, int* pnHeight, double* pdFrameRate, double* pdPixelAspectRatio, bool* pbInterlaced) = 0;

    /**
     * Return actual audio settings. */
    virtual DCResult GetAudioSettings(int* pnSampleRate, int* pnNumChannels) = 0;

	/**
     * Video render method.
	 * Returns 'DCERR_OK' when new frame is ready or 'DCERR_DataIsNotReady'.
     * Returns 'DCERR_OK_VideoSettingsNotMatch' if video settings on server are different with requested settings of 'pFrame'.
	 * Note: Frame should be allocated depending on video properties which you used in InitializeDirectConnection(). */
	virtual DCResult GetVideoFrame(VideoFrameParams* pFrame, double dTime) = 0;

	/**
     * Audio render method.
	 * Returns 'DCERR_OK' when new frame is ready or 'DCERR_DataIsNotReady'.
	 * Returns the error - 'DCERR_BufferIsNotEnough' when pFrame->m_nSampleCount is not enough for whole audio frame. 
	 * Note: GetAudioFrame will modify pFrame->m_nSampleCount to the actual count of samples which were copied to the buffer pFrame->m_pBuffer. */
	virtual DCResult GetAudioFrame(AudioFrameParams* pFrame, double dTime) = 0;

	/**
	 * Get metadata if it exists else it returns DCERR_DataIsNotReady. */
	virtual DCResult GetMetadataFrame(char** ppszMetadata) = 0;
	virtual DCResult FreeMetadataFrame(char* pszMetadata) = 0;

	/**
	 * Send a metadata frame. */
	virtual DCResult SendMetadataFrame(const char* pszMetadata) = 0;
};

}	// end namespace DCTL

#pragma pack( pop )
