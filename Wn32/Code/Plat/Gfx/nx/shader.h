#pragma once

#include <Sys/File/filesys.h>
#include <unordered_map>
#include <filesystem>
#include <Plat/Gfx/nx/nx_init.h>
#include <Plat/Sys/File/p_filesys.h>

namespace NxWn32
{
    struct CachedShader {
        std::string vertex_source;
        std::string fragment_source;
        NxWn32::sShader* shader;
    };

    // TODO: Hardronix, clear this cache on dispose!
    static std::unordered_map<std::string, CachedShader> shader_cache;
    static std::filesystem::path BASE_DIRECTORY = "shaders";

	class sShader
	{
		public:
			// Shader program and shaders
			GLuint program;
		public:
			sShader(const char *vertex, const char *fragment);
            static sShader* load_from_filesystem(const std::string& id, const std::filesystem::path& vertex_filename, const std::filesystem::path& fragment_filename);
			~sShader();

    };

	sShader *DirectShader();
	sShader *SpriteShader();
	sShader *BasicShader();
	sShader *BonedShader();
} // namespace NxWn32
