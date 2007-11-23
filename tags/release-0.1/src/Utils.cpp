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

#include <cassert>
#include <sstream>

#include "Utils.hpp"
#include "Exception.hpp"

namespace jmdb
{
  NumberType
  number_type(std::string const &number_string)
  {
    assert(number_string.size() != 0);
    
    if (number_string.size() <= 2)
    {
      return DEC;
    }
    else
    {
      if (0 == number_string.compare(0, 2, "0x"))
      {
        return HEX;
      }
      else
      {
        return DEC;
      }
    }
  }
  
  uint32_t
  convert_to_integer(std::string const &number_string)
  {
    std::stringstream s;
    uint32_t result;
    
    s << number_string;
    
    switch (number_type(number_string))
    {
    case DEC:
      s >> result;
      break;
      
    case HEX:
      s >> std::hex >> result;
      break;
      
    default:
      assert(1 != 1);
      break;
    }
    
    if (!s)
    {
      throw BadNumber();
    }
    else
    {
      return result;
    }
  }
}
