#include "switch/vfs_savedata.h"

#include <string.h>

#include "switch/savedata.h"

bool vfsMapSavedataPath(const char* path, VfsPath* out_vpath) {
	const char* mappedPath = vfsPathPrefix(path, VFS_SAVEDATA_PREFIX);

	if(!mappedPath) {
		return false;
	}

	out_vpath->type = VfsPathType_SaveData;
	out_vpath->uid.uid[0] = 0;
	out_vpath->uid.uid[1] = 0;
	char buf[17];

	if(*mappedPath) {
		if(!(mappedPath = vfsPathPrefixU64(mappedPath, &out_vpath->uid.uid[0], buf, false, false))) {
			return false;
		}
		if(!(mappedPath = vfsPathPrefixU64(mappedPath, &out_vpath->uid.uid[1], buf, true, true))) {
			return false;
		}
	}

	if(*mappedPath) {
		if(!(mappedPath = vfsPathPrefixU64(mappedPath, &out_vpath->tid, buf, true, true))) {
			return false;
		}

		const char* deviceName = savedataMount(out_vpath->tid, &out_vpath->uid);
		if(!deviceName) {
			return false;
		}

		snprintf(out_vpath->mapped, sizeof(out_vpath->mapped), "%s:/%s", deviceName, mappedPath);
		out_vpath->type = VfsPathType_Physical;
	}

	return true;
}

bool vfsUnmapSavedataPath(VfsPath* vpath) {
	if(!vpath->tid || !accountUidIsValid(&vpath->uid)) {
		return false;
	}

	savedataUnmount(vpath->tid, &vpath->uid);
	return true;
}

bool vfsDirOpenSavedata(VfsDir* dir) {
	Result rc;

	if(!accountUidIsValid(&dir->_.vpath.uid)) {
		rc = accountListAllUsers(&dir->_.accounts.uids[0], ACC_USER_LIST_SIZE, &dir->_.accounts.total);
		if(R_FAILED(rc)) {
			return false;
		}

		dir->_.index = 0;
		return true;
	}

	rc = fsOpenSaveDataInfoReader(&dir->_.saveInfoReader, FsSaveDataSpaceId_User);
	if(R_FAILED(rc)) {
		return false;
	}

	return true;
}

static bool _readSaveInfo(FsSaveDataInfoReader* reader, const AccountUid* uid, u64* out_tid) {
	Result rc;
	s64 total;
	FsSaveDataInfo info;

	do {
		rc = fsSaveDataInfoReaderRead(reader, &info, 1, &total);
		if(R_FAILED(rc) || total < 1) {
			return false;
		}
	} while(
		info.save_data_type != FsSaveDataType_Account 
		|| info.uid.uid[0] != uid->uid[0] 
		|| info.uid.uid[1] != uid->uid[1]
	);

	*out_tid = info.application_id;
	return true;
}

bool vfsDirReadSavedata(VfsDir* dir) {
	Result rc;

	if(!accountUidIsValid(&dir->_.vpath.uid)) {
		if(dir->_.index >= dir->_.accounts.total) {
			return false;
		}

		AccountProfile profile;
		AccountProfileBase profileInfos;

		rc = accountGetProfile(&profile, dir->_.accounts.uids[dir->_.index]);
		if(R_SUCCEEDED(rc)) {
			rc = accountProfileGet(&profile, NULL, &profileInfos);
			accountProfileClose(&profile);
		}
		if(R_SUCCEEDED(rc)) {
			snprintf(dir->_.entry._.name_buf, sizeof(dir->_.entry._.name_buf), "%016lx%016lx-%s", dir->_.accounts.uids[dir->_.index].uid[0], dir->_.accounts.uids[dir->_.index].uid[1], profileInfos.nickname);
		} else {
			snprintf(dir->_.entry._.name_buf, sizeof(dir->_.entry._.name_buf), "%016lx%016lx", dir->_.accounts.uids[dir->_.index].uid[0], dir->_.accounts.uids[dir->_.index].uid[1]);
		}

		dir->_.entry.d_name = dir->_.entry._.name_buf;
		dir->_.index++;
		return true;
	}

	u64 tid;
	if(!_readSaveInfo(&dir->_.saveInfoReader, &dir->_.vpath.uid, &tid)) {
		return false;
	}

	char tidStr[17];
	snprintf(tidStr, sizeof(tidStr), "%016lX", tid);

	vfsFormatTidFolder(tidStr, dir->_.entry._.name_buf, sizeof(dir->_.entry._.name_buf));

	dir->_.entry.d_name = dir->_.entry._.name_buf;

	return true;
}

bool vfsStatSaveData(VfsPath* vpath, VfsStat* out_stat) {
	vfsStatFillDummyDir(out_stat);
	return true;
}
