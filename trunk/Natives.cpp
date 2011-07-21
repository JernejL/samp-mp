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

#include "../main.h"
#include "Invoke.h"
#include "Natives.h"

using namespace Natives;

cell AMX_NATIVE_CALL Natives::Streamer_TickRate(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_TickRate");
	if ((int)params[1] > 0)
	{
		core->global.tickRate = (int)params[1];
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_MaxItems(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "Streamer_MaxItems");
	switch ((int)params[1])
	{
		case STREAMER_TYPE_OBJECT:
		{
			core->global.maxObjects = (int)params[2];
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			core->global.maxPickups = (int)params[2];
		}
		break;
		case STREAMER_TYPE_CP:
		{
			core->global.maxCheckpoints = (int)params[2];
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			core->global.maxRaceCheckpoints = (int)params[2];
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			core->global.maxMapIcons = (int)params[2];
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			core->global.maxTextLabels = (int)params[2];
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			core->global.maxAreas = (int)params[2];
		}
		break;
		default:
		{
			logprintf("*** Streamer_MaxItems: Invalid type specified");
			return 0;
		}
		break;
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_VisibleItems(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "Streamer_VisibleItems");
	switch ((int)params[1])
	{
		case STREAMER_TYPE_OBJECT:
		{
			core->global.visibleObjects = (int)params[2];
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			core->global.visiblePickups = (int)params[2];
		}
		break;
		case STREAMER_TYPE_CP:
		{
			break;
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			break;
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			core->global.visibleMapIcons = (int)params[2];
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			core->global.visibleTextLabels = (int)params[2];
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			break;
		}
		break;
		default:
		{
			logprintf("*** Streamer_VisibleItems: Invalid type specified");
			return 0;
		}
		break;
	}
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_CellDistance(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_CellDistance");
	core->global.cellDistance = amx_ctof(params[1]) * amx_ctof(params[1]);
	grid->rebuildGrid();
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_CellSize(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_CellSize");
	core->global.cellDistance = (amx_ctof(params[1]) * 1.5f) * (amx_ctof(params[1]) * 1.5f);
	core->global.cellSize = amx_ctof(params[1]);
	grid->rebuildGrid();
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_Update(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_Update");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			invoke->callNative(&PAWN::GetPlayerPos, p->playerID, &p->x, &p->y, &p->z);
			p->interiorID = invoke->callNative(&PAWN::GetPlayerInterior, p->playerID);
			p->worldID = invoke->callNative(&PAWN::GetPlayerVirtualWorld, p->playerID);
			if (!core->movingAreas.empty())
			{
				streamer->updateMovingAreas();
			}
			if (!core->movingObjects.empty())
			{
				streamer->updateMovingObjects();
			}
			if (!core->movingTextLabels.empty())
			{
				streamer->updateMovingTextLabels();
			}
			std::set<Grid::SharedCell, Grid::Cell> totalCells;
			std::vector<Grid::SharedCell> playerCells;
			grid->findNearbyCells(* p, playerCells, totalCells);
			if (!core->objects.empty())
			{
				streamer->processObjects(* p, playerCells);
			}
			if (!core->checkpoints.empty())
			{
				streamer->processCheckpoints(* p, playerCells);
			}
			if (!core->raceCheckpoints.empty())
			{
				streamer->processRaceCheckpoints(* p, playerCells);
			}
			if (!core->mapIcons.empty())
			{
				streamer->processMapIcons(* p, playerCells);
			}
			if (!core->textLabels.empty())
			{
				streamer->processTextLabels(* p, playerCells);
			}
			if (!core->areas.empty())
			{
				streamer->processAreas(* p, playerCells);
			}
			if (!core->pickups.empty())
			{
				grid->mergeRemainingCells(totalCells);
				streamer->processPickups(totalCells);
			}
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_UpdateEx(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_UpdateEx");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			p->interiorID = invoke->callNative(&PAWN::GetPlayerInterior, p->playerID);
			p->worldID = invoke->callNative(&PAWN::GetPlayerVirtualWorld, p->playerID);
			p->x = amx_ctof(params[2]);
			p->y = amx_ctof(params[3]);
			p->z = amx_ctof(params[4]);
			if (!core->movingAreas.empty())
			{
				streamer->updateMovingAreas();
			}
			if (!core->movingObjects.empty())
			{
				streamer->updateMovingObjects();
			}
			if (!core->movingTextLabels.empty())
			{
				streamer->updateMovingTextLabels();
			}
			std::set<Grid::SharedCell, Grid::Cell> totalCells;
			std::vector<Grid::SharedCell> playerCells;
			grid->findNearbyCells(* p, playerCells, totalCells);
			if (!core->objects.empty())
			{
				streamer->processObjects(* p, playerCells);
			}
			if (!core->checkpoints.empty())
			{
				streamer->processCheckpoints(* p, playerCells);
			}
			if (!core->raceCheckpoints.empty())
			{
				streamer->processRaceCheckpoints(* p, playerCells);
			}
			if (!core->mapIcons.empty())
			{
				streamer->processMapIcons(* p, playerCells);
			}
			if (!core->textLabels.empty())
			{
				streamer->processTextLabels(* p, playerCells);
			}
			if (!core->areas.empty())
			{
				streamer->processAreas(* p, playerCells);
			}
			if (!core->pickups.empty())
			{
				grid->mergeRemainingCells(totalCells);
				streamer->processPickups(totalCells);
			}
			return 1;
		}
	}
	return 0;
}


cell AMX_NATIVE_CALL Natives::Streamer_GetFloatData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_GetFloatData");
	return (cell)Manipulation::getFloatData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_SetFloatData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_SetFloatData");
	return (cell)Manipulation::setFloatData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_GetIntData(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "Streamer_GetIntData");
	return (cell)Manipulation::getIntData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_SetIntData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_SetIntData");
	return (cell)Manipulation::setIntData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_IsInArrayData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_IsInArrayData");
	return (cell)Manipulation::isInArrayData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_AppendArrayData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_AppendArrayData");
	return (cell)Manipulation::appendArrayData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_RemoveArrayData(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "Streamer_RemoveArrayData");
	return (cell)Manipulation::removeArrayData(amx, params);
}

cell AMX_NATIVE_CALL Natives::Streamer_GetUpperBound(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_GetUpperBound");
	switch ((int)params[1])
	{
		case STREAMER_TYPE_OBJECT:
		{
			int objectID = 0;
			if (!core->global.deletedObjects.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.begin(); o != core->objects.end(); ++o)
				{
					if (o->first > objectID)
					{
						objectID = o->first;
					}
				}
			}
			else
			{
				objectID = core->global.deletedObjects.second;
			}
			return (cell)(objectID + 1);
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			int pickupID = 0;
			if (!core->global.deletedPickups.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedPickup>::iterator p = core->pickups.begin(); p != core->pickups.end(); ++p)
				{
					if (p->first > pickupID)
					{
						pickupID = p->first;
					}
				}
			}
			else
			{
				pickupID = core->global.deletedPickups.second;
			}
			return (cell)(pickupID + 1);
		}
		break;
		case STREAMER_TYPE_CP:
		{
			int checkpointID = 0;
			if (!core->global.deletedCheckpoints.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedCheckpoint>::iterator c = core->checkpoints.begin(); c != core->checkpoints.end(); ++c)
				{
					if (c->first > checkpointID)
					{
						checkpointID = c->first;
					}
				}
			}
			else
			{
				checkpointID = core->global.deletedCheckpoints.second;
			}
			return (cell)(checkpointID + 1);
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			int checkpointID = 0;
			if (!core->global.deletedRaceCheckpoints.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedRaceCheckpoint>::iterator r = core->raceCheckpoints.begin(); r != core->raceCheckpoints.end(); ++r)
				{
					if (r->first > checkpointID)
					{
						checkpointID = r->first;
					}
				}
			}
			else
			{
				checkpointID = core->global.deletedRaceCheckpoints.second;
			}
			return (cell)(checkpointID + 1);
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			int mapIconID = 0;
			if (!core->global.deletedMapIcons.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedMapIcon>::iterator m = core->mapIcons.begin(); m != core->mapIcons.end(); ++m)
				{
					if (m->first > mapIconID)
					{
						mapIconID = m->first;
					}
				}
			}
			else
			{
				mapIconID = core->global.deletedMapIcons.second;
			}
			return (cell)(mapIconID + 1);
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			int textLabelID = 0;
			if (!core->global.deletedTextLabels.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedTextLabel>::iterator t = core->textLabels.begin(); t != core->textLabels.end(); ++t)
				{
					if (t->first > textLabelID)
					{
						textLabelID = t->first;
					}
				}
			}
			else
			{
				textLabelID = core->global.deletedTextLabels.second;
			}
			return (cell)(textLabelID + 1);
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			int areaID = 0;
			if (!core->global.deletedAreas.first.empty())
			{
				for (boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.begin(); a != core->areas.end(); ++a)
				{
					if (a->first > areaID)
					{
						areaID = a->first;
					}
				}
			}
			else
			{
				areaID = core->global.deletedAreas.second;
			}
			return (cell)(areaID + 1);
		}
		break;
		default:
		{
			logprintf("*** Streamer_GetUpperBound: Invalid type specified");
		}
		break;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_IsItemVisible(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "Streamer_IsItemVisible");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			switch ((int)params[2])
			{
				case STREAMER_TYPE_OBJECT:
				{
					boost::unordered_map<int, int>::iterator f = p->internalObjects.find((int)params[3]);
					if (f != p->internalObjects.end())
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_PICKUP:
				{
					boost::unordered_map<int, int>::iterator f = core->global.internalPickups.find((int)params[3]);
					if (f != core->global.internalPickups.end())
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_CP:
				{
					if (p->visibleCheckpoint == (int)params[3])
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_RACE_CP:
				{
					if (p->visibleRaceCheckpoint == (int)params[3])
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_MAP_ICON:
				{
					boost::unordered_map<int, int>::iterator f = p->internalMapIcons.find((int)params[3]);
					if (f != p->internalMapIcons.end())
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_3D_TEXT_LABEL:
				{
					boost::unordered_map<int, int>::iterator f = p->internalTextLabels.find((int)params[3]);
					if (f != p->internalTextLabels.end())
					{
						return 1;
					}
				}
				case STREAMER_TYPE_AREA:
				{
					boost::unordered_set<int>::iterator f = p->visibleAreas.find((int)params[3]);
					if (f != p->visibleAreas.end())
					{
						return 1;
					}
				}
				break;
				default:
				{
					logprintf("*** Streamer_IsItemVisible: Invalid type specified");
				}
				break;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_DestroyAllVisibleItems(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "Streamer_DestroyAllVisibleItems");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			switch ((int)params[2])
			{
				case STREAMER_TYPE_OBJECT:
				{
					for (boost::unordered_map<int, int>::iterator o = p->internalObjects.begin(); o != p->internalObjects.end(); ++o)
					{
						invoke->callNative(&PAWN::DestroyPlayerObject, p->playerID, o->second);
					}
					p->internalObjects.clear();
					return 1;
				}
				break;
				case STREAMER_TYPE_PICKUP:
				{
					for (boost::unordered_map<int, int>::iterator q = core->global.internalPickups.begin(); q != core->global.internalPickups.end(); ++q)
					{
						invoke->callNative(&PAWN::DestroyPickup, q->second);
					}
					core->global.internalPickups.clear();
					return 1;
				}
				break;
				case STREAMER_TYPE_CP:
				{
					if (p->visibleCheckpoint)
					{
						invoke->callNative(&PAWN::DisablePlayerCheckpoint, p->playerID);
						p->activeCheckpoint = 0;
						p->visibleCheckpoint = 0;
					}
					return 1;
				}
				break;
				case STREAMER_TYPE_RACE_CP:
				{
					if (p->visibleRaceCheckpoint)
					{
						invoke->callNative(&PAWN::DisablePlayerRaceCheckpoint, p->playerID);
						p->activeRaceCheckpoint = 0;
						p->visibleRaceCheckpoint = 0;
					}
					return 1;
				}
				break;
				case STREAMER_TYPE_MAP_ICON:
				{
					for (boost::unordered_map<int, int>::iterator m = p->internalMapIcons.begin(); m != p->internalMapIcons.end(); ++m)
					{
						invoke->callNative(&PAWN::RemovePlayerMapIcon, p->playerID, m->second);
					}
					p->internalMapIcons.clear();
					return 1;
				}
				break;
				case STREAMER_TYPE_3D_TEXT_LABEL:
				{
					for (boost::unordered_map<int, int>::iterator t = p->internalTextLabels.begin(); t != p->internalTextLabels.end(); ++t)
					{
						invoke->callNative(&PAWN::DeletePlayer3DTextLabel, p->playerID, t->second);
					}
					p->internalTextLabels.clear();
					return 1;
				}
				break;
				case STREAMER_TYPE_AREA:
				{
					p->visibleAreas.clear();
					return 1;
				}
				break;
				default:
				{
					logprintf("*** Streamer_DestroyAllVisibleItems: Invalid type specified");
				}
				break;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_CountVisibleItems(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "Streamer_CountVisibleItems");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			switch ((int)params[2])
			{
				case STREAMER_TYPE_OBJECT:
				{
					return (cell)p->internalObjects.size();
				}
				break;
				case STREAMER_TYPE_PICKUP:
				{
					return (cell)core->global.internalPickups.size();
				}
				break;
				case STREAMER_TYPE_CP:
				{
					if (p->visibleCheckpoint)
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_RACE_CP:
				{
					if (p->visibleRaceCheckpoint)
					{
						return 1;
					}
				}
				break;
				case STREAMER_TYPE_MAP_ICON:
				{
					return (cell)p->internalMapIcons.size();
				}
				break;
				case STREAMER_TYPE_3D_TEXT_LABEL:
				{
					return (cell)p->internalTextLabels.size();
				}
				break;
				case STREAMER_TYPE_AREA:
				{
					return (cell)p->visibleAreas.size();
				}
				break;
				default:
				{
					logprintf("*** Streamer_CountVisibleItems: Invalid type specified");
				}
				break;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::CreateDynamicObject(AMX * amx, cell * params)
{
	CHECK_PARAMS(11, "CreateDynamicObject");
	if (core->global.maxObjects == core->objects.size())
	{
		return 0;
	}
	int objectID = 0;
	if (!core->global.deletedObjects.first.empty())
	{
		objectID = * core->global.deletedObjects.first.begin();
		core->global.deletedObjects.first.erase(core->global.deletedObjects.first.begin());
	}
	else
	{
		objectID = ++core->global.deletedObjects.second;
	}
	Core::SharedObject object(new Element::Object);
	object->amx = amx;
	object->distance = amx_ctof(params[11]) * amx_ctof(params[11]);
	object->drawDistance = 0.0f;
	object->extraID = 0;
	if ((int)params[9] >= 0 && (int)params[9] < MAX_INTERIORS)
	{
		object->interiors.set((int)params[9]);
	}
	else
	{
		object->interiors.set();
	}
	object->modelID = (int)params[1];
	object->objectID = objectID;
	if ((int)params[10] >= 0 && (int)params[10] < MAX_PLAYERS)
	{
		object->players.set((int)params[10]);
	}
	else
	{
		object->players.set();
	}
	object->rX = amx_ctof(params[5]);
	object->rY = amx_ctof(params[6]);
	object->rZ = amx_ctof(params[7]);
	if ((int)params[8] >= 0)
	{
		object->worlds.insert((int)params[8]);
	}
	object->x = amx_ctof(params[2]);
	object->y = amx_ctof(params[3]);
	object->z = amx_ctof(params[4]);
	grid->addObject(object);
	core->objects.insert(std::make_pair(objectID, object));
	return (cell)objectID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicObject(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicObject");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalObjects.find(o->first);
			if (f != p->internalObjects.end())
			{
				invoke->callNative(&PAWN::DestroyPlayerObject, p->playerID, f->second);
				p->internalObjects.quick_erase(f);
			}
		}
		if (core->objects.size() > 1)
		{
			core->global.deletedObjects.first.insert(o->first);
		}
		else
		{
			core->global.deletedObjects = std::make_pair(std::set<int>(), 0);
		}
		grid->removeObject(o->second);
		core->objects.quick_erase(o);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicObject(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicObject");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectPos(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "SetDynamicObjectPos");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		if (o->second->move)
		{
			logprintf("*** SetDynamicObjectPos: Object must be stopped first");
			return 0;
		}
		float objectX = o->second->x, objectY = o->second->y;
		o->second->x = amx_ctof(params[2]);
		o->second->y = amx_ctof(params[3]);
		o->second->z = amx_ctof(params[4]);
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalObjects.find(o->first);
			if (f != p->internalObjects.end())
			{
				invoke->callNative(&PAWN::SetPlayerObjectPos, p->playerID, f->second, o->second->x, o->second->y, o->second->z);
			}
		}
		if (objectX != o->second->x || objectY != o->second->y)
		{
			if (o->second->cellID)
			{
				grid->removeObject(o->second, true);
			}
		}
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectPos(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "GetDynamicObjectPos");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		cell * x, * y, * z;
		amx_GetAddr(amx, params[2], &x);
		amx_GetAddr(amx, params[3], &y);
		amx_GetAddr(amx, params[4], &z);
		* x = amx_ftoc(o->second->x);
		* y = amx_ftoc(o->second->y);
		* z = amx_ftoc(o->second->z);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetDynamicObjectRot(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "SetDynamicObjectRot");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		o->second->rX = amx_ctof(params[2]);
		o->second->rY = amx_ctof(params[3]);
		o->second->rZ = amx_ctof(params[4]);
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalObjects.find(o->first);
			if (f != p->internalObjects.end())
			{
				invoke->callNative(&PAWN::SetPlayerObjectRot, p->playerID, f->second, o->second->rX, o->second->rY, o->second->rZ);
			}
		}
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::GetDynamicObjectRot(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "GetDynamicObjectRot");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		cell * rX, * rY, * rZ;
		amx_GetAddr(amx, params[2], &rX);
		amx_GetAddr(amx, params[3], &rY);
		amx_GetAddr(amx, params[4], &rZ);
		* rX = amx_ftoc(o->second->rX);
		* rY = amx_ftoc(o->second->rY);
		* rZ = amx_ftoc(o->second->rZ);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::MoveDynamicObject(AMX * amx, cell * params)
{
	CHECK_PARAMS(5, "MoveDynamicObject");
	if (!amx_ctof(params[5]))
	{
		return 0;
	}
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		o->second->move = boost::intrusive_ptr<Element::Object::Move>(new Element::Object::Move);
		o->second->move->duration = static_cast<int>((sqrt(pow(amx_ctof(params[2]) - o->second->x, 2) + pow(amx_ctof(params[3]) - o->second->y, 2) + pow(amx_ctof(params[4]) - o->second->z, 2)) * 1000.0f) / amx_ctof(params[5]));
		o->second->move->offsetX = (amx_ctof(params[2]) - o->second->x) / o->second->move->duration;
		o->second->move->offsetY = (amx_ctof(params[3]) - o->second->y) / o->second->move->duration;
		o->second->move->offsetZ = (amx_ctof(params[4]) - o->second->z) / o->second->move->duration;
		o->second->move->speed = amx_ctof(params[5]);
		o->second->move->startTime = core->obtainCurrentTime();
		o->second->move->startX = o->second->x;
		o->second->move->startY = o->second->y;
		o->second->move->startZ = o->second->z;
		o->second->move->x = amx_ctof(params[2]);
		o->second->move->y = amx_ctof(params[3]);
		o->second->move->z = amx_ctof(params[4]);
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalObjects.find(o->first);
			if (f != p->internalObjects.end())
			{
				invoke->callNative(&PAWN::MovePlayerObject, p->playerID, f->second, o->second->move->x, o->second->move->y, o->second->move->z, o->second->move->speed);
			}
		}
		core->movingObjects.insert(o->second);
		return (cell)o->second->move->duration;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::StopDynamicObject(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "StopDynamicObject");
	boost::unordered_map<int, Core::SharedObject>::iterator o = core->objects.find((int)params[1]);
	if (o != core->objects.end())
	{
		if (o->second->move)
		{
			for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator f = p->internalObjects.find(o->first);
				if (f != p->internalObjects.end())
				{
					invoke->callNative(&PAWN::StopPlayerObject, p->playerID, f->second);
				}
			}
			o->second->move.reset();
			core->movingObjects.erase(o->second);
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicObjects(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator o = p->internalObjects.begin(); o != p->internalObjects.end(); ++o)
		{
			invoke->callNative(&PAWN::DestroyPlayerObject, p->playerID, o->second);
		}
		p->internalObjects.clear();
	}
	core->global.deletedObjects = std::make_pair(std::set<int>(), 0);
	core->objects.clear();
	core->movingObjects.clear();
	grid->eraseAllItems(STREAMER_TYPE_OBJECT);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicObjects(AMX * amx, cell * params)
{
	return (cell)core->objects.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamicPickup(AMX * amx, cell * params)
{
	CHECK_PARAMS(9, "CreateDynamicPickup");
	if (core->global.maxPickups == core->pickups.size())
	{
		return 0;
	}
	int pickupID = 0;
	if (!core->global.deletedPickups.first.empty())
	{
		pickupID = * core->global.deletedPickups.first.begin();
		core->global.deletedPickups.first.erase(core->global.deletedPickups.first.begin());
	}
	else
	{
		pickupID = ++core->global.deletedPickups.second;
	}
	Core::SharedPickup pickup(new Element::Pickup);
	pickup->amx = amx;
	pickup->distance = amx_ctof(params[9]) * amx_ctof(params[9]);
	pickup->extraID = 0;
	if ((int)params[7] >= 0 && (int)params[7] < MAX_INTERIORS)
	{
		pickup->interiors.set((int)params[7]);
	}
	else
	{
		pickup->interiors.set();
	}
	pickup->modelID = (int)params[1];
	pickup->pickupID = pickupID;
	if ((int)params[8] >= 0 && (int)params[8] < MAX_PLAYERS)
	{
		pickup->players.set((int)params[8]);
	}
	else
	{
		pickup->players.set();
	}
	pickup->type = (int)params[2];
	pickup->worldID = (int)params[6];
	pickup->x = amx_ctof(params[3]);
	pickup->y = amx_ctof(params[4]);
	pickup->z = amx_ctof(params[5]);
	grid->addPickup(pickup);
	core->pickups.insert(std::make_pair(pickupID, pickup));
	return (cell)pickupID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicPickup(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicPickup");
	boost::unordered_map<int, Core::SharedPickup>::iterator p = core->pickups.find((int)params[1]);
	if (p != core->pickups.end())
	{
		boost::unordered_map<int, int>::iterator f = core->global.internalPickups.find(p->first);
		if (f != core->global.internalPickups.end())
		{
			invoke->callNative(&PAWN::DestroyPickup, f->second);
			core->global.internalPickups.quick_erase(f);
		}
		if (core->pickups.size() > 1)
		{
			core->global.deletedPickups.first.insert(p->first);
		}
		else
		{
			core->global.deletedPickups = std::make_pair(std::set<int>(), 0);
		}
		grid->removePickup(p->second);
		core->pickups.quick_erase(p);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicPickup(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicPickup");
	boost::unordered_map<int, Core::SharedPickup>::iterator p = core->pickups.find((int)params[1]);
	if (p != core->pickups.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicPickups(AMX * amx, cell * params)
{
	for (boost::unordered_map<int, int>::iterator p = core->global.internalPickups.begin(); p != core->global.internalPickups.end(); ++p)
	{
		invoke->callNative(&PAWN::DestroyPickup, p->second);
	}
	core->global.deletedPickups = std::make_pair(std::set<int>(), 0);
	core->global.internalPickups.clear();
	core->pickups.clear();
	grid->eraseAllItems(STREAMER_TYPE_PICKUP);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicPickups(AMX * amx, cell * params)
{
	return (cell)core->pickups.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(8, "CreateDynamicCP");
	if (core->global.maxCheckpoints == core->checkpoints.size())
	{
		return 0;
	}
	int checkpointID = 0;
	if (!core->global.deletedCheckpoints.first.empty())
	{
		checkpointID = * core->global.deletedCheckpoints.first.begin();
		core->global.deletedCheckpoints.first.erase(core->global.deletedCheckpoints.first.begin());
	}
	else
	{
		checkpointID = ++core->global.deletedCheckpoints.second;
	}
	Core::SharedCheckpoint checkpoint(new Element::Checkpoint);
	checkpoint->amx = amx;
	checkpoint->checkpointID = checkpointID;
	checkpoint->distance = amx_ctof(params[8]) * amx_ctof(params[8]);
	checkpoint->extraID = 0;
	if ((int)params[6] >= 0 && (int)params[6] < MAX_INTERIORS)
	{
		checkpoint->interiors.set((int)params[6]);
	}
	else
	{
		checkpoint->interiors.set();
	}
	if ((int)params[7] >= 0 && (int)params[7] < MAX_PLAYERS)
	{
		checkpoint->players.set((int)params[7]);
	}
	else
	{
		checkpoint->players.set();
	}
	checkpoint->size = amx_ctof(params[4]);
	if ((int)params[5] >= 0)
	{
		checkpoint->worlds.insert((int)params[5]);
	}
	checkpoint->x = amx_ctof(params[1]);
	checkpoint->y = amx_ctof(params[2]);
	checkpoint->z = amx_ctof(params[3]);
	grid->addCheckpoint(checkpoint);
	core->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
	return (cell)checkpointID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicCP");
	boost::unordered_map<int, Core::SharedCheckpoint>::iterator c = core->checkpoints.find((int)params[1]);
	if (c != core->checkpoints.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			if (p->visibleCheckpoint == c->first)
			{
				invoke->callNative(&PAWN::DisablePlayerCheckpoint, p->playerID);
				p->activeCheckpoint = 0;
				p->visibleCheckpoint = 0;
			}
			p->disabledCheckpoints.erase(c->first);
		}
		if (core->checkpoints.size() > 1)
		{
			core->global.deletedCheckpoints.first.insert(c->first);
		}
		else
		{
			core->global.deletedCheckpoints = std::make_pair(std::set<int>(), 0);
		}
		grid->removeCheckpoint(c->second);
		core->checkpoints.quick_erase(c);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicCP");
	boost::unordered_map<int, Core::SharedCheckpoint>::iterator c = core->checkpoints.find((int)params[1]);
	if (c != core->checkpoints.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerDynamicCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "TogglePlayerDynamicCP");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			boost::unordered_set<int>::iterator f = p->disabledCheckpoints.find((int)params[2]);
			if ((int)params[3])
			{
				if (f != p->disabledCheckpoints.end())
				{
					p->disabledCheckpoints.quick_erase(f);
					return 1;
				}
			}
			else
			{
				if (f == p->disabledCheckpoints.end())
				{
					if (p->visibleCheckpoint == (int)params[2])
					{
						invoke->callNative(&PAWN::DisablePlayerCheckpoint, p->playerID);
						p->activeCheckpoint = 0;
						p->visibleCheckpoint = 0;
					}
					p->disabledCheckpoints.insert((int)params[2]);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerAllDynamicCPs(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "TogglePlayerAllDynamicCPs");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			p->disabledCheckpoints.clear();
			if (!(int)params[2])
			{
				if (p->visibleCheckpoint != 0)
				{
					invoke->callNative(&PAWN::DisablePlayerCheckpoint, p->playerID);
					p->activeCheckpoint = 0;
					p->visibleCheckpoint = 0;
				}
				for (boost::unordered_map<int, Core::SharedCheckpoint>::iterator c = core->checkpoints.begin(); c != core->checkpoints.end(); ++c)
				{
					p->disabledCheckpoints.insert(c->first);
				}
			}
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerInDynamicCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IsPlayerInDynamicCP");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			if (p->activeCheckpoint == (int)params[2])
			{
				return 1;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicCPs(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		p->disabledCheckpoints.clear();
		if (p->visibleCheckpoint != 0)
		{
			invoke->callNative(&PAWN::DisablePlayerCheckpoint, p->playerID);
			p->activeCheckpoint = 0;
			p->visibleCheckpoint = 0;
		}
	}
	core->global.deletedCheckpoints = std::make_pair(std::set<int>(), 0);
	core->checkpoints.clear();
	grid->eraseAllItems(STREAMER_TYPE_CP);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicCPs(AMX * amx, cell * params)
{
	return (cell)core->checkpoints.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRaceCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(12, "CreateDynamicRaceCP");
	if (core->global.maxRaceCheckpoints == core->raceCheckpoints.size())
	{
		return 0;
	}
	int checkpointID = 0;
	if (!core->global.deletedRaceCheckpoints.first.empty())
	{
		checkpointID = * core->global.deletedRaceCheckpoints.first.begin();
		core->global.deletedRaceCheckpoints.first.erase(core->global.deletedRaceCheckpoints.first.begin());
	}
	else
	{
		checkpointID = ++core->global.deletedRaceCheckpoints.second;
	}
	Core::SharedRaceCheckpoint raceCheckpoint(new Element::RaceCheckpoint);
	raceCheckpoint->amx = amx;
	raceCheckpoint->checkpointID = checkpointID;
	raceCheckpoint->distance = amx_ctof(params[12]) * amx_ctof(params[12]);
	raceCheckpoint->extraID = 0;
	if ((int)params[10] >= 0 && (int)params[10] < MAX_INTERIORS)
	{
		raceCheckpoint->interiors.set((int)params[10]);
	}
	else
	{
		raceCheckpoint->interiors.set();
	}

	raceCheckpoint->nextX = amx_ctof(params[5]);
	raceCheckpoint->nextY = amx_ctof(params[6]);
	raceCheckpoint->nextZ = amx_ctof(params[7]);
	if ((int)params[11] >= 0 && (int)params[11] < MAX_PLAYERS)
	{
		raceCheckpoint->players.set((int)params[11]);
	}
	else
	{
		raceCheckpoint->players.set();
	}
	raceCheckpoint->size = amx_ctof(params[8]);
	raceCheckpoint->type = (int)params[1];
	if ((int)params[9] >= 0)
	{
		raceCheckpoint->worlds.insert((int)params[9]);
	}
	raceCheckpoint->x = amx_ctof(params[2]);
	raceCheckpoint->y = amx_ctof(params[3]);
	raceCheckpoint->z = amx_ctof(params[4]);
	grid->addRaceCheckpoint(raceCheckpoint);
	core->raceCheckpoints.insert(std::make_pair(checkpointID, raceCheckpoint));
	return (cell)checkpointID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicRaceCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicRaceCP");
	boost::unordered_map<int, Core::SharedRaceCheckpoint>::iterator r = core->raceCheckpoints.find((int)params[1]);
	if (r != core->raceCheckpoints.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			if (p->visibleRaceCheckpoint == r->first)
			{
				invoke->callNative(&PAWN::DisablePlayerRaceCheckpoint, p->playerID);
				p->activeRaceCheckpoint = 0;
				p->visibleRaceCheckpoint = 0;
			}
			p->disabledRaceCheckpoints.erase(r->first);
		}
		if (core->raceCheckpoints.size() > 1)
		{
			core->global.deletedRaceCheckpoints.first.insert(r->first);
		}
		else
		{
			core->global.deletedRaceCheckpoints = std::make_pair(std::set<int>(), 0);
		}
		grid->removeRaceCheckpoint(r->second);
		core->raceCheckpoints.quick_erase(r);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicRaceCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicRaceCP");
	boost::unordered_map<int, Core::SharedRaceCheckpoint>::iterator r = core->raceCheckpoints.find((int)params[1]);
	if (r != core->raceCheckpoints.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerDynamicRaceCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "TogglePlayerDynamicRaceCP");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			boost::unordered_set<int>::iterator f = p->disabledRaceCheckpoints.find((int)params[2]);
			if ((int)params[3])
			{
				if (f != p->disabledRaceCheckpoints.end())
				{
					p->disabledRaceCheckpoints.quick_erase(f);
					return 1;
				}
			}
			else
			{
				if (f == p->disabledRaceCheckpoints.end())
				{
					if (p->visibleRaceCheckpoint == (int)params[2])
					{
						invoke->callNative(&PAWN::DisablePlayerRaceCheckpoint, p->playerID);
						p->activeRaceCheckpoint = 0;
						p->visibleRaceCheckpoint = 0;
					}
					p->disabledRaceCheckpoints.insert((int)params[2]);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerAllDynamicRaceCPs(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "TogglePlayerAllDynamicRaceCPs");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			p->disabledRaceCheckpoints.clear();
			if (!(int)params[2])
			{
				if (p->visibleRaceCheckpoint != 0)
				{
					invoke->callNative(&PAWN::DisablePlayerRaceCheckpoint, p->playerID);
					p->activeRaceCheckpoint = 0;
					p->visibleRaceCheckpoint = 0;
				}
				for (boost::unordered_map<int, Core::SharedRaceCheckpoint>::iterator r = core->raceCheckpoints.begin(); r != core->raceCheckpoints.end(); ++r)
				{
					p->disabledRaceCheckpoints.insert(r->first);
				}
			}
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerInDynamicRaceCP(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IsPlayerInDynamicRaceCP");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			if (p->activeRaceCheckpoint == (int)params[2])
			{
				return 1;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicRaceCPs(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		p->disabledRaceCheckpoints.clear();
		if (p->visibleRaceCheckpoint != 0)
		{
			invoke->callNative(&PAWN::DisablePlayerRaceCheckpoint, p->playerID);
			p->activeRaceCheckpoint = 0;
			p->visibleRaceCheckpoint = 0;
		}
	}
	core->global.deletedRaceCheckpoints = std::make_pair(std::set<int>(), 0);
	core->raceCheckpoints.clear();
	grid->eraseAllItems(STREAMER_TYPE_RACE_CP);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicRaceCPs(AMX * amx, cell * params)
{
	return (cell)core->raceCheckpoints.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamicMapIcon(AMX * amx, cell * params)
{
	CHECK_PARAMS(9, "CreateDynamicMapIcon");
	if (core->global.maxMapIcons == core->mapIcons.size())
	{
		return 0;
	}
	int mapIconID = 0;
	if (!core->global.deletedMapIcons.first.empty())
	{
		mapIconID = * core->global.deletedMapIcons.first.begin();
		core->global.deletedMapIcons.first.erase(core->global.deletedMapIcons.first.begin());
	}
	else
	{
		mapIconID = ++core->global.deletedMapIcons.second;
	}
	Core::SharedMapIcon mapIcon(new Element::MapIcon);
	mapIcon->amx = amx;
	mapIcon->color = (int)params[5];
	mapIcon->distance = amx_ctof(params[9]) * amx_ctof(params[9]);
	mapIcon->extraID = 0;
	if ((int)params[7] >= 0 && (int)params[7] < MAX_INTERIORS)
	{
		mapIcon->interiors.set((int)params[7]);
	}
	else
	{
		mapIcon->interiors.set();
	}
	mapIcon->mapIconID = mapIconID;
	if ((int)params[8] >= 0 && (int)params[8] < MAX_PLAYERS)
	{
		mapIcon->players.set((int)params[8]);
	}
	else
	{
		mapIcon->players.set();
	}
	mapIcon->type = (int)params[4];
	if ((int)params[6] >= 0)
	{
		mapIcon->worlds.insert((int)params[6]);
	}
	mapIcon->style = 0;
	mapIcon->x = amx_ctof(params[1]);
	mapIcon->y = amx_ctof(params[2]);
	mapIcon->z = amx_ctof(params[3]);
	grid->addMapIcon(mapIcon);
	core->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
	return (cell)mapIconID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicMapIcon(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicMapIcon");
	boost::unordered_map<int, Core::SharedMapIcon>::iterator m = core->mapIcons.find((int)params[1]);
	if (m != core->mapIcons.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalMapIcons.find(m->first);
			if (f != p->internalMapIcons.end())
			{
				invoke->callNative(&PAWN::RemovePlayerMapIcon, p->playerID, f->second);
				p->sortedInternalMapIcons.erase(f->second);
				p->internalMapIcons.quick_erase(f);
			}
		}
		if (core->mapIcons.size() > 1)
		{
			core->global.deletedMapIcons.first.insert(m->first);
		}
		else
		{
			core->global.deletedMapIcons = std::make_pair(std::set<int>(), 0);
		}
		grid->removeMapIcon(m->second);
		core->mapIcons.quick_erase(m);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicMapIcon(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicMapIcon");
	boost::unordered_map<int, Core::SharedMapIcon>::iterator m = core->mapIcons.find((int)params[1]);
	if (m != core->mapIcons.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicMapIcons(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator m = p->internalMapIcons.begin(); m != p->internalMapIcons.end(); ++m)
		{
			invoke->callNative(&PAWN::RemovePlayerMapIcon, p->playerID, m->second);
		}
		p->internalMapIcons.clear();
		p->sortedInternalMapIcons.clear();
	}
	core->global.deletedMapIcons = std::make_pair(std::set<int>(), 0);
	core->mapIcons.clear();
	grid->eraseAllItems(STREAMER_TYPE_MAP_ICON);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicMapIcons(AMX * amx, cell * params)
{
	return (cell)core->mapIcons.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamic3DTextLabel(AMX * amx, cell * params)
{
	CHECK_PARAMS(13, "CreateDynamic3DTextLabel");
	if (core->global.maxTextLabels == core->textLabels.size())
	{
		return 0;
	}
	int textLabelID = 0;
	if (!core->global.deletedTextLabels.first.empty())
	{
		textLabelID = * core->global.deletedTextLabels.first.begin();
		core->global.deletedTextLabels.first.erase(core->global.deletedTextLabels.first.begin());
	}
	else
	{
		textLabelID = ++core->global.deletedTextLabels.second;
	}
	Core::SharedTextLabel textLabel(new Element::TextLabel);
	char * text;
	amx_StrParam(amx, params[1], text);
	textLabel->amx = amx;
	textLabel->color = (int)params[2];
	textLabel->distance = amx_ctof(params[13]) * amx_ctof(params[13]);
	textLabel->drawDistance = amx_ctof(params[6]);
	textLabel->extraID = 0;
	if ((int)params[11] >= 0 && (int)params[11] < MAX_INTERIORS)
	{
		textLabel->interiors.set((int)params[11]);
	}
	else
	{
		textLabel->interiors.set();
	}
	if ((int)params[12] >= 0 && (int)params[12] < MAX_PLAYERS)
	{
		textLabel->players.set((int)params[12]);
	}
	else
	{
		textLabel->players.set();
	}
	textLabel->testLOS = (int)params[9];
	textLabel->text = text ? text : "";
	textLabel->textLabelID = textLabelID;
	if ((int)params[10] >= 0)
	{
		textLabel->worlds.insert((int)params[10]);
	}
	textLabel->x = amx_ctof(params[3]);
	textLabel->y = amx_ctof(params[4]);
	textLabel->z = amx_ctof(params[5]);
	if ((int)params[7] != INVALID_GENERIC_ID || (int)params[8] != INVALID_GENERIC_ID)
	{
		textLabel->attach = boost::intrusive_ptr<Element::TextLabel::Attach>(new Element::TextLabel::Attach);
		textLabel->attach->player = (int)params[7];
		textLabel->attach->vehicle = (int)params[8];
		if (fabs(textLabel->x) > 50.0f || fabs(textLabel->y) > 50.0f || fabs(textLabel->z) > 50.0f)
		{
			textLabel->x = 0.0f;
			textLabel->y = 0.0f;
			textLabel->z = 0.0f;
		}
		core->movingTextLabels.insert(textLabel);
	}
	grid->addTextLabel(textLabel);
	core->textLabels.insert(std::make_pair(textLabelID, textLabel));
	return (cell)textLabelID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamic3DTextLabel(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamic3DTextLabel");
	boost::unordered_map<int, Core::SharedTextLabel>::iterator t = core->textLabels.find((int)params[1]);
	if (t != core->textLabels.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalTextLabels.find(t->first);
			if (f != p->internalTextLabels.end())
			{
				invoke->callNative(&PAWN::DeletePlayer3DTextLabel, p->playerID, f->second);
				p->internalTextLabels.quick_erase(f);
			}
		}
		if (core->textLabels.size() > 1)
		{
			core->global.deletedTextLabels.first.insert(t->first);
		}
		else
		{
			core->global.deletedTextLabels = std::make_pair(std::set<int>(), 0);
		}
		grid->removeTextLabel(t->second);
		core->textLabels.quick_erase(t);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamic3DTextLabel(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamic3DTextLabel");
	boost::unordered_map<int, Core::SharedTextLabel>::iterator t = core->textLabels.find((int)params[1]);
	if (t != core->textLabels.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::UpdateDynamic3DTextLabelText(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "UpdateDynamic3DTextLabelText");
	char * text;
	amx_StrParam(amx, params[3], text);
	boost::unordered_map<int, Core::SharedTextLabel>::iterator t = core->textLabels.find((int)params[1]);
	if (t != core->textLabels.end())
	{
		t->second->color = (int)params[2];
		t->second->text = text ? text : "";
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator f = p->internalTextLabels.find(t->first);
			if (f != p->internalTextLabels.end())
			{
				invoke->callNative(&PAWN::UpdatePlayer3DTextLabelText, p->playerID, f->second, t->second->color, t->second->text.c_str());
			}
		}
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamic3DTextLabels(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator t = p->internalTextLabels.begin(); t != p->internalTextLabels.end(); ++t)
		{
			invoke->callNative(&PAWN::DeletePlayer3DTextLabel, p->playerID, t->second);
		}
		p->internalTextLabels.clear();
	}
	core->global.deletedTextLabels = std::make_pair(std::set<int>(), 0);
	core->textLabels.clear();
	core->movingTextLabels.clear();
	grid->eraseAllItems(STREAMER_TYPE_3D_TEXT_LABEL);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamic3DTextLabels(AMX * amx, cell * params)
{
	return (cell)core->textLabels.size();
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCircle(AMX * amx, cell * params)
{
	CHECK_PARAMS(6, "CreateDynamicCircle");
	if (core->global.maxAreas == core->areas.size())
	{
		return 0;
	}
	int areaID = 0;
	if (!core->global.deletedAreas.first.empty())
	{
		areaID = * core->global.deletedAreas.first.begin();
		core->global.deletedAreas.first.erase(core->global.deletedAreas.first.begin());
	}
	else
	{
		areaID = ++core->global.deletedAreas.second;
	}
	Core::SharedArea area(new Element::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->extraID = 0;
	if ((int)params[5] >= 0 && (int)params[5] < MAX_INTERIORS)
	{
		area->interiors.set((int)params[5]);
	}
	else
	{
		area->interiors.set();
	}
	if ((int)params[6] >= 0 && (int)params[6] < MAX_PLAYERS)
	{
		area->players.set((int)params[6]);
	}
	else
	{
		area->players.set();
	}
	area->size = amx_ctof(params[3]) * amx_ctof(params[3]);
	area->type = 0;
	if ((int)params[4] >= 0)
	{
		area->worlds.insert((int)params[4]);
	}
	area->x1 = amx_ctof(params[1]);
	area->y1 = amx_ctof(params[2]);
	grid->addArea(area);
	core->areas.insert(std::make_pair(areaID, area));
	return (cell)areaID;
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRectangle(AMX * amx, cell * params)
{
	CHECK_PARAMS(7, "CreateDynamicRectangle");
	if (core->global.maxAreas == core->areas.size())
	{
		return 0;
	}
	int areaID = 0;
	if (!core->global.deletedAreas.first.empty())
	{
		areaID = * core->global.deletedAreas.first.begin();
		core->global.deletedAreas.first.erase(core->global.deletedAreas.first.begin());
	}
	else
	{
		areaID = ++core->global.deletedAreas.second;
	}
	Core::SharedArea area(new Element::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->extraID = 0;
	if ((int)params[6] >= 0 && (int)params[6] < MAX_INTERIORS)
	{
		area->interiors.set((int)params[6]);
	}
	else
	{
		area->interiors.set();
	}
	if ((int)params[7] >= 0 && (int)params[7] < MAX_PLAYERS)
	{
		area->players.set((int)params[7]);
	}
	else
	{
		area->players.set();
	}
	area->type = 1;
	if ((int)params[5] >= 0)
	{
		area->worlds.insert((int)params[5]);
	}
	area->x1 = amx_ctof(params[1]);
	area->y1 = amx_ctof(params[2]);
	area->x2 = amx_ctof(params[3]);
	area->y2 = amx_ctof(params[4]);
	if (area->x1 > area->x2)
	{
		std::swap(area->x1, area->x2);
	}
	if (area->y1 > area->y2)
	{
		std::swap(area->y1, area->y2);
	}
	grid->addArea(area);
	core->areas.insert(std::make_pair(areaID, area));
	return (cell)areaID;
}

cell AMX_NATIVE_CALL Natives::CreateDynamicSphere(AMX * amx, cell * params)
{
	CHECK_PARAMS(7, "CreateDynamicSphere");
	if (core->global.maxAreas == core->areas.size())
	{
		return 0;
	}
	int areaID = 0;
	if (!core->global.deletedAreas.first.empty())
	{
		areaID = * core->global.deletedAreas.first.begin();
		core->global.deletedAreas.first.erase(core->global.deletedAreas.first.begin());
	}
	else
	{
		areaID = ++core->global.deletedAreas.second;
	}
	Core::SharedArea area(new Element::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->extraID = 0;
	if ((int)params[6] >= 0 && (int)params[6] < MAX_INTERIORS)
	{
		area->interiors.set((int)params[6]);
	}
	else
	{
		area->interiors.set();
	}
	if ((int)params[7] >= 0 && (int)params[7] < MAX_PLAYERS)
	{
		area->players.set((int)params[7]);
	}
	else
	{
		area->players.set();
	}
	area->size = amx_ctof(params[4]) * amx_ctof(params[4]);
	area->type = 2;
	if ((int)params[5] >= 0)
	{
		area->worlds.insert((int)params[5]);
	}
	area->x1 = amx_ctof(params[1]);
	area->y1 = amx_ctof(params[2]);
	area->z1 = amx_ctof(params[3]);
	grid->addArea(area);
	core->areas.insert(std::make_pair(areaID, area));
	return (cell)areaID;
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCube(AMX * amx, cell * params)
{
	CHECK_PARAMS(9, "CreateDynamicCube");
	if (core->global.maxAreas == core->areas.size())
	{
		return 0;
	}
	int areaID = 0;
	if (!core->global.deletedAreas.first.empty())
	{
		areaID = * core->global.deletedAreas.first.begin();
		core->global.deletedAreas.first.erase(core->global.deletedAreas.first.begin());
	}
	else
	{
		areaID = ++core->global.deletedAreas.second;
	}
	Core::SharedArea area(new Element::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->extraID = 0;
	if ((int)params[8] >= 0 && (int)params[8] < MAX_INTERIORS)
	{
		area->interiors.set((int)params[8]);
	}
	else
	{
		area->interiors.set();
	}
	if ((int)params[9] >= 0 && (int)params[9] < MAX_PLAYERS)
	{
		area->players.set((int)params[9]);
	}
	else
	{
		area->players.set();
	}
	area->type = 3;
	if ((int)params[7] >= 0)
	{
		area->worlds.insert((int)params[7]);
	}
	area->x1 = amx_ctof(params[1]);
	area->y1 = amx_ctof(params[2]);
	area->z1 = amx_ctof(params[3]);
	area->x2 = amx_ctof(params[4]);
	area->y2 = amx_ctof(params[5]);
	area->z2 = amx_ctof(params[6]);
	if (area->x1 > area->x2)
	{
		std::swap(area->x1, area->x2);
	}
	if (area->y1 > area->y2)
	{
		std::swap(area->y1, area->y2);
	}
	if (area->z1 > area->z2)
	{
		std::swap(area->z1, area->z2);
	}
	grid->addArea(area);
	core->areas.insert(std::make_pair(areaID, area));
	return (cell)areaID;
}

cell AMX_NATIVE_CALL Natives::DestroyDynamicArea(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "DestroyDynamicArea");
	boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.find((int)params[1]);
	if (a != core->areas.end())
	{
		for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
		{
			p->disabledAreas.erase(a->first);
			p->visibleAreas.erase(a->first);
		}
		if (core->areas.size() > 1)
		{
			core->global.deletedAreas.first.insert(a->first);
		}
		else
		{
			core->global.deletedAreas = std::make_pair(std::set<int>(), 0);
		}
		grid->removeArea(a->second);
		core->areas.quick_erase(a);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsValidDynamicArea(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "IsValidDynamicArea");
	boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.find((int)params[1]);
	if (a != core->areas.end())
	{
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerDynamicArea(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "TogglePlayerDynamicArea");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			boost::unordered_set<int>::iterator f = p->disabledAreas.find((int)params[2]);
			if ((int)params[3])
			{
				if (f != p->disabledAreas.end())
				{
					p->disabledAreas.quick_erase(f);
					return 1;
				}
			}
			else
			{
				if (f == p->disabledAreas.end())
				{
					p->disabledAreas.insert((int)params[2]);
					p->visibleAreas.erase((int)params[2]);
					return 1;
				}
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::TogglePlayerAllDynamicAreas(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "TogglePlayerAllDynamicAreas");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			p->disabledAreas.clear();
			if (!(int)params[2])
			{
				for (boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.begin(); a != core->areas.end(); ++a)
				{
					p->disabledAreas.insert(a->first);
				}
				p->visibleAreas.clear();
			}
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::IsPlayerInDynamicArea(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "IsPlayerInDynamicArea");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			boost::unordered_set<int>::iterator f = p->visibleAreas.find((int)params[2]);
			if (f != p->visibleAreas.end())
			{
				return 1;
			}
			break;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::AttachDynamicAreaToPlayer(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "AttachDynamicAreaToPlayer");
	boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.find((int)params[1]);
	if (a != core->areas.end())
	{
		if (a->second->type == 1 || a->second->type == 3)
		{
			logprintf("*** AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
			return 0;
		}
		if ((int)params[2] != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Element::Area::Attach>(new Element::Area::Attach);
			a->second->attach->player = (int)params[2];
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			core->movingAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->player != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->movingAreas.erase(a->second);
				}
			}
		}
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::AttachDynamicAreaToVehicle(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "AttachDynamicAreaToVehicle");
	boost::unordered_map<int, Core::SharedArea>::iterator a = core->areas.find((int)params[1]);
	if (a != core->areas.end())
	{
		if (a->second->type == 1 || a->second->type == 3)
		{
			logprintf("*** AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
			return 0;
		}
		if ((int)params[2] != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Element::Area::Attach>(new Element::Area::Attach);
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = (int)params[2];
			core->movingAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->vehicle != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->movingAreas.erase(a->second);
				}
			}
		}
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::DestroyAllDynamicAreas(AMX * amx, cell * params)
{
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		p->disabledAreas.clear();
		p->visibleAreas.clear();
	}
	core->global.deletedAreas = std::make_pair(std::set<int>(), 0);
	core->areas.clear();
	core->movingAreas.clear();
	grid->eraseAllItems(STREAMER_TYPE_AREA);
	return 1;
}

cell AMX_NATIVE_CALL Natives::CountDynamicAreas(AMX * amx, cell * params)
{
	return (cell)core->areas.size();
}

cell AMX_NATIVE_CALL Natives::Streamer_AddPlayer(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_AddPlayer");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			return 0;
		}
	}
	Data::Player player;
	player.activeCheckpoint = 0;
	player.activeRaceCheckpoint = 0;
	player.interiorID = 0;
	player.playerID = (int)params[1];
	player.visibleCheckpoint = 0;
	player.visibleRaceCheckpoint = 0;
	player.visibleObjects = core->global.visibleObjects;
	player.worldID = 0;
	player.x = 0.0f;
	player.y = 0.0f;
	player.z = 0.0f;
	core->players.push_back(player);
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_RemovePlayer(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "Streamer_RemovePlayer");
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			core->players.erase(p);
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_VerifyPickup(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "Streamer_VerifyPickup");
	if (core->pickups.empty())
	{
		return 0;
	}
	boost::unordered_map<int, int>::iterator f = core->global.internalPickups.end();
	for (boost::unordered_map<int, int>::iterator p = core->global.internalPickups.begin(); p != core->global.internalPickups.end(); ++p)
	{
		if (p->second == (int)params[2])
		{
			f = p;
			break;
		}
	}
	if (f == core->global.internalPickups.end())
	{
		return 0;
	}
	boost::unordered_map<int, Core::SharedPickup>::iterator g = core->pickups.find(f->first);
	if (g != core->pickups.end())
	{
		if (g->second->amx != amx)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
	{
		if (p->playerID == (int)params[1])
		{
			int pickupID = f->first;
			for (std::list<AMX *>::iterator a = invoke->amx_list.begin(); a != invoke->amx_list.end(); ++a)
			{
				if (!amx_FindPublic(* a, "OnPlayerPickUpDynamicPickup", &invoke->amx_idx))
				{
					amx_Push(* a, pickupID);
					amx_Push(* a, p->playerID);
					amx_Exec(* a, NULL, invoke->amx_idx);
				}
			}
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_VerifyCheckpoint(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "Streamer_VerifyCheckpoint");
	switch ((int)params[2])
	{
		case STREAMER_TYPE_CP:
		{
			if (core->checkpoints.empty())
			{
				return 0;
			}
			if (!(int)params[3])
			{
				for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
				{
					if (p->playerID == (int)params[1])
					{
						if (p->activeCheckpoint != p->visibleCheckpoint)
						{
							int checkpointID = p->visibleCheckpoint;
							p->activeCheckpoint = checkpointID;
							for (std::list<AMX *>::iterator a = invoke->amx_list.begin(); a != invoke->amx_list.end(); ++a)
							{
								if (!amx_FindPublic(* a, "OnPlayerEnterDynamicCP", &invoke->amx_idx))
								{
									amx_Push(* a, checkpointID);
									amx_Push(* a, p->playerID);
									amx_Exec(* a, NULL, invoke->amx_idx);
								}
							}
							return 1;
						}
						break;
					}
				}
			}
			else
			{
				for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
				{
					if (p->playerID == (int)params[1])
					{
						if (p->activeCheckpoint == p->visibleCheckpoint)
						{
							int checkpointID = p->activeCheckpoint;
							p->activeCheckpoint = 0;
							for (std::list<AMX *>::iterator a = invoke->amx_list.begin(); a != invoke->amx_list.end(); ++a)
							{
								if (!amx_FindPublic(* a, "OnPlayerLeaveDynamicCP", &invoke->amx_idx))
								{
									amx_Push(* a, checkpointID);
									amx_Push(* a, p->playerID);
									amx_Exec(* a, NULL, invoke->amx_idx);
								}
							}
							return 1;
						}
						break;
					}
				}
			}
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			if (core->raceCheckpoints.empty())
			{
				return 0;
			}
			if (!(int)params[3])
			{
				for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
				{
					if (p->playerID == (int)params[1])
					{
						if (p->activeRaceCheckpoint != p->visibleRaceCheckpoint)
						{
							int checkpointID = p->visibleRaceCheckpoint;
							p->activeRaceCheckpoint = checkpointID;
							for (std::list<AMX *>::iterator a = invoke->amx_list.begin(); a != invoke->amx_list.end(); ++a)
							{
								if (!amx_FindPublic(* a, "OnPlayerEnterDynamicRaceCP", &invoke->amx_idx))
								{
									amx_Push(* a, checkpointID);
									amx_Push(* a, p->playerID);
									amx_Exec(* a, NULL, invoke->amx_idx);
								}
							}
							return 1;
						}
						break;
					}
				}
			}
			else
			{
				for (std::vector<Data::Player>::iterator p = core->players.begin(); p != core->players.end(); ++p)
				{
					if (p->playerID == (int)params[1])
					{
						if (p->activeRaceCheckpoint == p->visibleRaceCheckpoint)
						{
							int checkpointID = p->activeRaceCheckpoint;
							p->activeRaceCheckpoint = 0;
							for (std::list<AMX *>::iterator a = invoke->amx_list.begin(); a != invoke->amx_list.end(); ++a)
							{
								if (!amx_FindPublic(* a, "OnPlayerLeaveDynamicRaceCP", &invoke->amx_idx))
								{
									amx_Push(* a, checkpointID);
									amx_Push(* a, p->playerID);
									amx_Exec(* a, NULL, invoke->amx_idx);
								}
							}
							return 1;
						}
						break;
					}
				}
			}
		}
		break;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_RegisterInterface(AMX * amx, cell * params)
{
	if (!invoke->getAddresses())
	{
		logprintf("*** Streamer Plugin: Error obtaining native addresses");
		return 0;
	}
	return 1;
}
