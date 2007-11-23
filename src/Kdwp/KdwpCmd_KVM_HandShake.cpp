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

#include "KdwpCmd_KVM_HandShake.hpp"
#include "../Exception.hpp"

namespace jmdb
{
  /////////////////////////////////// Private /////////////////////////////////
  
  std::string const KdwpCmd_KVM_HandShake::HANDSHAKE_STRING = "JDWP-Handshake";
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_KVM_HandShake &/* cmd */)
  {
    // :JDWP: Wei 2004-Jun-11:
    //
    // The handshake process has the following steps:
    //
    // * The debugger side sends 14 bytes to the VM side,
    //   consisting of the 14 ASCII characters of the string "JDWP-Handshake".
    // * The VM side replies with the same 14 bytes: JDWP-Handshake
    
    // :NOTE: Wei 2004-Jun-11:
    //
    // Althought we are about to send 'string' to the agent (or KVM),
    // however, the way to send handshake string is different from the normal way to send string.
    //
    // When we send string, we first send the length of the string, and then the content
    // of the string. However, when we send the handshake string, we don't send the string
    // length first, instead, we send the contents of the string directly.
    
    // :NOTE: Wei 2004-Jun-11:
    //
    // We don't need to send the command header for handshake.
    
    for (uint32_t i = 0; i < KdwpCmd_KVM_HandShake::HANDSHAKE_STRING.size(); ++i)
    {
      socket << KdwpCmd_KVM_HandShake::HANDSHAKE_STRING[i];
    }
    
    // has reply
    
    {
      std::string handshake_reply;
      
      for (uint32_t i = 0; i < KdwpCmd_KVM_HandShake::HANDSHAKE_STRING.size(); ++i)
      {
        uint8_t ch;
        
        socket >> ch;
        
        handshake_reply.insert(handshake_reply.end(), ch);
      }
      
      if (handshake_reply != KdwpCmd_KVM_HandShake::HANDSHAKE_STRING)
      {
        throw HandShakeError();
      }
    }
    
    return socket;
  }
}
