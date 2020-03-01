#include "switch/vfs_ams_contents.h"

#include <string.h>

#include "switch/hos_tid.h"

bool vfsMapAmsContentsPath(const char* path, VfsPath* out_vpath) {
	const char* mappedPath = vfsPathPrefix(path, VFS_AMS_CONTENTS_PREFIX);

	if(!mappedPath) {
		return false;
	}

	if(!*mappedPath) {
		out_vpath->type = VfsPathType_AmsContents;
		strncpy(out_vpath->mapped, VFS_AMS_CONTENTS_ROOT, sizeof(out_vpath->mapped));
		return true;
	}

	char tidStr[17];
	u64 tid;
	mappedPath = vfsPathPrefixU64(mappedPath, &tid, tidStr, true, true);

	if(!mappedPath) {
		return false;
	}

	if(*mappedPath) {
		snprintf(out_vpath->mapped, sizeof(out_vpath->mapped), "%s/%s/%s", VFS_AMS_CONTENTS_ROOT, tidStr, mappedPath);
	} else {
		snprintf(out_vpath->mapped, sizeof(out_vpath->mapped), "%s/%s", VFS_AMS_CONTENTS_ROOT, tidStr);
	}
	out_vpath->type = VfsPathType_Physical;

	return true;
}

bool vfsDirReadAmsContents(VfsDir* dir) {
	if(!vfsDirReadPhysical(dir)) {
		return false;
	}

	const char* formatted = vfsFormatTidFolder(dir->_.entry.d_name, dir->_.entry._.name_buf, sizeof(dir->_.entry._.name_buf));

	if(!formatted) {
		return false;
	}

	dir->_.entry.d_name = formatted;

	return true;
}
