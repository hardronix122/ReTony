#include "shader.h"

#include <string>

namespace NxWn32
{
    std::unordered_map<std::string, CachedShader> sShader::shader_cache;

	// Shader compile
	static GLuint CompileSource(GLenum type, const char *src)
	{
		// Create shader
		GLuint shader = glCreateShader(type);

		// Compile source
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// Check for errors
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status != GL_TRUE)
		{
			char msg[512];
			glGetShaderInfoLog(shader, 512, nullptr, msg);
			Dbg_MsgAssert(status == GL_TRUE, (msg))
			return 0;
		}

		return shader;
	}

	sShader::sShader(const char *vertex, const char *fragment)
	{


        // Compile shaders
		GLuint vertex_shader = CompileSource(GL_VERTEX_SHADER, vertex);
		GLuint fragment_shader = CompileSource(GL_FRAGMENT_SHADER, fragment);

		// Create program
		program = glCreateProgram();

		// Attach shaders
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		// Link program
		glLinkProgram(program);

		// Check for errors
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status != GL_TRUE)
		{
			char msg[512];
			glGetProgramInfoLog(program, 512, nullptr, msg);
			Dbg_MsgAssert(status == GL_TRUE, (msg))
			return;
		}

		// Delete shaders
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		// Set texture uniforms
		glUseProgram(program);
		for (int i = 0; i < 4; i++)
			glUniform1i(glGetUniformLocation(program, ("u_texture[" + std::to_string(i) + "]").c_str()), i);
	}

	sShader::~sShader()
	{
		// Delete program
		glDeleteProgram(program);
	}

    sShader *sShader::load_from_filesystem(const std::string& id, const std::filesystem::path& vertex_filename, const std::filesystem::path& fragment_filename) {
        if(sShader::shader_cache.contains(id)) {
            CachedShader cached_shader = sShader::shader_cache[id];
            return cached_shader.shader;
        }

        printf("Loading shader %s (%s/%s) from the filesystem...\n", id.c_str(), vertex_filename.c_str(), fragment_filename.c_str());

        std::string vertex_source, fragment_source;
        std::filesystem::path vertex_filepath = BASE_DIRECTORY / vertex_filename;
        std::filesystem::path fragment_filepath = BASE_DIRECTORY / fragment_filename;

        auto *vertex_file = (File::sFileHandle*) File::Open(vertex_filepath.c_str(), "r");
        auto *fragment_file = (File::sFileHandle*) File::Open(fragment_filepath.c_str(), "r");

        File::ReadAllText(&vertex_source, vertex_file);
        File::ReadAllText(&fragment_source, fragment_file);

        File::Close(vertex_file);
        File::Close(fragment_file);

        CachedShader cached_shader(vertex_source, fragment_source);
        auto* shader = new sShader(vertex_source.c_str(), fragment_source.c_str());
        cached_shader.shader = shader;

        sShader::shader_cache[id] = cached_shader;
        return shader;
    }

    // Shader programs
	sShader *DirectShader()
	{
		return sShader::load_from_filesystem("direct", "direct.vsh", "direct.fsh");
	}

	sShader *SpriteShader()
	{
        return sShader::load_from_filesystem("sprite", "sprite.vsh", "sprite.fsh");
	}

	sShader *BasicShader()
	{
        return sShader::load_from_filesystem("basic", "basic.vsh", "basic.fsh");
	}

	sShader *BonedShader()
	{
        return sShader::load_from_filesystem("boned", "boned.vsh", "basic.fsh");
	}

} // namespace NxWn32
