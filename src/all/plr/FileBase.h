////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 John Chapman -- http://john-chapman.net
// This software is distributed freely under the terms of the MIT License.
// See http://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////////////////////////
#ifndef plr_File_h
	#error plr: Do not include plr/FileBase.h, include plr/File.h
#endif

#include <plr/def.h>

namespace plr { namespace internal {

////////////////////////////////////////////////////////////////////////////////
/// \class FileBase
/// Base class for File implementations.
/// \internal \ingroup plr_core
////////////////////////////////////////////////////////////////////////////////
class FileBase
{
protected:
	FileBase();
	~FileBase();

	void        swap(FileBase& _file_);
	
	/// _data may be 0 in which case m_data is allocated but not initialized.
	void        setData(const char* _data, uint64 _size);

	const char* getPath() const              { return m_path; }
	void        setPath(const char* _path)   { m_path.set(_path); }
	const char* getData() const              { return m_data; }
	char*       getData()                    { return m_data; }
	uint64      getDataSize() const          { return m_dataSize; }

	typedef String<64> PathStr;
	char*   m_data;
	uint64  m_dataSize;
	PathStr m_path;

}; // class FileBase

} } // namespace plr::internal
