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

#include "KdwpCmd_Method_LineTable.hpp"
#include "Constant.hpp"
#include "../Exception.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_Method_LineTable::dump_all_lines() const
  {
    std::cout << "There are " << std::dec << m_line_count << " line"
              << ((m_line_count <= 1) ? "" : "s") << " in this method." << std::endl;
    
    std::cout << "Line" << ((m_line_count <= 1) ? "" : "s")
              << " information " << ((m_line_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<LineInfo>::const_iterator iter = m_lines.begin();
         iter != m_lines.end();
         ++iter)
    {
      std::cout << std::dec << "Opcode index: " << iter->m_code_index << " ";
      std::cout << std::dec << "Line number: " << iter->m_line_number << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  int32_t
  KdwpCmd_Method_LineTable::get_line_number(uint32_t const offset) const
  {
    std::vector<LineInfo>::const_iterator iter;
    
    for (iter = m_lines.begin(); iter != m_lines.end(); ++iter)
    {
      if (iter->m_code_index >= offset)
      {
        break;
      }
    }
    
    if (iter == m_lines.end())
    {
      return -1;
    }
    else
    {
      return iter->m_line_number;
    }
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_Method_LineTable &cmd)
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
        socket >> cmd.m_start;
        socket >> cmd.m_end;
        socket >> cmd.m_line_count;
        
        KdwpCmd_Method_LineTable::LineInfo li;
        
        for (uint32_t i = 0; i < cmd.m_line_count; ++i)
        {
          socket >> li.m_code_index;
          socket >> li.m_line_number;
          
          cmd.m_lines.push_back(li);
        }
      }
      break;
      
    case ERROR_INVALID_METHOD_ID:
      throw KdwpErrorInvalidMethodId();
      
    case ERROR_NOT_FOUND:
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
