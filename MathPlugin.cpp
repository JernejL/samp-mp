#include "MathPlugin.h"

#include <math.h>
#include <logprintf.h>
#include <samp.h>
#include <players.h>
#include <wrapper.h>
#include <plugin.h>
#include <vehicles.h>

using namespace sampgdk;

    int curridx[MAX_PLAYERS]; // at which point is the cycle?
    psnapshot datacache[MAX_PLAYERS][MAX_SNAPSHOTS];

	// And an array containing the native function-names and the functions
	// specified with them.
	AMX_NATIVE_INFO natives[ ] =
	{
	    { "MPInit",						nat_MPInit},
		{ "MPGetVehicleDriver",			nat_MPGetVehicleDriver },
		{ "MPGetVehicleDriverCount",	nat_MPGetVehicleDriverCount },
		{ "MPGetVehicleOccupantCnt",	nat_MPGetVehicleOccupantCnt },
		{ "MPGetVehicleSurfersCnt", 	nat_MPGetVehicleSurfersCnt},
		{ "MPCrossProduct",				nat_MPCrossProduct },
		{ "MPDotProduct",				nat_MPDotProduct },
		{ "MPDistanceCameraToLocation",	nat_MPDistanceCameraTargetToLocation },
		{ "MPProjectPointOnVehicle",	nat_MPProjectPointOnVehicle},
		{ "MPProjectPointOnPlayer",		nat_MPProjectPointOnPlayer},
		{ "MPGetAimTarget",				nat_MPGetAimTarget },
		{ "MPDistancePointLine",		nat_MPDistancePointLine },
		{ "MPInterpolatePoint",			nat_MPInterpolatePoint},
		{ "MPDistance",					nat_MPDistance },
		{ "MPFDistance",				nat_MPFDistance },
		{ "MPPLogAdd",					nat_MPPLogAdd },
		{ "MPFSQRT",					nat_MPFSQRT },
		{ "MPClamp360",					nat_MPClamp360 },
		{ "MPFNormalize",				nat_MPFNormalize },
		{ "MPPLogConnect",				nat_MPPLogConnect },
		{ "MPGetVehicleUpsideDown",		nat_MPGetVehicleUpsideDown },
		{ "MPGetTrailerTowingVehicle",	nat_MPGetTrailerTowingVehicle },
		{ "MPVecLength",				nat_MPVecLength },
		{ "FMPVecLength",				nat_FMPVecLength },

		{ 0,					    	0 }
	};

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

int GetTrailerTowingVehicle(int trailerid) {

    if ((trailerid == 0) || (trailerid == INVALID_VEHICLE_ID) || (trailerid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;

    for (i = 0; i < MAX_VEHICLES; i++) {

		if (GetVehicleModel(i) == 0) // invalid vehicle.
			continue;

        if (GetVehicleTrailer(i) == trailerid)
			return i;

    }

    return INVALID_VEHICLE_ID; // found no driver.

}

int GetVehicleDriver(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

            if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid;
            thisplayervehid = GetPlayerVehicleID(i);

            // todo: maybe use IsPlayerInVehicle instead.

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                return i;

    }

    return INVALID_PLAYER_ID; // found no driver.

}

SCRIPT_NATIVE nat_MPGetVehicleDriver(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleDriver, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriver(vehicleid);
}

SCRIPT_NATIVE nat_MPGetTrailerTowingVehicle(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetTrailerTowingVehicle, 1);

	vehicleid = (int)params[1];

	return GetTrailerTowingVehicle(vehicleid);
}

int GetVehicleDriverCount(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totaldrivers = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

            if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid;
            thisplayervehid = GetPlayerVehicleID(i);

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                totaldrivers++;

    }

    return totaldrivers; // return how many there are.

}

int GetVehicleOccupantCnt(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totalocc = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

			if (GetPlayerVirtualWorld(i) != GetVehicleVirtualWorld(vehicleid)) // solves specific issues if you put paused players into separate virtual world.
				continue;

            int thisplayervehid;
            thisplayervehid = GetPlayerVehicleID(i);

            if (thisplayervehid == vehicleid) // found a connected player in this car.
                totalocc++;

    }

    return totalocc; // return how many there are.

}

void ProjectPointOnPlayer(int PlayerID, float incoords_x, float incoords_y, float incoords_z, float &outx, float &outy, float &outz) {

	float playerpos[3];
	float playerheading;
	GetPlayerPos(PlayerID, playerpos[0], playerpos[1], playerpos[2]);
	GetPlayerFacingAngle(PlayerID, playerheading);

	float originalangle;
	originalangle = atan2(incoords_x, incoords_y) * 180 / PI;

	float originaldistance;
	originaldistance = FastSqrt(incoords_x * incoords_x + incoords_y * incoords_y);

	originalangle -= playerheading;

	// *PI/180 = degtorad
	outx = playerpos[0] + sin(originalangle * PI/180) * originaldistance;
	outy = playerpos[1] + cos(originalangle * PI/180) * originaldistance;
	outz = incoords_z + playerpos[2]; // nothing magical on Z!

}

int GetVehicleSurfersCnt(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totalocc = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

			if (GetPlayerVirtualWorld(i) != GetVehicleVirtualWorld(vehicleid)) // solves specific issues if you put paused players into separate virtual world.
				continue;

            int thisplayervehid;
            thisplayervehid = GetPlayerSurfingVehicleID(i);

            if (thisplayervehid == vehicleid) // found a connected player in this car.
                totalocc++;

    }

    return totalocc; // return how many there are.

}

SCRIPT_NATIVE nat_MPGetVehicleDriverCount(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleDriverCount, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriverCount(vehicleid);
}

SCRIPT_NATIVE nat_MPGetVehicleOccupantCnt(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleOccupantCnt, 1);

	vehicleid = (int)params[1];

	return GetVehicleOccupantCnt(vehicleid);
}

SCRIPT_NATIVE nat_MPGetVehicleSurfersCnt(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleSurfersCnt, 1);

	vehicleid = (int)params[1];

	return GetVehicleSurfersCnt(vehicleid);
}

void MPCrossProduct(float v1[3], float v2[3], float &resx, float &resy, float &resz) {

	resx = (v1[1]*v2[2]) - (v1[2]*v2[1]);
	resy = (v1[2]*v2[0]) - (v1[0]*v2[2]);
	resz = (v1[0]*v2[1]) - (v1[1]*v2[0]);

}

SCRIPT_NATIVE nat_MPCrossProduct(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPCrossProduct, 9);

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

	return 1;

}

float DotProduct(float AX, float AY, float AZ, float BX, float BY, float BZ) {
	return AX * BX + AY * BY + AZ * BZ;
}

SCRIPT_NATIVE nat_MPDotProduct(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_MPDotProduct, 6);

	float ret = DotProduct(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(ret);
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

    CHECK_PARAM_COUNT(nat_MPDistanceCameraTargetToLocation, 9);

	float resdist = DistanceCameraTargetToLocation(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),   amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]),   amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));

    return amx_ftoc(resdist);

}

// transforms a vector using a 4x4 matrix (translation and scaling is ignored, only 3x3 part is used, we only need rotation!)
void MatrixTransformVector(float vector[3], float m[4][4], float &resx, float &resy, float &resz) {
	resz = vector[2] * m[0][0] + vector[1] * m[0][1] + vector[0] * m[0][2] + m[0][3];
	resy = vector[2] * m[1][0] + vector[1] * m[1][1] + vector[0] * m[1][2] + m[1][3];
	resx = -(vector[2] * m[2][0] + vector[1] * m[2][1] + vector[0] * m[2][2] + m[2][3]); // don't ask why -x was needed, i don't know either.
}


void MPProjectPointOnVehicle(int vehid, float Invector[3], float &resx, float &resy, float &resz, int worldspace) {

	float Quaternion[4];
	float transformationmatrix[4][4];

	// to native!
	GetVehicleRotationQuat(vehid, Quaternion[0], Quaternion[1], Quaternion[2], Quaternion[3]);

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

		GetVehiclePos(vehid, fX, fY, fZ);

		resx += fX;
		resy += fY;
		resz += fZ;
	}
}

float Distance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return sqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}

float FDistance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return FastSqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}


SCRIPT_NATIVE nat_MPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPDistance, 6);

    float tmpret;
	tmpret = Distance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

SCRIPT_NATIVE nat_MPFDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFDistance, 6);

    float tmpret;
	tmpret = FDistance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

float VecLength(float fsx, float fsy, float fsz) {

	return sqrt(fsx * fsx + fsy * fsy + fsz * fsz);

}

SCRIPT_NATIVE nat_MPVecLength(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPVecLength, 3);

    float tmpret;
	tmpret = VecLength(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));

	return amx_ftoc(tmpret);

}

float FVecLength(float fsx, float fsy, float fsz) {

	return FastSqrt(fsx * fsx + fsy * fsy + fsz * fsz);

}

SCRIPT_NATIVE nat_FMPVecLength(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_FMPVecLength, 3);

    float tmpret;
	tmpret = FVecLength(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));

	return amx_ftoc(tmpret);

}

SCRIPT_NATIVE nat_FMPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_FMPDistance, 6);

    float tmpret;
	tmpret = FDistance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}


SCRIPT_NATIVE nat_MPProjectPointOnVehicle(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPProjectPointOnVehicle, 8);

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

SCRIPT_NATIVE nat_MPProjectPointOnPlayer(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPProjectPointOnVehicle, 7);

    float resx, resy, resz;

    int playerid = params[1];

	ProjectPointOnPlayer(playerid, amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), resx, resy, resz);

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

int MPGetAimTarget(int playerid, float searchradius) {

	//logprintf("MPGetAimTarget called for player %d with radius %0.3f", playerid, searchradius);

	if ((IsPlayerConnected(playerid) == false) || (playerid > MAX_PLAYERS)) {
		logprintf("MPGetAimTarget called for invalid player.");
		return INVALID_PLAYER_ID;
	}

	float ccpos[3];
	float ccfront[3];

	GetPlayerCameraFrontVector(playerid, ccfront[0], ccfront[1], ccfront[2]);

	GetPlayerCameraPos(playerid, ccpos[0], ccpos[1], ccpos[2]);

	//logprintf("camera location for %d = (%0.5f %0.5f %0.5f) front (%0.5f %0.5f %0.5f)", playerid, ccpos[0], ccpos[1], ccpos[2], ccfront[0], ccfront[1], ccfront[2]);

	int closestplayer = INVALID_PLAYER_ID;
	float distmax = 90000.0;
	int i;

    for (i = 0; i < MAX_PLAYERS; i++) {

    	if (i == playerid) // smartass
			continue;

		if (IsPlayerConnected(i) == false) // no connected player in slot?
			continue;

		float thiscoords[3];

		GetPlayerPos(i, thiscoords[0], thiscoords[1], thiscoords[2]);

		float dist;

		dist = Distance(ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2]);

		//logprintf("(%0.5f %0.5f %0.5f) (%0.5f %0.5f %0.5f) distance %0.5f", ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2], dist);

		if (dist < searchradius) {

			// distance between camera and this player has to be
			float camdist = DistanceCameraTargetToLocation(ccpos[0], ccpos[1], ccpos[2],   thiscoords[0], thiscoords[1], thiscoords[2],   ccfront[0], ccfront[1], ccfront[2] );

			//logprintf("camdist = %0.5f", camdist);

			if ((camdist < distmax) && (camdist < camera_actor_range_threshhold)) {
				closestplayer = i;
				distmax = dist;
			}
		}

	}

	return closestplayer;

}

SCRIPT_NATIVE nat_MPGetAimTarget(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPGetAimTarget, 2);

    return MPGetAimTarget(params[1], amx_ctof(params[2]));

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

//	logprintf("(%0.5f %0.5f %0.5f ) (%0.5f %0.5f %0.5f ) (%0.5f %0.5f %0.5f ) ", Point->X, Point->Y, Point->Z, LineStart->X, LineStart->Y, LineStart->Z, LineEnd->X, LineEnd->Y, LineEnd->Z);

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

void interpolatebetweenvectors(float ax, float ay, float az, float bx, float by, float bz, float &rx, float &ry, float &rz, float distance) {
	rx = ax + distance * (bx - ax);
	ry = ay + distance * (by - ay);
	rz = az + distance * (bz - az);
}

bool ACPlayerConnect(int playerid) {

	// todo: also clear out all old data!
    curridx[playerid] = 0;

    return true;

}

SCRIPT_NATIVE nat_MPPLogConnect(AMX* amx, cell* params) {
	int playerid;

    CHECK_PARAM_COUNT(nat_MPPLogConnect, 1);

	playerid = (int)params[1];

	return ACPlayerConnect(playerid);
}

SCRIPT_NATIVE nat_MPInterpolatePoint(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPInterpolatePoint, 10);

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

bool ACProcessUpdate(int playerid) {

    int aridx = curridx[playerid];

    datacache[playerid][ aridx ].vehicleid = GetPlayerVehicleID(playerid);

/*
// tickcount doesn't work - no idea why.
    int gran = 0;
    datacache[playerid][ aridx ].SnapTime = invoke->callNative(&PAWN::tickcount, &gran);
    logprintf("%d -> %d", gran, datacache[playerid][ aridx ].SnapTime);
*/
    datacache[playerid][ aridx ].SnapTime = 0; //obtainCurrentTime();//invoke->callNative(&PAWN::GetTickCount);

    if (datacache[playerid][ aridx ].vehicleid == 0) {
        GetPlayerPos(playerid, datacache[playerid][ aridx ].Position[0], datacache[playerid][ aridx ].Position[1], datacache[playerid][ aridx ].Position[2]);
        GetPlayerVelocity(playerid, datacache[playerid][ aridx ].Velocity[0], datacache[playerid][ aridx ].Velocity[1], datacache[playerid][ aridx ].Velocity[2]);
        GetPlayerFacingAngle(playerid, datacache[playerid][ aridx ].Heading);
    } else {
        GetVehiclePos(datacache[playerid][ aridx ].vehicleid, datacache[playerid][ aridx ].Position[0], datacache[playerid][ aridx ].Position[1], datacache[playerid][ aridx ].Position[2]);
        GetVehicleVelocity(datacache[playerid][ aridx ].vehicleid, datacache[playerid][ aridx ].Velocity[0], datacache[playerid][ aridx ].Velocity[1], datacache[playerid][ aridx ].Velocity[2]);
        GetVehicleZAngle(datacache[playerid][ aridx ].vehicleid, datacache[playerid][ aridx ].Heading);
    }


    // source of mega crashes.
    //MapAndreas_FindZ_For2DCoord(playerid, datacache[playerid][ aridx ].Position[0], datacache[playerid][ aridx ].Position[1], &datacache[playerid][ aridx ].groundlevel);

    GetPlayerCameraPos(playerid, datacache[playerid][ aridx ].CamPos[0], datacache[playerid][ aridx ].CamPos[1], datacache[playerid][ aridx ].CamPos[2]);
    GetPlayerCameraFrontVector(playerid, datacache[playerid][ aridx ].CamFront[0], datacache[playerid][ aridx ].CamFront[1], datacache[playerid][ aridx ].CamFront[2]);

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

	CHECK_PARAM_COUNT(nat_MPClamp360, 1);

	float ret = Clamp360(amx_ctof(params[1]));

	return amx_ftoc(ret);
}

SCRIPT_NATIVE nat_MPFNormalize(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFNormalize, 3);

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

	return 1;

}

SCRIPT_NATIVE nat_MPFSQRT(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFSQRT, 1);

	float ret = FastSqrt(amx_ctof(params[1]));

	return amx_ftoc(ret);

}

SCRIPT_NATIVE nat_MPPLogAdd(AMX* amx, cell* params) {
	int playerid;

    CHECK_PARAM_COUNT(nat_MPPLogAdd, 1);

	playerid = (int)params[1];

	return ACProcessUpdate(playerid);
}

SCRIPT_NATIVE nat_MPDistancePointLine(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPDistancePointLine, 9);

    XYZ LineStart, LineEnd, Point;

    Point.X     =  amx_ctof(params[1]); Point.Y =   	amx_ctof(params[2]); Point.Z =  	amx_ctof(params[3]);
    LineStart.X =  amx_ctof(params[4]); LineStart.Y =   amx_ctof(params[5]); LineStart.Z =  amx_ctof(params[6]);
    LineEnd.X   =  amx_ctof(params[7]); LineEnd.Y =   	amx_ctof(params[8]); LineEnd.Z =  	amx_ctof(params[9]);

    float linedist;

// DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )

    int isok = DistancePointLine( &Point, &LineStart, &LineEnd, &linedist );

	if (isok == 0) // out of range
		linedist = 10000000.0;

    return amx_ftoc(linedist);
}

SCRIPT_NATIVE nat_MPGetVehicleUpsideDown(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPGetVehicleUpsideDown, 1);

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
	logprintf("*** Math Plugin: Initialized.");
	return 1;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()  {
	return SUPPORTS_VERSION  | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppPluginData)  {
    // This always must be called first
    Wrapper::GetInstance()->Initialize(ppPluginData);
    logprintf("Math Plugin Loaded.");
    // Set our gamemode as the main event handler
    //EventHandler::SetEventHandler(&::theGameMode);
    return true;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) {
	return amx_Register( amx, natives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) {

	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    // todo: code that checks if server is laggy / tick not going at a sufficient rate

}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
    return;
}



