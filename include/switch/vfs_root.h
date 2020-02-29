#pragma once

#include "vfs_common.h"

bool vfsMapRootPath(const char* path, VfsPath* out_vpath);
bool vfsDirOpenRoot(VfsDir* dir);
bool vfsDirReadRoot(VfsDir* dir);
bool vfsStatRoot(VfsPath* vpath, VfsStat* out_stat);
