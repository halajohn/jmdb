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

#ifndef KdwpCmd_StackFrame_GetValues_hpp
#define KdwpCmd_StackFrame_GetValues_hpp

#include <vector>

#include "KdwpCmd_StackFrame.hpp"
#include "Value.hpp"

namespace jmdb
{
  typedef class KdwpCmd_StackFrame_GetValues KdwpCmd_StackFrame_GetValues;
  class KdwpCmd_StackFrame_GetValues : public KdwpCmd_StackFrame
  {
  private:
    
    struct VariableInfo
    {
      uint32_t m_index;
      uint8_t m_tag;
    };
    typedef struct VariableInfo VariableInfo;
    
    // Out data
    
    uint32_t m_thread_id;
    uint32_t m_frame_id;
    uint32_t m_wanted_count;
    
    std::vector<VariableInfo> m_wanted_variables;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_StackFrame_GetValues::m_thread_id)) +
                                               sizeof(get_size(&KdwpCmd_StackFrame_GetValues::m_frame_id)) +
                                               sizeof(get_size(&KdwpCmd_StackFrame_GetValues::m_wanted_count)));
    
    // Reply data
    
    uint32_t m_return_count;
    std::vector<ValueBase *> m_return_variables;
    
  public:
    
    // Life cycle
    
    KdwpCmd_StackFrame_GetValues(uint32_t const thread_id, uint32_t const frame_id)
      : KdwpCmd_StackFrame(FLAG_NONE, GET_VALUES, EXTRA_HEADER_SIZE),
        m_thread_id(thread_id),
        m_frame_id(frame_id),
        m_wanted_count(0)
    { }
    
    ~KdwpCmd_StackFrame_GetValues();
    
    // Operation
    
    void
    add_wanted_variable(uint32_t const index, uint8_t const tag)
    {
      VariableInfo vi;
      
      vi.m_index = index;
      vi.m_tag = tag;
      
      m_wanted_variables.push_back(vi);
      
      ++m_wanted_count;
      
      add_extra_packet_size(sizeof(get_size(&KdwpCmd_StackFrame_GetValues::VariableInfo::m_index)) +
                            sizeof(get_size(&KdwpCmd_StackFrame_GetValues::VariableInfo::m_tag)));
    }
    
    uint64_t
    get_variable_value(uint32_t const index)
    {
      return (m_return_variables[index])->get_value();
    }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_StackFrame_GetValues &cmd);
  };
}

#endif
