
	// game helpers
	native Float:MPDistanceCameraToLocation(Float:CamX, Float:CamY, Float:CamZ, Float:ObjX, Float:ObjY, Float:ObjZ, Float:FrX, Float:FrY, Float:FrZ); // calculates how distant target aim is from camera data pointed towards a certain point
	native Float:MPGetVehicleUpsideDown(vehicleid); // returns values such as 1.0 as pointing up, -1.0 is totally upside down. returns -5.0 if car id is not 1..2000.
	native MPGetAimTarget(PlayerID, Float:SeekRadius = 50.0); // returns player that this player is aiming at or invalid player id if no player in target area.
	native MPGetTrailerTowingVehicle(vehicleid); // finds the vehicle that this trailer is attached to, returns invalid_vehicle_id if invalid or not attached to any towing vehicle.
	native MPGetVehicleDriver(vehicleid); // gets vehicle driver id or invalid player id - does a quick reverse vehicle to player id lookup.
	native MPGetVehicleDriverCount(vehicleid); // returns number of drivers a car has (important to solve 2 drivers 1 car issue - if you wrote any decent anticheat you know what i mean)
	native MPGetVehicleOccupantCnt(vehicleid); // returns number of player a vehicle is carrying
	native MPGetVehicleSurfersCnt(vehicleid); // returns number of players surfing a vehicle

	native MPProjectPointOnVehicle(vehicleid, Float:v1x, Float:v1y, Float:v1z, &Float:resx, &Float:resy, &Float:resz, worldspace = 0); // projects a point on vehicle's rotation on all 3 axes.
	native MPProjectPointOnPlayer(playerid, Float:v1x, Float:v1y, Float:v1z, &Float:resx, &Float:resy, &Float:resz); // projects a point on player's facing angle (x - sideways, y front/back, z = up/down).
	native MPProjectPointXYZA(Float:OriginX, Float:OriginY, Float:OriginZ, Float:HeadingAngle,      Float:v1x, Float:v1y, Float:v1z,      &Float:resx, &Float:resy, &Float:resz); // project a point based on xyz position + heading angle ONLY (no pitch / roll)

	// pure math
	native Float:MPFVecLength(Float:v1x, Float:v1y, Float:v1z); // calculates length of a simple XYZ 3d vector (FAST,less precision)
	native Float:MPClamp360(Float:value);
	native Float:MPDistance(Float:v1x, Float:v1y, Float:v1z, Float:v2x, Float:v2y, Float:v2z);	// distance between 2 points
	native Float:MPDistancePointLine(Float:PointX, Float:PointY, Float:PointZ, Float:LineSx, Float:LineSy, Float:LineSz, Float:LineEx, Float:LineEy, Float:LineEz); // http://paulbourke.net/geometry/pointline/ returns super huge number 10000000 if outside of range of specified the lie segment.
	native Float:MPDotProduct(Float:v1x, Float:v1y, Float:v1z, Float:v2x, Float:v2y, Float:v2z);
	native Float:MPFDistance(Float:v1x, Float:v1y, Float:v1z, Float:v2x, Float:v2y, Float:v2z);	// distance between 2 points (faster but less precise)
	native Float:MPFSQRT(Float:value);	// Faster sqrt (google the 0x5f3759df method)
	native Float:MPVecLength(Float:v1x, Float:v1y, Float:v1z); // calculates length of a simple XYZ 3d vector
	native MPCrossProduct(Float:v1x, Float:v1y, Float:v1z, Float:v2x, Float:v2y, Float:v2z, &Float:resx, &Float:resy, &Float:resz);
	native MPFNormalize(&Float:vx, &Float:vy, &Float:vz); // fast float normalization of a vector to unit-length (makes whatever vector 1.0 long, purely to preserve direction and be able to scale it controllably)
	native MPInterpolatePoint(Float:v1x, Float:v1y, Float:v1z, Float:v2x, Float:v2y, Float:v2z, &Float:resx, &Float:resy, &Float:resz, Float:distance);

	native MPInRange(Float:xs, Float:xe, Float:xc); // returns 1 if xc is between xs and xe (range start/stop doesn't matter)
	native MPPtInRect2D(Float:xs, Float:ys, Float:xe, Float:ye, Float:xc, Float:yc); // returns 1 if xc,yc is within rectangle of xs..xe and ys..ye
	native MPPtInRect3D(Float:xs, Float:ys, Float:zs, Float:xe, Float:ye, Float:ze, Float:xc, Float:yc, Float:zx); // returns 1 if xc,yc,zc is within rectangle of xs..xe, ys..ye and zs..ze

	native MPFloatIsFinite(Float:in); // returns 1 if floating point is finite (when it is NOT NaN or INF)
	native Float:MPFiniteFloat(Float:in, Float:alternative = 0.0); // returns the same floating point as received, but if the number is not finite (INF / NaN) it will return the second parameter.

	native Float:MPReturnInf(positive); // returns a invalid float - infinite (+inf / -inf)
	native Float:MPReturnNan(positive); // returns a invalid float - NaN (+NaN / -NaN)

