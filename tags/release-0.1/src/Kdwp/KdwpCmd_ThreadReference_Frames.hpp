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

#ifndef KdwpCmd_ThreadReference_Frames_hpp
#define KdwpCmd_ThreadReference_Frames_hpp

#include <cassert>
#include <vector>

#include "KdwpCmd_ThreadReference.hpp"
#include "Location.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ThreadReference_Frames KdwpCmd_ThreadReference_Frames;
  class KdwpCmd_ThreadReference_Frames : public KdwpCmd_ThreadReference
  {
  public:
    
    struct FrameInfo
    {
      uint32_t m_id;
      Location m_location;
    };
    typedef struct FrameInfo FrameInfo;
    
  private:
    
    // Out data
    
    uint32_t m_thread_id;
    uint32_t m_start_frame;
    int32_t m_wanted_count;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_ThreadReference_Frames::m_thread_id)) +
                                               sizeof(get_size(&KdwpCmd_ThreadReference_Frames::m_start_frame)) +
                                               sizeof(get_size(&KdwpCmd_ThreadReference_Frames::m_wanted_count)));
    
    // Reply data
    
    uint32_t m_frame_count;
    
    std::vector<FrameInfo> m_frames;
    
  public:
    
    // Life cycle
    
    KdwpCmd_ThreadReference_Frames(uint32_t const thread_id,
                                   uint32_t const start_index,
                                   int32_t const wanted_count)
      : KdwpCmd_ThreadReference(FLAG_NONE, FRAMES, EXTRA_HEADER_SIZE),
        m_thread_id(thread_id),
        m_start_frame(start_index),
        m_wanted_count(wanted_count)
    {
      // :NOTE: Wei 2004-Jun-19:
      //
      // If wanted_count == 0, then KVM will send us a INTERNAL error.
      assert(wanted_count != 0);
    }
    
    // Operation
    
    void dump_all_frames() const;
    int32_t get_last_frame_id() const;
    FrameInfo const &get_frame_info(uint32_t const frame_id) const;
    
    uint32_t
    get_frame_count() const
    { return m_frames.size(); }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Frames &cmd);
  };
}

#endif
