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

#ifndef KdwpCmd_EventRequest_hpp
#define KdwpCmd_EventRequest_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_EventRequest KdwpCmd_EventRequest;
  struct KdwpCmd_EventRequest : public PacketHeader
  {
    enum Cmd
    {
      SET = 1,
      CLEAR = 2,
      CLEAR_ALL_BREAKPOINTS = 3
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_EventRequest(Flags const flags,
                         Cmd const cmd,
                         uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     EVENT_REQUEST,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_EventRequest() = 0;
  };
}

#endif
