#ifndef SAMPLE_GAMEMODE_H
#define SAMPLE_GAMEMODE_H

#include "sampgdk.h"

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

#define camera_actor_range_threshhold 0.75
#define PI 3.14159265

	/*
	 * Natives
	 */

    SCRIPT_NATIVE nat_MPGetVehicleDriver(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetVehicleDriverCount(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPCrossProduct(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistanceCameraTargetToLocation(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPProjectPointOnVehicle(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetAimTarget(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistancePointLine(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPGetVehicleUpsideDown(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDistance(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_returninf(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_returnnan(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_normalizefloat(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPFloatIsFinite(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPVecLength(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_FMPDistance(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPInterpolatePoint(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPFSQRT(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPFNormalize(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPClamp360(AMX* amx, cell* params);
    SCRIPT_NATIVE nat_MPDotProduct(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetTrailerTowingVehicle(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPVecLength(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPFVecLength(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPFDistance(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetVehicleOccupantCnt(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetVehicleSurfersCnt(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPProjectPointOnPlayer(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPProjectPointXYZA(AMX* amx, cell* params);

	SCRIPT_NATIVE nat_MPWithinRange(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPPtInRect2D(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPPtInRect3D(AMX* amx, cell* params);
