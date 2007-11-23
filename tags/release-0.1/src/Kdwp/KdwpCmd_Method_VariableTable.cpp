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

#include "KdwpCmd_Method_VariableTable.hpp"
#include "Constant.hpp"
#include "../Exception.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_Method_VariableTable::dump_all_variables() const
  {
    std::cout << "There are " << std::dec << m_slot_count << " local variable"
              << ((m_slot_count <= 1) ? "" : "s") << " in this method." << std::endl;
    
    std::cout << "Variable" << ((m_slot_count <= 1) ? "" : "s")
              << " information " << ((m_slot_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<VariableInfo>::const_iterator iter = m_slots.begin();
         iter != m_slots.end();
         ++iter)
    {
      std::cout << std::dec << "index: " << iter->m_slot_index << ", ";
      std::cout << std::dec << "Opcode index: " << iter->m_code_index << ", ";
      std::cout << std::dec << "length: " << iter->m_length << ", ";
      std::cout << "name: " << iter->m_name << ", ";
      std::cout << "signature: " << iter->m_signature << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  KdwpCmd_Method_VariableTable::VariableInfo const *
  KdwpCmd_Method_VariableTable::get_variable_info(std::string const &name) const
  {
    for (std::vector<VariableInfo>::const_iterator iter = m_slots.begin();
         iter != m_slots.end();
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
  operator<<(ClientSocket const &socket, KdwpCmd_Method_VariableTable &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_class_id;
    socket << cmd.m_method_id;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      {
        socket >> cmd.m_arg_count;
        socket >> cmd.m_slot_count;
        
        KdwpCmd_Method_VariableTable::VariableInfo si;
        
        for (uint32_t i = 0; i < cmd.m_slot_count; ++i)
        {
          socket >> si.m_code_index;
          socket >> si.m_name;
          socket >> si.m_signature;
          socket >> si.m_length;
          socket >> si.m_slot_index;
          
          cmd.m_slots.push_back(si);
        }
      }
      break;
      
    case ERROR_INVALID_METHOD_ID:
      throw KdwpErrorInvalidMethodId();
      
    case ERROR_ABSENT_INFORMATION:
      // :NOTE: Wei 2004-Jun-19:
      //
      // For now, I throw the same exception when I receive
      // ERROR_NOT_FOUND & ERROR_ABSENT_INFORMATION.
      //
      // Because I think this 2 error mean the same thing.
      throw KdwpErrorNotFound();
      
    case ERROR_INVALID_CLASS:
    case ERROR_INVALID_OBJECT:
    case ERROR_VM_DEAD:
      assert(1 != 1);
      break;
      
    default:
      std::cerr << cmd.get_error_code() << std::endl;
      assert(1 != 1);
      break;
    }
    
    return socket;
  }
}
