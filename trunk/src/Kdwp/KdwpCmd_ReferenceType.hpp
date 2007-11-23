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

#ifndef KdwpCmd_ReferenceType_hpp
#define KdwpCmd_ReferenceType_hpp

#include "PacketHeader.hpp"

namespace jmdb
{
  typedef struct KdwpCmd_ReferenceType KdwpCmd_ReferenceType;
  struct KdwpCmd_ReferenceType : public PacketHeader
  {
    enum Cmd
    {
      SIGNATURE = 1,
      CLASS_LOADER = 2,
      MODIFIERS = 3,
      FIELDS = 4,
      METHODS = 5,
      GET_VALUES = 6,
      SOURCE_FILE = 7,
      NESTED_TYPES = 8,
      STATUS = 9,
      INTERFACES = 10,
      CLASS_OBJECT = 11,
      SOURCE_DEBUG_EXTENSION = 12
    };
    typedef enum Cmd Cmd;
    
    KdwpCmd_ReferenceType(Flags const flags,
                          Cmd const cmd,
                          uint32_t const extra_size)
      : PacketHeader(PacketHeader::COMMAND_PACKET,
                     flags,
                     REFERENCE_TYPE,
                     cmd,
                     extra_size)
    { }
    
    virtual ~KdwpCmd_ReferenceType() = 0;
  };
}

#endif
