#include <stdio.h>
#include <string.h>

#include <Core/Defines.h>
#include <Core/support.h>

#include "p_filesys.h"
#include <Sys/File/AsyncFilesys.h>
#include <Sys/File/PRE.h>
#include <Sys/Config/config.h>

// #include <Gfx/xbox/nx/nx_init.h>

#ifdef __PLAT_WN32__
#include <Windows.h>
#endif
#ifdef __PLAT_LINUX__
#include <unistd.h>
#endif

#include <fstream>

namespace File
{
	// File handle
	struct sFileHandle
	{
		std::ifstream file;
		size_t filesize = 0;

		sFileHandle(const std::filesystem::path &path) : file(path, std::ios::binary)
		{
			if (file.is_open())
				filesize = std::filesystem::file_size(path);
		}
	};

	// Return module path
	static std::filesystem::path GetModulePath()
	{
#if defined(__PLAT_WN32__)
		// Get module name
		std::basic_string<WCHAR> module_name(MAX_PATH, '\0');

		while (1)
		{
			DWORD decceded = GetModuleFileNameW(nullptr, module_name.data(), module_name.size());
			if (decceded < module_name.size())
			{
				module_name.resize(decceded);
				break;
			}
			else
			{
				module_name.resize(module_name.size() * 2);
			}
		}

		// Remove filename
		std::filesystem::path module_path(module_name);
		module_path.remove_filename();
		return module_path;
#elif defined(__PLAT_LINUX__)
		// Get proc/self/exe name
		std::basic_string<char> module_name(0x100, '\0');

		while (1)
		{
			ssize_t decceded = readlink("proc/self/exe", module_name.data(), module_name.size());
			if (decceded <= module_name.size())
			{
				module_name.resize(decceded);
				break;
			}
			else
			{
				module_name.resize(module_name.size() * 2);
			}
		}

		// Remove filename
		std::filesystem::path module_path(module_name);
		module_path.remove_filename();
		return module_path;
#endif
	}

	std::filesystem::path ModulePath()
	{
		static std::filesystem::path module_path = GetModulePath();
		return module_path;
	}

	// Return data path
	static std::filesystem::path GetDataPath()
	{
		// Get module path
		return ModulePath() / "Data";
	}

	std::filesystem::path DataPath()
	{
		static std::filesystem::path data_path = GetDataPath();
		return data_path;
	}

	static void* prefopen( const char *filename, const char *mode )
	{
		// Process file name
		std::filesystem::path file_path(filename);

		// If name ends with .xbx, remove
		if (file_path.extension() == ".xbx" || file_path.extension() == ".Xbx")
			file_path.replace_extension();

		// If name ends with .ps2, remove
		if (file_path.extension() == ".ps2")
			file_path.replace_extension();

		// Open the file
		std::filesystem::path full_path = DataPath() / file_path;
		sFileHandle *h_file = new sFileHandle(full_path);

		if (!h_file->file.is_open())
		{
			delete h_file;
			return nullptr;
		}

		return h_file;
	}

	void InstallFileSystem( void )
	{
	#	if 0
		// This is where we start the thread that will deal with copying commonly used data from the DVD to the utility
		// region (z:\) on the HD.
		pLoader = new CThreadedLevelLoader();

		SLevelDesc level_descs[] = {{ "pre\\alc.prx" },
									{ "pre\\alccol.prx" },
									{ "pre\\alcscn.prx" },
									{ "pre\\anims.prx" },
									{ "pre\\bits.prx" },
									{ "pre\\cnv.prx" },
									{ "pre\\cnvcol.prx" },
									{ "pre\\cnvscn.prx" },
									{ "pre\\fonts.prx" },
									{ "pre\\jnk.prx" },
									{ "pre\\jnkcol.prx" },
									{ "pre\\jnkscn.prx" },
									{ "pre\\kon.prx" },
									{ "pre\\koncol.prx" },
									{ "pre\\konscn.prx" },
									{ "pre\\lon.prx" },
									{ "pre\\loncol.prx" },
									{ "pre\\lonscn.prx" },
									{ "pre\\qb.prx" },
									{ "pre\\sch.prx" },
									{ "pre\\schcol.prx" },
									{ "pre\\schscn.prx" },
									{ "pre\\sf2.prx" },
									{ "pre\\sf2col.prx" },
									{ "pre\\sf2scn.prx" },
									{ "pre\\skaterparts.prx" },
									{ "pre\\skater_sounds.prx" },
									{ "pre\\skateshop.prx" },
									{ "pre\\skateshopcol.prx" },
									{ "pre\\skateshopscn.prx" },
									{ "pre\\skeletons.prx" },
									{ "pre\\zoo.prx" },
									{ "pre\\zoocol.prx" },
									{ "pre\\zooscn.prx" }};

		static BYTE data_buffer[32 * 1024];
		pLoader->Initialize( level_descs, 34, data_buffer, 32 * 1024, &OkayToUseUtilityDrive );
		pLoader->AsyncStreamLevel( 0 );
	#	endif

		// Initialise the async file system.
		File::CAsyncFileLoader::sInit();
	}

	long GetFileSize( void* pFP )
	{
		Dbg_MsgAssert( pFP, ( "nullptr pFP sent to GetFileSize" ));

		if( PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_get_file_size( (PreFile::FileHandle *) pFP );
			if( PreMgr::sPreExecuteSuccess())
				return retval;
		}
	
		sFileHandle *h_file = (sFileHandle*)pFP;
		return h_file->filesize;
	}

	long GetFilePosition( void *pFP )
	{
		Dbg_MsgAssert( pFP, ( "nullptr pFP sent to GetFilePosition" ));

		if( PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_get_file_position((PreFile::FileHandle*)pFP );
			if( PreMgr::sPreExecuteSuccess())
				return retval;
		}

		sFileHandle *h_file = (sFileHandle *)pFP;
		return h_file->file.tellg();
	}

	void InitQuickFileSystem( void )
	{
	}

	uint32	CanFileBeLoadedQuickly( const char* filename )
	{
		(void)filename;
		return 0;
	}

	bool LoadFileQuicklyPlease( const char* filename, char *addr )
	{
		(void)filename;
		(void)addr;
		return false;
	}

	void StopStreaming( void )
	{
	}

	void UninstallFileSystem( void )
	{
	}

	bool Exist( const char *filename )
	{
		if (PreMgr::sPreEnabled())
		{
			bool retval = PreMgr::pre_fexist(filename);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}

		void *p_result = prefopen( filename, "rb" );
		if( p_result != nullptr )
		{
			Close( p_result );
		}

		return( p_result != nullptr );
	}

	void *Open( const char *filename, const char *access )
	{
		if (PreMgr::sPreEnabled())
		{
			void * retval = PreMgr::pre_fopen(filename, access);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}

		return prefopen( filename, access );
	}

	int Close( void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_fclose((PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}

		sFileHandle *h_file = (sFileHandle *)pFP;
		delete h_file;
		return 0;
	}

	size_t Read( void *addr, size_t size, size_t count, void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			size_t retval = PreMgr::pre_fread( addr, size, count, (PreFile::FileHandle*)pFP );
			if( PreMgr::sPreExecuteSuccess())
				return retval;
		}

		sFileHandle *h_file = (sFileHandle*)pFP;
		h_file->file.read((char*)addr, size * count);
		return h_file->file.gcount();
	}
	
	size_t ReadInt( void *addr, void *pFP )
	{
		return Read( addr, 4, 1, pFP );
	}

	size_t Write( const void *addr, size_t size, size_t count, void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			size_t retval = PreMgr::pre_fwrite(addr, size, count, (PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}
		return 0;
	}

	char * GetS( char *buffer, int maxlen, void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			char * retval = PreMgr::pre_fgets(buffer, maxlen, (PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}
		return nullptr;
	}

	int PutS( const char *buffer, void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_fputs(buffer, (PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}
		return 0;
	}

	int Eof( void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_feof((PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}
		return 0;
	}

	int Seek( void *pFP, long offset, int origin )
	{
		if( PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_fseek((PreFile::FileHandle *) pFP, offset, origin);
			if( PreMgr::sPreExecuteSuccess())
				return retval;
		}

		sFileHandle *h_file = (sFileHandle*)pFP;
		h_file->file.seekg(offset, (origin == SEEK_CUR) ? std::ios::cur : ((origin == SEEK_SET) ? std::ios::beg : std::ios::end));
		return h_file->file.tellg();
	}

	int Flush( void *pFP )
	{
		if (PreMgr::sPreEnabled())
		{
			int retval = PreMgr::pre_fflush((PreFile::FileHandle *) pFP);
			if (PreMgr::sPreExecuteSuccess())
				return retval;
		}
		return 0;
	}

} // namespace File


