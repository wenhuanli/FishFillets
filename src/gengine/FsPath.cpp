/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "FsPath.h"

#include "PathException.h"

#include <filesystem>

namespace fs = std::filesystem;

//-----------------------------------------------------------------
/**
 * Get native filename.
 * @param file posix filename
 */
    std::string
FsPath::getNative(const std::string &file)
{
    return fs::path(file).make_preferred().string();
}
//-----------------------------------------------------------------
/**
 * Returns true when file or directory exists.
 * @param file posix filename
 */
    bool
FsPath::exists(const std::string &file)
{
    if (file.empty()) {
        return true;
    }

    return fs::exists(file);
}
//-----------------------------------------------------------------
/**
 * Joint two paths.
 * @param dir posix filename
 * @param file posix filename
 * @return "dir/file"
 */
    std::string
FsPath::join(const std::string &dir, const std::string &file)
{
    if (dir.empty()) {
        return file;
    }

    return (fs::path(dir) / file).generic_string();
}
//-----------------------------------------------------------------
/**
 * Create all directories in path (like "mkdir -p").
 * @param file posix filename
 * @throws PathException when the directories cannot be created
 */
void
FsPath::createPath(const std::string &file)
{
    fs::path parent = fs::path(file).parent_path();
    if (parent.empty()) {
        return;
    }

    std::error_code error;
    fs::create_directories(parent, error);
    if (error) {
        throw PathException(ExInfo("cannot create dir")
                .addInfo("error", error.message())
                .addInfo("dir", parent.string()));
    }
}
