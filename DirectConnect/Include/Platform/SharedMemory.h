/*	SharedMemory.h
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

#include <string>
#include "Platform.h"

// Win
//
// To implement CSharedMemory, we used Win32 API functions CreateFileMapping/OpenFileMapping and MapViewOfFile/UnmapViewOfFile.
// https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa366537(v=vs.85).aspx

// OS X
//
// https://developer.apple.com/library/content/documentation/Darwin/Conceptual/MacOSXNotifcationOv/AlternativestoNotification/AlternativestoNotification.html
//
// For general memory sharing, the easiest mechanism to use is the mmap(2) system call.
// Two other ways to share memory are the POSIX and System V shared memory APIs. Because the POSIX shared memory API is newer and more flexible,
// you should favor the POSIX shared memory API for new applications unless you need to support other computing platforms where it are not available.
//
// Following this description above we use POSIX shared memory API.
// http://man7.org/training/download/posix_shm_slides.pdf
//
// TODO: add semaphore object.
class CSharedMemory
{
    CSharedMemory(const CSharedMemory &str);	// not implemented
    CSharedMemory& operator=(const CSharedMemory &str);	// not implemented

public:

    CSharedMemory();
    ~CSharedMemory();

    // Initializes the shared memory object with given parameters.
    bool InitSharedMemory(const std::string& resourceName, bool bCreate, unsigned int bufferSize);

    // Releases the shared memory object.
    void FreeSharedMemory();

    // Returns 'true' if the shared memory object was initialized successfully.
    bool IsInited() const;

    // Returns the pointer on data buffer.
    void* GetBuffer() const;

    // Recreates current shared memory with new size if new 'bufferSize' more than GetCurrentSize().
    bool ResizeBuffer(unsigned int bufferSize);

    // Returns current size of mapped memory
    unsigned int GetCurrentSize() const;

    // Returns the name of shared memory.
    std::string GetName() const;

    // Returns 'true' if CSharedMemory is 'owner' of shared memory.
    bool IsOwner() const;

private:

#ifdef _WIN32
    typedef HANDLE ShMemId;
#elif __APPLE__
    typedef int ShMemId;
#else
#   error Unsupported platform
#endif

    ShMemId         m_hObject = 0;          // System object descriptor of shared memory.
    void*           m_pBuffer = nullptr;    // Buffer pointer on mapped memory.
    std::string     m_userResourceName;     // Unique name of system object from InitSharedMemory().
    unsigned int    m_nMappedSize = 0;      // Current size of mapped memory
    bool            m_bCreated = false;     // 'bCreate' flag from InitSharedMemory() method.
    std::string     m_actualResourceName;   // For Win system m_userResourceName == m_actualResourceName.
                                            // For OSX system m_userResourceName != m_actualResourceName,
                                            // because the system of shared memory has some restriction and
                                            // we convert m_actualResourceName to appropriate format and
                                            // put it to m_actualResourceName.
};

