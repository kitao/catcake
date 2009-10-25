#!/bin/sh
#
#   Copyright (c) 2007-2009 Takashi Kitao
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#   `  notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#   `  notice, this list of conditions and the following disclaimer in the
#   `  documentation and/or other materials provided with the distribution.
#
#   3. The name of the author may not be used to endorse or promote products
#   `  derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


target_dir=`dirname $1`
target_name=`basename $1`
target_app=$target_name.app
template_dir=`dirname $0`
template_app=packapp.app


if [ $# -eq 1 -a -e $target_dir/$target_name -a -e $template_dir/$TEMPLATE_NAME ]
then
    rm -rf $target_dir/$target_app
    cp -rf $template_dir/$template_app $target_dir/$target_app
    cp -f $target_dir/$target_name $target_dir/$target_app
    sed -e "s/TARGET_NAME/$target_name/g" $target_dir/$target_app/Info.plist > $target_dir/$target_app/Info.plist~
    mv -f $target_dir/$target_app/Info.plist~ $target_dir/$target_app/Info.plist
    rm -rf $target_dir/$target_app/.svn
else
    echo 'Usage: packapp FILE'
    echo 'Make the app package to execute the FILE on iPhone.'
fi
