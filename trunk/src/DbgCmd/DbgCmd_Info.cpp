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

#include "DbgCmd_Info.hpp"
#include "DbgCmd_Info_Thread.hpp"
#include "DbgCmd_Info_Class.hpp"
#include "DbgCmd_Info_Method.hpp"
#include "DbgCmd_Info_Field.hpp"
#include "DbgCmd_Info_LineTable.hpp"
#include "DbgCmd_Info_Variable.hpp"
#include "DbgCmd_Info_Frame.hpp"

#include "MemFunctor.hpp"
#include "../Exception.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  ////////////////////////////////// Private //////////////////////////////////
  //============================== Operation ==================================
  
  void
  DbgCmd_Info::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Print several information.") << std::endl;
    std::cout << gettext(" Syntax       : help [command]") << std::endl;
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
  DbgCmd_Info::print_info_0arg()
  {
    throw BadCommand();
  }
  
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  DbgCmd_Info::DbgCmd_Info(Debugger * const debugger,
                           std::string const &name)
    : DbgCmd<HAS_SUB_MENU>(debugger, name),
      mp_thread(new DbgCmd_Info_Thread(debugger, "thread")),
      mp_class(new DbgCmd_Info_Class(debugger, "class")),
      mp_method(new DbgCmd_Info_Method(debugger, "method")),
      mp_field(new DbgCmd_Info_Field(debugger, "field")),
      mp_linetable(new DbgCmd_Info_LineTable(debugger, "linetable")),
      mp_variable(new DbgCmd_Info_Variable(debugger, "variable")),
      mp_frame(new DbgCmd_Info_Frame(debugger, "frame"))
  {
    add_command_to_menu(mp_thread.get());
    add_command_to_menu(mp_class.get());
    add_command_to_menu(mp_method.get());
    add_command_to_menu(mp_field.get());
    add_command_to_menu(mp_linetable.get());
    add_command_to_menu(mp_variable.get());
    add_command_to_menu(mp_frame.get());
  }
  
  void
  DbgCmd_Info::do_command(std::string &command_list)
  {
    do_command_by_category<DO_COMMAND>(command_list,
                                       mem_fun(this, &DbgCmd_Info::print_info_0arg));
  }
  
  void
  DbgCmd_Info::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Info::print_help_0arg));
  }
}
