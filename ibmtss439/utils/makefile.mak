#################################################################################
#										#
#			Windows MinGW TPM2 Makefile				#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	      $Id: makefile.mak 438 2015-11-23 19:07:05Z kgoldman $		#
#										#
# (c) Copyright IBM Corporation 2015.						#
# 										#
# All rights reserved.								#
# 										#
# Redistribution and use in source and binary forms, with or without		#
# modification, are permitted provided that the following conditions are	#
# met:										#
# 										#
# Redistributions of source code must retain the above copyright notice,	#
# this list of conditions and the following disclaimer.				#
# 										#
# Redistributions in binary form must reproduce the above copyright		#
# notice, this list of conditions and the following disclaimer in the		#
# documentation and/or other materials provided with the distribution.		#
# 										#
# Neither the names of the IBM Corporation nor the names of its			#
# contributors may be used to endorse or promote products derived from		#
# this software without specific prior written permission.			#
# 										#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		#
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		#
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR		#
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		#
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	#
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		#
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,		#
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY		#
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		#
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE		#
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		#
#										#
#################################################################################


CC = c:/progra~1/mingw/bin/gcc.exe

CCFLAGS = 				\
	-DTPM_WINDOWS			\
	-DTPM_TSS			\
	-Ic:/progra~1/MinGW/include	\
	-Ic:/progra~1/openssl/include	\

LNFLAGS =				\
	-D_MT				\
	-DTPM_WINDOWS			\
	-Ic:/progra~1/MinGW/include	\
	-Ic:/progra~1/openssl/include	\
	-I.

LNLIBS = 	c:/progra~1/openssl/lib/mingw/libeay32.a \
		c:/progra~1/openssl/lib/mingw/ssleay32.a \
		c:/progra~1/MinGW/lib/libws2_32.a

# executable extension

EXE=.exe

# shared library

LIBTSE=libtse.dll

include makefile-common

# Uncomment for TBSI

# CCFLAGS +=	-DTPM_WINDOWS_TBSI		\
# 		-DTPM_WINDOWS_TBSI_WIN8		\
# 		-D_WIN32_WINNT=0x0600

# TSE_OBJS += tsttbsi.o 

# LNLIBS += C:\PROGRA~2\WI3CF2~1\8.0\Lib\win8\um\x86\Tbs.lib
# #LNLIBS += c:/progra~1/Micros~2/Windows/v7.1/lib/Tbs.lib

# default build target

all:	$(ALL)

# TSS library source shared with TPM

Marshal.o: 			../src/Marshal.c
				$(CC) $(CCFLAGS) ../src/Marshal.c
Unmarshal.o: 			../src/Unmarshal.c
				$(CC) $(CCFLAGS) ../src/Unmarshal.c
Commands.o: 			../src/Commands.c
				$(CC) $(CCFLAGS) ../src/Commands.c
CommandCodeAttributes.o: 	../src/CommandCodeAttributes.c
				$(CC) $(CCFLAGS) ../src/CommandCodeAttributes.c
CpriHash.o: 			../src/CpriHash.c
				$(CC) $(CCFLAGS) ../src/CpriHash.c
CpriSym.o: 			../src/CpriSym.c
				$(CC) $(CCFLAGS) ../src/CpriSym.c

# TSS shared library build

$(LIBTSE): 	$(TSE_OBJS)
		$(CC) $(LNFLAGS) -shared -o $(LIBTSE) $(TSE_OBJS) -Wl,--out-implib,libtse.a $(LNLIBS)

.PHONY:		clean
.PRECIOUS:	%.o

clean:		
		rm -f *.o  *~ 	\
		$(LIBTSE)	\
		$(ALL)

%.exe:		%.o applink.o $(LIBTSE)
		$(CC) $(LNFLAGS) -L. -ltse $< -o $@ applink.o $(LNLIBS) $(LIBTSE)

%.o:		%.c
		$(CC) $(CCFLAGS) $< -o $@
