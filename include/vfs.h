#pragma once

#ifdef __SWITCH__

#include "switch/vfs.h"

#else

#ifdef _3DS
#define lstat stat
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

typedef DIR VfsDir;
typedef struct dirent VfsDirEntry;
typedef struct stat VfsStat;
typedef FILE VfsFile;

static inline VfsDir* vfsDirOpen(const char* path) {
	return opendir(path);
}
static inline VfsDirEntry* vfsDirRead(VfsDir* dir) {
	return readdir(dir);
}
static inline void vfsDirClose(VfsDir* dir) {
	closedir(dir);
}

static inline int vfsDirCreate(const char* path, mode_t mode) {
	return mkdir(path, mode);
}
static inline int vfsDirRemove(const char* path) {
	return rmdir(path);
}

static inline int vfsStat(const char* path, VfsStat* out_stat) {
	return lstat(path, out_stat);
}

static inline int vfsUnlink(const char* path) {
	return unlink(path);
}

static inline int vfsRename(const char* from, const char* to) {
	return rename(from, to);
}


static inline VfsFile* vfsFileOpen(const char* path, const char* mode) {
	return fopen(path, mode);
}
static inline void vfsFileClose(VfsFile* f) {
	fclose(f);
}

static inline bool vfsInit() {
	return true;
}
static inline void vfsDeinit() {}

static inline int vfsFileStat(VfsFile* f, VfsStat* out_stat) {
	return fstat(fileno(f), out_stat);
}

static inline size_t vfsFileRead(void* dest, size_t size, size_t count, VfsFile* f) {
	return fread(dest, size, count, f);
}
static inline int vfsFileSeek(VfsFile* f, long int offset, int origin) {
	return fseek(f, offset, origin);
}
static inline size_t vfsFileWrite(const void* data, size_t size, size_t count, VfsFile* f) {
	return fwrite(data, size, count, f);
}

static inline size_t vfsFileSetBuffer(VfsFile* f, void* buf, size_t size) {
	return setvbuf(f, buf, _IOFBF, size);
}

#endif
