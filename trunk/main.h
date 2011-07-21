// ----------------==========================================--------------- |
//									Macros
// ----------------==========================================--------------- |
	// Makes it easier to check the parameter count of 'natives', and
	// immediately report it's wrong & exit function.
	#define CHECK_PARAM_COUNT(sFunc, nCount) \
			{ if (params[0] != nCount * sizeof(cell)) { \
				logprintf ("* ERROR -> PARAMETER-COUNT != " #nCount " (" #sFunc ")."); \
				return 0; } }

	// Allows us to easily define natives..
	#define SCRIPT_NATIVE static cell AMX_NATIVE_CALL

#ifdef _WIN32
	#include <windows.h>
#endif
#include <algorithm>
#include <bitset>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <set>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

// ----------------==========================================--------------- |
//									Constants
// ----------------==========================================--------------- |

#define max_players 500
#define MAX_SNAPSHOTS 60
#define camera_actor_range_threshhold 0.75
#define camera_actor_seek_radius 50.0

#define MAX_PLAYER_NAME							24
#define MAX_PLAYERS								500
#define MAX_VEHICLES							2000
#define INVALID_PLAYER_ID						0xFFFF
#define INVALID_VEHICLE_ID						0xFFFF

#define PLAYER_STATE_NONE						0
#define PLAYER_STATE_ONFOOT						1
#define PLAYER_STATE_DRIVER						2
#define PLAYER_STATE_PASSENGER					3

// ----------------==========================================--------------- |
//								User defined types
// ----------------==========================================--------------- |

struct psnapshot {

    unsigned SnapTime;

    float Position[3];
    float Velocity[3];

    float CamPos[3];
    float CamFront[3];

    float groundlevel;

    float Heading;
    uint16_t vehicleid;

};

// ----------------==========================================--------------- |
//								Function Prototypes
// ----------------==========================================--------------- |
	typedef void (*logprintf_t)(char* format, ...);

    bool ACProcessUpdate(int playerid);
    bool ACPlayerConnect(int playerid);
    bool nat_MPInit();

	/*
	 * Natives
	 */
    SCRIPT_NATIVE nat_MPPLogAdd(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPPLogConnect(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPInit(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetVehicleDriver(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetVehicleDriverCount(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPCrossProduct(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistanceCameraTargetToLocation(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPProjectPointOnVehicle(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetAimTarget(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistancePointLine(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetVehicleUpsideDown(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistance(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_FMPDistance(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPInterpolatePoint(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPFSQRT(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPFNormalize(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPClamp360(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDotProduct(AMX* amx, cell* params);


