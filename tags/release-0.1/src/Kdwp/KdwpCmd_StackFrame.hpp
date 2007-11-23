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

#ifndef KdwpCmd_StackFrame_hpp
#define KdwpCmd_StackFrame_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_StackFrame KdwpCmd_StackFrame;
  struct KdwpCmd_StackFrame : public PacketHeader
  {
    enum Cmd
    {
      GET_VALUES = 1,
      SET_VALUES = 2,
      THIS_OBJECT = 3,
      POP_FRAMES = 4
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_StackFrame(Flags const flags,
                       Cmd const cmd,
                       uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     STACK_FRAME,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_StackFrame() = 0;
  };
}

#endif
