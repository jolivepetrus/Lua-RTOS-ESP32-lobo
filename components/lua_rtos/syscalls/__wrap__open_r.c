#include "esp_attr.h"

#include <limits.h>
#include <reent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/mount.h>

extern int __real__open_r(struct _reent *r, const char *path, int flags, int mode);

int IRAM_ATTR __wrap__open_r(struct _reent *r, const char *path, int flags, int mode) {
	char *ppath;
	int res;

	uart_writes(1, "1\r\n");
	if ((strncmp(path,"/dev/uart/",10) == 0) || (strncmp(path,"/dev/tty/",9) == 0) || (strncmp(path,"/dev/socket/",12) == 0)) {
		uart_writes(1, "2\r\n");
		return __real__open_r(r, path, flags, mode);
	} else {
		ppath = mount_resolve_to_physical(path);
		res = __real__open_r(r, ppath, flags, mode);
		free(ppath);
		return res;
	}
}
