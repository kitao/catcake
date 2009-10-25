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


#include "test.h"


static u32 s_thread_func_value;
static void* s_mh;


static void threadFuncTest(void* user_param)
{
    pgAssert(user_param == s_mh);

    pgSysMgr::lockMutex(s_mh);

    pgAssert(s_thread_func_value == 1);

    pgSysMgr::unlockMutex(s_mh);

    pgSysMgr::sleepUsec(10000);

    s_thread_func_value = 2;
}


void pgSysMgrTest()
{
    /*
        static void printf(const char* str, ...)
        static void sprintf(char* buf, u32 buf_size, const char* str, ...)
    */
    {
        pgSysMgr::printf("Printf test %x\n", 65535);

        char buf[10];
        pgSysMgr::sprintf(buf, 10, "%d %3.1f", 123, 456.0f);

        pgStr<char, 10> str = buf;
        pgAssert(str == "123 456.0");

        pgAssertThrow(pgSysMgr::printf(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::sprintf(NULL, 10, "dummy"), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::sprintf(buf, 0, "dummy"), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::sprintf(buf, 10, NULL), pgSysMgr::ExceptionInvalidArgument);
    }

    /*
        static void wprintf(const wchar_t* str, ...)
        static void swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...)
    */
    {
        pgSysMgr::wprintf(L"Printf test %x\n", 65535);

        wchar_t buf[10];
        pgSysMgr::swprintf(buf, 10, L"%d %3.1f", 123, 456.0f);

        pgStr<wchar_t, 10> str = buf;
        pgAssert(str == L"123 456.0");

        pgAssertThrow(pgSysMgr::wprintf(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::swprintf(NULL, 10, L"dummy"), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::swprintf(buf, 0, L"dummy"), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::swprintf(buf, 10, NULL), pgSysMgr::ExceptionInvalidArgument);
    }

    /*
        static void* openFile(const char* filename, FileMode file_mode)
        static u32 getFileSize(void* file_handler)
        static void readFile(void* buf, u32 offset, u32 size, void* file_handler)
        static void writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
        static void closeFile(void* file_handler)
    */
    {
        void* fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data1.dat", pgSysMgr::FILE_MODE_READ);
        pgAssert(fh);
        pgAssert(pgSysMgr::getFileSize(fh) == 10);

        u8 buf[10];
        pgMemMgr::memset(buf, 255, 10);

        pgSysMgr::readFile(buf, 0, 3, fh);
        pgAssert(buf[0] == '0' && buf[1] == '1' && buf[2] == '2' && buf[3] == 255);

        pgSysMgr::readFile(buf, 5, 2, fh);
        pgAssert(buf[0] == '5' && buf[1] == '6' && buf[2] == '2' && buf[3] == 255);

        pgSysMgr::readFile(buf, 8, 2, fh);
        pgAssert(buf[0] == '8' && buf[1] == '9' && buf[2] == '2' && buf[3] == 255);

        pgAssertThrow(pgSysMgr::readFile(buf, 8, 4, fh), pgSysMgr::ExceptionCannotReadFile);
        pgAssertThrow(pgSysMgr::writeFile(0, buf, 3, fh), pgSysMgr::ExceptionCannotWriteFile);

        pgSysMgr::closeFile(fh);

        pgAssertThrow(pgSysMgr::openFile(TEST_DATA_DIR "dummy.dat", pgSysMgr::FILE_MODE_READ), pgSysMgr::ExceptionCannotOpenFile);

        fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", pgSysMgr::FILE_MODE_WRITE);
        pgSysMgr::closeFile(fh);

        fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", pgSysMgr::FILE_MODE_READ);
        pgAssert(pgSysMgr::getFileSize(fh) == 0);

        pgAssertThrow(pgSysMgr::getFileSize(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::readFile(buf, 0, 1, fh), pgSysMgr::ExceptionCannotReadFile);

        pgSysMgr::closeFile(fh);

        fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", pgSysMgr::FILE_MODE_WRITE);

        buf[0] = 1;
        buf[1] = 2;
        buf[2] = 3;

        pgSysMgr::writeFile(0, buf, 3, fh);
        pgSysMgr::writeFile(2, buf, 3, fh);

        pgAssertThrow(pgSysMgr::writeFile(0, NULL, 3, fh), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::readFile(buf, 0, 3, fh), pgSysMgr::ExceptionCannotReadFile);

        pgSysMgr::closeFile(fh);

        fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", pgSysMgr::FILE_MODE_READ);
        pgAssert(pgSysMgr::getFileSize(fh) == 5);

        pgSysMgr::readFile(buf, 0, 5, fh);
        pgAssert(buf[0] = 1 && buf[1] == 2 && buf[2] == 1 && buf[3] == 2 && buf[4] == 3);

        pgSysMgr::closeFile(fh);

        fh = pgSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", pgSysMgr::FILE_MODE_WRITE);
        pgSysMgr::closeFile(fh);

        fh = reinterpret_cast<void*>(1);

        pgAssertThrow(pgSysMgr::openFile(NULL, pgSysMgr::FILE_MODE_READ), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::readFile(NULL, 0, 4, fh), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::readFile(NULL, 0, 0, fh), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::readFile(buf, 0, 4, NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::writeFile(0, buf, 0, fh), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::writeFile(0, buf, 3, NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::closeFile(NULL), pgSysMgr::ExceptionInvalidArgument);
    }

    /*
        static u64 getUsecTime()
        static void sleepUsec(u64 usec)
    */
    {
        u64 usec = pgSysMgr::getUsecTime();
        pgAssert(usec > 0);

        pgSysMgr::sleepUsec(1000 * 100);
        u64 elapsed = pgSysMgr::getUsecTime() - usec;
        pgAssert(elapsed > 1000 * 50 && elapsed < 1000 * 150);
    }

    /*
        static void* newThread(void (*start_func)(void*), void* user_param)
        static void deleteThread(void* thread_handler)
        static void joinThread(void* thread_handler)
        static void* newMutex()
        static void deleteMutex(void* mutex_handler)
        static void lockMutex(void* mutex_handler)
        static void unlockMutex(void* mutex_handler)
    */
    {
        s_thread_func_value = 0;

        s_mh = pgSysMgr::newMutex();

        pgSysMgr::lockMutex(s_mh);

        void* th = pgSysMgr::newThread(threadFuncTest, s_mh);

        s_thread_func_value = 1;

        pgSysMgr::unlockMutex(s_mh);

        pgSysMgr::joinThread(th);

        pgAssert(s_thread_func_value == 2);

        pgSysMgr::deleteThread(th);
        pgSysMgr::deleteMutex(s_mh);

        pgAssertThrow(pgSysMgr::newThread(NULL, NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::deleteThread(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::joinThread(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::deleteMutex(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::lockMutex(NULL), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::unlockMutex(NULL), pgSysMgr::ExceptionInvalidArgument);
    }

    /*
        static bool isCreated()
        static void createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag)
        static void destroyBeforeMem()
        static u16 getFramebufferWidth()
        static u16 getFramebufferHeight()
        static bool isFramebufferSizeChanged()
        static bool isFullScreen()
        static void toggleFullScreen(u16 width, u16 height)
        static void updateForSystem()
    */
    {
        pgAssert(!pgSysMgr::isCreated());

        pgAssertThrow(pgSysMgr::getFramebufferWidth(), pgSysMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSysMgr::getFramebufferHeight(), pgSysMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSysMgr::isFramebufferSizeChanged(), pgSysMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSysMgr::isFullScreen(), pgSysMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSysMgr::toggleFullScreen(0, 0), pgSysMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSysMgr::updateForSystem(), pgSysMgr::ExceptionNotInitialized);

        pgAssertThrow(pgSysMgr::createAfterMem(NULL, 100, 100, 0), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::createAfterMem("test", 0, 100, 0), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::createAfterMem("test", 100, 0, 0), pgSysMgr::ExceptionInvalidArgument);

        pgSysMgr::createAfterMem("test1", 160, 170, 0);

        pgAssert(pgSysMgr::isCreated());
        pgAssert(pgSysMgr::getFramebufferWidth() == 160 && pgSysMgr::getFramebufferHeight() == 170);
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());
        pgAssert(!pgSysMgr::isFullScreen());

        pgSysMgr::updateForSystem();
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgSysMgr::createAfterMem("test2", 640, 480, pgSysMgr::FLAG_FULLSCREEN_START);

        pgAssert(pgSysMgr::isCreated());
        pgAssert(pgSysMgr::getFramebufferWidth() >= 640 && pgSysMgr::getFramebufferHeight() >= 480);
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());
        pgAssert(pgSysMgr::isFullScreen());

        pgSysMgr::updateForSystem();
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgSysMgr::toggleFullScreen(640, 480);
        pgAssert(pgSysMgr::getFramebufferWidth() == 640 && pgSysMgr::getFramebufferHeight() == 480);
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());
        pgAssert(!pgSysMgr::isFullScreen());

        pgSysMgr::updateForSystem();
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgSysMgr::toggleFullScreen(800, 600);
        pgAssert(pgSysMgr::getFramebufferWidth() >= 800 && pgSysMgr::getFramebufferHeight() >= 600);
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());
        pgAssert(pgSysMgr::isFullScreen());

        pgSysMgr::updateForSystem();
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgAssertThrow(pgSysMgr::toggleFullScreen(0, 100), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::toggleFullScreen(100, 0), pgSysMgr::ExceptionInvalidArgument);

        pgSysMgr::createAfterMem("test3", 234, 345, pgSysMgr::FLAG_VARIABLE_SIZE | pgSysMgr::FLAG_DISABLE_SHADER);

        pgAssert(pgSysMgr::isCreated());
        pgAssert(pgSysMgr::getFramebufferWidth() == 234 && pgSysMgr::getFramebufferHeight() == 345);
        pgAssert(!pgSysMgr::isFullScreen());
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgSysMgr::updateForSystem();
        pgAssert(!pgSysMgr::isFramebufferSizeChanged());

        pgSysMgr::destroyBeforeMem();
        pgSysMgr::destroyBeforeMem();

        pgAssert(!pgSysMgr::isCreated());
    }

    /*
        static void readLittleEndianForSystem(void* dest, const void* src, u32 size)
        template<class T> static T readLittleEndianForSystem(const void* data)
        static void writeLittleEndianForSystem(void* dest, const void* src, u32 size)
        template<class T> static void writeLittleEndianForSystem(void* dest, const T& value)
    */
    {
        static const u8 data1[] =
        {
            0x78, 0x56, 0x34, 0x12
        };

        static const u8 data2[] =
        {
            0xcd, 0xab
        };

        u32 val1;
        u16 val2;

        pgSysMgr::readLittleEndianForSystem(&val1, data1, sizeof(u32));
        pgAssert(val1 == 0x12345678);

        pgSysMgr::readLittleEndianForSystem(&val2, data2, sizeof(u16));
        pgAssert(val2 == 0xabcd);

        pgAssert(pgSysMgr::readLittleEndianForSystem<u32>(data1) == 0x12345678);
        pgAssert(pgSysMgr::readLittleEndianForSystem<u16>(data2) == 0xabcd);

        u8 data3[4], data4[2];

        s32 val3 = -123456;
        s16 val4 = -789;

        pgSysMgr::writeLittleEndianForSystem(data3, &val3, sizeof(s32));
        pgAssert(pgSysMgr::readLittleEndianForSystem<s32>(data3) == val3);

        pgSysMgr::writeLittleEndianForSystem(data4, &val4, sizeof(s16));
        pgAssert(pgSysMgr::readLittleEndianForSystem<s16>(data4) == val4);

        pgSysMgr::writeLittleEndianForSystem<s16>(data3, -4321);
        pgAssert(pgSysMgr::readLittleEndianForSystem<s16>(data3) == -4321);

        pgSysMgr::writeLittleEndianForSystem<s8>(data4, -21);
        pgAssert(pgSysMgr::readLittleEndianForSystem<s8>(data4) == -21);
    }

    /*
        static void setInitialDirectoryForSystem(s32 argc, char** argv)
    */
    {
        static char str1[] = "dummy1";
        static char str2[] = "dummy2";
        static char str3[] = "dummy3";

        static char* dummy[] =
        {
            str1, str2, str3
        };

        pgAssertThrow(pgSysMgr::setInitialDirectoryForSystem(-1, dummy), pgSysMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSysMgr::setInitialDirectoryForSystem(1, NULL), pgSysMgr::ExceptionInvalidArgument);

        pgSysMgr::setInitialDirectoryForSystem(0, NULL);
    }
}
