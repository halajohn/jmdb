# Jmdb - A J2ME debugger with Emacs interface
# Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

include $(BUILD_ROOT_DIR)/options.mak

CC     = /usr/bin/gcc
CXX    = /usr/bin/g++
ECHO   = /bin/echo
RM     = /bin/rm
CP     = /bin/cp
SED    = /bin/sed
MKDIR  = /bin/mkdir
STRIP  = /usr/bin/strip
MSGFMT = /usr/bin/msgfmt
TOUCH  = /usr/bin/touch

###############################################################################

BUILD_SRC_DIR  = $(BUILD_ROOT_DIR)/src \
		 $(BUILD_ROOT_DIR)/src/DbgCmd \
		 $(BUILD_ROOT_DIR)/src/Kdwp \

BUILD_OBJ_DIR  = $(BUILD_ROOT_DIR)/obj
BUILD_BIN_DIR  = $(BUILD_ROOT_DIR)/bin
BUILD_I18N_DIR = $(BUILD_ROOT_DIR)/i18n

###############################################################################

INSTALL_BIN_DIR    = $(INSTALL_PREFIX)/bin
INSTALL_I18N_DIR   = $(INSTALL_PREFIX)/share/locale

###############################################################################

PACKAGE_I18N_FILE = jmdb.mo
PACKAGE_EXEC_FILE = $(BUILD_BIN_DIR)/jmdb

###############################################################################

CFLAGS = -march=pentium3 -mmmx -msse2 -mfpmath=sse -Wall -W -Wundef -Wwrite-strings -Wredundant-decls -Winline -Wpointer-arith -Woverloaded-virtual -Wconversion `pkg-config gtkmm-2.4 --cflags`

ifeq ($(DEBUG_MODE), true)
CFLAGS += -g3 -gdwarf-2
else
CFLAGS += -O3 -fomit-frame-pointer -finline-functions -funroll-loops -fexpensive-optimizations -fschedule-insns2 -malign-double -fforce-addr -fprefetch-loop-arrays -DNDEBUG
endif

LDFLAGS = 

###############################################################################

KDWP_OBJFILES = $(BUILD_OBJ_DIR)/KdwpCmd_KVM.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_KVM_HandShake.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_VM.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_VM_AllClasses.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_VM_AllThreads.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_VM_Suspend.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_VM_Resume.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_StackFrame.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_StackFrame_GetValues.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_StackFrame_ThisObject.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_Name.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_Frames.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_Status.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_ThreadGroup.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_Resume.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ThreadReference_Suspend.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_EventRequest.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_EventRequest_Set.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_EventRequest_Clear.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ObjectReference.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ObjectReference_GetValues.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType_Methods.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType_GetValues.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType_Fields.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType_SourceFile.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_ReferenceType_Signature.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_Method.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_Method_LineTable.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_Method_VariableTable.o \
		$(BUILD_OBJ_DIR)/KdwpCmd_Event_Composite.o \
	   	$(BUILD_OBJ_DIR)/PacketHeader.o \
		$(BUILD_OBJ_DIR)/Value.o \
		$(BUILD_OBJ_DIR)/Location.o \
		$(BUILD_OBJ_DIR)/Constant.o

DBGCMD_OBJFILES = $(BUILD_OBJ_DIR)/DbgCmd.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Top.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Break.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Continue.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Next.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Quit.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Up.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Down.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Print.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Help.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Class.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Thread.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Method.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Field.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_LineTable.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Variable.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Info_Frame.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Thread.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Thread_Resume.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Thread_Resume_All.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Thread_Suspend.o \
		  $(BUILD_OBJ_DIR)/DbgCmd_Thread_Suspend_All.o

COMMON_OBJFILES = $(BUILD_OBJ_DIR)/ClientSocket.o \
	   	  $(BUILD_OBJ_DIR)/Main.o \
	   	  $(BUILD_OBJ_DIR)/Debugger.o \
	   	  $(BUILD_OBJ_DIR)/History.o \
	   	  $(BUILD_OBJ_DIR)/SrcFiles.o \
	   	  $(BUILD_OBJ_DIR)/Utils.o

OBJFILES = $(COMMON_OBJFILES) $(DBGCMD_OBJFILES) $(KDWP_OBJFILES)
