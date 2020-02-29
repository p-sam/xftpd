#pragma once

#include "vfs_common.h"

bool vfsDirOpenPhysical(VfsDir* dir);
bool vfsDirReadPhysical(VfsDir* dir);
static inline void vfsDirClosePhysical(VfsDir* dir) {
	closedir(dir->_.dp);
}
static inline bool vfsDirCreatePhysical(VfsPath* vpath, mode_t mode) {
	return mkdir(vpath->mapped, mode) == 0;
}

static inline bool vfsDirRemovePhysical(VfsPath* vpath) {
	return rmdir(vpath->mapped) == 0;
}

static inline bool vfsStatPhysical(VfsPath* vpath, VfsStat* out_stat) {
	return stat(vpath->mapped, out_stat) == 0;
}

static inline bool vfsUnlinkPhysical(VfsPath* vpath) {
	return unlink(vpath->mapped) == 0;
}

static inline bool vfsRenamePhysical(VfsPath* from, VfsPath* to) {
	return rename(from->mapped, to->mapped) == 0;
}

bool vfsFileOpenPhysical(VfsFile* f, const char* mode);
static inline void vfsFileClosePhysical(VfsFile* f) {
	fclose(f->_.fp);
}

static inline int vfsFileStatPhysical(VfsFile* f, VfsStat* out_stat) {
	return fstat(fileno(f->_.fp), out_stat);
}

static inline size_t vfsFileReadPhysical(void* dest, size_t size, size_t count, VfsFile* f) {
	return fread(dest, size, count, f->_.fp);
}

static inline int vfsFileSeekPhysical(VfsFile* f, long int offset, int origin) {
	return fseek(f->_.fp, offset, origin);
}

static inline size_t vfsFileWritePhysical(const void* data, size_t size, size_t count, VfsFile* f) {
	return fwrite(data, size, count, f->_.fp);
}

static inline size_t vfsFileSetBufferPhysical(VfsFile* f, void* buf, size_t size) {
	return setvbuf(f->_.fp, buf, _IOFBF, size);
}
