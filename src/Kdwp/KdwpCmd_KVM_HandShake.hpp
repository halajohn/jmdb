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

#ifndef KdwpCmd_KVM_HandShake_hpp
#define KdwpCmd_KVM_HandShake_hpp

#include "KdwpCmd_KVM.hpp"

namespace jmdb
{
  typedef class KdwpCmd_KVM_HandShake KdwpCmd_KVM_HandShake;
  class KdwpCmd_KVM_HandShake : public KdwpCmd_KVM
  {
  private:
    
    static std::string const HANDSHAKE_STRING;
    
  public:
    
    // Life cycle
    
    KdwpCmd_KVM_HandShake()
      : KdwpCmd_KVM(FLAG_NONE, HANDSHAKE, 0)
    { }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_KVM_HandShake &cmd);
  };
}

#endif
