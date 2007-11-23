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

#ifndef KdwpCmd_Method_hpp
#define KdwpCmd_Method_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_Method KdwpCmd_Method;
  struct KdwpCmd_Method : public PacketHeader
  {
    enum Cmd
    {
      LINE_TABLE = 1,
      VARIABLE_TABLE = 2,
      BYTECODES = 3,
      IS_OBSOLETE = 4
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_Method(Flags const flags,
                   Cmd const cmd,
                   uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     METHOD,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_Method() = 0;
  };
}

#endif
