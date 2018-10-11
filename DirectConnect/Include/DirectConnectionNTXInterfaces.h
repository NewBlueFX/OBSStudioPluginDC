/*  DirectConnectionNTXInterface.h
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
#include "DirectConnectionInterface.h"
#include "DirectConnectionTypes.h"


#pragma pack( push, 4 )

// namespace DirectConnection Titler Live
namespace DCTL
{

// Forward declaration
__interface IDirectConnectionNTXHostServer;

// The copy of RenderInfo from NewBlue code.
struct VideoRenderInfo
{
    VideoRenderInfo()
    { 
        m_nSize = sizeof(VideoRenderInfo);
        m_fTime = 0.0;
        m_nValidFlags = 2;
        m_fAspectRatio = m_fScale = 1.0;
		m_fProgress = m_fDuration = m_fFramesPerSec = 0.0;
		m_nFrame = 0;
		m_nField = 0;
    };

    int		m_nSize;        // Size of this structure.
    int		m_nValidFlags;  // Which fields are filled.
    double	m_fProgress;    // Percentage of time in this effect (0 to 1)
    double	m_fTime;        // Actual time, in seconds.
    double	m_fDuration;    // Total duration (or 0, if not available.)
    double	m_fFramesPerSec;// Frame rate.
    double	m_fAspectRatio; // Pixel aspect ratio (for non-square pixels).
    int		m_nFrame;       // Frame number.
    int		m_nField;       // 1 for first interlace field, 2 for second, 3 for both.  
    double	m_fScale;       // If the frame is larger or smaller than the intended resolution, this 
                            // tells the effect to scale up or down (for example, blur width in pixels.)
};

// {AFBF33D0-4BF5-4F7C-99BD-53E47ECA40A8}
DEFINE_GUID(IID_DirectConnectionNTXExtension,
    0xafbf33d0, 0x4bf5, 0x4f7c, 0x99, 0xbd, 0x53, 0xe4, 0x7e, 0xca, 0x40, 0xa8);

__interface IDirectConnectionNTXExtension : public IUnknown
{
	/**
	 * Retrieve the DC instance ID */
	virtual DCTLInstanceID GetInstanceID() = 0;

	/**
	 * Set the ipc client host server to handle calls back to NLE */
	virtual DCResult SetHostServer(IDirectConnectionNTXHostServer* pHostServerInterface) = 0;

    /**
     * Free shared memory on app side. */
    virtual DCResult FreeSharedMemory(DCTLSharedMemoryType type) = 0;
};


// {B9733328-0AC2-48E6-AF53-B83E5FF2827B}
DEFINE_GUID(IID_DirectConnectionNTXHostServer,
	0xb9733328, 0xac2, 0x48e6, 0xaf, 0x53, 0xb8, 0x3e, 0x5f, 0xf2, 0x82, 0x7b);

__interface IDirectConnectionNTXHostServer : public IUnknown
{
	/**
	 * Retrieves a frame from a NLE designated by the provided instance id */
	virtual DCResult GetHostFrame(DCTLInstanceID instanceID, int nVideoFrameType,
		VideoRenderInfo* pRenderInfo, VideoFrameParams* pFrame) = 0;
};


// {DADE089B-5FE3-4A82-92BE-79CF3A3CF5C8}
DEFINE_GUID(IID_DirectConnectionNTXHostClient,
	0xdade089b, 0x5fe3, 0x4a82, 0x92, 0xbe, 0x79, 0xcf, 0x3a, 0x3c, 0xf5, 0xc8);

__interface IDirectConnectionNTXHostClient : public IUnknown
{
	/**
	 * GetHostFrame() will retrieve a frame from the NLE Host for the current title */
	virtual ptrdiff_t GetHostFrame(int nVideoFrameType, VideoRenderInfo* pRenderInfo, VideoFrameParams* pFrame) = 0;

	/*
	 * Release frame which was retrived from GetHostFrame() function. */
	virtual bool ReleaseHostFrame(ptrdiff_t frameId) = 0;
};

}	// end namespace DCTL

#pragma pack( pop )
