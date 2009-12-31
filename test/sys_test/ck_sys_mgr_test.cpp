/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#include "test.h"


static u32 s_thread_func_value;
static void* s_mh;


static void threadFuncTest(void* user_param)
{
    ckAssert(user_param == s_mh);

    ckSysMgr::lockMutex(s_mh);

    ckAssert(s_thread_func_value == 1);

    ckSysMgr::unlockMutex(s_mh);

    ckSysMgr::sleepUsec(10000);

    s_thread_func_value = 2;
}


void ckSysMgrTest()
{
    /*
        static void printf(const char* str, ...)
        static void sprintf(char* buf, u32 buf_size, const char* str, ...)
    */
    {
        ckSysMgr::printf("Printf test %x\n", 65535);

        char buf[10];
        ckSysMgr::sprintf(buf, 10, "%d %3.1f", 123, 456.0f);

        ckStr<char, 10> str = buf;
        ckAssert(str == "123 456.0");

        ckAssertThrow(ckSysMgr::printf(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::sprintf(NULL, 10, "dummy"), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::sprintf(buf, 0, "dummy"), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::sprintf(buf, 10, NULL), ckSysMgr::ExceptionInvalidArgument);
    }

    /*
        static void wprintf(const wchar_t* str, ...)
        static void swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...)
    */
    {
        ckSysMgr::wprintf(L"Printf test %x\n", 65535);

        wchar_t buf[10];
        ckSysMgr::swprintf(buf, 10, L"%d %3.1f", 123, 456.0f);

        ckStr<wchar_t, 10> str = buf;
        ckAssert(str == L"123 456.0");

        ckAssertThrow(ckSysMgr::wprintf(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::swprintf(NULL, 10, L"dummy"), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::swprintf(buf, 0, L"dummy"), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::swprintf(buf, 10, NULL), ckSysMgr::ExceptionInvalidArgument);
    }

    /*
        static void* openFile(const char* filename, FileMode file_mode)
        static u32 getFileSize(void* file_handler)
        static void readFile(void* buf, u32 offset, u32 size, void* file_handler)
        static void writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
        static void closeFile(void* file_handler)
    */
    {
        void* fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data1.dat", ckSysMgr::FILE_MODE_READ);
        ckAssert(fh);
        ckAssert(ckSysMgr::getFileSize(fh) == 10);

        u8 buf[10];
        ckMemMgr::memset(buf, 255, 10);

        ckSysMgr::readFile(buf, 0, 3, fh);
        ckAssert(buf[0] == '0' && buf[1] == '1' && buf[2] == '2' && buf[3] == 255);

        ckSysMgr::readFile(buf, 5, 2, fh);
        ckAssert(buf[0] == '5' && buf[1] == '6' && buf[2] == '2' && buf[3] == 255);

        ckSysMgr::readFile(buf, 8, 2, fh);
        ckAssert(buf[0] == '8' && buf[1] == '9' && buf[2] == '2' && buf[3] == 255);

        ckAssertThrow(ckSysMgr::readFile(buf, 8, 4, fh), ckSysMgr::ExceptionCannotReadFile);
        ckAssertThrow(ckSysMgr::writeFile(0, buf, 3, fh), ckSysMgr::ExceptionCannotWriteFile);

        ckSysMgr::closeFile(fh);

        ckAssertThrow(ckSysMgr::openFile(TEST_DATA_DIR "dummy.dat", ckSysMgr::FILE_MODE_READ), ckSysMgr::ExceptionCannotOpenFile);

        fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", ckSysMgr::FILE_MODE_WRITE);
        ckSysMgr::closeFile(fh);

        fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", ckSysMgr::FILE_MODE_READ);
        ckAssert(ckSysMgr::getFileSize(fh) == 0);

        ckAssertThrow(ckSysMgr::getFileSize(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::readFile(buf, 0, 1, fh), ckSysMgr::ExceptionCannotReadFile);

        ckSysMgr::closeFile(fh);

        fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", ckSysMgr::FILE_MODE_WRITE);

        buf[0] = 1;
        buf[1] = 2;
        buf[2] = 3;

        ckSysMgr::writeFile(0, buf, 3, fh);
        ckSysMgr::writeFile(2, buf, 3, fh);

        ckAssertThrow(ckSysMgr::writeFile(0, NULL, 3, fh), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::readFile(buf, 0, 3, fh), ckSysMgr::ExceptionCannotReadFile);

        ckSysMgr::closeFile(fh);

        fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", ckSysMgr::FILE_MODE_READ);
        ckAssert(ckSysMgr::getFileSize(fh) == 5);

        ckSysMgr::readFile(buf, 0, 5, fh);
        ckAssert(buf[0] = 1 && buf[1] == 2 && buf[2] == 1 && buf[3] == 2 && buf[4] == 3);

        ckSysMgr::closeFile(fh);

        fh = ckSysMgr::openFile(TEST_DATA_DIR "test_data2.dat", ckSysMgr::FILE_MODE_WRITE);
        ckSysMgr::closeFile(fh);

        fh = reinterpret_cast<void*>(1);

        ckAssertThrow(ckSysMgr::openFile(NULL, ckSysMgr::FILE_MODE_READ), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::readFile(NULL, 0, 4, fh), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::readFile(NULL, 0, 0, fh), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::readFile(buf, 0, 4, NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::writeFile(0, buf, 0, fh), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::writeFile(0, buf, 3, NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::closeFile(NULL), ckSysMgr::ExceptionInvalidArgument);
    }

    /*
        static u64 getUsecTime()
        static void sleepUsec(u64 usec)
    */
    {
        u64 usec = ckSysMgr::getUsecTime();
        ckAssert(usec > 0);

        ckSysMgr::sleepUsec(1000 * 100);
        u64 elapsed = ckSysMgr::getUsecTime() - usec;
        ckAssert(elapsed > 1000 * 50 && elapsed < 1000 * 150);
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

        s_mh = ckSysMgr::newMutex();

        ckSysMgr::lockMutex(s_mh);

        void* th = ckSysMgr::newThread(threadFuncTest, s_mh);

        s_thread_func_value = 1;

        ckSysMgr::unlockMutex(s_mh);

        ckSysMgr::joinThread(th);

        ckAssert(s_thread_func_value == 2);

        ckSysMgr::deleteThread(th);
        ckSysMgr::deleteMutex(s_mh);

        ckAssertThrow(ckSysMgr::newThread(NULL, NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::deleteThread(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::joinThread(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::deleteMutex(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::lockMutex(NULL), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::unlockMutex(NULL), ckSysMgr::ExceptionInvalidArgument);
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
        ckAssert(!ckSysMgr::isCreated());

        ckAssertThrow(ckSysMgr::getFramebufferWidth(), ckSysMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSysMgr::getFramebufferHeight(), ckSysMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSysMgr::isFramebufferSizeChanged(), ckSysMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSysMgr::isFullScreen(), ckSysMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSysMgr::toggleFullScreen(0, 0), ckSysMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSysMgr::updateForSystem(), ckSysMgr::ExceptionNotInitialized);

        ckAssertThrow(ckSysMgr::createAfterMem(NULL, 100, 100, 0), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::createAfterMem("test", 0, 100, 0), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::createAfterMem("test", 100, 0, 0), ckSysMgr::ExceptionInvalidArgument);

        ckSysMgr::createAfterMem("test1", 160, 170, 0);

        ckAssert(ckSysMgr::isCreated());
        ckAssert(ckSysMgr::getFramebufferWidth() == 160 && ckSysMgr::getFramebufferHeight() == 170);
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());
        ckAssert(!ckSysMgr::isFullScreen());

        ckSysMgr::updateForSystem();
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckSysMgr::createAfterMem("test2", 640, 480, ckSysMgr::FLAG_FULLSCREEN_START);

        ckAssert(ckSysMgr::isCreated());
        ckAssert(ckSysMgr::getFramebufferWidth() >= 640 && ckSysMgr::getFramebufferHeight() >= 480);
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());
        ckAssert(ckSysMgr::isFullScreen());

        ckSysMgr::updateForSystem();
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckSysMgr::toggleFullScreen(640, 480);
        ckAssert(ckSysMgr::getFramebufferWidth() == 640 && ckSysMgr::getFramebufferHeight() == 480);
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());
        ckAssert(!ckSysMgr::isFullScreen());

        ckSysMgr::updateForSystem();
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckSysMgr::toggleFullScreen(800, 600);
        ckAssert(ckSysMgr::getFramebufferWidth() >= 800 && ckSysMgr::getFramebufferHeight() >= 600);
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());
        ckAssert(ckSysMgr::isFullScreen());

        ckSysMgr::updateForSystem();
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckAssertThrow(ckSysMgr::toggleFullScreen(0, 100), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::toggleFullScreen(100, 0), ckSysMgr::ExceptionInvalidArgument);

        ckSysMgr::createAfterMem("test3", 234, 345, ckSysMgr::FLAG_VARIABLE_SIZE | ckSysMgr::FLAG_DISABLE_SHADER);

        ckAssert(ckSysMgr::isCreated());
        ckAssert(ckSysMgr::getFramebufferWidth() == 234 && ckSysMgr::getFramebufferHeight() == 345);
        ckAssert(!ckSysMgr::isFullScreen());
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckSysMgr::updateForSystem();
        ckAssert(!ckSysMgr::isFramebufferSizeChanged());

        ckSysMgr::destroyBeforeMem();
        ckSysMgr::destroyBeforeMem();

        ckAssert(!ckSysMgr::isCreated());
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

        ckSysMgr::readLittleEndianForSystem(&val1, data1, sizeof(u32));
        ckAssert(val1 == 0x12345678);

        ckSysMgr::readLittleEndianForSystem(&val2, data2, sizeof(u16));
        ckAssert(val2 == 0xabcd);

        ckAssert(ckSysMgr::readLittleEndianForSystem<u32>(data1) == 0x12345678);
        ckAssert(ckSysMgr::readLittleEndianForSystem<u16>(data2) == 0xabcd);

        u8 data3[4], data4[2];

        s32 val3 = -123456;
        s16 val4 = -789;

        ckSysMgr::writeLittleEndianForSystem(data3, &val3, sizeof(s32));
        ckAssert(ckSysMgr::readLittleEndianForSystem<s32>(data3) == val3);

        ckSysMgr::writeLittleEndianForSystem(data4, &val4, sizeof(s16));
        ckAssert(ckSysMgr::readLittleEndianForSystem<s16>(data4) == val4);

        ckSysMgr::writeLittleEndianForSystem<s16>(data3, -4321);
        ckAssert(ckSysMgr::readLittleEndianForSystem<s16>(data3) == -4321);

        ckSysMgr::writeLittleEndianForSystem<s8>(data4, -21);
        ckAssert(ckSysMgr::readLittleEndianForSystem<s8>(data4) == -21);
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

        ckAssertThrow(ckSysMgr::setInitialDirectoryForSystem(-1, dummy), ckSysMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSysMgr::setInitialDirectoryForSystem(1, NULL), ckSysMgr::ExceptionInvalidArgument);

        ckSysMgr::setInitialDirectoryForSystem(0, NULL);
    }
}
