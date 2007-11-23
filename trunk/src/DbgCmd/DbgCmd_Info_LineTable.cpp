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

#include "DbgCmd_Info_LineTable.hpp"
#include "MemFunctor.hpp"
#include "../Kdwp/KdwpCmd_Method_LineTable.hpp"
#include "../Debugger.hpp"
#include "../Exception.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================ Operation ====================================
  
  void
  DbgCmd_Info_LineTable::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Print line table of the specified method.") << std::endl;
    std::cout << gettext(" Syntax       : info linetable <class ID> <method ID>") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Info_LineTable::get_line_info_0arg()
  {
    throw BadCommand();
  }
  
  void
  DbgCmd_Info_LineTable::get_line_info_1arg(std::string const &/* class_id_string */)
  {
    throw BadCommand();
  }
  
  void
  DbgCmd_Info_LineTable::get_line_info_2arg(std::string const &class_id_string,
                                            std::string const &method_id_string)
  {
    try
    {
      uint32_t const class_id = convert_to_integer(class_id_string);
      uint32_t const method_id = convert_to_integer(method_id_string);
      
      KdwpCmd_Method_LineTable kdwp_cmd(class_id, method_id);
      
      mp_debugger->socket() << kdwp_cmd;
      
      kdwp_cmd.dump_all_lines();
    }
    catch (BadNumber &/* e */)
    {
      throw BadCommand();
    }
    catch (KdwpErrorInvalidMethodId &/* e */)
    {
      std::cout << gettext("Unknown method, probably you typed an invalid method ID.") << std::endl;
    }
    catch (KdwpErrorNotFound &/* e */)
    {
      std::cout << gettext("No line number table found for this method.") << std::endl;
    }
  }
  
  ////////////////////////////// Public ///////////////////////////////////////
  //============================ Operation ====================================
  
  void
  DbgCmd_Info_LineTable::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Info_LineTable::get_line_info_0arg),
                                         mem_fun(this, &DbgCmd_Info_LineTable::get_line_info_1arg),
                                         mem_fun(this, &DbgCmd_Info_LineTable::get_line_info_2arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Info_LineTable::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Info_LineTable::print_help_0arg));
  }
}
