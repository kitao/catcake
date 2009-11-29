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
    @ingroup ckSys
    System manager.
*/
class CK_API ckSysMgr
{
public:
    ckDefineException(ExceptionCannotCreateMutex);
    ckDefineException(ExceptionCannotCreateThread);
    ckDefineException(ExceptionCannotOpenFile);
    ckDefineException(ExceptionCannotReadFile);
    ckDefineException(ExceptionCannotWriteFile);
    ckDefineException(ExceptionCreateFramebufferFailed);
    ckDefineException(ExceptionEndCatcake);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotInitialized);

    /*!
        The system flags.
    */
    enum SysFlag
    {
        FLAG_FULLSCREEN_START = 0x0001, //!< Starts with full screen mode.
        FLAG_VARIABLE_SIZE = 0x0002, //!< Enables resize the window.
        FLAG_DISABLE_SHADER = 0x0004 //!< Disables programmable shader.
    };

    /*!
        The file modes.
    */
    enum FileMode
    {
        FILE_MODE_READ, //!< Read-only.
        FILE_MODE_WRITE //!< Read and write.
    };

    /*!
        Writes a string.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void printf(const char* str, ...);

    /*!
        Writes a wchar string.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void wprintf(const wchar_t* str, ...);

    /*!
        Writes a string to the specified buffer.
        @param[out] buf A buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void sprintf(char* buf, u32 buf_size, const char* str, ...);

    /*!
        Writes a wchar string to the specified buffer.
        @param[out] buf A buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...);

    /*!
        Opens the specified file.
        @param[in] filename A file name.
        @param[in] file_mode A file mode.
        @return The file handler.
    */
    static void* openFile(const char* filename, FileMode file_mode);

    /*!
        Returns the size of the specified file.
        @param[in] file_handler A file handler.
        @return The size of a file.
    */
    static u32 getFileSize(void* file_handler);

    /*!
        Reads the speficified file.
        @param[out] buf A buffer.
        @param[in] offset The position of reading.
        @param[in] size The size of reading.
        @param[in] file_handler A file handler.
    */
    static void readFile(void* buf, u32 offset, u32 size, void* file_handler);

    /*
        Writes the specified file.
        @param[in] offset The position of writing.
        @param[in] buf Data to be written.
        @param[in] size The size of data.
        @param[in] file_handler A file handler.
    */
    static void writeFile(u32 offset, const void* buf, u32 size, void* file_handler);

    /*!
        Closes the specified file.
        @param[in] file_handler A file handler.
    */
    static void closeFile(void* file_handler);

    /*!
        Returns the current time in usec.
    */
    static u64 getUsecTime();

    /*!
        Sleeps for the specified time.
        @param[in] usec The period to sleep in usec.
    */
    static void sleepUsec(u64 usec);

    /*!
        Creates a thread.
        @param[in] start_func A starting function.
        @param[in] user_param An user param.
        @return A thread handler.
    */
    static void* newThread(void (*start_func)(void*), void* user_param);

    /*!
        Destroys the specified thread.
        @param[in] thread handler A thread handler.
    */
    static void deleteThread(void* thread_handler);

    /*!
        Waits for the end of the specified thread.
        @param[in] thread handler A thread handler.
    */
    static void joinThread(void* thread_handler);

    /*!
        Creates a mutex.
        @return A mutex handler.
    */
    static void* newMutex();

    /*!
        Destroys the specified mutex.
        @parma[in] mutex_handler A mutex handler.
    */
    static void deleteMutex(void* mutex_handler);

    /*!
        Locks the specified mutex.
        @parma[in] mutex_handler A mutex handler.
    */
    static void lockMutex(void* mutex_handler);

    /*!
        Unlocks the specified mutex.
        @parma[in] mutex_handler A mutex handler.
    */
    static void unlockMutex(void* mutex_handler);

    /*!
        Returns the system manager singleton is created.
        @return The system manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the system manager singleton.
        @param[in] title The title of the application.
        @param[in] width The width of the framebuffer.
        @param[in] height The height of the framebuffer.
        @param[in] sys_flag System flag.
    */
    static void createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag);

    /*!
        Destroys the system manager singleton.
    */
    static void destroyBeforeMem();

    /*!
        Returns the width of the framebuffer.
        @return The width of the framebuffer.
    */
    static u16 getFramebufferWidth();

    /*!
        Returns the height of the framebuffer.
        @return The height of the framebuffer.
    */
    static u16 getFramebufferHeight();

    /*!
        Returns whether the size of the framebuffer is changed.
        @return Whether the size of the framebuffer is changed.
    */
    static bool isFramebufferSizeChanged();

    /*!
        Returns whether the framebuffer is full screen.
        @return Whether the framebuffer is full screen.
    */
    static bool isFullScreen();

    /*!
        Toggles the full screen.
        @param[in] width The width of the framebuffer.
        @param[in] height The height of the framebuffer.
    */
    static void toggleFullScreen(u16 width, u16 height);

    /*!
        Updates the current frame.
    */
    static void updateForSystem();

    /*!
        Reads the data in little endian.
        @param[out] dest The destination.
        @param[in] src The source.
        @param[in] size The size of the data.
    */
    static void readLittleEndianForSystem(void* dest, const void* src, u32 size);

    /*!
        Reads the data in little endian. This method is only for system.
        @tparam T The class type.
        @param[in] data The source data.
    */
    template<class T> static T readLittleEndianForSystem(const void* data)
    {
        T value;

        readLittleEndianForSystem(&value, data, sizeof(T));

        return value;
    }

    /*!
        Writes the data in little endian.
        @param[out] dest The destination.
        @param[in] src The source.
        @param[in] size The size of the data.
    */
    static void writeLittleEndianForSystem(void* dest, const void* src, u32 size);

    /*!
        Writes the data in little endian. This method is only for system.
        @tparam T The class type.
        @param[out] dest The destination.
        @param[in] value The value.
    */
    template<class T> static void writeLittleEndianForSystem(void* dest, const T& value)
    {
        writeLittleEndianForSystem(dest, &value, sizeof(T));
    }

    /*!
        Sets the initial directory. This method is only for system.
        @param[in] args The number of the arguments of the main function.
        @param[in] argv The arguments of the main function.
    */
    static void setInitialDirectoryForSystem(s32 argc, char** argv);

private:
    ckSysMgr(const char* title, u16 width, u16 height, u16 sys_flag);
    ~ckSysMgr();
    void operator=(const ckSysMgr&);

    static ckSysMgr* instance();

    static ckSysMgr* m_instance;
};
