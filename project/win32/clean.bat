::
::  Copyright (c) 2007-2009 Takashi Kitao
::  All rights reserved.
::
::  Redistribution and use in source and binary forms, with or without
::  modification, are permitted provided that the following conditions
::  are met:
::
::  1. Redistributions of source code must retain the above copyright
::  `  notice, this list of conditions and the following disclaimer.
::
::  2. Redistributions in binary form must reproduce the above copyright
::  `  notice, this list of conditions and the following disclaimer in the
::  `  documentation and/or other materials provided with the distribution.
::
::  3. The name of the author may not be used to endorse or promote products
::  `  derived from this software without specific prior written permission.
::
::  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
::  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
::  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
::  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
::  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
::  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
::  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
::  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
::  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
::  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
::


%~d0
cd %~p0\..\..

del /s /q *.ncb
del /s /q *.user
del /s /q /ah *.suo
del /s /q /ah Thumbs.db

cd doc\
rmdir /s /q .\reference\english\html
rmdir /s /q .\reference\japanese\html
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
