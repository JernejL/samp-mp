/*
    SA-MP Math Plugin beta
    Copyright © 2011 JernejL

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

    #include "main.h"
    #include "Invoke.h"
    #include <math.h>

    using namespace std;

//----------------------------------------------------------


	extern void* pAMXFunctions;
	logprintf_t logprintf;
	void** ppPluginData;

    int curridx[500]; // at which point is the cycle?
    psnapshot datacache[500][MAX_SNAPSHOTS];


	// And an array containing the native function-names and the functions
	// specified with them.
	AMX_NATIVE_INFO natives[ ] =
	{
	    { "MPInit",						nat_MPInit},
		{ "MPGetVehicleDriver",			nat_MPGetVehicleDriver },
		{ "MPGetVehicleDriverCount",	nat_MPGetVehicleDriverCount },
		{ "MPCrossProduct",				nat_MPCrossProduct },
		{ "MPDotProduct",				nat_MPDotProduct },
		{ "MPDistanceCameraToLocation",	nat_MPDistanceCameraTargetToLocation },
		{ "MPProjectPointOnVehicle",	nat_MPProjectPointOnVehicle},
		{ "MPGetAimTarget",				nat_MPGetAimTarget },
		{ "MPDistancePointLine",		nat_MPDistancePointLine },
		{ "MPInterpolatePoint",			nat_MPInterpolatePoint},
		{ "MPDistance",					nat_MPDistance },
		{ "MPPLogAdd",					nat_MPPLogAdd },
		{ "MPFSQRT",					nat_MPFSQRT },
		{ "MPClamp360",					nat_MPClamp360 },
		{ "MPFNormalize",				nat_MPFNormalize },
		{ "MPPLogConnect",				nat_MPPLogConnect },
		{ "MPGetVehicleUpsideDown",		nat_MPGetVehicleUpsideDown },
		{ 0,					    	0 }
	};

	const char STRING_NOT_DEFINED[] = "String not defined for language.";


//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) {
	invoke->amx_list.push_back(amx);
	return amx_Register( amx, natives, -1 );
}

//----------------------------------------------------------
// When a gamemode is over or a filterscript gets unloaded, this
// function gets called. No special actions needed in here.

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) {

	for (std::list<AMX *>::iterator i = invoke->amx_list.begin(); i != invoke->amx_list.end(); ++i)
	{
		if (* i == amx)
		{
			invoke->amx_list.erase(i);
			break;
		}
	}

	return AMX_ERR_NONE;
}

//----------------------------------------------------------
// The Load() function is called when the plugin is being
// loaded, it gets passed a pointer to (useful) plugin-data.
// Should return true if loading the plugin has succeeded.

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) {
	invoke = new Invoke;
	// Store pointer to plugin data.
	ppPluginData = ppData;
	// Store pointer to amx function table.
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	// Store pointer to logprintf() function, so we can print stuff to the
	// server console.
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("Math plugin loaded.");

	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload( ) {

	logprintf("Math plugin unloaded.");
}

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server.

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

//----------------------------------------------------------

// google 0x5f3759df
float FastSqrt(float number) {
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}

void Normalize(float &vx, float &vy, float &vz) {

	float Length;

	Length = 1 /FastSqrt(vx * vx + vy * vy + vz * vz);

	vx = vx * Length;
	vy = vy * Length;
	vz = vz * Length;
}

int GetVehicleDriver(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;

    for (i = 0; i < max_players; i++) {

        if (invoke->callNative(&PAWN::IsPlayerConnected, i) == false) // no connected player in slot?
            continue;

            if (invoke->callNative(&PAWN::GetPlayerState, i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid;
            thisplayervehid = invoke->callNative(&PAWN::GetPlayerVehicleID, i);

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                return i;

    }

    return INVALID_PLAYER_ID; // found no driver.

}

int GetVehicleDriverCount(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totaldrivers = 0;

    for (i = 0; i < max_players; i++) {

        if (invoke->callNative(&PAWN::IsPlayerConnected, i) == false) // no connected player in slot?
            continue;

            if (invoke->callNative(&PAWN::GetPlayerState, i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid;
            thisplayervehid = invoke->callNative(&PAWN::GetPlayerVehicleID, i);

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                totaldrivers++;

    }

    return totaldrivers; // return how many there are.

}

bool ACPlayerConnect(int playerid) {

    curridx[playerid] = 0;

    return true;

}

int obtainCurrentTime()
{
	#ifdef WIN32
		return static_cast<int>(GetTickCount());
	#else
		struct timeval time;
		gettimeofday(&time, NULL);
		return static_cast<int>((time.tv_sec * 1000) + (time.tv_usec / 1000));
	#endif
}

bool ACProcessUpdate(int playerid) {

    int aridx = curridx[playerid];

    datacache[playerid][ aridx ].vehicleid = invoke->callNative(&PAWN::GetPlayerVehicleID, playerid);

/*
// tickcount doesn't work - no idea why.
    int gran = 0;
    datacache[playerid][ aridx ].SnapTime = invoke->callNative(&PAWN::tickcount, &gran);
    logprintf("%d -> %d", gran, datacache[playerid][ aridx ].SnapTime);
*/
    datacache[playerid][ aridx ].SnapTime = obtainCurrentTime();//invoke->callNative(&PAWN::GetTickCount);

    if (datacache[playerid][ aridx ].vehicleid == 0) {
        invoke->callNative(&PAWN::GetPlayerPos, playerid, &datacache[playerid][ aridx ].Position[0], &datacache[playerid][ aridx ].Position[1], &datacache[playerid][ aridx ].Position[2]);
        invoke->callNative(&PAWN::GetPlayerVelocity, playerid, &datacache[playerid][ aridx ].Velocity[0], &datacache[playerid][ aridx ].Velocity[1], &datacache[playerid][ aridx ].Velocity[2]);
        invoke->callNative(&PAWN::GetPlayerFacingAngle, playerid, &datacache[playerid][ aridx ].Heading);
    } else {
        invoke->callNative(&PAWN::GetVehiclePos, datacache[playerid][ aridx ].vehicleid, &datacache[playerid][ aridx ].Position[0], &datacache[playerid][ aridx ].Position[1], &datacache[playerid][ aridx ].Position[2]);
        invoke->callNative(&PAWN::GetVehicleVelocity, datacache[playerid][ aridx ].vehicleid, &datacache[playerid][ aridx ].Velocity[0], &datacache[playerid][ aridx ].Velocity[1], &datacache[playerid][ aridx ].Velocity[2]);
        invoke->callNative(&PAWN::GetVehicleZAngle, datacache[playerid][ aridx ].vehicleid, &datacache[playerid][ aridx ].Heading);
    }


    // source of mega crashes.
    //invoke->callNative(&PAWN::MapAndreas_FindZ_For2DCoord, playerid, datacache[playerid][ aridx ].Position[0], datacache[playerid][ aridx ].Position[1], &datacache[playerid][ aridx ].groundlevel);

    invoke->callNative(&PAWN::GetPlayerCameraPos, playerid, &datacache[playerid][ aridx ].CamPos[0], &datacache[playerid][ aridx ].CamPos[1], &datacache[playerid][ aridx ].CamPos[2]);
    invoke->callNative(&PAWN::GetPlayerCameraFrontVector, playerid, &datacache[playerid][ aridx ].CamFront[0], &datacache[playerid][ aridx ].CamFront[1], &datacache[playerid][ aridx ].CamFront[2]);

/*
		ping					= GetPlayerPing(PlayerID);
		player_state			= GetPlayerState(PlayerID);
		specialaction			= GetPlayerSpecialAction(PlayerID);
		surfingcar				= GetPlayerSurfingVehicleID(PlayerID);
		onfoot_anim				= GetPlayerAnimationIndex(PlayerID);
        seatid = GetPlayerVehicleSeat(PlayerID);
*/

    logprintf("%d player %d idx %d vehicle %d heading %0.5f LOC (%0.5f %0.5f %0.5f) groundz %0.5f VELOC (%0.5f %0.5f %0.5f) CAMPOS (%0.5f %0.5f %0.5f) CAMFRONT (%0.5f %0.5f %0.5f)",
              datacache[playerid][ aridx ].SnapTime,
              playerid, aridx,
              datacache[playerid][ aridx ].vehicleid,
              datacache[playerid][ aridx ].Heading,
              datacache[playerid][ aridx ].Position[0], datacache[playerid][ aridx ].Position[1], datacache[playerid][ aridx ].Position[2],
              datacache[playerid][ aridx ].groundlevel,
              datacache[playerid][ aridx ].Velocity[0], datacache[playerid][ aridx ].Velocity[1], datacache[playerid][ aridx ].Velocity[2],
              datacache[playerid][ aridx ].CamPos[0], datacache[playerid][ aridx ].CamPos[1], datacache[playerid][ aridx ].CamPos[2],
              datacache[playerid][ aridx ].CamFront[0], datacache[playerid][ aridx ].CamFront[1], datacache[playerid][ aridx ].CamFront[2]
              );

    // increase/cycle counter

    curridx[playerid]++;

    if (curridx[playerid] > 59)
        curridx[playerid] = 0;

    return true;
}

float DotProduct(float AX, float AY, float AZ, float BX, float BY, float BZ) {
	return AX * BX + AY * BY + AZ * BZ;
}

SCRIPT_NATIVE nat_MPDotProduct(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(PushString, 6);

	float ret = DotProduct(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(ret);
}


void MPCrossProduct(float v1[3], float v2[3], float &resx, float &resy, float &resz) {

	resx = (v1[1]*v2[2]) - (v1[2]*v2[1]);
	resy = (v1[2]*v2[0]) - (v1[0]*v2[2]);
	resz = (v1[0]*v2[1]) - (v1[1]*v2[0]);

}

// transforms a vector using a 4x4 matrix (translation and scaling is ignored, only 3x3 part is used, we only need rotation!)
void MatrixTransformVector(float vector[3], float m[4][4], float &resx, float &resy, float &resz) {
	resz = vector[2] * m[0][0] + vector[1] * m[0][1] + vector[0] * m[0][2] + m[0][3];
	resy = vector[2] * m[1][0] + vector[1] * m[1][1] + vector[0] * m[1][2] + m[1][3];
	resx = -(vector[2] * m[2][0] + vector[1] * m[2][1] + vector[0] * m[2][2] + m[2][3]); // don't ask why -x was needed, i don't know either.
}

//========================================================================================
//
// Portion of DistancePointLine Unit Test
// Copyright (c) 2002, All rights reserved
//
// Damian Coventry
// Tuesday, 16 July 2002
//
// Implementation of theory by Paul Bourke
// http://paulbourke.net/geometry/pointline/
//
//========================================================================================

typedef struct tagXYZ
{
    float X, Y, Z;
}
XYZ;

float Magnitude( XYZ *Point1, XYZ *Point2 )
{
    XYZ Vector;

    Vector.X = Point2->X - Point1->X;
    Vector.Y = Point2->Y - Point1->Y;
    Vector.Z = Point2->Z - Point1->Z;

    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

int DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )
{
    float LineMag;
    float U;
    XYZ Intersection;

    LineMag = Magnitude( LineEnd, LineStart );

    U = ( ( ( Point->X - LineStart->X ) * ( LineEnd->X - LineStart->X ) ) +
        ( ( Point->Y - LineStart->Y ) * ( LineEnd->Y - LineStart->Y ) ) +
        ( ( Point->Z - LineStart->Z ) * ( LineEnd->Z - LineStart->Z ) ) ) /
        ( LineMag * LineMag );

    if( U < 0.0f || U > 1.0f )
        return 0;   // closest point does not fall within the line segment

    Intersection.X = LineStart->X + U * ( LineEnd->X - LineStart->X );
    Intersection.Y = LineStart->Y + U * ( LineEnd->Y - LineStart->Y );
    Intersection.Z = LineStart->Z + U * ( LineEnd->Z - LineStart->Z );

    *Distance = Magnitude( Point, &Intersection );

    return 1;
}

SCRIPT_NATIVE nat_MPDistancePointLine(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 9);

    XYZ LineStart, LineEnd, Point;

    LineStart.X =  amx_ctof(params[1]); LineStart.Y =   amx_ctof(params[2]); LineStart.Z =  amx_ctof(params[3]);
    LineEnd.X   =  amx_ctof(params[4]); LineStart.Y =   amx_ctof(params[5]); LineStart.Z =  amx_ctof(params[6]);
    Point.X     =  amx_ctof(params[7]); LineStart.Y =   amx_ctof(params[8]); LineStart.Z =  amx_ctof(params[9]);

    float linedist;

    int isok = DistancePointLine( &Point, &LineStart, &LineEnd, &linedist );

	if (isok == 0) // out of range
		linedist = -1;

    return amx_ftoc(linedist);
}

float Distance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return sqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}

float FDistance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return FastSqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}


SCRIPT_NATIVE nat_MPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 6);

    float tmpret;
	tmpret = Distance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

SCRIPT_NATIVE nat_FMPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 6);

    float tmpret;
	tmpret = FDistance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

/**
 *
 */

SCRIPT_NATIVE nat_MPPLogAdd(AMX* amx, cell* params) {
	int playerid;

    CHECK_PARAM_COUNT(PushString, 1);

	playerid = (int)params[1];

	return ACProcessUpdate(playerid);
}


SCRIPT_NATIVE nat_MPPLogConnect(AMX* amx, cell* params) {
	int playerid;

    CHECK_PARAM_COUNT(PushString, 1);

	playerid = (int)params[1];

	return ACPlayerConnect(playerid);
}

SCRIPT_NATIVE nat_MPGetVehicleDriver(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(PushString, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriver(vehicleid);
}

SCRIPT_NATIVE nat_MPGetVehicleDriverCount(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(PushString, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriverCount(vehicleid);
}

float DistanceCameraTargetToLocation(float CamX, float CamY, float CamZ,   float ObjX, float ObjY, float ObjZ,   float FrX, float FrY, float FrZ) {

	float TGTDistance;

	// get distance from camera to target
	TGTDistance = sqrt((CamX - ObjX) * (CamX - ObjX) + (CamY - ObjY) * (CamY - ObjY) + (CamZ - ObjZ) * (CamZ - ObjZ));

	float tmpX, tmpY, tmpZ;

	tmpX = FrX * TGTDistance + CamX;
	tmpY = FrY * TGTDistance + CamY;
	tmpZ = FrZ * TGTDistance + CamZ;

	return sqrt((tmpX - ObjX) * (tmpX - ObjX) + (tmpY - ObjY) * (tmpY - ObjY) + (tmpZ - ObjZ) * (tmpZ - ObjZ));
}


SCRIPT_NATIVE nat_MPDistanceCameraTargetToLocation(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 9);

	float resdist = DistanceCameraTargetToLocation(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),   amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]),   amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));

    return amx_ftoc(resdist);

}



SCRIPT_NATIVE nat_MPCrossProduct(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 9);

    float v1[3];
    float v2[3];
    float resx, resy, resz;

    v1[0] = amx_ctof(params[1]);
    v1[1] = amx_ctof(params[2]);
    v1[2] = amx_ctof(params[3]);

    v2[0] = amx_ctof(params[4]);
    v2[1] = amx_ctof(params[5]);
    v2[2] = amx_ctof(params[6]);

    MPCrossProduct(v1, v2, resx, resy, resz);;

	cell* padd;

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resx);
	amx_GetAddr(amx, params[8], &padd);
	*padd = amx_ftoc(resy);
	amx_GetAddr(amx, params[9], &padd);
	*padd = amx_ftoc(resz);

}

int MPGetAimTarget(int playerid) {


	float ccpos[3];
	float ccfront[3];

	invoke->callNative(&PAWN::GetPlayerCameraPos, playerid, &ccpos[0], &ccpos[1], &ccpos[2]);
	invoke->callNative(&PAWN::GetPlayerCameraFrontVector, playerid, &ccfront[0], &ccfront[1], &ccfront[2]);

//	logprintf("camera location for %d = (%0.5f %0.5f %0.5f) front (%0.5f %0.5f %0.5f)", playerid, ccpos[0], ccpos[1], ccpos[2], ccfront[0], ccfront[1], ccfront[2]);

	int closestplayer = INVALID_PLAYER_ID;
	float distmax = 90000.0;
	int i;

    for (i = 0; i < max_players; i++) {

    	if (i == playerid) // smartass
			continue;

		if (invoke->callNative(&PAWN::IsPlayerConnected, i) == false) // no connected player in slot?
			continue;

		float thiscoords[3];

		invoke->callNative(&PAWN::GetPlayerPos, i, &thiscoords[0], &thiscoords[1], &thiscoords[2]);

		float dist;

		dist = Distance(ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2]);

		if (dist < camera_actor_seek_radius) {

			// logprintf("(%0.5f %0.5f %0.5f) (%0.5f %0.5f %0.5f) distance %0.5f", ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2], dist);

			// distance between camera and this player has to be
			float camdist = DistanceCameraTargetToLocation(ccpos[0], ccpos[1], ccpos[2],   thiscoords[0], thiscoords[1], thiscoords[2],   ccfront[0], ccfront[1], ccfront[2] );

			// logprintf("camdist = %0.5f", camdist);

			if ((distmax < dist) && (camdist < camera_actor_range_threshhold)) {
				closestplayer = i;
				distmax = dist;
			}
		}

	}

	return closestplayer;

}

SCRIPT_NATIVE nat_MPGetAimTarget(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 1);

    return MPGetAimTarget(params[1]);

}

void interpolatebetweenvectors(float ax, float ay, float az, float bx, float by, float bz, float &rx, float &ry, float &rz, float distance) {
	rx = ax + distance * (bx - ax);
	ry = ay + distance * (by - ay);
	rz = az + distance * (bz - az);
}

void MPProjectPointOnVehicle(int vehid, float Invector[3], float &resx, float &resy, float &resz, int worldspace) {

	float Quaternion[4];
	float transformationmatrix[4][4];

	// to native!
	invoke->callNative(&PAWN::GetVehicleRotationQuat, vehid, &Quaternion[0], &Quaternion[1], &Quaternion[2], &Quaternion[3]);

	// build a transformation matrix out of the quaternion
	float xx = Quaternion[0] * Quaternion[0];
	float xy = Quaternion[0] * Quaternion[1];
	float xz = Quaternion[0] * Quaternion[2];
	float xw = Quaternion[0] * Quaternion[3];
	float yy = Quaternion[1] * Quaternion[1];
	float yz = Quaternion[1] * Quaternion[2];
	float yw = Quaternion[1] * Quaternion[3];
	float zz = Quaternion[2] * Quaternion[2];
	float zw = Quaternion[2] * Quaternion[3];

	transformationmatrix[0][0] = 1 - 2 * ( yy + zz );
	transformationmatrix[0][1] =     2 * ( xy - zw );
	transformationmatrix[0][2] =     2 * ( xz + yw );
	transformationmatrix[0][3] = 0.0;

	transformationmatrix[1][0] =     2 * ( xy + zw );
	transformationmatrix[1][1] = 1 - 2 * ( xx + zz );
	transformationmatrix[1][2] =     2 * ( yz - xw );
	transformationmatrix[1][3] = 0.0;

	transformationmatrix[2][0] =     2 * ( xz - yw );
	transformationmatrix[2][1] =     2 * ( yz + xw );
	transformationmatrix[2][2] = 1 - 2 * ( xx + yy );
	transformationmatrix[2][3] = 0;

	transformationmatrix[3][0] = 0;
	transformationmatrix[3][1] = 0;
	transformationmatrix[3][2] = 0;
	transformationmatrix[3][3] = 1;

	// transform the point thru car's rotation
	MatrixTransformVector(Invector, transformationmatrix, resx, resy, resz);

	// if worldspace is set it'll return the coords in global space - useful to check tire coords against tire spike proximity directly, etc..

	if (worldspace == 1) {
		float fX, fY, fZ;

		invoke->callNative(&PAWN::GetVehiclePos, vehid, &fX, &fY, &fZ);

		resx += fX;
		resy += fY;
		resz += fZ;
	}
}

SCRIPT_NATIVE nat_MPProjectPointOnVehicle(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 8);

    float v1[3];
    float resx, resy, resz;

    int vehid = params[1];

    v1[0] = amx_ctof(params[2]);
    v1[1] = amx_ctof(params[3]);
    v1[2] = amx_ctof(params[4]);

    MPProjectPointOnVehicle(vehid, v1, resx, resy, resz, params[8]);

	//logprintf("%0.5f %0.5f %0.5f", resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[5], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[6], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resz);

	return 1;
}

SCRIPT_NATIVE nat_MPInterpolatePoint(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 10);

    float resx, resy, resz;

    interpolatebetweenvectors(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),  amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]), resx, resy, resz, amx_ctof(params[10]));

	cell* padd = NULL;

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[8], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[9], &padd);
	*padd = amx_ftoc(resz);

	return 1;
}

float Clamp360(float angle) {

	int roundedone;

	roundedone = trunc(angle);
	roundedone = roundedone / 360;

	if (angle >= 360.0)
		return angle - (roundedone * 360.0);

	if (angle < 0.0)
		return 360.0 + angle - (roundedone * 360.0);

	return angle;

}

SCRIPT_NATIVE nat_MPClamp360(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(PushString, 1);

	float ret = Clamp360(amx_ctof(params[1]));

	return amx_ftoc(ret);
}

SCRIPT_NATIVE nat_MPFNormalize(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 3);

	float resx, resy, resz;

	resx = amx_ctof(params[1]);
	resz = amx_ctof(params[2]);
	resx = amx_ctof(params[3]);

    Normalize(resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[1], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[2], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[3], &padd);
	*padd = amx_ftoc(resz);

}

SCRIPT_NATIVE nat_MPFSQRT(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 1);

	float ret = FastSqrt(amx_ctof(params[1]));

	return amx_ftoc(ret);

}

SCRIPT_NATIVE nat_MPGetVehicleUpsideDown(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(PushString, 1);

    float v1[3];
    float resx, resy, resz;

    int vehid = params[1];

    if ((vehid == 0) || (vehid == INVALID_VEHICLE_ID) || (vehid > MAX_VEHICLES)) {
    	resz = -5.0;
    	return amx_ftoc(resz);
	}

    v1[0] = 0.0;
    v1[1] = 0.0;
    v1[2] = 1.0;

	MPProjectPointOnVehicle(vehid, v1, resx, resy, resz, 0);

	return amx_ftoc(resz);
}

SCRIPT_NATIVE nat_MPInit(AMX * amx, cell * params)
{
	if (!invoke->getAddresses())
	{
		logprintf("*** Math Plugin: Error obtaining native addresses");
		return 0;
	}
	return 1;
}


PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    // todo: code that checks if server is laggy / tick not going at a sufficient rate
}
