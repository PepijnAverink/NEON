#pragma once
#include <string>

namespace Neon
{
	namespace Core
	{
		namespace FileSystem
		{
			bool FileExists(const std::string& _file);

			bool MakeFile(const std::string& _file);
			bool RemoveFile(const std::string& _file);

			unsigned int FileSize(const std::string& _file);

			bool WriteFile(const std::string& _file, const std::string& _text);
			bool WriteFile(const std::string& _file, void* _data, size_t _size);

			bool ReadFile(const std::string& _file, std::string& _text);
			bool ReadFile(const std::string& _file, void** _data, size_t& _size);

			unsigned long GetFileModificationTimeHash(const std::string& _file);
			unsigned long GetFileCreationTimeHash(const std::string& _file);

			inline bool HasFileChanged(const std::string& _file, unsigned long _hash) { return (GetFileModificationTimeHash(_file) != _hash); }
		}
	}
}