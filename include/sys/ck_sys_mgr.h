/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*!
    @ingroup pgSys
    TODO
*/
class PG_API pgSysMgr
{
public:
    pgDefineException(ExceptionCannotCreateMutex);
    pgDefineException(ExceptionCannotCreateThread);
    pgDefineException(ExceptionCannotOpenFile);
    pgDefineException(ExceptionCannotReadFile);
    pgDefineException(ExceptionCannotWriteFile);
    pgDefineException(ExceptionCreateFramebufferFailed);
    pgDefineException(ExceptionEndPogolyn);
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotInitialized);

    /*!
        TODO
    */
    enum SysFlag
    {
        FLAG_FULLSCREEN_START = 0x0001, //
        FLAG_VARIABLE_SIZE = 0x0002, //
        FLAG_DISABLE_SHADER = 0x0004
    };

    enum FileMode
    {
        FILE_MODE_READ, //
        FILE_MODE_WRITE
    };

    static void printf(const char* str, ...);
    static void wprintf(const wchar_t* str, ...);
    static void sprintf(char* buf, u32 buf_size, const char* str, ...);
    static void swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...);

    static void* openFile(const char* filename, FileMode file_mode);
    static u32 getFileSize(void* file_handler);
    static void readFile(void* buf, u32 offset, u32 size, void* file_handler);
    static void writeFile(u32 offset, const void* buf, u32 size, void* file_handler);
    static void closeFile(void* file_handler);

    static u64 getUsecTime();
    static void sleepUsec(u64 usec);

    static void* newThread(void (*start_func)(void*), void* user_param);
    static void deleteThread(void* thread_handler);
    static void joinThread(void* thread_handler);
    static void* newMutex();
    static void deleteMutex(void* mutex_handler);
    static void lockMutex(void* mutex_handler);
    static void unlockMutex(void* mutex_handler);

    static bool isCreated();
    static void createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag);
    static void destroyBeforeMem();

    static u16 getFramebufferWidth();
    static u16 getFramebufferHeight();
    static bool isFramebufferSizeChanged();

    static bool isFullScreen();
    static void toggleFullScreen(u16 width, u16 height);

    static void updateForSystem();

    static void readLittleEndianForSystem(void* dest, const void* src, u32 size);
    template<class T> static T readLittleEndianForSystem(const void* data)
    {
        T value;

        readLittleEndianForSystem(&value, data, sizeof(T));

        return value;
    }

    static void writeLittleEndianForSystem(void* dest, const void* src, u32 size);
    template<class T> static void writeLittleEndianForSystem(void* dest, const T& value)
    {
        writeLittleEndianForSystem(dest, &value, sizeof(T));
    }

    static void setInitialDirectoryForSystem(s32 argc, char** argv);

private:
    pgSysMgr(const char* title, u16 width, u16 height, u16 sys_flag);
    ~pgSysMgr();
    void operator=(const pgSysMgr&);

    static pgSysMgr* instance();

    static pgSysMgr* m_instance;
};
