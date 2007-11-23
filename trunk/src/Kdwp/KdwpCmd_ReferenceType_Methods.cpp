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
#include <string>

#include "KdwpCmd_ReferenceType_Methods.hpp"
#include "Constant.hpp"
#include "../Exception.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_ReferenceType_Methods::dump_all_methods() const
  {
    std::cout << "There are " << std::dec << m_method_count << " method"
              << ((m_method_count <= 1) ? "" : "s") << " in this class (or interface)." << std::endl;
    
    std::cout << "Method" << ((m_method_count <= 1) ? "" : "s")
              << " information " << ((m_method_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<MethodInfo>::const_iterator iter = m_methods.begin();
         iter != m_methods.end();
         ++iter)
    {
      std::cout << std::dec << iter->m_id << " ";
      std::cout << iter->m_name << " ";
      std::cout << iter->m_signature << " ";
      std::cout << *(get_access_flag_string(iter->m_access_flags)) << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  std::string const &
  KdwpCmd_ReferenceType_Methods::get_method_name(uint32_t const method_id) const
  {
    for (std::vector<MethodInfo>::const_iterator iter = m_methods.begin();
         iter != m_methods.end();
         ++iter)
    {
      if (iter->m_id == method_id)
      {
        return iter->m_name;
      }
    }
    
    throw UnknownMethod();
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_ReferenceType_Methods &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_reference_id;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      {
        KdwpCmd_ReferenceType_Methods::MethodInfo mi;
        
        socket >> cmd.m_method_count;
        
        for (uint32_t i = 0; i < cmd.m_method_count; ++i)
        {
          socket >> mi.m_id;
          socket >> mi.m_name;
          socket >> mi.m_signature;
          socket >> mi.m_access_flags;
          
          cmd.m_methods.push_back(mi);
        }
      }
      break;
      
    case ERROR_CLASS_NOT_PREPARED:
    case ERROR_INVALID_CLASS:
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
