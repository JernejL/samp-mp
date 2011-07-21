/*
    SA-MP Streamer Plugin v2.5.2
    Copyright © 2010 Incognito

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

namespace PAWN
{
	struct Native
	{
		const char * name;
		const char * data;
	};

	static const char * const names[] =
	{
		"CreatePickup",
		"CreatePlayer3DTextLabel",
		"CreatePlayerObject",
		"DeletePlayer3DTextLabel",
		"DestroyPickup",
		"DestroyPlayerObject",
		"DisablePlayerCheckpoint",
		"DisablePlayerRaceCheckpoint",
		"GetPlayerInterior",
		"GetPlayerPos",
		"GetVehicleRotationQuat",
		"GetPlayerCameraPos",
		"GetPlayerCameraFrontVector",
		//"GetPlayerDistanceFromPoint",
		"GetPlayerState",
		"GetPlayerVirtualWorld",
		"GetPlayerVehicleID",
		"GetPlayerFacingAngle",
		"GetVehicleZAngle",
		"GetVehiclePos",
		"GetPlayerVelocity",
		"GetVehicleVelocity",
		"MovePlayerObject",
		"RemovePlayerMapIcon",
		"SetPlayerCheckpoint",
		"SetPlayerMapIcon",
		"SetPlayerObjectPos",
		"SetPlayerObjectRot",
		"SetPlayerRaceCheckpoint",
		"StopPlayerObject",
		"UpdatePlayer3DTextLabelText",
//		"tickcount",
		"GetTickCount",
//		"MapAndreas_FindZ_For2DCoord",
		"IsPlayerConnected"
	};

	static const Native CreatePickup = { "CreatePickup", "ddfffd" };
	static const Native CreatePlayer3DTextLabel = { "CreatePlayer3DTextLabel", "dsdffffddd" };
	static const Native CreatePlayerObject = { "CreatePlayerObject", "ddfffffff" };
	static const Native DeletePlayer3DTextLabel = { "DeletePlayer3DTextLabel", "dd" };
	static const Native DestroyPickup = { "DestroyPickup", "d" };
	static const Native DestroyPlayerObject = { "DestroyPlayerObject", "dd" };
	static const Native DisablePlayerCheckpoint = { "DisablePlayerCheckpoint", "d" };
	static const Native DisablePlayerRaceCheckpoint = { "DisablePlayerRaceCheckpoint", "d" };
	static const Native GetPlayerInterior = { "GetPlayerInterior", "d" };
	static const Native GetPlayerPos = { "GetPlayerPos", "dvvv" };
	static const Native GetVehicleRotationQuat = { "GetVehicleRotationQuat", "dvvvv" };
	static const Native GetPlayerCameraPos = { "GetPlayerCameraPos", "dvvv" };
	static const Native GetPlayerCameraFrontVector = { "GetPlayerCameraFrontVector", "dvvv" };
	//static const Native GetPlayerDistanceFromPoint = { "GetPlayerDistanceFromPoint", "dfff" };
	static const Native GetPlayerVelocity = { "GetPlayerVelocity", "dvvv" };
	static const Native GetVehicleVelocity = { "GetVehicleVelocity", "dvvv" };
	static const Native GetPlayerState = { "GetPlayerState", "d" };
	static const Native GetPlayerVirtualWorld = { "GetPlayerVirtualWorld", "d" };
	static const Native GetPlayerVehicleID = { "GetPlayerVehicleID", "d" };
	static const Native GetPlayerFacingAngle = { "GetPlayerFacingAngle", "dv" };
	static const Native GetVehicleZAngle = { "GetVehicleZAngle", "dv" };
	static const Native GetVehiclePos = { "GetVehiclePos", "dvvv" };
	static const Native MovePlayerObject = { "MovePlayerObject", "ddffff" };
	static const Native RemovePlayerMapIcon = { "RemovePlayerMapIcon", "dd" };
	static const Native SetPlayerCheckpoint = { "SetPlayerCheckpoint", "dffff" };
	static const Native SetPlayerMapIcon = { "SetPlayerMapIcon", "ddfffddd" };
	static const Native SetPlayerObjectPos = { "SetPlayerObjectPos", "ddfff" };
	static const Native SetPlayerObjectRot = { "SetPlayerObjectRot", "ddfff" };
	static const Native SetPlayerRaceCheckpoint = { "SetPlayerRaceCheckpoint", "ddfffffff" };
	static const Native StopPlayerObject = { "StopPlayerObject", "dd" };
	static const Native UpdatePlayer3DTextLabelText = { "UpdatePlayer3DTextLabelText", "ddds" };
//	static const Native tickcount = { "tickcount", "v" };
	static const Native GetTickCount = { "GetTickCount", "" };
//	static const Native MapAndreas_FindZ_For2DCoord = { "MapAndreas_FindZ_For2DCoord", "ffv" };
	static const Native IsPlayerConnected = { "IsPlayerConnected", "d" };
};

class
	Invoke
{
public:
	Invoke()
	{
		gotAddresses = false;
	}

	int callNative(const PAWN::Native * native, ...);
	int getAddresses();

	int amx_idx;

	std::list<AMX *> amx_list;
private:
	bool gotAddresses;

	boost::unordered_map<std::string, unsigned int> amx_map;
};

typedef int (* amx_Function_t)(AMX * amx, cell * params);

extern Invoke * invoke;
