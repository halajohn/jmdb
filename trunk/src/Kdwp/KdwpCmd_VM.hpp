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

#ifndef KdwpCmd_VM_hpp
#define KdwpCmd_VM_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_VM KdwpCmd_VM;
  struct KdwpCmd_VM : public PacketHeader
  {
    enum Cmd
    {
      VERSION = 1,
      CLASSES_BY_SIGNATURE = 2,
      ALL_CLASSES = 3,
      ALL_THREADS = 4,
      TOP_LEVEL_THREAD_GROUP = 5,
      DISPOSE = 6,
      ID_SIZES = 7,
      SUSPEND = 8,
      RESUME = 9,
      exit = 10,
      CREATE_STRING = 11,
      CAPABILITIES = 12,
      CLASS_PATHS = 13,
      DISPOSE_OBJECTS = 14,
      HOLD_EVENTS = 15,
      RELEASE_EVENTS = 16
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_VM(Flags const flags,
               Cmd const cmd,
               uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     VIRTUAL_MACHINE,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_VM() = 0;
  };
}

#endif
