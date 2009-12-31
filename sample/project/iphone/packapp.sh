#!/bin/sh
#
#   Copyright (c) 2007-2010 Takashi Kitao
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#   THE SOFTWARE.
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
