#pragma once

#include <Core/Defines.h>

#include <Sys/File/filesys.h>

#include <filesystem>
#include <fstream>

namespace File
{
	// Return module path
	std::filesystem::path ModulePath();

	// Return data path
	std::filesystem::path DataPath();

    // File handle
    struct sFileHandle
    {
        std::ifstream file;
        size_t filesize = 0;

        sFileHandle(const std::filesystem::path &path) : file(path, std::ios::binary)
        {
            if (file.is_open())
                filesize = (size_t)std::filesystem::file_size(path);
        }
    };
}
