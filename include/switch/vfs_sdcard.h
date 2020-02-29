#pragma once

#include "vfs_common.h"

#define VFS_SDCARD_PREFIX "sdcard"

bool vfsMapSdcardPath(const char* path, VfsPath* out_vpath);
