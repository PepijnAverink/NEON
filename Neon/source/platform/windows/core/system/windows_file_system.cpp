#include "./core/system/file_system.h"
#include "./utilities/hashing/hashing_helper.h"

#include <fstream>
#include <windows.h>

namespace Neon
{
	namespace Core
	{
		namespace FileSystem
		{
			bool FileExists(const std::string& _file)
			{
				std::ifstream file(_file.c_str());
				return file.good();
			}

			bool MakeFile(const std::string& _file)
			{
				return CreateFile(_file.c_str(), GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			}

			bool RemoveFile(const std::string& _file)
			{
				return DeleteFile(_file.c_str());
			}

			unsigned int FileSize(const std::string& _file)
			{
				std::ifstream file(_file.c_str(), std::ios::in);
				file.seekg(0, std::ios::end);

				return (unsigned int)file.tellg();
			}

			bool WriteFile(const std::string& _file, const std::string& _text)
			{
				std::ofstream file;
				file.open(_file.c_str(), std::ios::out | std::ios::binary);

				file.write(_text.c_str(), _text.length());

				file.close();
				return true;
			}

			bool WriteFile(const std::string& _file, void* _data, size_t _size)
			{
				std::ofstream file;
				file.open(_file.c_str(), std::ios::out | std::ios::binary);

				file.write((char*)_data, _size);

				file.close();
				return true;
			}

			bool ReadFile(const std::string& _file, std::string& _text)
			{
				std::ifstream file(_file.c_str());
				_text = std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
				return true;
			}

			bool ReadFile(const std::string& _file, void** _data, size_t& _size)
			{
				_size = FileSize(_file);
				char* buffer = new char[_size];

				std::ifstream rf(_file.c_str(), std::ios::out | std::ios::binary);
				rf.read(buffer, _size);

				*_data = buffer;
				return true;
			}

			unsigned long GetFileModificationTimeHash(const std::string& _file)
			{
				HANDLE hr = CreateFile(_file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
				if (hr != INVALID_HANDLE_VALUE)
				{
					FILETIME ftWrite;
					GetFileTime(hr, NULL, NULL, &ftWrite);
					return NEON_SIMPLE_HASH_ULONG(ftWrite.dwHighDateTime, ftWrite.dwLowDateTime);
				}

				return 0;
			}

			unsigned long GetFileCreationTimeHash(const std::string& _file)
			{
				HANDLE hr = CreateFile(_file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
				if (hr != INVALID_HANDLE_VALUE)
				{
					FILETIME ftWrite;
					GetFileTime(hr, &ftWrite, NULL, NULL);
					return NEON_SIMPLE_HASH_ULONG(ftWrite.dwHighDateTime, ftWrite.dwLowDateTime);
				}

				return 0;
			}
		}
	}
}