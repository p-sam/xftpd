#include "switch/vfs_sdcard.h"

bool vfsMapSdcardPath(const char* path, VfsPath* out_vpath) {
	const char* mappedPath = vfsPathPrefix(path, VFS_SDCARD_PREFIX);

	if(!mappedPath) {
		return false;
	}

	snprintf(out_vpath->mapped, sizeof(out_vpath->mapped), "sdmc:/%s", mappedPath);
	out_vpath->type = VfsPathType_Physical;

	return true;
}
