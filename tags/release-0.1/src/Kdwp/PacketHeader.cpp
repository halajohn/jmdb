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

#include <cassert>

#include "PacketHeader.hpp"

namespace jmdb
{
  PacketHeader::CommandPacket const PacketHeader::COMMAND_PACKET = PacketHeader::CommandPacket();
  PacketHeader::EventPacket const PacketHeader::EVENT_PACKET = PacketHeader::EventPacket();
  
  uint32_t PacketHeader::ID = 0;
  
  ////////////////////////////////// Public ///////////////////////////////////
  //=============================== Life cycle ================================
  
  // pure virtual destructor have to be defined.
  
  PacketHeader::~PacketHeader()
  { }
  
  //================================= Friend ==================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, PacketHeader &hdr)
  {
    assert(PacketHeader::PT_COMMAND == hdr.m_packet_type);
    
    socket << hdr.m_length;
    socket << hdr.m_id;
    socket << hdr.m_flags;
    socket << hdr.u.m_cmd_packet.m_cmdset;
    socket << hdr.u.m_cmd_packet.m_cmd;
    
    return socket;
  }
  
  ClientSocket const &
  operator>>(ClientSocket const &socket, PacketHeader &hdr)
  {
    socket >> hdr.m_length;
    socket >> hdr.m_id;
    socket >> hdr.m_flags;
    
    if (PacketHeader::PT_COMMAND == hdr.m_packet_type)
    {
      // Get reply
      socket >> hdr.u.m_reply_packet.m_error_code;
    }
    else
    {
      // Receive event
      socket >> hdr.u.m_cmd_packet.m_cmdset;
      socket >> hdr.u.m_cmd_packet.m_cmd;
    }
    
    return socket;
  }
}
