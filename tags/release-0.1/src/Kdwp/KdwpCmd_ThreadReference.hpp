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

#ifndef KdwpCmd_ThreadReference_hpp
#define KdwpCmd_ThreadReference_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_ThreadReference KdwpCmd_ThreadReference;
  struct KdwpCmd_ThreadReference : public PacketHeader
  {
    enum Cmd
    {
      NAME           = 1,
      SUSPEND        = 2,
      RESUME         = 3,
      STATUS         = 4,
      THREAD_GROUP   = 5,
      FRAMES         = 6,
      FRAME_COUNT    = 7,
      OWNED_MONITORS = 8,
      CURRENT_CONTENDED_MONITOR = 9,
      STOP           = 10,
      INTERRUPT      = 11,
      SUSPEND_COUNT  = 12
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_ThreadReference(Flags const flags,
                            Cmd const cmd,
                            uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     THREAD_REFERENCE,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_ThreadReference() = 0;
  };
}

#endif
