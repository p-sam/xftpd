#include "switch/vfs.h"

#include "switch/savedata.h"
#include "switch/vfs_ams_contents.h"
#include "switch/vfs_physical.h"
#include "switch/vfs_root.h"
#include "switch/vfs_savedata.h"
#include "switch/vfs_sdcard.h"

u32 __nx_fs_num_sessions = 5;

void vfsPathMap(const char* path, VfsPath* out_vpath) {
	if(*path != '/') {
		out_vpath->type = VfsPathType_Invalid;
		return;
	}
	path++;

	if(vfsMapRootPath(path, out_vpath)) {
		return;
	}

	if(vfsMapSdcardPath(path, out_vpath)) {
		return;
	}

	if(vfsMapAmsContentsPath(path, out_vpath)) {
		return;
	}

	if(vfsMapSavedataPath(path, out_vpath)) {
		return;
	}

	out_vpath->type = VfsPathType_Invalid;
}

void vfsPathUnmap(VfsPath* vpath) {
	vfsUnmapSavedataPath(vpath);
}

VfsDir* vfsDirOpen(const char* path) {
	VfsDir* dir = (VfsDir*)malloc(sizeof(VfsDir));
	if(!dir) {
		return NULL;
	}

	vfsPathMap(path, &dir->_.vpath);
	if(dir->_.vpath.type == VfsPathType_Root && vfsDirOpenRoot(dir)) {
		return dir;
	}

	if(dir->_.vpath.type == VfsPathType_Physical && vfsDirOpenPhysical(dir)) {
		return dir;
	}

	if(dir->_.vpath.type == VfsPathType_AmsContents && vfsDirOpenAmsContents(dir)) {
		return dir;
	}

	if(dir->_.vpath.type == VfsPathType_SaveData && vfsDirOpenSavedata(dir)) {
		return dir;
	}

	vfsSetErrno(EINVAL);
	vfsDirClose(dir);
	return NULL;
}

VfsDirEntry* vfsDirRead(VfsDir* dir) {
	if(dir->_.vpath.type == VfsPathType_Root && vfsDirReadRoot(dir)) {
		return &dir->_.entry;
	}

	if(dir->_.vpath.type == VfsPathType_Physical && vfsDirReadPhysical(dir)) {
		return &dir->_.entry;
	}

	if(dir->_.vpath.type == VfsPathType_AmsContents && vfsDirReadAmsContents(dir)) {
		return &dir->_.entry;
	}

	if(dir->_.vpath.type == VfsPathType_SaveData && vfsDirReadSavedata(dir)) {
		return &dir->_.entry;
	}

	return NULL;
}

void vfsDirClose(VfsDir* dir) {
	if(dir->_.vpath.type == VfsPathType_Physical) {
		vfsDirClosePhysical(dir);
	}

	if(dir->_.vpath.type == VfsPathType_AmsContents) {
		vfsDirCloseAmsContents(dir);
	}

	vfsPathUnmap(&dir->_.vpath);
	free(dir);
}

static inline int _vfsDirCreate(VfsPath* vpath, mode_t mode) {
	if(vpath->type == VfsPathType_Physical && vfsDirCreatePhysical(vpath, mode)) {
		return 0;
	}

	vfsSetErrno(EINVAL);
	return -1;
}

int vfsDirCreate(const char* path, mode_t mode) {
	VfsPath vpath;
	vfsPathMap(path, &vpath);

	int res = _vfsDirCreate(&vpath, mode);

	vfsPathUnmap(&vpath);

	return res;
}

static inline int _vfsDirRemove(VfsPath* vpath) {
	if(vpath->type == VfsPathType_Physical && vfsDirRemovePhysical(vpath)) {
		return 0;
	}

	vfsSetErrno(EINVAL);
	return -1;
}

int vfsDirRemove(const char* path) {
	VfsPath vpath;
	vfsPathMap(path, &vpath);

	int res = _vfsDirRemove(&vpath);

	vfsPathUnmap(&vpath);

	return res;
}

static inline int _vfsStat(VfsPath* vpath, VfsStat* out_stat) {
	if(vpath->type == VfsPathType_Root && vfsStatRoot(vpath, out_stat)) {
		return 0;
	}

	if(vpath->type == VfsPathType_Physical && vfsStatPhysical(vpath, out_stat)) {
		return 0;
	}

	if(vpath->type == VfsPathType_AmsContents && vfsStatAmsContents(vpath, out_stat)) {
		return 0;
	}

	if(vpath->type == VfsPathType_SaveData && vfsStatSaveData(vpath, out_stat)) {
		return 0;
	}

	vfsSetErrno(EINVAL);
	return -1;
}

int vfsStat(const char* path, VfsStat* out_stat) {
	VfsPath vpath;
	vfsPathMap(path, &vpath);

	int res = _vfsStat(&vpath, out_stat);

	vfsPathUnmap(&vpath);

	return res;
}

static inline int _vfsUnlink(VfsPath* vpath) {
	if(vpath->type == VfsPathType_Physical && vfsUnlinkPhysical(vpath)) {
		return 0;
	}

	vfsSetErrno(EINVAL);
	return -1;
}

int vfsUnlink(const char* path) {
	VfsPath vpath;
	vfsPathMap(path, &vpath);

	int res = _vfsUnlink(&vpath);

	vfsPathUnmap(&vpath);

	return res;
}

static inline int _vfsRename(VfsPath* from, VfsPath* to) {
	if(from->type == to->type) {
		if(from->type == VfsPathType_Physical && vfsRenamePhysical(from, to)) {
			return 0;
		}
	}

	vfsSetErrno(EINVAL);
	return -1;
}

int vfsRename(const char* from, const char* to) {
	VfsPath vpathFrom;
	VfsPath vpathTo;
	vfsPathMap(from, &vpathFrom);
	vfsPathMap(to, &vpathTo);

	int res = _vfsRename(&vpathFrom, &vpathTo);

	vfsPathUnmap(&vpathFrom);
	vfsPathUnmap(&vpathTo);

	return res;
}

VfsFile* vfsFileOpen(const char* path, const char* mode) {
	VfsFile* f = (VfsFile*)malloc(sizeof(VfsFile));
	if(!f) {
		return NULL;
	}

	vfsPathMap(path, &f->_.vpath);

	if(f->_.vpath.type == VfsPathType_Physical && vfsFileOpenPhysical(f, mode)) {
		return f;
	}

	vfsSetErrno(EINVAL);
	vfsFileClose(f);
	return NULL;
}

void vfsFileClose(VfsFile* f) {
	if(f->_.vpath.type == VfsPathType_Physical) {
		vfsFileClosePhysical(f);
	}

	vfsPathUnmap(&f->_.vpath);
	free(f);
}

bool vfsInit() {
	if(R_FAILED(nsInitialize())) {
		return false;
	}
	if(R_FAILED(accountInitialize(AccountServiceType_Application))) {
		return false;
	}
	return savedataInit();
}

void vfsDeinit() {
	fsSetPriority(FsPriority_Normal);
	accountExit();
	nsExit();
	savedataDeinit();
}
