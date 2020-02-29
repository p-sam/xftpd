#include "switch/vfs_ams_contents.h"
#include "switch/vfs_common.h"
#include "switch/vfs_savedata.h"
#include "switch/vfs_sdcard.h"

static const char* g_root_dirs[] = {
	VFS_AMS_CONTENTS_PREFIX,
	VFS_SAVEDATA_PREFIX,
	VFS_SDCARD_PREFIX,
	NULL
};

bool vfsMapRootPath(const char* path, VfsPath* out_vpath) {
	if(!*path) {
		out_vpath->type = VfsPathType_Root;
		return true;
	}

	return false;
}

bool vfsDirOpenRoot(VfsDir* dir) {
	dir->_.index = 0;
	return true;
}

bool vfsDirReadRoot(VfsDir* dir) {
	if(!g_root_dirs[dir->_.index]) {
		return false;
	}

	dir->_.entry.d_name = g_root_dirs[dir->_.index];
	dir->_.index++;

	return true;
}

bool vfsStatRoot(VfsPath* vpath, VfsStat* out_stat) {
	vfsStatFillDummyDir(out_stat);
	return true;
}
