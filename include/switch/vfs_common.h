#pragma once

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <switch.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum {
	VfsPathType_Invalid,
	VfsPathType_Root,
	VfsPathType_Physical,
	VfsPathType_AmsContents,
	VfsPathType_SaveData,
} VfsPathType;

typedef struct {
	char mapped[FS_MAX_PATH];
	VfsPathType type;
	AccountUid uid;
	u64 tid;
} VfsPath;

typedef struct {
	const char* d_name;
	struct {
		char name_buf[0x100];
	} _;
} VfsDirEntry;

typedef struct {
	struct {
		VfsDirEntry entry;
		VfsPath vpath;
		u32 index;
		union {
			DIR* dp;
			FsSaveDataInfoReader saveInfoReader;
			struct {
				AccountUid uids[ACC_USER_LIST_SIZE];
				s32 total;
			} accounts;
		};
	} _;
} VfsDir;

typedef struct stat VfsStat;

typedef struct {
	struct {
		FILE* fp;
		VfsPath vpath;
	} _;
} VfsFile;

const char* vfsPathPrefix(const char* str, const char* prefix);
const char* vfsPathPrefixU64(const char* path, u64* out_uint, char* out_str, bool end, bool endPad);
const char* vfsFormatTidFolder(const char* folder, char* buf, size_t size);

void vfsStatFillDummyDir(VfsStat* out_stat);

static inline void vfsSetErrno(int new_errno) {
	if(errno == 0) {
		errno = new_errno;
	}
}
