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

#ifndef PacketHeader_hpp
#define PacketHeader_hpp

#include "../Type.hpp"
#include "../ClientSocket.hpp"

namespace jmdb
{
  template<typename T, typename C>
  inline T get_size(T C::*);
  
  typedef struct PacketHeader PacketHeader;
  class PacketHeader
  {
  private:
    
    static uint32_t ID;
    
    enum PacketTypeEnum
    {
      PT_COMMAND,
      PT_EVENT
    };
    typedef enum PacketTypeEnum PacketTypeEnum;
    
    PacketTypeEnum m_packet_type;
    
    uint32_t m_length;
    uint32_t m_id;
    uint8_t  m_flags;
    
    union CmdReplyUnion
    {
      struct CmdPacket
      {
        uint8_t  m_cmdset;
        uint8_t  m_cmd;
      } m_cmd_packet;
      
      struct ReplyPacket
      {
        uint16_t m_error_code;
      } m_reply_packet;
    } u;
    
    static uint32_t const CMD_HEADER_SIZE = (sizeof(get_size(&PacketHeader::m_length)) +
                                             sizeof(get_size(&PacketHeader::m_id)) +
                                             sizeof(get_size(&PacketHeader::m_flags)) +
                                             sizeof(get_size(&PacketHeader::CmdReplyUnion::CmdPacket::m_cmdset)) +
                                             sizeof(get_size(&PacketHeader::CmdReplyUnion::CmdPacket::m_cmd)));
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, PacketHeader &hdr);
    friend ClientSocket const &operator>>(ClientSocket const &socket, PacketHeader &hdr);
    
  protected:
    
    // Operation
    
    uint32_t
    RemainderDataLength() const
    {
      return m_length - CMD_HEADER_SIZE;
    }
    
    enum CmdSetEnum
    {
      KVM_CMDSET = 128,
      VIRTUAL_MACHINE = 1,
      REFERENCE_TYPE = 2,
      CLASS_TYPE = 3,
      ARRAY_TYPE = 4,
      INTERFACE_TYPE = 5,
      METHOD = 6,
      FIELD = 8,
      OBJECT_REFERENCE = 9,
      STRING_REFERENCE = 10,
      THREAD_REFERENCE = 11,
      THREAD_GROUP_REFERENCE = 12,
      ARRAY_REFERENCE = 13,
      CLASS_LOADER_REFERENCE = 14,
      EVENT_REQUEST = 15,
      STACK_FRAME = 16,
      CLASS_OBJECT_REFERENCE = 17
    };
    typedef enum CmdSetEnum CmdSetEnum;
    
    enum SuspendPolicyEnum
    {
      SP_NONE = 0,
      SP_EVENT_THREAD = 1,
      SP_ALL = 2
    };
    typedef enum SuspendPolicyEnum SuspendPolicyEnum;
        
    enum Flags
    {
      FLAG_NONE = 0x0,
      FLAG_REPLY = 0x80
    };
    typedef enum Flags Flags;
    
    enum HasReply
    {
      REPLY_YES,
      REPLY_NO
    };
    typedef enum HasReply HasReply;
    
    struct CommandPacket {};
    typedef struct CommandPacket CommandPacket;
    
    struct EventPacket {};
    typedef struct EventPacket EventPacket;
    
    static CommandPacket const COMMAND_PACKET;
    static EventPacket const EVENT_PACKET;
    
    // Life cycle
    
    PacketHeader(CommandPacket const &/* packet */,
                 Flags const flags,
                 CmdSetEnum const cmd_set_enum,
                 uint32_t const cmd,
                 uint32_t const extra_size)
      : m_packet_type(PT_COMMAND),
        m_length(CMD_HEADER_SIZE + extra_size),
        m_id(ID++),
        m_flags(flags)
    {
      u.m_cmd_packet.m_cmdset = cmd_set_enum;
      u.m_cmd_packet.m_cmd = cmd;
    }
    
    PacketHeader(EventPacket const &/* packet */)
      : m_packet_type(PT_EVENT)
    { }
    
    virtual ~PacketHeader() = 0;
    
    // Operation
    
    uint16_t
    get_error_code() const
    {
      return u.m_reply_packet.m_error_code;
    }
    
    void
    add_extra_packet_size(uint32_t const extra_size)
    {
      m_length += extra_size;
    }
  };
}

#endif
