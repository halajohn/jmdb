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

#include "KdwpCmd_ThreadReference_Suspend.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Suspend &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_thread_id;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      break;
      
    case ERROR_INVALID_THREAD:
      throw KdwpErrorInvalidThread();
      
    case ERROR_INVALID_OBJECT:
    case ERROR_VM_DEAD:
      assert(1 != 1);
      break;
      
    default:
      assert(1 != 1);
      break;
    }
    
    return socket;
  }
}
