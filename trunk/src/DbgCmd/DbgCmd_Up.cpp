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

#include "DbgCmd_Up.hpp"
#include "MemFunctor.hpp"
#include "../Debugger.hpp"
#include "../Kdwp/KdwpCmd_VM_Resume.hpp"
#include "../Kdwp/KdwpCmd_ThreadReference_Frames.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  /////////////////////////////// Private /////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Up::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Go up 1 stack frame.") << std::endl;
    std::cout << gettext(" Syntax       : up") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Up::frame_up_0arg()
  {
    try
    {
      KdwpCmd_ThreadReference_Frames kdwp_cmd(mp_debugger->get_last_event_thread_id(), 0, -1);
      
      mp_debugger->socket() << kdwp_cmd;
      
      if (mp_debugger->current_frame_id() >= (kdwp_cmd.get_frame_count() - 1))
      {
        std::cout << gettext("Already the toppest frame.") << std::endl;
      }
      else
      {
        mp_debugger->handle_filename_and_line_number(kdwp_cmd.get_frame_info(++(mp_debugger->current_frame_id())).m_location);
      }
    }
    catch (KdwpErrorInvalidThread &/* e */)
    {
      std::cout << "Invalid thread." << std::endl;
    }
  }
  
  /////////////////////////////// Public //////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Up::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Up::frame_up_0arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Up::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Up::print_help_0arg));
  }
}
