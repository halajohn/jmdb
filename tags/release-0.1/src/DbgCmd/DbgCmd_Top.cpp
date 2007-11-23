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

#include <map>

#include "DbgCmd_Top.hpp"
#include "DbgCmd_Thread.hpp"
#include "DbgCmd_Quit.hpp"
#include "DbgCmd_Info.hpp"
#include "DbgCmd_Help.hpp"
#include "DbgCmd_Break.hpp"
#include "DbgCmd_Continue.hpp"
#include "DbgCmd_Next.hpp"
#include "DbgCmd_Print.hpp"
#include "DbgCmd_Up.hpp"
#include "DbgCmd_Down.hpp"

#include "MemFunctor.hpp"
#include "../Exception.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Operation ==================================
  
  void
  DbgCmd_Top::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Possible commands : ");
    
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
  DbgCmd_Top::search_cmd_0arg()
  {
    throw BadCommand();
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  DbgCmd_Top::DbgCmd_Top(Debugger * const debugger)
    : DbgCmd<HAS_SUB_MENU>(debugger, ""),
      mp_thread(new DbgCmd_Thread(debugger, "thread")),
      mp_quit(new DbgCmd_Quit(debugger, "quit")),
      mp_info(new DbgCmd_Info(debugger, "info")),
      mp_help(new DbgCmd_Help(debugger, "help")),
      mp_break(new DbgCmd_Break(debugger, "break")),
      mp_continue(new DbgCmd_Continue(debugger, "continue")),
      mp_next(new DbgCmd_Next(debugger, "next")),
      mp_print(new DbgCmd_Print(debugger, "print")),
      mp_up(new DbgCmd_Up(debugger, "up")),
      mp_down(new DbgCmd_Down(debugger, "down"))
  {
    add_command_to_menu(mp_thread.get());
    add_command_to_menu(mp_quit.get());
    add_command_to_menu(mp_info.get());
    add_command_to_menu(mp_help.get());
    add_command_to_menu(mp_break.get());
    add_command_to_menu(mp_continue.get());
    add_command_to_menu(mp_next.get());
    add_command_to_menu(mp_print.get());
    add_command_to_menu(mp_up.get());
    add_command_to_menu(mp_down.get());
    
    static_cast<DbgCmd_Help *>(mp_help.get())->set_command_map(mp_map.get());
  }
  
  //============================== Operation ==================================
  
  void
  DbgCmd_Top::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Top::search_cmd_0arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid commands." << std::endl;
      
      print_help_0arg();
    }
  }
}
