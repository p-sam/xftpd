#pragma once

#include "switch/vfs_common.h"
#include "switch/vfs_physical.h"

void vfsPathMap(const char* path, VfsPath* out_vpath);

VfsDir* vfsDirOpen(const char* path);
VfsDirEntry* vfsDirRead(VfsDir* dir);
void vfsDirClose(VfsDir* dir);
int vfsDirCreate(const char* path, mode_t mode);
int vfsDirRemove(const char* path);

int vfsStat(const char* path, VfsStat* out_stat);
int vfsUnlink(const char* path);
int vfsRename(const char* from, const char* to);

VfsFile* vfsFileOpen(const char* path, const char* mode);
void vfsFileClose(VfsFile* f);

bool vfsInit();
void vfsDeinit();

static inline int vfsFileStat(VfsFile* f, VfsStat* out_stat) {
	return vfsFileStatPhysical(f, out_stat);
}

static inline size_t vfsFileRead(void* dest, size_t size, size_t count, VfsFile* f) {
	return vfsFileReadPhysical(dest, size, count, f);
}

static inline int vfsFileSeek(VfsFile* f, long int offset, int origin) {
	return vfsFileSeekPhysical(f, offset, origin);
}

static inline size_t vfsFileWrite(const void* data, size_t size, size_t count, VfsFile* f) {
	return vfsFileWritePhysical(data, size, count, f);
}

static inline size_t vfsFileSetBuffer(VfsFile* f, void* buf, size_t size) {
	return vfsFileSetBufferPhysical(f, buf, size);
}
