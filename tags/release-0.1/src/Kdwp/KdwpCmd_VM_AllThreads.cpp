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
#include <iostream>
#include <sstream>

#include "KdwpCmd_VM_AllThreads.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_VM_AllThreads::dump_thread_id() const
  {
    uint32_t i = 1;
    std::stringstream s;
    std::string index_string;
    
    std::cout << "There are " << std::dec << m_thread_count << " thread"
              << ((m_thread_count <= 1) ? "" : "s") << " in the KVM." << std::endl;
    
    std::cout << "Thread" << ((m_thread_count <= 1) ? "" : "s")
              << " ID " << ((m_thread_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<uint32_t>::const_iterator iter = m_thread_id.begin();
         iter != m_thread_id.end();
         ++iter)
    {
      s << i;
      s >> index_string;
      
      std::cout << std::dec << index_string << ": " << *iter << std::endl;
      
      ++i;
      
      s.clear();
      s.str("");
      index_string.clear();
    }
    
    std::cout << std::endl;
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_VM_AllThreads &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      socket >> cmd.m_thread_count;
      
      for (uint32_t i = 0; i < cmd.m_thread_count; ++i)
      {
        uint32_t id;
        
        socket >> id;
        
        cmd.m_thread_id.push_back(id);
      }
      break;
      
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
