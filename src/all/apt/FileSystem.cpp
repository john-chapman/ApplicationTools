#include <apt/FileSystem.h>

#include <apt/log.h>
#include <apt/File.h>
#include <apt/String.h>

#include <cstring>

using namespace apt;

// PUBLIC

const char* FileSystem::GetRoot(RootType _type)
{
	return s_roots[_type];
}

void FileSystem::SetRoot(RootType _type, const char* _path)
{
	s_roots[_type].set(_path); 
	s_rootLengths[_type] = (int)strlen(_path);
}

bool FileSystem::Read(File& file_, const char* _path, RootType _rootHint)
{
	PathStr fullPath;
	if (!FindExisting(fullPath, _path ? _path : file_.getPath(), _rootHint)) {
		APT_LOG_ERR("Error loading '%s':\n\tFile not found", _path);
		APT_ASSERT(false);
		return false;
	}
	return File::Read(file_, (const char*)fullPath);
}

bool FileSystem::ReadIfExists(File& file_, const char* _path, RootType _rootHint)
{
	PathStr fullPath;
	if (!FindExisting(fullPath, _path ? _path : file_.getPath(), _rootHint)) {
		return false;
	}
	return File::Read(file_, (const char*)fullPath);
}

bool FileSystem::Write(const File& _file, const char* _path, RootType _root)
{
	PathStr fullPath;
	MakePath(fullPath, _path ? _path : _file.getPath(), _root);
	return File::Write(_file, (const char*)fullPath);
}

bool FileSystem::Exists(const char* _path, RootType _rootHint)
{
	PathStr buf;
	return FindExisting(buf, _path, _rootHint);
}

void FileSystem::StripRoot(StringBase& ret_, const char* _path)
{
	for (int r = 0; r < RootType_Count; ++r) {
		if (s_rootLengths[r] == 0) {
			continue;
		}
		const char* rootBeg = strstr(_path, s_roots[r]);
		if (rootBeg != nullptr) {
			ret_.set(rootBeg + s_rootLengths[r] + 1);
			return;
		}
	}
 // no root found, strip the whole path
	StripPath(ret_, _path);
}

void FileSystem::StripPath(StringBase& ret_, const char* _path)
{
	const char* beg = _path;
	while (*_path) {
		if (*_path == '/' || *_path == '\\') {
			beg = _path + 1;
		}
		++_path;
	}
	ret_.set(beg);
}

void FileSystem::GetPath(StringBase& ret_, const char* _path)
{
	const char* beg = _path;
	const char* end = _path;
	while (*_path) {
		if (*_path == '/' || *_path == '\\') {
			end = _path + 1;
		}
		++_path;
	}
	ret_.set(beg, end - beg);
}

void FileSystem::GetFileName(StringBase& ret_, const char* _path)
{
	const char* beg = _path;
	while (*_path) {
		if (*_path == '/' || *_path == '\\') {
			beg = _path + 1;
		}
		++_path;
	}
	const char* end = beg;
	while (*end && *end != '.') {
		++end;
	}
	ret_.set(beg, end - beg);
}

const char* FileSystem::GetExtension(const char* _path)
{
	const char* ret = strrchr(_path, (int)'.');
	return ret == nullptr ? nullptr : ret + 1;
}

// PRIVATE

FileSystem::PathStr FileSystem::s_roots[RootType_Count];
int FileSystem::s_rootLengths[RootType_Count];

void FileSystem::MakePath(StringBase& ret_, const char* _path, RootType _root)
{
	APT_ASSERT(_root < RootType_Count);
	bool useRoot = !s_roots[_root].isEmpty();
	if (useRoot) {
	 // check if the root already exists in path as a directory
		const char* r = strstr(s_roots[_root], _path);
		if (!r || *(r + s_rootLengths[_root]) != s_separator) {
			ret_.setf("%s%c%s", (const char*)s_roots[_root], s_separator, _path);
			return;
		}
	}
	ret_.set(_path);
}

bool FileSystem::FindExisting(PathStr& ret_, const char* _path, RootType _rootHint)
{
	for (int r = (int)_rootHint; r != -1; --r) {
		MakePath(ret_, _path, (RootType)r);
		if (File::Exists((const char*)ret_)) {
			return true;
		}
	}
	return false;
}
