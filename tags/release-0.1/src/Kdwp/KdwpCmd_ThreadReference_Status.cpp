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

#include <iostream>
#include <cassert>

#include "KdwpCmd_ThreadReference_Status.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Operation ==================================
  
  std::auto_ptr<std::string>
  KdwpCmd_ThreadReference_Status::get_thread_status_string()
  {
    std::auto_ptr<std::string> mp_status_string(new std::string);
    
    switch (m_thread_status)
    {
    case ZOMBIE:
      *mp_status_string = "Dead";
      break;
      
    case RUNNING:
      *mp_status_string = "Running";
      break;
      
    case SLEEPING:
      // :NOTE: Wei 2004-Jun-14:
      //
      // KVM doesn't have this kind of thread status.
      assert(1 != 1);
      break;
      
    case MONITOR:
      *mp_status_string = "Monitor";
      break;
      
    case WAIT:
      *mp_status_string = "Wait";
      break;
      
    case UNKNOWN:
      *mp_status_string = "Unknown";
      break;
      
    default:
      std::cerr << m_thread_status << std::endl;
      assert(1 != 1);
      break;
    }
    
    return mp_status_string;
  }
  
  std::auto_ptr<std::string>
  KdwpCmd_ThreadReference_Status::get_suspend_status_string()
  {
    std::auto_ptr<std::string> mp_status_string(new std::string);
    
    switch (m_suspend_status)
    {
    case SUSPENDED:
      *mp_status_string = "Suspended";
      break;
      
    default:
      break;
    }
    
    return mp_status_string;
  }
  
  //=================================== Friend ================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Status &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_thread_id;
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      socket >> cmd.m_thread_status;
      socket >> cmd.m_suspend_status;
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
