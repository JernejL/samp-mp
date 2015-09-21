#ifndef SAMPLE_GAMEMODE_H
#define SAMPLE_GAMEMODE_H

#include "sampgdk.h"

#endif

//#define NO_EVENT_HANDLER

#pragma warning (disable:4756) // 4756 = overflow in constant arthimetic (inf / nan needs it)
#pragma warning (disable:4056) // overflow in floating - point constant arithmetic
#pragma warning (disable:4244) // conversion from 'double' to 'float', possible loss of data - math uses double for many functions so we turn these warnings off.

#include "time.h"
#include "sstream"
#include "fstream"
#include "iostream"
#include "vector"
#include "queue"
#include "list"
#include "string.h"
#include <limits.h>

#define max_players 1000

#define invalid_time -1

#define TickCount GetTickCount

struct WaterDataTriQuad {
	float zlevel;
	float pointmin[2];
	float pointmax[2];

	float pointa[2];
	float pointb[2];
	float pointc[2];

};

struct WaterDataQuad {
	float zlevel;
	float pointmin[2];
	float pointmax[2];
};

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
	SCRIPT_NATIVE nat_MPGetPlayerVehicleOtherDriver(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPVecLength(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPFVecLength(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPFDistance(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetVehicleOccupantCnt(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetVehicleNonDriverTeamOccupants(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPGetVehicleSurfersCnt(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPProjectPointOnPlayer(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPProjectPointXYZA(AMX* amx, cell* params);

	SCRIPT_NATIVE nat_MPWithinRange(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPPtInRect2D(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_MPPtInRect3D(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_GetTimeDistance(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_GetTimeMs(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_GetPlayerIdleTime(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_GetPlayerTimeSinceUpdate(AMX* amx, cell* params);

	SCRIPT_NATIVE nat_IsBitSet(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_BitToOn(AMX* amx, cell* params);
	SCRIPT_NATIVE nat_BitToOff(AMX* amx, cell* params);

	SCRIPT_NATIVE nat_UtilGetWaterZlevel(AMX* amx, cell* params);

#ifndef NO_EVENT_HANDLER

	#include <sampgdk/eventhandler.h>

	class samputils : public sampgdk::EventHandler {
	public:
		// track vehicle changes
		int oldvehicleid[max_players];
		int oldseatid[max_players];
		int oldvehiclechangetime[max_players];

		// track animation changes
		
		int LastAnimSetTime[max_players];
		int oldanim[max_players];

		// input handler
		int oldkeys[max_players];
		int oldupdown[max_players];
		int oldleftright[max_players];

		samputils();
		virtual ~samputils();
		//virtual int getidletime(int playerid);
		virtual void OnGameModeInit();
		virtual bool OnPlayerConnect(int playerid);
		virtual bool OnPlayerUpdate(int playerid);
		virtual bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys);
	};



#endif