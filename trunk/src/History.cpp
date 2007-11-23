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

#include "History.hpp"

namespace jmdb
{
  ////////////////////////////// Public ///////////////////////////////////////
  //============================= Operation ===================================
  
  void
  History::push_back(std::string const &command)
  {
    if (true == command.empty())
    {
      return;
    }
    else
    {
      if (HISTORY_SIZE == m_history.size())
      {
        m_history.pop_front();
      }
      
      m_history.push_back(command);
      
      m_got = 0;
    }
  }
  
  std::string
  History::get_recent_cmd()
  {
    if (0 == m_history.size())
    {
      return std::string();
    }
    else
    {
      if (m_got == m_history.size())
      {
        return m_history.front();
      }
      else
      {
        ++m_got;
        
        return m_history[m_history.size() - m_got];
      }
    }
  }
  
  std::string
  History::put_recent_cmd()
  {
    if (0 == m_history.size())
    {
      return std::string();
    }
    else
    {
      switch (m_got)
      {
      case 0:
      case 1:
        m_got = 0;
        return std::string();
        
      default:
        --m_got;
        
        return m_history[m_history.size() - m_got];
      }
    }
  }
}
