echo off

rem Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009 Mark Aylett <mark.aylett@gmail.com>
rem
rem This file is part of Aug written by Mark Aylett.
rem
rem Aug is released under the GPL with the additional exemption that compiling,
rem linking, and/or using OpenSSL is allowed.
rem
rem Aug is free software; you can redistribute it and/or modify it under the
rem terms of the GNU General Public License as published by the Free Software
rem Foundation; either version 2 of the License, or (at your option) any later
rem version.
rem
rem Aug is distributed in the hope that it will be useful, but WITHOUT ANY
rem WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
rem FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
rem details.
rem
rem You should have received a copy of the GNU General Public License along with
rem this program; if not, write to the Free Software Foundation, Inc., 51
rem Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

call "%VS90COMNTOOLS%vsvars32.bat"

set PATH=%OPENSSL_HOME%\bin;%PATH%
set INCLUDE=%OPENSSL_HOME%\include;%INCLUDE%
set LIB=%OPENSSL_HOME%\lib\VC;%LIB%

set PATH=%PYTHON_HOME%;%PATH%
set INCLUDE=%PYTHON_HOME%\include;%INCLUDE%
set LIB=%PYTHON_HOME%\libs;%LIB%

set PATH=%RUBY_HOME%\bin;%PATH%
set INCLUDE=%RUBY_HOME%\lib\ruby\1.8\i386-mswin32;%INCLUDE%
set LIB=%RUBY_HOME%\lib;%LIB%

vcexpress.exe aug.sln /useenv


