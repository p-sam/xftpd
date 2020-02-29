#include "switch/vfs_common.h"

#include <string.h>

#include "switch/hos_tid.h"

static bool _charIsAlphaNum(char c) {
	if(c >= '0' && c <= '9') {
		return true;
	}

	if(c >= 'A' && c <= 'Z') {
		return true;
	}

	if(c >= 'a' && c <= 'z') {
		return true;
	}

	return false;
}

const char* vfsPathPrefix(const char* path, const char* prefix) {
	while(*prefix) {
		if(*prefix != *path) {
			return NULL;
		}
		prefix++;
		path++;
	}

	if(*path == '/') {
		path++;
	} else if(*path) {
		return NULL;
	}

	return path;
}

const char* vfsPathPrefixU64(const char* path, u64* out_uint, char* out_str, bool atEnd, bool padAtEnd) {
	u32 i = 0;
	while(*path && *path != '/') {
		if(i < 16) {
			out_str[i] = *path;
		} else if(i == 16) {
			if(atEnd && (!padAtEnd || *path != '-')) {
				return NULL;
			}
			if(!atEnd) {
				break;
			}
		}
		path++;
		i++;
	}

	if(i < 16) {
		return NULL;
	}
	out_str[16] = 0;
	if(atEnd) {
		if(*path == '/') {
			path++;
		} else if(*path) {
			return NULL;
		}
	}

	char* end;
	*out_uint = strtoul(out_str, &end, 16);
	if((end - out_str) != 16 || !*out_uint) {
		return NULL;
	}

	return path;
}

const char* vfsFormatTidFolder(const char* folder, char* buf, size_t size) {
	char* end;
	u64 tid = strtoul(folder, &end, 16);

	if(*end || (end - folder) != 16) {
		return false;
	}

	if((tid & 0xFFFF0000000000FF) == 0x00FF0000000000FF) {
		char code[6] = {0};
		char* c = &code[0];

		for(u8 i = 0; i < 5; i++) {
			*c = (tid >> 8 * (5 - i)) & 0xFF;
			if(_charIsAlphaNum(*c)) {
				c++;
			}
		}
		*c = 0;
		snprintf(buf, size, "%s-sys-%s", folder, code);
		return buf;
	}

	const char* hosName = hosTidFindName(tid);
	if(hosName) {
		snprintf(buf, size, "%s-hos-%s", folder, hosName);
		return buf;
	}

	NsApplicationControlData controlData;
	NacpLanguageEntry* langEntry = NULL;
	size_t controlSize;

	if(
		R_SUCCEEDED(nsGetApplicationControlData(NsApplicationControlSource_Storage, tid, &controlData, sizeof(controlData), &controlSize))
		&& R_SUCCEEDED(nacpGetLanguageEntry(&controlData.nacp, &langEntry))
		&& langEntry->name
	) {
		snprintf(buf, size, "%s-app-%s", folder, langEntry->name);
		return buf;
	}

	return folder;
}

void vfsStatFillDummyDir(VfsStat* out_stat) {
	memset(out_stat, 0, sizeof(VfsStat));
	out_stat->st_mode = S_IFDIR | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
	out_stat->st_nlink = 1;
	out_stat->st_uid = 1000;
	out_stat->st_gid = 1000;
}
