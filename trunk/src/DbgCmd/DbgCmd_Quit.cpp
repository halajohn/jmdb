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

#include "DbgCmd_Quit.hpp"
#include "MemFunctor.hpp"
#include "../Debugger.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  /////////////////////////////// Private /////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Quit::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Quit jmdb.") << std::endl;
    std::cout << gettext(" Syntax       : quit") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Quit::stop_debug_0arg()
  {
    mp_debugger->set_quit_flag();
  }
  
  /////////////////////////////// Public //////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Quit::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Quit::stop_debug_0arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Quit::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Quit::print_help_0arg));
  }
}
