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

#ifndef KdwpCmd_VM_AllThreads_hpp
#define KdwpCmd_VM_AllThreads_hpp

// System include
//

#include <vector>

// Local include
//

#include "KdwpCmd_VM.hpp"

namespace jmdb
{
  typedef class KdwpCmd_VM_AllThreads KdwpCmd_VM_AllThreads;
  class KdwpCmd_VM_AllThreads : public KdwpCmd_VM
  {
  private:
    
    uint32_t m_thread_count;
    std::vector<uint32_t> m_thread_id;
    
  public:
    
    // Life cycle
    
    KdwpCmd_VM_AllThreads()
      : KdwpCmd_VM(FLAG_NONE, ALL_THREADS, 0)
    { }
    
    // Operation
    
    void dump_thread_id() const;
    
    std::vector<uint32_t> const &
    thread_id() const
    { return m_thread_id; }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_VM_AllThreads &cmd);
  };
}

#endif
