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

#include "DbgCmd_Next.hpp"
#include "MemFunctor.hpp"
#include "../Debugger.hpp"
#include "../Kdwp/KdwpCmd_VM_Resume.hpp"
#include "../Kdwp/KdwpCmd_EventRequest_Set.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  /////////////////////////////// Private /////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Next::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Execute to the next source line.")
              << std::endl;
    std::cout << gettext(" Syntax       : next") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Next::step_running_0arg()
  {
    KdwpCmd_EventRequest_Set kdwp_cmd;
    
    kdwp_cmd.set_running_next(mp_debugger->get_last_event_thread_id());
    
    mp_debugger->socket() << kdwp_cmd;
  }
  
  /////////////////////////////// Public //////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Next::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Next::step_running_0arg));
      
      // :NOTE: Wei 2004-Jun-17:
      //
      // The policy of jmdb is:
      //
      // When jmdb sets event request to KVM, jmdb will set the suspend policy to
      // 'All', this means when the event occurs in the KVM, all threads in the
      // KVM will be suspended.
      //
      // Thus I have to resume the execuation of all threads in the KVM before
      // listening another event generated from the KVM.
      
      KdwpCmd_VM_Resume resume_cmd;
      
      mp_debugger->socket() << resume_cmd;
      
      // listen another event.
      
      mp_debugger->listen_event();
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Next::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Next::print_help_0arg));
  }
}
