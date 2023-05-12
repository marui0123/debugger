#include <3ds/types.h>
#include <3ds/srv.h>
#include <3ds/archive.h>
#include <3ds/services/fs.h>
#include <3ds/services/hid.h>

void    __appInit(void)
{
	// Initialize services
	srvInit();
	hidInit();

	fsInit();
	archiveMountSdmc();
}
