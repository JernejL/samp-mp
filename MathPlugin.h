#ifndef SAMPLE_GAMEMODE_H
#define SAMPLE_GAMEMODE_H

#include "samp/samp.h"

#endif

#define CHECK_PARAM_COUNT(sFunc, nCount) \
		{ if (params[0] != nCount * sizeof(cell)) { \
			logprintf ("* ERROR -> PARAMETER-COUNT != " #nCount " (" #sFunc ")."); \
			return 0; } }

// Allows us to easily define natives..
#define SCRIPT_NATIVE static cell AMX_NATIVE_CALL


// ----------------==========================================--------------- |
//									Constants
// ----------------==========================================--------------- |

#define MAX_SNAPSHOTS 60
#define camera_actor_range_threshhold 0.75
#define camera_actor_seek_radius 50.0

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
	SCRIPT_NATIVE nat_returnit(AMX* amx, cell* params);


