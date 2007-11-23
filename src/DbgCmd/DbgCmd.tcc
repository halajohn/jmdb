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
#include <vector>

#include "DbgCmd.hpp"
#include "../Utils.hpp"
#include "../Exception.hpp"

namespace jmdb
{
  //////////////////////////////// Protected //////////////////////////////////
  //============================== Life cycle =================================
  
  template<SubMenuEnum T_sub>
  DbgCmd<T_sub>::~DbgCmd<T_sub>()
  {
  }
  
  //============================== Operation ==================================
  
  // do_command: 0 arg
  
  template<SubMenuEnum T_sub>
  template<DoTypeEnum T_do_type, typename T_return, typename T_obj>
  void
  DbgCmd<T_sub>::do_command_by_category(std::string &command_list,
                                        MemFunctor0<T_return, T_obj> const &func)
  {
    std::auto_ptr<std::string> const command1 = pick_command(command_list);
    
    if (true == command1->empty())
    {
      func();
    }
    else
    {
      switch (T_sub)
      {
      case HAS_SUB_MENU:
        {
          DbgCmdBase::Iterator iter;
          std::string::size_type const sz = (*command1).size();
          std::vector<DbgCmdBase *> m_found;
          
          for (iter = mp_map->begin(); iter != mp_map->end(); ++iter)
          {
            if (0 == (*command1).compare(0, sz, iter->second->name(), 0, sz))
            {
              m_found.push_back(iter->second);
            }
          }
          
          if (0 == m_found.size())
          {
            throw BadCommand();
          }
          else if (1 == m_found.size())
          {
            switch (T_do_type)
            {
            case DO_COMMAND:
              m_found[0]->do_command(command_list);
              break;
              
            case DO_HELP:
              m_found[0]->do_help(command_list);
              break;
            }
          }
          else
          {
            // more than one possible
            
            std::cout << gettext("jmdb can't determine which command is the one you wanted.")
                      << std::endl;
            std::cout << gettext("The possible choices are: ");
            
            std::vector<DbgCmdBase *>::iterator found_iter;
            
            for (found_iter = m_found.begin(); found_iter != m_found.end(); ++found_iter)
            {
              std::cout << (*found_iter)->name() << " ";
            }
            
            std::cout << std::endl;
          }
        }
        break;
        
      case NO_SUB_MENU:
        throw BadCommand();
      }
    }
  }
  
  // do_command: 1 arg
  
  template<SubMenuEnum T_sub>
  template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1>
  void
  DbgCmd<T_sub>::do_command_by_category(std::string &command_list,
                                        MemFunctor0<T_return, T_obj> const &func_0arg,
                                        MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg)
  {
    std::auto_ptr<std::string> const command1 = pick_command(command_list);
    
    if (true == command1->empty())
    {
      func_0arg();
    }
    else
    {
      switch (T_sub)
      {
      case HAS_SUB_MENU:
        {
          DbgCmdBase::Iterator iter;
          std::string::size_type const sz = (*command1).size();
          std::vector<DbgCmdBase *> m_found;
          
          for (iter = mp_map->begin(); iter != mp_map->end(); ++iter)
          {
            if (0 == (*command1).compare(0, sz, iter->second->name(), 0, sz))
            {
              m_found.push_back(iter->second);
            }
          }
          
          if (0 == m_found.size())
          {
            // I know this is dirty.
            goto have_arg;
          }
          else if (1 == m_found.size())
          {
            switch (T_do_type)
            {
            case DO_COMMAND:
              m_found[0]->do_command(command_list);
              break;
              
            case DO_HELP:
              m_found[0]->do_help(command_list);
              break;
            }
          }
          else
          {
            // more than one possible
            
            std::cout << gettext("jmdb can't determine which command is the one you wanted.")
                      << std::endl;
            std::cout << gettext("The possible choices are: ");
            
            std::vector<DbgCmdBase *>::iterator found_iter;
            
            for (found_iter = m_found.begin(); found_iter != m_found.end(); ++found_iter)
            {
              std::cout << (*found_iter)->name() << " ";
            }
            
            std::cout << std::endl;
          }
        }
        break;
        
      case NO_SUB_MENU:
      have_arg:
        // make sure we just have 1 arg.
        if (command_list.empty() != true)
        {
          throw BadCommand();
        }
        else
        {
          func_1arg(*command1);
        }
        break;
      }
    }
  }
  
  // do_command: 2 arg
  
  template<SubMenuEnum T_sub>
  template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
  void
  DbgCmd<T_sub>::do_command_by_category(std::string &command_list,
                                        MemFunctor0<T_return, T_obj> const &func_0arg,
                                        MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg,
                                        MemFunctor2<T_return, T_obj, T_arg1, T_arg2> const &func_2arg)
  {
    std::auto_ptr<std::string> const command1 = pick_command(command_list);
    
    if (true == command1->empty())
    {
      func_0arg();
    }
    else
    {
      switch (T_sub)
      {
      case HAS_SUB_MENU:
        {
          DbgCmdBase::Iterator iter;
          std::string::size_type const sz = (*command1).size();
          std::vector<DbgCmdBase *> m_found;
          
          for (iter = mp_map->begin(); iter != mp_map->end(); ++iter)
          {
            if (0 == (*command1).compare(0, sz, iter->second->name(), 0, sz))
            {
              m_found.push_back(iter->second);
            }
          }
          
          if (0 == m_found.size())
          {
            // I know this is dirty.
            goto have_arg;
          }
          else if (1 == m_found.size())
          {
            switch (T_do_type)
            {
            case DO_COMMAND:
              m_found[0]->do_command(command_list);
              break;
              
            case DO_HELP:
              m_found[0]->do_help(command_list);
              break;
            }
          }
          else
          {
            // more than one possible
            
            std::cout << gettext("jmdb can't determine which command is the one you wanted.")
                      << std::endl;
            std::cout << gettext("The possible choices are: ");
            
            std::vector<DbgCmdBase *>::iterator found_iter;
            
            for (found_iter = m_found.begin(); found_iter != m_found.end(); ++found_iter)
            {
              std::cout << (*found_iter)->name() << " ";
            }
            
            std::cout << std::endl;
          }
        }
        break;
        
      case NO_SUB_MENU:
      have_arg:
        std::auto_ptr<std::string> const command2 = pick_command(command_list);
        
        if (true == command2->empty())
        {
          func_1arg(*command1);
        }
        else
        {
          if (command_list.empty() != true)
          {
            throw BadCommand();
          }
          else
          {
            func_2arg(*command1, *command2);
          }
        }
        break;
      }
    }
  }
  
  // do_command: 3 arg
  
  template<SubMenuEnum T_sub>
  template<DoTypeEnum T_do_type, typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
  void
  DbgCmd<T_sub>::do_command_by_category(std::string &command_list,
                                        MemFunctor0<T_return, T_obj> const &func_0arg,
                                        MemFunctor1<T_return, T_obj, T_arg1> const &func_1arg,
                                        MemFunctor2<T_return, T_obj, T_arg1, T_arg2> const &func_2arg,
                                        MemFunctor3<T_return, T_obj, T_arg1, T_arg2, T_arg3> const &func_3arg)
  {
    std::auto_ptr<std::string> const command1 = pick_command(command_list);
    
    if (true == command1->empty())
    {
      func_0arg();
    }
    else
    {
      switch (T_sub)
      {
      case HAS_SUB_MENU:
        {
          DbgCmdBase::Iterator iter;
          std::string::size_type const sz = (*command1).size();
          std::vector<DbgCmdBase *> m_found;
          
          for (iter = mp_map->begin(); iter != mp_map->end(); ++iter)
          {
            if (0 == (*command1).compare(0, sz, iter->second->name(), 0, sz))
            {
              m_found.push_back(iter->second);
            }
          }
          
          if (0 == m_found.size())
          {
            // I know this is dirty.
            goto have_arg;
          }
          else if (1 == m_found.size())
          {
            switch (T_do_type)
            {
            case DO_COMMAND:
              m_found[0]->do_command(command_list);
              break;
              
            case DO_HELP:
              m_found[0]->do_help(command_list);
              break;
            }
          }
          else
          {
            // more than one possible
            
            std::cout << gettext("jmdb can't determine which command is the one you wanted.")
                      << std::endl;
            std::cout << gettext("The possible choices are: ");
            
            std::vector<DbgCmdBase *>::iterator found_iter;
            
            for (found_iter = m_found.begin(); found_iter != m_found.end(); ++found_iter)
            {
              std::cout << (*found_iter)->name() << " ";
            }
            
            std::cout << std::endl;
          }
        }
        break;
        
      case NO_SUB_MENU:
      have_arg:
        std::auto_ptr<std::string> const command2 = pick_command(command_list);
        
        if (true == command2->empty())
        {
          func_1arg(*command1);
        }
        else
        {
          std::auto_ptr<std::string> const command3 = pick_command(command_list);
          
          if (true == command3->empty())
          {
            func_2arg(*command1, *command2);
          }
          else
          {
            if (command_list.empty() != true)
            {
              throw BadCommand();
            }
            else
            {
              func_3arg(*command1, *command2, *command3);
            }
          }
        }
        break;
      }
    }
  }
}
