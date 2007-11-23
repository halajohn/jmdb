// Jmdb - A J2ME debugger with Emacs interface
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef Constant_hpp
#define Constant_hpp

#include <memory>
#include <string>

#include "../Type.hpp"

namespace jmdb
{
  enum ErrorCode
  {
    ERROR_NONE                          =   0,
    ERROR_INVALID_THREAD                =  10,
    ERROR_INVALID_THREAD_GROUP          =  11,
    ERROR_INVALID_PRIORITY              =  12,
    ERROR_THREAD_NOT_SUSPENDED          =  13,
    ERROR_THREAD_SUSPENDED              =  14,
    ERROR_INVALID_OBJECT                =  20,
    ERROR_INVALID_CLASS                 =  21,
    ERROR_CLASS_NOT_PREPARED            =  22,
    ERROR_INVALID_METHOD_ID             =  23,
    ERROR_INVALID_LOCATION              =  24,
    ERROR_INVALID_FIELD_ID              =  25,
    ERROR_INVALID_FRAME_ID              =  30,
    ERROR_NO_MORE_FRAMES                =  31,
    ERROR_OPAQUE_FRAME                  =  32,
    ERROR_NOT_CURRENT_FRAME             =  33,
    ERROR_TYPE_MISMATCH                 =  34,
    ERROR_INVALID_SLOT                  =  35,
    ERROR_DUPLICATE                     =  40,
    ERROR_NOT_FOUND                     =  41,
    ERROR_INVALID_MONITOR               =  50,
    ERROR_NOT_MONITOR_OWNER             =  51,
    ERROR_INTERRUPT                     =  52,
    ERROR_INVALID_CLASS_FORMAT          =  60,
    ERROR_CIRCULAR_CLASS_DEFINITION     =  61,
    ERROR_FAILS_VERIFICATION            =  62,
    ERROR_ADD_METHOD_NOT_IMPLEMENTED    =  63,
    ERROR_SCHEMA_CHANGE_NOT_IMPLEMENTED =  64,
    ERROR_INVALID_TYPESTATE             =  65,
    ERROR_NOT_IMPLEMENTED               =  99,
    ERROR_NULL_POINTER                  = 100,
    ERROR_ABSENT_INFORMATION            = 101,
    ERROR_INVALID_EVENT_TYPE            = 102,
    ERROR_ILLEGAL_ARGUMENT              = 103,
    ERROR_OUT_OF_MEMORY                 = 110,
    ERROR_ACCESS_DENIED                 = 111,
    ERROR_VM_DEAD                       = 112,
    ERROR_INTERNAL                      = 113,
    ERROR_UNATTACHED_THREAD             = 115,
    ERROR_INVALID_TAG                   = 500,
    ERROR_ALREADY_INVOKING              = 502,
    ERROR_INVALID_INDEX                 = 503,
    ERROR_INVALID_LENGTH                = 504,
    ERROR_INVALID_STRING                = 506,
    ERROR_INVALID_CLASS_LOADER          = 507,
    ERROR_INVALID_ARRAY                 = 508,
    ERROR_TRANSPORT_LOAD                = 509,
    ERROR_TRANSPORT_INIT                = 510,
    ERROR_NATIVE_METHOD                 = 511,
    ERROR_INVALID_COUNT                 = 512
  };
  typedef enum ErrorCode ErrorCode;
  
  enum EventKindEnum
  {
    EVENT_SINGLE_STEP        = 1,
    EVENT_BREAKPOINT         = 2,
    EVENT_FRAME_POP          = 3,
    EVENT_EXCEPTION          = 4,
    EVENT_USER_DEFINED       = 5,
    EVENT_THREAD_START       = 6,
    EVENT_THREAD_END         = 7,
    EVENT_CLASS_PREPARE      = 8,
    EVENT_CLASS_UNLOAD       = 9,
    EVENT_CLASS_LOAD         = 10,
    EVENT_FIELD_ACCESS       = 20,
    EVENT_FIELD_MODIFICATION = 21,
    EVENT_EXCEPTION_CATCH    = 30,
    EVENT_METHOD_ENTRY       = 40,
    EVENT_METHOD_EXIT        = 41,
    EVENT_VM_START           = 90,
    EVENT_VM_DEATH           = 99
  };
  typedef enum EventKindEnum EventKindEnum;
  
  enum StepSizeEnum
  {
    SS_MIN = 0,
    SS_LINE = 1
  };
  typedef enum StepSizeEnum StepSizeEnum;
  
  enum StepDepthEnum
  {
    SD_INTO = 0,
    SD_OVER = 1,
    SD_OUT = 2
  };
  typedef enum StepDepthEnum StepDepthEnum;
  
  enum ThreadStatus
  {
    // :NOTE: Wei 2004-Jun-12:
    //
    // Althought JDWP doesn't define this UNKNOWN value,
    // however, KVM indeed transfer this value to me.
    UNKNOWN = -1,
    
    ZOMBIE = 0,
    RUNNING = 1,
    SLEEPING = 2,
    MONITOR = 3,
    WAIT = 4
  };
  typedef enum ThreadStatus ThreadStatus;
  
  enum SuspendStatus
  {
    SUSPENDED = 1
  };
  typedef enum SuspendStatus SuspendStatus;
  
  enum AccessFlags
  {
    ACC_PUBLIC       = 0x0001,
    ACC_PRIVATE      = 0x0002,
    ACC_PROTECTED    = 0x0004,
    ACC_STATIC       = 0x0008,
    ACC_FINAL        = 0x0010,
    ACC_SYNCHRONIZED = 0x0020,
    ACC_VOLATILE     = 0x0040,
    ACC_TRANSIENT    = 0x0080,
    ACC_NATIVE       = 0x0100,
    ACC_INTERFACE    = 0x0200,
    ACC_ABSTRACT     = 0x0400
  };
  typedef enum AccessFlags AccessFlags;
  
  std::auto_ptr<std::string> get_access_flag_string(uint32_t const access_flags);
  
  enum ClassTypeEnum
  {
    CT_CLASS = 1,
    CT_INTERFACE = 2,
    CT_ARRAY = 3
  };
  typedef enum ClassTypeEnum ClassTypeEnum;
  
  std::auto_ptr<std::string> class_type_to_string(ClassTypeEnum const type);
}

#endif
