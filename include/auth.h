#pragma once

#include <stdbool.h>
#include <string.h>

#define AUTH_USER "switch"
#define AUTH_PASSWORD "switch"

typedef struct {
	bool userSet;
	bool userOk;
	bool passSet;
	bool passOk;
} AuthSessionStatus;

static inline void authInit(AuthSessionStatus* status) {
	status->userSet = false;
	status->userOk = false;
	status->passSet = false;
	status->passOk = false;
}

static inline bool authIsAuthenticated(const AuthSessionStatus* status) {
	return status->userOk && status->passOk;
}

static inline bool authIsComplete(const AuthSessionStatus* status) {
	return status->userSet && status->passSet;
}

static inline void authFillUser(AuthSessionStatus* status, const char* user) {
	status->userSet = true;
	status->userOk = (strcmp(user, AUTH_USER) == 0);
}

static inline void authFillPassword(AuthSessionStatus* status, const char* pass) {
	status->passSet = true;
	status->passOk = (strcmp(pass, AUTH_PASSWORD) == 0);
}

static inline bool authCommandNeedsAuth(const char* command) {
	return command && strcasecmp("USER", command) && strcasecmp("PASS", command) && strcasecmp("QUIT", command);
}
