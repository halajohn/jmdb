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

#include "KdwpCmd_ReferenceType_Fields.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_ReferenceType_Fields::dump_all_fields() const
  {
    std::cout << "There are " << std::dec << m_field_count << " field"
              << ((m_field_count <= 1) ? "" : "s") << " in this class (or interface)." << std::endl;
    
    std::cout << "Field" << ((m_field_count <= 1) ? "" : "s")
              << " information " << ((m_field_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<FieldInfo>::const_iterator iter = m_fields.begin();
         iter != m_fields.end();
         ++iter)
    {
      std::cout << std::dec << iter->m_id << " ";
      std::cout << iter->m_name << " ";
      std::cout << iter->m_signature << " ";
      std::cout << *(get_access_flag_string(iter->m_access_flags)) << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  KdwpCmd_ReferenceType_Fields::FieldInfo const *
  KdwpCmd_ReferenceType_Fields::get_field_info(std::string const &name) const
  {
    for (std::vector<FieldInfo>::const_iterator iter = m_fields.begin();
         iter != m_fields.end();
         ++iter)
    {
      if (iter->m_name == name)
      {
        return &(*iter);
      }
    }
    
    // not found.
    return 0;
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_ReferenceType_Fields &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_reference_id;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      {
        KdwpCmd_ReferenceType_Fields::FieldInfo fi;
        
        socket >> cmd.m_field_count;
        
        for (uint32_t i = 0; i < cmd.m_field_count; ++i)
        {
          socket >> fi.m_id;
          socket >> fi.m_name;
          socket >> fi.m_signature;
          socket >> fi.m_access_flags;
          
          cmd.m_fields.push_back(fi);
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
