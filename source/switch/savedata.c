#include "switch/savedata.h"

#include <map.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef map_t(SavedataMount*) savedata_map_t;
static savedata_map_t g_mount_table;
static bool g_initialized = false;

static inline SavedataMount* _tableGet(const SavedataKey* key) {
	SavedataMount** p = map_get(&g_mount_table, key->str);
	if(p && *p) {
		return *p;
	}

	return NULL;
}

static inline bool _tableSet(const SavedataKey* key, SavedataMount* v) {
	return map_set(&g_mount_table, key->str, v) == 0;
}

static inline void _tableDel(const SavedataKey* key) {
	map_remove(&g_mount_table, key->str);
}

static bool _makeKey(u64 tid, const AccountUid* uid, SavedataKey* out_key) {
	if(!accountUidIsValid(uid) || !tid) {
		return false;
	}

	snprintf(out_key->str, sizeof(out_key->str), "%016lx%016lx%016lx", tid, uid->uid[0], uid->uid[1]);
	return true;
}

bool savedataInit() {
	if(!g_initialized) {
		map_init(&g_mount_table);
		g_initialized = true;
	}
	return true;
}

void savedataDeinit() {
	if(!g_initialized) {
		return;
	}
	const char* key;
	map_iter_t iter = map_iter(&g_mount_table);

	while((key = map_next(&g_mount_table, &iter))) {
		free(*map_get(&g_mount_table, key));
	}

	map_deinit(&g_mount_table);
	g_initialized = false;
}

const char* savedataMount(u64 tid, const AccountUid* uid) {
	SavedataKey key;
	if(!_makeKey(tid, uid, &key)) {
		return NULL;
	}

	SavedataMount* mount = _tableGet(&key);
	if(mount) {
		mount->refs++;
		return mount->fsdev_name;
	}

	mount = (SavedataMount*)malloc(sizeof(SavedataMount));
	if(!mount) {
		return NULL;
	}
	snprintf(mount->fsdev_name, sizeof(mount->fsdev_name), "save-%lx", armGetSystemTick());
	mount->refs = 1;

	if(_tableSet(&key, mount)) {
		if(R_SUCCEEDED(fsdevMountSaveData(mount->fsdev_name, tid, *uid))) {
			return mount->fsdev_name;
		}
		_tableDel(&key);
	}

	free(mount);
	return NULL;
}

void savedataUnmount(u64 tid, const AccountUid* uid) {
	SavedataKey key;
	if(!_makeKey(tid, uid, &key)) {
		return;
	}

	SavedataMount* mount = _tableGet(&key);
	if(!mount) {
		return;
	}

	mount->refs--;
	if(mount->refs <= 0) {
		fsdevCommitDevice(mount->fsdev_name);
		fsdevUnmountDevice(mount->fsdev_name);
		_tableDel(&key);
	}
}
