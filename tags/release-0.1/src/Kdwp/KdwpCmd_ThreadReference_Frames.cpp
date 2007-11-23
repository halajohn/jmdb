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

#include "KdwpCmd_ThreadReference_Frames.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_ThreadReference_Frames::dump_all_frames() const
  {
    std::cout << "There are " << std::dec << m_frame_count << " frame"
              << ((m_frame_count <= 1) ? "" : "s") << " in this thread." << std::endl;
    
    std::cout << "Frame" << ((m_frame_count <= 1) ? "" : "s")
              << " information " << ((m_frame_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<FrameInfo>::const_iterator iter = m_frames.begin();
         iter != m_frames.end();
         ++iter)
    {
      std::cout << std::dec << "id: " << iter->m_id << ", ";
      (iter->m_location).dump_to_console();
      std::cout << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  int32_t
  KdwpCmd_ThreadReference_Frames::get_last_frame_id() const
  {
    if (0 == m_frames.size())
    {
      return -1;
    }
    else
    {
      return (m_frames.begin())->m_id;
    }
  }
  
  KdwpCmd_ThreadReference_Frames::FrameInfo const &
  KdwpCmd_ThreadReference_Frames::get_frame_info(uint32_t const frame_id) const
  {
    if (frame_id < m_frames.size())
    {
      return m_frames[frame_id];
    }
    
    throw UnknownFrame();
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Frames &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_thread_id;
    socket << cmd.m_start_frame;
    socket << cmd.m_wanted_count;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      {
        socket >> cmd.m_frame_count;
        
        KdwpCmd_ThreadReference_Frames::FrameInfo fi;
        
        for (uint32_t i = 0; i < cmd.m_frame_count; ++i)
        {
          socket >> fi.m_id;
          socket >> fi.m_location;
          
          cmd.m_frames.push_back(fi);
        }
      }
      break;
      
    case ERROR_INVALID_THREAD:
      throw KdwpErrorInvalidThread();
      
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
