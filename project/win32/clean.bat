::
::  Copyright (c) 2007-2010 Takashi Kitao
::
::  Permission is hereby granted, free of charge, to any person obtaining a copy
::  of this software and associated documentation files (the "Software"), to deal
::  in the Software without restriction, including without limitation the rights
::  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
::  copies of the Software, and to permit persons to whom the Software is
::  furnished to do so, subject to the following conditions:
::
::  The above copyright notice and this permission notice shall be included in
::  all copies or substantial portions of the Software.
::
::  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
::  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
::  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
::  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
::  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
::  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
::  THE SOFTWARE.
::


%~d0
cd %~p0\..\..

del /s /q *.o *.ncb *.user log
del /s /q /ah *.suo Thumbs.db

cd doc\
rmdir /s /q .\reference\html
cd ..

cd project\win32\catcake
rmdir /s /q .\debug
rmdir /s /q .\release
rmdir /s /q catcake\Debug
rmdir /s /q catcake\Release
rmdir /s /q test\Debug
rmdir /s /q test\Release
del /q  ..\..\..\lib\*
cd ..\..\..

cd sample\project\win32\sample
rmdir /s /q .\debug
rmdir /s /q .\release
rmdir /s /q sample01_debug\Debug
rmdir /s /q sample01_debug\Release
rmdir /s /q sample02_primitive\Debug
rmdir /s /q sample02_primitive\Release
rmdir /s /q sample03_sprite\Debug
rmdir /s /q sample03_sprite\Release
rmdir /s /q sample04_font\Debug
rmdir /s /q sample04_font\Release
rmdir /s /q sample05_screen\Debug
rmdir /s /q sample05_screen\Release
rmdir /s /q sample06_sound\Debug
rmdir /s /q sample06_sound\Release
rmdir /s /q sample07_shader\Debug
rmdir /s /q sample07_shader\Release
rmdir /s /q sample08_light\Debug
rmdir /s /q sample08_light\Release
rmdir /s /q sample09_collision\Debug
rmdir /s /q sample09_collision\Release
rmdir /s /q sample10_config\Debug
rmdir /s /q sample10_config\Release
rmdir /s /q sample11_model\Debug
rmdir /s /q sample11_model\Release
rmdir /s /q sample12_motion\Debug
rmdir /s /q sample12_motion\Release
del /q ..\icon\*.aps
del /q  ..\..\..\bin\*
cd ..\..\..\..
