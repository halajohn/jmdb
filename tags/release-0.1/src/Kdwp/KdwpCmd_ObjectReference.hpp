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

#ifndef KdwpCmd_ObjectReference_hpp
#define KdwpCmd_ObjectReference_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_ObjectReference KdwpCmd_ObjectReference;
  struct KdwpCmd_ObjectReference : public PacketHeader
  {
    enum Cmd
    {
      REFERENCE_TYPE = 1,
      GET_VALUES = 2,
      SET_VALUES = 3,
      MONITOR_INFO = 5,
      INVOKE_MOTHOD = 6,
      DISABLE_COLLECTION = 7,
      ENABLE_COLLECTION = 8,
      IS_COLLECTED = 9
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_ObjectReference(Flags const flags,
                            Cmd const cmd,
                            uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     OBJECT_REFERENCE,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_ObjectReference() = 0;
  };
}

#endif
