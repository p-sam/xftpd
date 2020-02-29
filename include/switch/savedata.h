#pragma once

#include <switch.h>

#define SAVEDATA_MAX_MOUNT 20
#define SAVEDATA_KEY_STR_LEN (sizeof(SavedataKeyElements) * 4)

typedef struct {
	u64 tid;
	AccountUid uid;
} SavedataKeyElements;

typedef struct {
	char str[SAVEDATA_KEY_STR_LEN + 1];
} SavedataKey;

typedef struct {
	char fsdev_name[0x20];
	int refs;
} SavedataMount;

bool savedataInit();
void savedataDeinit();
const char* savedataMount(u64 tid, const AccountUid* uid);
void savedataUnmount(u64 tid, const AccountUid* uid);
