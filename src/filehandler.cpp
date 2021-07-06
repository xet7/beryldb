/*
 * BerylDB - A lightweight database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 - Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"

#include <fstream>
#include <dirent.h>

FileLoader::FileLoader(const std::string& filename)
{
	Load(filename);
}

void FileLoader::Load(const std::string& filename)
{
	cached_config::const_iterator it = Kernel->Config->Files.find(filename);
	
	if (it != Kernel->Config->Files.end())
	{
		this->lines = it->second;
	}
	else
	{
		const std::string real_name = Kernel->Config->Paths.SetWDConfig(filename);
		lines.clear();

		std::ifstream stream(real_name.c_str());
	
		if (!stream.is_open())
		{
			throw KernelException(filename + " does not exist.");
		}

		std::string line;
		
		while (std::getline(stream, line))
		{
			lines.push_back(line);
			Size += line.size() + 2;
		}

		stream.close();
	}
}

std::string FileLoader::as_string() const
{
	std::string buffer;
	
	for (file_cache::const_iterator it = this->lines.begin(); it != this->lines.end(); ++it)
	{
		buffer.append(*it);
		buffer.append("\r\n");
	}
	
	return buffer;
}

std::string FileSystem::get_real_path(const std::string& base, const std::string& fragment)
{
	if (fragment[0] == '/')
	{
		return fragment;
	}

	if (!fragment.compare(0, 2, "~/", 2))
	{
		const char* home_directory = getenv("HOME");

		if (home_directory && *home_directory)
		{
			return std::string(home_directory) + '/' + fragment.substr(2);
		}
	}

	return base + '/' + fragment;
}

bool FileSystem::Exists(const std::string& file)
{
	struct stat sb;

	if (stat(file.c_str(), &sb) == -1)
	{
		return false;
	}

	if ((sb.st_mode & S_IFDIR) > 0)
	{
		return false;
	}

	return !access(file.c_str(), F_OK);
}

bool FileSystem::AsFileList(const std::string& directory, std::vector<std::string>& entries, const std::string& match)
{
	DIR* library = opendir(directory.c_str());

	if (!library)
	{
		return false;
	}

	dirent* entry = NULL;
	
	while ((entry = readdir(library)))
	{
		if (Daemon::Match(entry->d_name, match, ascii_case_insensitive))
		{
			entries.push_back(entry->d_name);
		}
	}
	
	closedir(library);
	return true;
}

std::string FileSystem::GetName(const std::string& name)
{
	size_t pos = name.rfind('/');
	return pos == std::string::npos ? name : name.substr(++pos);
}

int FileSystem::remove_directory(const char *path) 
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int answer = -1;

   if (d) 
   {
      struct dirent *p;

      answer = 0;
      
      while (!answer && (p = readdir(d))) 
      {
          int r2 = -1;
          char *buf;
          size_t len;

          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
            }

          len = path_len + strlen(p->d_name) + 2; 
          buf = (char*)malloc(len);

          if (buf) 
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);
             
             if (!stat(buf, &statbuf)) 
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                    r2 = remove_directory(buf);
                }
                else
                {
                    r2 = unlink(buf);
                }
             }
             
             free(buf);
          }
          
          answer = r2;
      }
      
      closedir(d);
   }

   if (!answer)
   {
      answer = rmdir(path);
   }

   return answer;
}