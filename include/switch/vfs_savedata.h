#pragma once

#include "vfs_common.h"
#include "vfs_physical.h"

#define VFS_SAVEDATA_PREFIX "savedata"

bool vfsMapSavedataPath(const char* path, VfsPath* out_vpath);
bool vfsUnmapSavedataPath(VfsPath* vpath);
bool vfsDirOpenSavedata(VfsDir* dir);
bool vfsDirReadSavedata(VfsDir* dir);
bool vfsStatSaveData(VfsPath* vpath, VfsStat* out_stat);
