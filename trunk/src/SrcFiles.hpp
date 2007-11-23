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

#ifndef SrcFiles_hpp
#define SrcFiles_hpp

#include <cassert>
#include <string>
#include <deque>
#include <vector>
#include <fstream>

#include "Type.hpp"

namespace jmdb
{
  // Forward declaration
  //
  
  class Debugger;
  
  typedef class SrcFiles SrcFiles;
  class SrcFiles
  {
  private:
    
    enum FileState
    {
      FILE_NORMAL,
      FILE_IS_DIR,
      FILE_NON_EXIST
    };
    typedef enum FileState FileState;
    
    struct FileInfo
    {
      std::string m_fullname;
      std::ifstream *mp_filestream;
    };
    typedef struct FileInfo FileInfo;
    
    Debugger * const mp_debugger;
    std::deque<FileInfo> m_files;
    std::vector<std::string> m_src_search_path;
    
    // Operation
    
    std::string get_basename_from_signature(std::string const &signature);
    FileState file_state(std::string const &root_name) const;
    
  public:
    
    // Life cycle
    
    SrcFiles(Debugger * const debugger)
      : mp_debugger(debugger)
    { }
    
    ~SrcFiles();
    
    // Operation
    
    void set_src_search_path(std::string const &search_path);
    std::string search_src_file(std::string const &signature, std::string const &filename);
  };
}

#endif
