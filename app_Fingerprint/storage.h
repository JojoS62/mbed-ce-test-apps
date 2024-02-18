#pragma once

#include "SDIOBlockDevice.h"
#include "FATFileSystem.h"

extern SDIOBlockDevice bd;
extern FATFileSystem fs;

void print_dir(FileSystem *fs, const char* dirname);
