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

#include "DbgCmd_Thread_Resume.hpp"
#include "DbgCmd_Thread_Resume_All.hpp"
#include "MemFunctor.hpp"
#include "../Debugger.hpp"
#include "../Kdwp/KdwpCmd_ThreadReference_Resume.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  /////////////////////////////// Private /////////////////////////////////////
  //=============================== Operation =================================
  
  void
  DbgCmd_Thread_Resume::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Thread resume operation.") << std::endl;
    std::cout << gettext(" Syntax       : thread resume <thread ID>") << std::endl;
    std::cout << gettext("                thread resume <sub-command>") << std::endl;
    std::cout << std::endl;
    std::cout << gettext(" Sub-command  : ");
    
    for (Iterator iter = mp_map->begin();
         iter != mp_map->end();
         ++iter)
    {
      std::cout << iter->second->name() << " ";
    }
    
    std::cout << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Thread_Resume::resume_thread_0arg()
  {
    throw BadCommand();
  }
  
  void
  DbgCmd_Thread_Resume::resume_thread_1arg(std::string const &thread_id_string)
  {
    try
    {
      uint32_t const thread_id = convert_to_integer(thread_id_string);
      
      KdwpCmd_ThreadReference_Resume kdwp_cmd(thread_id);
      
      mp_debugger->socket() << kdwp_cmd;
    }
    catch (KdwpErrorInvalidThread &/* e */)
    {
      std::cout << "Invalid thread." << std::endl;
    }
    catch (BadNumber &/* e */)
    {
      throw BadCommand();
    }
  }
  
  ////////////////////////////////// Public ///////////////////////////////////
  //=========================== Life cycle ====================================
  
  DbgCmd_Thread_Resume::DbgCmd_Thread_Resume(Debugger * const debugger,
                                             std::string const &name)
    : DbgCmd<HAS_SUB_MENU>(debugger, name),
      mp_all(new DbgCmd_Thread_Resume_All(debugger, "all"))
  {
    add_command_to_menu(mp_all.get());
  }
  
  //=========================== Operation =====================================
  
  void
  DbgCmd_Thread_Resume::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Thread_Resume::resume_thread_0arg),
                                         mem_fun(this, &DbgCmd_Thread_Resume::resume_thread_1arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Thread_Resume::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Thread_Resume::print_help_0arg));
  }
}
