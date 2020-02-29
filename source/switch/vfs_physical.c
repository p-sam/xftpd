#include "switch/vfs_physical.h"

bool vfsDirOpenPhysical(VfsDir* dir) {
	DIR* dp = opendir(dir->_.vpath.mapped);
	if(!dp) {
		return false;
	}

	dir->_.dp = dp;
	return true;
}

bool vfsDirReadPhysical(VfsDir* dir) {
	struct dirent* dent = readdir(dir->_.dp);
	if(!dent) {
		return false;
	}
	dir->_.entry.d_name = dent->d_name;

	return true;
}

bool vfsFileOpenPhysical(VfsFile* f, const char* mode) {
	FILE* fp = fopen(f->_.vpath.mapped, mode);
	if(!fp) {
		return false;
	}

	f->_.fp = fp;
	return true;
}
