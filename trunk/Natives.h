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

#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("*** %s: Expecting %d parameter(s), but found %d", n, m, params[0] / 4); \
		return 0; \
	}

namespace Natives
{
	cell AMX_NATIVE_CALL Streamer_TickRate(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_MaxItems(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_VisibleItems(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_CellDistance(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_CellSize(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_Update(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_UpdateEx(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_GetFloatData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_SetFloatData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_GetIntData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_SetIntData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_IsInArrayData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_AppendArrayData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_RemoveArrayData(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_GetUpperBound(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_IsItemVisible(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_DestroyAllVisibleItems(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_CountVisibleItems(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicObject(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicObject(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicObject(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL SetDynamicObjectPos(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL GetDynamicObjectPos(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL SetDynamicObjectRot(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL GetDynamicObjectRot(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL MoveDynamicObject(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL StopDynamicObject(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicObjects(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicObjects(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicPickup(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicPickup(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicPickup(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicPickups(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicPickups(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerDynamicCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerAllDynamicCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsPlayerInDynamicCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicRaceCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicRaceCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicRaceCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerDynamicRaceCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerAllDynamicRaceCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsPlayerInDynamicRaceCP(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicRaceCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicRaceCPs(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicMapIcon(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicMapIcon(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicMapIcon(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicMapIcons(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicMapIcons(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamic3DTextLabel(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamic3DTextLabel(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamic3DTextLabel(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL UpdateDynamic3DTextLabelText(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamic3DTextLabels(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamic3DTextLabels(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicCircle(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicRectangle(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicSphere(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CreateDynamicCube(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyDynamicArea(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsValidDynamicArea(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerDynamicArea(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL TogglePlayerAllDynamicAreas(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL IsPlayerInDynamicArea(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL AttachDynamicAreaToPlayer(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL AttachDynamicAreaToVehicle(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL DestroyAllDynamicAreas(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL CountDynamicAreas(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_AddPlayer(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_RemovePlayer(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_VerifyPickup(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_VerifyCheckpoint(AMX * amx, cell * params);
	cell AMX_NATIVE_CALL Streamer_RegisterInterface(AMX * amx, cell * params);
};
