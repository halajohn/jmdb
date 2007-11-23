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

#include "DbgCmd_Break.hpp"
#include "MemFunctor.hpp"
#include "../Debugger.hpp"
#include "../Kdwp/KdwpCmd_EventRequest_Set.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  /////////////////////////////// Private /////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Break::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Install a break point.") << std::endl;
    std::cout << gettext(" Syntax       : break <class ID> <method ID> [offset]") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Break::set_breakpoint_0arg()
  {
    throw BadCommand();
  }
  
  void
  DbgCmd_Break::set_breakpoint_1arg(std::string const &/* class_id_string */)
  {
    throw BadCommand();
  }
  
  void
  DbgCmd_Break::set_breakpoint_2arg(std::string const &class_id_string,
                                    std::string const &method_id_string)
  {
    set_breakpoint_3arg(class_id_string, method_id_string, "0");
  }
  
  void
  DbgCmd_Break::set_breakpoint_3arg(std::string const &class_id_string,
                                    std::string const &method_id_string,
                                    std::string const &offset_string)
  {
    try
    {
      uint32_t class_id = convert_to_integer(class_id_string);
      uint32_t method_id = convert_to_integer(method_id_string);
      uint32_t offset = convert_to_integer(offset_string);
      
      KdwpCmd_EventRequest_Set kdwp_cmd;
      
      kdwp_cmd.set_breakpoint(class_id, method_id, offset);
      
      mp_debugger->socket() << kdwp_cmd;
    }
    catch (BadNumber &/* e */)
    {
      throw BadCommand();
    }
  }
  
  /////////////////////////////// Public //////////////////////////////////////
  //============================= Operation ===================================
  
  void
  DbgCmd_Break::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Break::set_breakpoint_0arg),
                                         mem_fun(this, &DbgCmd_Break::set_breakpoint_1arg),
                                         mem_fun(this, &DbgCmd_Break::set_breakpoint_2arg),
                                         mem_fun(this, &DbgCmd_Break::set_breakpoint_3arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Break::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Break::print_help_0arg));
  }
}
