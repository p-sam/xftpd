#pragma once

#include "vfs_common.h"
#include "vfs_physical.h"

#define VFS_AMS_CONTENTS_PREFIX "ams_contents"
#define VFS_AMS_CONTENTS_ROOT "sdmc:/atmosphere/contents"

bool vfsMapAmsContentsPath(const char* path, VfsPath* out_vpath);
bool vfsDirReadAmsContents(VfsDir* dir);

static inline bool vfsDirOpenAmsContents(VfsDir* dir) {
	return vfsDirOpenPhysical(dir);
}

static inline void vfsDirCloseAmsContents(VfsDir* dir) {
	vfsDirClosePhysical(dir);
}

static inline bool vfsStatAmsContents(VfsPath* vpath, VfsStat* out_stat) {
	return vfsStatPhysical(vpath, out_stat);
}
