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

#ifndef DbgCmd_hpp
#define DbgCmd_hpp

#include <map>
#include <memory>
#include <string>

#include "../Type.hpp"

namespace jmdb
{
  enum SubMenuEnum
  {
    NO_SUB_MENU,
    HAS_SUB_MENU
  };
  typedef enum SubMenuEnum SubMenuEnum;
  
  enum DoTypeEnum
  {
    DO_COMMAND,
    DO_HELP
  };
  typedef enum DoTypeEnum DoTypeEnum;
  
  // Forward declaration
  //
  
  class Debugger;
  
  typedef class DbgCmdBase DbgCmdBase;
  class DbgCmdBase
  {
  protected:
    
    // Typedef
    
    typedef std::map<std::string const, DbgCmdBase * const> Map;
    typedef Map::iterator Iterator;
    Debugger * const mp_debugger;
    
    std::auto_ptr<Map> mp_map;
    
    std::string m_name;
    
    // Life cycle
    
    virtual ~DbgCmdBase() = 0;
    
    // Operation
    
    std::auto_ptr<std::string> pick_command(std::string &command_list);
    
  public:
    
    // Life cycle
    
    DbgCmdBase(Debugger * const debugger,
               std::string const &name)
      : mp_debugger(debugger),
        m_name(name)
    { }
    
    // Operation
    
    virtual void do_command(std::string &command_list) = 0;
    virtual void do_help(std::string &/* command_list */) {};
    
    std::string &
    name()
    { return m_name; }
  };
  
  // Forward declaration
  //
  
  template<typename T_return, typename T_obj>
  class MemFunctor0;
  
  template<typename T_return, typename T_obj, typename T_arg1>
  class MemFunctor1;
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
  class MemFunctor2;
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
  class MemFunctor3;
  
  template<SubMenuEnum T_sub>
  class DbgCmd : public DbgCmdBase
  {
  protected:
    
    // Operation
    
    template<DoTypeEnum T_do_type, typename T_return, typename T_obj>
    void do_command_by_category(std::string &command_list,
                                MemFunctor0<T_return, T_obj> const &func);
    
    template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1>
    void do_command_by_category(std::string &command_list,
                                MemFunctor0<T_return, T_obj> const &func_0arg,
                                MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg);
    
    template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
    void do_command_by_category(std::string &command_list,
                                MemFunctor0<T_return, T_obj> const &func_0arg,
                                MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg,
                                MemFunctor2<T_return, T_obj, T_arg1, T_arg2> const &func_2arg);
    
    template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
    void do_command_by_category(std::string &command_list,
                                MemFunctor0<T_return, T_obj> const &func_0arg,
                                MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg,
                                MemFunctor2<T_return, T_obj, T_arg1, T_arg2> const &func_2arg,
                                MemFunctor3<T_return, T_obj, T_arg1, T_arg2, T_arg3> const &func_3arg);
    
    template<typename T_cmd>
    void
    add_command_to_menu(T_cmd * const p_cmd)
    {
      mp_map->insert(std::pair<std::string const, DbgCmdBase * const>(p_cmd->name(), p_cmd));
    }
    
    // Friend
    
    template<typename T_return, typename T_obj>
    friend class MemFunctor0;
    
    template<typename T_return, typename T_obj, typename T_arg1>
    friend class MemFunctor1;
    
    template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
    friend class MemFunctor2;
    
    template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
    friend class MemFunctor3;
    
  public:
    
    // Life cycle
    
    DbgCmd(Debugger * const debugger,
           std::string const &name)
      : DbgCmdBase(debugger, name)
    {
      if (HAS_SUB_MENU == T_sub)
      {
        mp_map.reset(new Map);
      }
    }
    
    virtual ~DbgCmd() = 0;
  };
}

#endif
