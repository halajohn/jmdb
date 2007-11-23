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

#include "KdwpCmd_StackFrame_GetValues.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  KdwpCmd_StackFrame_GetValues::~KdwpCmd_StackFrame_GetValues()
  {
    for (std::vector<ValueBase *>::iterator iter = m_return_variables.begin();
         iter != m_return_variables.end();
         ++iter)
    {
      delete (*iter);
    }
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_StackFrame_GetValues &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_thread_id;
    socket << cmd.m_frame_id;
    socket << cmd.m_wanted_count;
    
    for (std::vector<KdwpCmd_StackFrame_GetValues::VariableInfo>::iterator iter = cmd.m_wanted_variables.begin();
         iter != cmd.m_wanted_variables.end();
         ++iter)
    {
      socket << iter->m_index;
      socket << iter->m_tag;
    }
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      {
        uint8_t type;
        ValueBase *value;
        
        socket >> cmd.m_return_count;
        
        for (uint32_t i = 0; i < cmd.m_return_count; ++i)
        {
          socket >> type;
          
          switch (type)
          {
          case V_ARRAY:
            value = new Value<V_ARRAY>;
            break;
            
          case V_BYTE:
            value = new Value<V_BYTE>;
            break;
            
          case V_CHAR:
            value = new Value<V_CHAR>;
            break;
            
          case V_OBJECT:
            value = new Value<V_OBJECT>;
            break;
            
          case V_FLOAT:
            value = new Value<V_FLOAT>;
            break;
            
          case V_DOUBLE:
            value = new Value<V_DOUBLE>;
            break;
            
          case V_INT:
            value = new Value<V_INT>;
            break;
            
          case V_LONG:
            value = new Value<V_LONG>;
            break;
            
          case V_SHORT:
            value = new Value<V_SHORT>;
            break;
            
          case V_VOID:
            value = new Value<V_VOID>;
            break;
            
          case V_BOOLEAN:
            value = new Value<V_BOOLEAN>;
            break;
            
          case V_STRING:
            value = new Value<V_STRING>;
            break;
            
          case V_THREAD:
            value = new Value<V_THREAD>;
            break;
            
          case V_THREAD_GROUP:
            value = new Value<V_THREAD_GROUP>;
            break;
            
          case V_CLASS_LOADER:
            value = new Value<V_CLASS_LOADER>;
            break;
            
          case V_CLASS_OBJECT:
            value = new Value<V_CLASS_OBJECT>;
            break;
            
          default:
            assert(1 != 1);
            break;
          }
          
          socket >> (*value);
          
          cmd.m_return_variables.push_back(value);
        }
      }
      break;
      
    case ERROR_INVALID_THREAD:
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
