/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

// Eluna
#include "LuaEngine.h"
#include "Includes.h"
// Methods
#include "GlobalMethods.h"
#include "ObjectMethods.h"
#include "WorldObjectMethods.h"
#include "UnitMethods.h"
#include "PlayerMethods.h"
#include "CreatureMethods.h"
#include "GroupMethods.h"
#include "GuildMethods.h"
#include "GameObjectMethods.h"
#include "QueryMethods.h"
#include "AuraMethods.h"
#include "ItemMethods.h"
#include "WorldPacketMethods.h"
#include "SpellMethods.h"
#include "QuestMethods.h"
#include "MapMethods.h"
#include "CorpseMethods.h"
#include "WeatherMethods.h"
#include "VehicleMethods.h"

ElunaFunctions GlobalFunctions[] =
{
    // Hooks
    { ENV_GLOBAL, "RegisterServerEvent", &LuaGlobalFunctions::RegisterServerEvent },                       // RegisterServerEvent(event, function)
    { ENV_GLOBAL, "RegisterPacketEvent", &LuaGlobalFunctions::RegisterPacketEvent },                       // RegisterPacketEvent(opcodeID, function)
    { ENV_GLOBAL, "RegisterPlayerEvent", &LuaGlobalFunctions::RegisterPlayerEvent },                       // RegisterPlayerEvent(event, function)
    { ENV_GLOBAL, "RegisterGuildEvent", &LuaGlobalFunctions::RegisterGuildEvent },                         // RegisterGuildEvent(event, function)
    { ENV_GLOBAL, "RegisterGroupEvent", &LuaGlobalFunctions::RegisterGroupEvent },                         // RegisterGroupEvent(event, function)
    { ENV_GLOBAL, "RegisterCreatureEvent", &LuaGlobalFunctions::RegisterCreatureEvent },                   // RegisterCreatureEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterGameObjectEvent", &LuaGlobalFunctions::RegisterGameObjectEvent },               // RegisterGameObjectEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterItemEvent", &LuaGlobalFunctions::RegisterItemEvent },                           // RegisterItemEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterCreatureGossipEvent", &LuaGlobalFunctions::RegisterCreatureGossipEvent },       // RegisterCreatureGossipEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterGameObjectGossipEvent", &LuaGlobalFunctions::RegisterGameObjectGossipEvent },   // RegisterGameObjectGossipEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterItemGossipEvent", &LuaGlobalFunctions::RegisterItemGossipEvent },               // RegisterItemGossipEvent(entry, event, function)
    { ENV_GLOBAL, "RegisterPlayerGossipEvent", &LuaGlobalFunctions::RegisterPlayerGossipEvent },           // RegisterPlayerGossipEvent(menu_id, event, function)

    // Getters
    { ENV_GLOBAL, "GetLuaEngine", &LuaGlobalFunctions::GetLuaEngine },                                     // GetLuaEngine() - Returns ElunaEngine
    { ENV_GLOBAL, "GetCoreName", &LuaGlobalFunctions::GetCoreName },                                       // GetCoreName() - Returns core name
    { ENV_GLOBAL, "GetCoreVersion", &LuaGlobalFunctions::GetCoreVersion },                                 // GetCoreVersion() - Returns core version string
    { ENV_GLOBAL, "GetCoreExpansion", &LuaGlobalFunctions::GetCoreExpansion },                             // GetCoreExpansion() - Returns core expansion number (0 for classic, 1 for tbc, 2 for wotlk, 3 for cata), returns nil if not found.
    { ENV_GLOBAL, "GetQuest", &LuaGlobalFunctions::GetQuest },                                             // GetQuest(questId) - Returns quest object
    { ENV_GLOBAL, "GetPlayerByGUID", &LuaGlobalFunctions::GetPlayerByGUID },                               // GetPlayerByGUID(guid) - Returns player object by GUID
    { ENV_GLOBAL, "GetPlayerByName", &LuaGlobalFunctions::GetPlayerByName },                               // GetPlayerByName(name) - Returns player object by player name
    { ENV_GLOBAL, "GetGameTime", &LuaGlobalFunctions::GetGameTime },                                       // GetGameTime() - Returns game time
    { ENV_GLOBAL, "GetPlayersInMap", &LuaGlobalFunctions::GetPlayersInMap },                               // GetPlayersInWorld(mapId[, instanceId, team]) - Returns a table with all player objects in map. Team can be 0 for ally, 1 for horde and 2 for neutral
    { ENV_GLOBAL, "GetGuildByName", &LuaGlobalFunctions::GetGuildByName },                                 // GetGuildByName(name) - Returns guild object by the guild name
    { ENV_GLOBAL, "GetGuildByLeaderGUID", &LuaGlobalFunctions::GetGuildByLeaderGUID },                     // GetGuildByLeaderGUID(guid) - Returns guild by it's leader's guid
    { ENV_GLOBAL, "GetPlayerCount", &LuaGlobalFunctions::GetPlayerCount },                                 // GetPlayerCount() - Returns the server's player count
    { ENV_GLOBAL, "GetPlayerGUID", &LuaGlobalFunctions::GetPlayerGUID },                                   // GetPlayerGUID(lowguid) - Generates GUID (uint64) string from player lowguid UNDOCUMENTED
    { ENV_GLOBAL, "GetItemGUID", &LuaGlobalFunctions::GetItemGUID },                                       // GetItemGUID(lowguid) - Generates GUID (uint64) string from item lowguid UNDOCUMENTED
    { ENV_GLOBAL, "GetObjectGUID", &LuaGlobalFunctions::GetObjectGUID },                                   // GetObjectGUID(lowguid, entry) - Generates GUID (uint64) string from gameobject lowguid and entry UNDOCUMENTED
    { ENV_GLOBAL, "GetUnitGUID", &LuaGlobalFunctions::GetUnitGUID },                                       // GetUnitGUID(lowguid, entry) - Generates GUID (uint64) string from unit (creature) lowguid and entry UNDOCUMENTED
    { ENV_GLOBAL, "GetGUIDLow", &LuaGlobalFunctions::GetGUIDLow },                                         // GetGUIDLow(guid) - Returns GUIDLow (uint32) from guid (uint64 as string) UNDOCUMENTED
    { ENV_GLOBAL, "GetGUIDType", &LuaGlobalFunctions::GetGUIDType },                                       // GetGUIDType(guid) - Returns Type (uint32) from guid (uint64 as string) UNDOCUMENTED
    { ENV_GLOBAL, "GetGUIDEntry", &LuaGlobalFunctions::GetGUIDEntry },                                     // GetGUIDEntry(guid) - Returns Entry (uint32) from guid (uint64 as string), may be always 0 UNDOCUMENTED
    { ENV_GLOBAL, "bit_not", &LuaGlobalFunctions::bit_not },                                               // bit_not(a) - Returns ~a UNDOCUMENTED
    { ENV_GLOBAL, "bit_xor", &LuaGlobalFunctions::bit_xor },                                               // bit_xor(a, b) - Returns a ^ b UNDOCUMENTED
    { ENV_GLOBAL, "bit_rshift", &LuaGlobalFunctions::bit_rshift },                                         // bit_rshift(a, b) - Returns a >> b UNDOCUMENTED
    { ENV_GLOBAL, "bit_lshift", &LuaGlobalFunctions::bit_lshift },                                         // bit_lshift(a, b) - Returns a << b UNDOCUMENTED
    { ENV_GLOBAL, "bit_or", &LuaGlobalFunctions::bit_or },                                                 // bit_or(a, b) - Returns a | b UNDOCUMENTED
    { ENV_GLOBAL, "bit_and", &LuaGlobalFunctions::bit_and },                                               // bit_and(a, b) - Returns a & b UNDOCUMENTED
    { ENV_GLOBAL, "GetItemLink", &LuaGlobalFunctions::GetItemLink },                                       // GetItemLink(entry[, localeIndex]) - Returns the shift clickable link of the item. Item name translated if translate available for provided locale index UNDOCUMENTED
    { ENV_GLOBAL, "GetMap", &LuaGlobalFunctions::GetMap },                                                 // GetMap() - Returns map object of the current lua state, if state is global, returns nil

    // Other
    // { ENV_GLOBAL, "ReloadEluna", &LuaGlobalFunctions::ReloadEluna },                                    // ReloadEluna() - Reload's Eluna engine. Returns true if reload succesful.
    { ENV_GLOBAL, "SendWorldMessage", &LuaGlobalFunctions::SendWorldMessage },                             // SendWorldMessage(msg) - Sends a broadcast message to everyone
    { ENV_GLOBAL, "WorldDBQuery", &LuaGlobalFunctions::WorldDBQuery },                                     // WorldDBQuery(sql) - Executes given SQL query to world database instantly and returns a QueryResult object
    { ENV_GLOBAL, "WorldDBExecute", &LuaGlobalFunctions::WorldDBExecute },                                 // WorldDBExecute(sql) - Executes given SQL query to world database (not instant)
    { ENV_GLOBAL, "CharDBQuery", &LuaGlobalFunctions::CharDBQuery },                                       // CharDBQuery(sql) - Executes given SQL query to character database instantly and returns a QueryResult object
    { ENV_GLOBAL, "CharDBExecute", &LuaGlobalFunctions::CharDBExecute },                                   // CharDBExecute(sql) - Executes given SQL query to character database (not instant)
    { ENV_GLOBAL, "AuthDBQuery", &LuaGlobalFunctions::AuthDBQuery },                                       // AuthDBQuery(sql) - Executes given SQL query to auth/logon database instantly and returns a QueryResult object
    { ENV_GLOBAL, "AuthDBExecute", &LuaGlobalFunctions::AuthDBExecute },                                   // AuthDBExecute(sql) - Executes given SQL query to auth/logon database (not instant)
    { ENV_GLOBAL, "CreateLuaEvent", &LuaGlobalFunctions::CreateLuaEvent },                                 // CreateLuaEvent(function, delay, calls) - Creates a global timed event. Returns Event ID. Calls set to 0 calls infinitely.
    { ENV_GLOBAL, "RemoveEventById", &LuaGlobalFunctions::RemoveEventById },                               // RemoveEventById(eventId, [all_events]) - Removes a global timed event by it's ID. If all_events is true, can remove any timed event by ID (unit, gameobject, global..)
    { ENV_GLOBAL, "RemoveEvents", &LuaGlobalFunctions::RemoveEvents },                                     // RemoveEvents([all_events]) - Removes all global timed events. Removes all timed events (unit, gameobject, global) if all_events is true
    { ENV_GLOBAL, "PerformIngameSpawn", &LuaGlobalFunctions::PerformIngameSpawn },                         // PerformIngameSpawn(spawntype, entry, mapid, instanceid, x, y, z, o[, save, DurOrResptime, phase]) - spawntype: 1 Creature, 2 Object. DurOrResptime is respawntime for gameobjects and despawntime for creatures if creature is not saved. Returns spawned creature/gameobject
    { ENV_GLOBAL, "CreatePacket", &LuaGlobalFunctions::CreatePacket },                                     // CreatePacket(opcode, size) - Creates a new packet object
    { ENV_GLOBAL, "AddVendorItem", &LuaGlobalFunctions::AddVendorItem },                                   // AddVendorItem(entry, itemId, maxcount, incrtime, extendedcost) - Adds an item to vendor entry.
    { ENV_GLOBAL, "VendorRemoveItem", &LuaGlobalFunctions::VendorRemoveItem },                             // VendorRemoveItem(entry, item) - Removes an item from vendor entry
    { ENV_GLOBAL, "VendorRemoveAllItems", &LuaGlobalFunctions::VendorRemoveAllItems },                     // VendorRemoveAllItems(entry) - Removes all items from vendor entry
    { ENV_GLOBAL, "Kick", &LuaGlobalFunctions::Kick },                                                     // Kick(player) - Kicks given player
    { ENV_GLOBAL, "Ban", &LuaGlobalFunctions::Ban },                                                       // Ban(banMode(integer), nameOrIP(string), duration(string), reason(string), player(whoBanned)) - Banmode: 0 account, 1 character, 2 IP
    { ENV_GLOBAL, "SaveAllPlayers", &LuaGlobalFunctions::SaveAllPlayers },                                 // SaveAllPlayers() - Saves all players
    { ENV_GLOBAL, "SendMail", &LuaGlobalFunctions::SendMail },                                             // SendMail(subject, text, receiverLowGUID[, senderLowGUID, stationary, delay, itemEntry, itemAmount, itemEntry2, itemAmount2...]) - Sends a mail to player with lowguid. use nil to use default values on optional arguments. UNDOCUMENTED
    { ENV_GLOBAL, "AddTaxiPath", &LuaGlobalFunctions::AddTaxiPath },                                       // AddTaxiPath(pathTable, mountA, mountH[, price, pathId]) - Adds a new taxi path. Returns the path's ID. Will replace an existing path if pathId provided and already used. path table structure: T = {{map, x, y, z[, actionFlag, delay, arrivalEvId, departEvId]}, {...}, ...} UDOCUMENTED
    { ENV_GLOBAL, "AddCorpse", &LuaGlobalFunctions::AddCorpse },                                           // AddCorpse(corpse) - Adds the player's corpse to the world. More specifically, the cell.
    { ENV_GLOBAL, "RemoveCorpse", &LuaGlobalFunctions::RemoveCorpse },                                     // RemoveCorpse(corpse) - Removes the player's corpse from the world.
    { ENV_GLOBAL, "ConvertCorpseForPlayer", &LuaGlobalFunctions::ConvertCorpseForPlayer },                 // ConvertCorpseFromPlayer(guid[, insignia]) - Converts the player's corpse to bones. Adding insignia for PvP is optional (true or false).
    { ENV_GLOBAL, "RemoveOldCorpses", &LuaGlobalFunctions::RemoveOldCorpses },                             // RemoveOldCorpses() - Converts (removes) old corpses that aren't bones.
    { ENV_GLOBAL, "FindWeather", &LuaGlobalFunctions::FindWeather },                                       // FindWeather(zoneId) - Finds the weather by zoneId and returns the weather
    { ENV_GLOBAL, "AddWeather", &LuaGlobalFunctions::AddWeather },                                         // AddWeather(zoneId) - Adds weather to the following zone, also returns weather
    { ENV_GLOBAL, "RemoveWeather", &LuaGlobalFunctions::RemoveWeather },                                   // RemoveWeather(zoneId) - Removes weather from a zone
    { ENV_GLOBAL, "SendFineWeatherToPlayer", &LuaGlobalFunctions::SendFineWeatherToPlayer },               // SendFineWeatherToPlayer(player) - Sends WEATHER_STATE_FINE weather to the
    { ENV_GLOBAL, "DoForPlayersInWorld", &LuaGlobalFunctions::DoForPlayersInWorld },                       // DoForPlayersInWorld(function(player)) - Calls given function for all players in world. Passes player to the function.
    { ENV_MAP, "SendStateMsg", &LuaGlobalFunctions::map_SendStateMsg },                                    // SendStateMsg([target_map, target_instanceid, value1, value2...]) - Sends an async message to given map lua state if exists (triggers ELUNA_EVENT_ON_STATE_MSG). Sends to world state if map is nil.
    { ENV_WORLD, "SendStateMsg", &LuaGlobalFunctions::world_SendStateMsg },                                // SendStateMsg([target_map, target_instanceid, value1, value2...]) - Sends a direct message to given map lua state if exists (triggers ELUNA_EVENT_ON_STATE_MSG). Sends to world state if map is nil (or -1).

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Object> ObjectMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetEntry", &LuaObject::GetEntry },                     // :GetEntry() - Returns the object's entryId
    { ENV_GLOBAL, "GetGUID", &LuaObject::GetGUID },                       // :GetGUID() - Returns uint64 guid as hex string
    { ENV_GLOBAL, "GetGUIDLow", &LuaObject::GetGUIDLow },                 // :GetGUIDLow() - Returns uint32 guid (low guid) that is used in database.
    { ENV_GLOBAL, "GetInt32Value", &LuaObject::GetInt32Value },           // :GetInt32Value(index) - returns an int value from object fields
    { ENV_GLOBAL, "GetUInt32Value", &LuaObject::GetUInt32Value },         // :GetUInt32Value(index) - returns an uint value from object fields
    { ENV_GLOBAL, "GetFloatValue", &LuaObject::GetFloatValue },           // :GetFloatValue(index) - returns a float value from object fields
    { ENV_GLOBAL, "GetByteValue", &LuaObject::GetByteValue },             // :GetByteValue(index, offset) - returns a byte value from object fields
    { ENV_GLOBAL, "GetUInt16Value", &LuaObject::GetUInt16Value },         // :GetUInt16Value(index, offset) - returns a uint16 value from object fields
    { ENV_GLOBAL, "GetUInt64Value", &LuaObject::GetUInt64Value },         // :GetUInt64Value(index) - returns a uint64 value from object fields
    { ENV_GLOBAL, "GetScale", &LuaObject::GetScale },                     // :GetScale()
    { ENV_GLOBAL, "GetTypeId", &LuaObject::GetTypeId },                   // :GetTypeId() - Returns the object's typeId

    // Setters
    { ENV_GLOBAL, "SetInt32Value", &LuaObject::SetInt32Value },           // :SetInt32Value(index, value) - Sets an int value for the object
    { ENV_GLOBAL, "SetUInt32Value", &LuaObject::SetUInt32Value },         // :SetUInt32Value(index, value) - Sets an uint value for the object
    { ENV_GLOBAL, "UpdateUInt32Value", &LuaObject::UpdateUInt32Value },   // :UpdateUInt32Value(index, value) - Updates an uint value for the object
    { ENV_GLOBAL, "SetFloatValue", &LuaObject::SetFloatValue },           // :SetFloatValue(index, value) - Sets a float value for the object
    { ENV_GLOBAL, "SetByteValue", &LuaObject::SetByteValue },             // :SetByteValue(index, offset, value) - Sets a byte value for the object
    { ENV_GLOBAL, "SetUInt16Value", &LuaObject::SetUInt16Value },         // :SetUInt16Value(index, offset, value) - Sets an uint16 value for the object
    { ENV_GLOBAL, "SetInt16Value", &LuaObject::SetInt16Value },           // :SetInt16Value(index, offset, value) - Sets an int16 value for the object
    { ENV_GLOBAL, "SetUInt64Value", &LuaObject::SetUInt64Value },         // :SetUInt64Value(index, value) - Sets an uint64 value for the object
    { ENV_GLOBAL, "SetScale", &LuaObject::SetScale },                     // :SetScale(scale)
    { ENV_GLOBAL, "SetFlag", &LuaObject::SetFlag },                       // :SetFlag(index, flag)

    // Boolean
    { ENV_GLOBAL, "IsInWorld", &LuaObject::IsInWorld },                   // :IsInWorld() - Returns if the object is in world
    { ENV_GLOBAL, "HasFlag", &LuaObject::HasFlag },                       // :HasFlag(index, flag)

    // Other
    { ENV_GLOBAL, "ToGameObject", &LuaObject::ToGameObject },             // :ToGameObject()
    { ENV_GLOBAL, "ToUnit", &LuaObject::ToUnit },                         // :ToUnit()
    { ENV_GLOBAL, "ToCreature", &LuaObject::ToCreature },                 // :ToCreature()
    { ENV_GLOBAL, "ToPlayer", &LuaObject::ToPlayer },                     // :ToPlayer()
    { ENV_GLOBAL, "ToCorpse", &LuaObject::ToCorpse },                     // :ToCorpse()
    { ENV_GLOBAL, "RemoveFlag", &LuaObject::RemoveFlag },                 // :RemoveFlag(index, flag)

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<WorldObject> WorldObjectMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetName", &LuaWorldObject::GetName },                              // :GetName()
    { ENV_GLOBAL, "GetMap", &LuaWorldObject::GetMap },                                // :GetMap() - Returns the WorldObject's current map object
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "GetPhaseMask", &LuaWorldObject::GetPhaseMask },                    // :GetPhaseMask()
#endif
    { ENV_GLOBAL, "GetInstanceId", &LuaWorldObject::GetInstanceId },                  // :GetInstanceId()
    { ENV_GLOBAL, "GetAreaId", &LuaWorldObject::GetAreaId },                          // :GetAreaId()
    { ENV_GLOBAL, "GetZoneId", &LuaWorldObject::GetZoneId },                          // :GetZoneId()
    { ENV_GLOBAL, "GetMapId", &LuaWorldObject::GetMapId },                            // :GetMapId() - Returns the WorldObject's current map ID
    { ENV_GLOBAL, "GetX", &LuaWorldObject::GetX },                                    // :GetX()
    { ENV_GLOBAL, "GetY", &LuaWorldObject::GetY },                                    // :GetY()
    { ENV_GLOBAL, "GetZ", &LuaWorldObject::GetZ },                                    // :GetZ()
    { ENV_GLOBAL, "GetO", &LuaWorldObject::GetO },                                    // :GetO()
    { ENV_GLOBAL, "GetLocation", &LuaWorldObject::GetLocation },                      // :GetLocation() - returns X, Y, Z and O co - ords (in that order)
    { ENV_GLOBAL, "GetPlayersInRange", &LuaWorldObject::GetPlayersInRange },          // :GetPlayersInRange([range]) - Returns a table with players in range of the WorldObject.
    { ENV_GLOBAL, "GetCreaturesInRange", &LuaWorldObject::GetCreaturesInRange },      // :GetCreaturesInRange([range, entry]) - Returns a table with creatures of given entry in range of the WorldObject.
    { ENV_GLOBAL, "GetGameObjectsInRange", &LuaWorldObject::GetGameObjectsInRange },  // :GetGameObjectsInRange([range, entry]) - Returns a table with gameobjects of given entry in range of the WorldObject.
    { ENV_GLOBAL, "GetNearestPlayer", &LuaWorldObject::GetNearestPlayer },            // :GetNearestPlayer([range]) - Returns nearest player in sight or given range.
    { ENV_GLOBAL, "GetNearestGameObject", &LuaWorldObject::GetNearestGameObject },    // :GetNearestGameObject([range, entry]) - Returns nearest gameobject with given entry in sight or given range entry can be 0 or nil for any.
    { ENV_GLOBAL, "GetNearestCreature", &LuaWorldObject::GetNearestCreature },        // :GetNearestCreature([range, entry]) - Returns nearest creature with given entry in sight or given range entry can be 0 or nil for any.
    { ENV_GLOBAL, "GetNearObject", &LuaWorldObject::GetNearObject },                  // :GetNearObject([nearest, range, typemask, entry, hostile]) - Returns nearest WorldObject or table of objects in given range with given typemask (can contain several types) with given entry if given. Hostile can be 0 for any, 1 hostile, 2 friendly
    { ENV_GLOBAL, "GetWorldObject", &LuaWorldObject::GetWorldObject },                // :GetWorldObject(guid) - Returns a world object (creature, player, gameobject) from the guid. The world object returned must be on the same map as the world object in the arguments.
    { ENV_GLOBAL, "GetDistance", &LuaWorldObject::GetDistance },                      // :GetDistance(WorldObject or x, y, z) - Returns the distance between 2 objects or location
    { ENV_GLOBAL, "GetRelativePoint", &LuaWorldObject::GetRelativePoint },            // :GetRelativePoint(dist, rad) - Returns the x, y and z of a point dist away from worldobject.
    { ENV_GLOBAL, "GetAngle", &LuaWorldObject::GetAngle },                            // :GetAngle(WorldObject or x, y) - Returns angle between world object and target or x and y coords.

    // Other
    { ENV_GLOBAL, "SummonGameObject", &LuaWorldObject::SummonGameObject },            // :SummonGameObject(entry, x, y, z, o[, respawnDelay]) - Spawns an object to location. Returns the object or nil
    { ENV_GLOBAL, "SpawnCreature", &LuaWorldObject::SpawnCreature },                  // :SpawnCreature(entry, x, y, z, o[, spawnType, despawnDelay]) - Spawns a creature to location that despawns after given time (0 for infinite). Returns the creature or nil
    { ENV_GLOBAL, "SendPacket", &LuaWorldObject::SendPacket },                        // :SendPacket(packet) - Sends a specified packet to everyone around

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Unit> UnitMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetLevel", &LuaUnit::GetLevel },                                   // :GetLevel()
    { ENV_GLOBAL, "GetHealth", &LuaUnit::GetHealth },                                 // :GetHealth()
    { ENV_GLOBAL, "GetDisplayId", &LuaUnit::GetDisplayId },                           // :GetDisplayId()
    { ENV_GLOBAL, "GetNativeDisplayId", &LuaUnit::GetNativeDisplayId },               // :GetNativeDisplayId()
    { ENV_GLOBAL, "GetPower", &LuaUnit::GetPower },                                   // :GetPower(index) - returns power at index. Index can be omitted
    { ENV_GLOBAL, "GetMaxPower", &LuaUnit::GetMaxPower },                             // :GetMaxPower(index) - returns power at index. Index can be omitted
    { ENV_GLOBAL, "GetPowerType", &LuaUnit::GetPowerType },                           // :GetPowerType() - Returns the power type
    { ENV_GLOBAL, "GetMaxHealth", &LuaUnit::GetMaxHealth },                           // :GetMaxHealth()
    { ENV_GLOBAL, "GetHealthPct", &LuaUnit::GetHealthPct },                           // :GetHealthPct()
    { ENV_GLOBAL, "GetPowerPct", &LuaUnit::GetPowerPct },                             // :GetPowerPct(power_id)
    { ENV_GLOBAL, "GetGender", &LuaUnit::GetGender },                                 // :GetGender() - returns the gender where male = 0 female = 1
    { ENV_GLOBAL, "GetRace", &LuaUnit::GetRace },                                     // :GetRace()
    { ENV_GLOBAL, "GetClass", &LuaUnit::GetClass },                                   // :GetClass()
    { ENV_GLOBAL, "GetClassAsString", &LuaUnit::GetClassAsString },                   // :GetClassAsString()
    { ENV_GLOBAL, "GetAura", &LuaUnit::GetAura },                                     // :GetAura(spellID) - returns aura object
    { ENV_GLOBAL, "GetCombatTime", &LuaUnit::GetCombatTime },                         // :GetCombatTime() - Returns how long the unit has been in combat
    { ENV_GLOBAL, "GetFaction", &LuaUnit::GetFaction },                               // :GetFaction() - Returns the unit's factionId
    { ENV_GLOBAL, "GetCurrentSpell", &LuaUnit::GetCurrentSpell },                     // :GetCurrentSpell(type) - Returns the currently casted spell of given type if any
    { ENV_GLOBAL, "GetCreatureType", &LuaUnit::GetCreatureType },                     // :GetCreatureType() - Returns the unit's type
    { ENV_GLOBAL, "GetMountId", &LuaUnit::GetMountId },                               // :GetMountId()
    { ENV_GLOBAL, "GetOwnerGUID", &LuaUnit::GetOwnerGUID },                           // :GetOwnerGUID() - Returns the GUID of the owner
    { ENV_GLOBAL, "GetOwner", &LuaUnit::GetOwner },                                   // :GetOwner() - Returns the owner
    { ENV_GLOBAL, "GetFriendlyUnitsInRange", &LuaUnit::GetFriendlyUnitsInRange },     // :GetFriendlyUnitsInRange([range]) - Returns a list of friendly units in range, can return nil
    { ENV_GLOBAL, "GetUnfriendlyUnitsInRange", &LuaUnit::GetUnfriendlyUnitsInRange }, // :GetUnfriendlyUnitsInRange([range]) - Returns a list of unfriendly units in range, can return nil
    { ENV_GLOBAL, "GetOwnerGUID", &LuaUnit::GetOwnerGUID },                           // :GetOwnerGUID() - Returns the UNIT_FIELD_SUMMONEDBY owner
    { ENV_GLOBAL, "GetCreatorGUID", &LuaUnit::GetCreatorGUID },                       // :GetCreatorGUID() - Returns the UNIT_FIELD_CREATEDBY creator
    { ENV_GLOBAL, "GetMinionGUID", &LuaUnit::GetMinionGUID },                         // :GetMinionGUID() - Returns the UNIT_FIELD_SUMMON unit's minion GUID
    { ENV_GLOBAL, "GetCharmerGUID", &LuaUnit::GetCharmerGUID },                       // :GetCharmerGUID() - Returns the UNIT_FIELD_CHARMEDBY charmer
    { ENV_GLOBAL, "GetCharmGUID", &LuaUnit::GetCharmGUID },                           // :GetCharmGUID() - Returns the unit's UNIT_FIELD_CHARM guid
    { ENV_GLOBAL, "GetPetGUID", &LuaUnit::GetPetGUID },                               // :GetPetGUID() - Returns the unit's pet GUID
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "GetCritterGUID", &LuaUnit::GetCritterGUID },                       // :GetCritterGUID() - Returns the critter's GUID
#endif
    { ENV_GLOBAL, "GetControllerGUID", &LuaUnit::GetControllerGUID },                 // :GetControllerGUID() - Returns the Charmer or Owner GUID
    { ENV_GLOBAL, "GetControllerGUIDS", &LuaUnit::GetControllerGUIDS },               // :GetControllerGUIDS() - Returns the charmer, owner or unit's own GUID
    { ENV_GLOBAL, "GetStandState", &LuaUnit::GetStandState },                         // :GetStandState() - Returns the unit's stand state
    { ENV_GLOBAL, "GetVictim", &LuaUnit::GetVictim },                                 // :GetVictim() - Returns creature's current target
    { ENV_GLOBAL, "GetStat", &LuaUnit::GetStat },                                     // :GetStat(stat)
    { ENV_GLOBAL, "GetBaseSpellPower", &LuaUnit::GetBaseSpellPower },                 // :GetBaseSpellPower()
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "GetVehicleKit", &LuaUnit::GetVehicleKit },                         // :GetVehicleKit() - Gets unit's Vehicle kit if the unit is a vehicle
    // {"GetVehicle", &LuaUnit::GetVehicle},                            // :GetVehicle() - Gets the Vehicle kit of the vehicle the unit is on
#endif

    // Setters
    { ENV_GLOBAL, "SetFaction", &LuaUnit::SetFaction },                   // :SetFaction(factionId) - Sets the unit's faction
    { ENV_GLOBAL, "SetLevel", &LuaUnit::SetLevel },                       // :SetLevel(amount)
    { ENV_GLOBAL, "SetHealth", &LuaUnit::SetHealth },                     // :SetHealth(amount)
    { ENV_GLOBAL, "SetMaxHealth", &LuaUnit::SetMaxHealth },               // :SetMaxHealth(amount)
    { ENV_GLOBAL, "SetPower", &LuaUnit::SetPower },                       // :SetPower(index, amount)
    { ENV_GLOBAL, "SetMaxPower", &LuaUnit::SetMaxPower },                 // :SetMaxPower(index, amount)
    { ENV_GLOBAL, "SetDisplayId", &LuaUnit::SetDisplayId },               // :SetDisplayId(id)
    { ENV_GLOBAL, "SetNativeDisplayId", &LuaUnit::SetNativeDisplayId },   // :SetNativeDisplayId(id)
    { ENV_GLOBAL, "SetFacing", &LuaUnit::SetFacing },                     // :SetFacing(o) - Sets the Unit facing to arg
    { ENV_GLOBAL, "SetFacingToObject", &LuaUnit::SetFacingToObject },     // :SetFacingToObject(worldObject) - Sets the Unit facing towards the WorldObject
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "SetPhaseMask", &LuaUnit::SetPhaseMask },               // :SetPhaseMask(Phase[, update]) - Sets the phase of the unit
#endif
    { ENV_GLOBAL, "SetSpeed", &LuaUnit::SetSpeed },                       // :SetSpeed(type, speed[, forced]) - Sets speed for the movement type (0 = walk, 1 = run ..)
    // {"SetStunned", &LuaUnit::SetStunned},                // :SetStunned([enable]) - Stuns or removes stun
    // {"SetRooted", &LuaUnit::SetRooted},                  // :SetRooted([enable]) - Roots or removes root
    // {"SetConfused", &LuaUnit::SetConfused},              // :SetConfused([enable]) - Sets confused or removes confusion
    // {"SetFeared", &LuaUnit::SetFeared},                  // :SetFeared([enable]) - Fears or removes fear
    { ENV_GLOBAL, "SetPvP", &LuaUnit::SetPvP },                           // :SetPvP([apply]) - Sets the units PvP on or off
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "SetFFA", &LuaUnit::SetFFA },                           // :SetFFA([apply]) - Sets the units FFA tag on or off
    { ENV_GLOBAL, "SetSanctuary", &LuaUnit::SetSanctuary },               // :SetSanctuary([apply]) - Enables or disables units sanctuary flag
#endif
    // {"SetCanFly", &LuaUnit::SetCanFly},                  // :SetCanFly(apply)
    // {"SetVisible", &LuaUnit::SetVisible},                // :SetVisible(x)
    { ENV_GLOBAL, "SetOwnerGUID", &LuaUnit::SetOwnerGUID },               // :SetOwnerGUID(guid) - Sets the guid of the owner
    { ENV_GLOBAL, "SetName", &LuaUnit::SetName },                         // :SetName(name) - Sets the unit's name
    { ENV_GLOBAL, "SetSheath", &LuaUnit::SetSheath },                     // :SetSheath(SheathState) - Sets unit's sheathstate
    { ENV_GLOBAL, "SetCreatorGUID", &LuaUnit::SetCreatorGUID },           // :SetOwnerGUID(uint64 ownerGUID) - Sets the owner's guid of a summoned creature, etc
    { ENV_GLOBAL, "SetMinionGUID", &LuaUnit::SetMinionGUID },             // :SetCreatorGUID(uint64 creatorGUID) - Sets the UNIT_FIELD_CREATEDBY creator's guid
    { ENV_GLOBAL, "SetCharmerGUID", &LuaUnit::SetCharmerGUID },           // :SetCharmerGUID(uint64 ownerGUID) - Sets the UNIT_FIELD_CHARMEDBY charmer GUID
    { ENV_GLOBAL, "SetPetGUID", &LuaUnit::SetPetGUID },                   // :SetPetGUID(uint64 guid) - Sets the pet's guid
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "SetCritterGUID", &LuaUnit::SetCritterGUID },           // :SetCritterGUID(uint64 guid) - Sets the critter's guid
#endif
    { ENV_GLOBAL, "SetWaterWalk", &LuaUnit::SetWaterWalk },               // :SetWaterWalk([enable]) - Sets WaterWalk on or off
    { ENV_GLOBAL, "SetStandState", &LuaUnit::SetStandState },             // :SetStandState(state) - Sets the stand state (Stand, Kneel, sleep, etc) of the unit

    // Boolean
    { ENV_GLOBAL, "IsAlive", &LuaUnit::IsAlive },                                 // :IsAlive()
    { ENV_GLOBAL, "IsDead", &LuaUnit::IsDead },                                   // :IsDead() - Returns true if the unit is dead, false if they are alive
    { ENV_GLOBAL, "IsDying", &LuaUnit::IsDying },                                 // :IsDying() - Returns true if the unit death state is JUST_DIED.
    { ENV_GLOBAL, "IsPvPFlagged", &LuaUnit::IsPvPFlagged },                       // :IsPvPFlagged()
    { ENV_GLOBAL, "IsInCombat", &LuaUnit::IsInCombat },                           // :IsInCombat()
    { ENV_GLOBAL, "IsBanker", &LuaUnit::IsBanker },                               // :IsBanker() - Returns true if the unit is a banker, false if not
    { ENV_GLOBAL, "IsBattleMaster", &LuaUnit::IsBattleMaster },                   // :IsBattleMaster() - Returns true if the unit is a battle master, false if not
    { ENV_GLOBAL, "IsCharmed", &LuaUnit::IsCharmed },                             // :IsCharmed() - Returns true if the unit is charmed, false if not
    { ENV_GLOBAL, "IsArmorer", &LuaUnit::IsArmorer },                             // :IsArmorer() - Returns true if the unit is an Armorer, false if not
    { ENV_GLOBAL, "IsAttackingPlayer", &LuaUnit::IsAttackingPlayer },             // :IsAttackingPlayer() - Returns true if the unit is attacking a player, false if not
    { ENV_GLOBAL, "IsInWater", &LuaUnit::IsInWater },                             // :IsInWater() - Returns true if the unit is in water
    { ENV_GLOBAL, "IsUnderWater", &LuaUnit::IsUnderWater },                       // :IsUnderWater() - Returns true if the unit is under water
    { ENV_GLOBAL, "IsAuctioneer", &LuaUnit::IsAuctioneer },                       // :IsAuctioneer()
    { ENV_GLOBAL, "IsGuildMaster", &LuaUnit::IsGuildMaster },                     // :IsGuildMaster()
    { ENV_GLOBAL, "IsInnkeeper", &LuaUnit::IsInnkeeper },                         // :IsInnkeeper()
    { ENV_GLOBAL, "IsTrainer", &LuaUnit::IsTrainer },                             // :IsTrainer()
    { ENV_GLOBAL, "IsGossip", &LuaUnit::IsGossip },                               // :IsGossip()
    { ENV_GLOBAL, "IsTaxi", &LuaUnit::IsTaxi },                                   // :IsTaxi()
    { ENV_GLOBAL, "IsSpiritHealer", &LuaUnit::IsSpiritHealer },                   // :IsSpiritHealer()
    { ENV_GLOBAL, "IsSpiritGuide", &LuaUnit::IsSpiritGuide },                     // :IsSpiritGuide()
    { ENV_GLOBAL, "IsTabardDesigner", &LuaUnit::IsTabardDesigner },               // :IsSpiritGuide()
    { ENV_GLOBAL, "IsServiceProvider", &LuaUnit::IsServiceProvider },             // :IsServiceProvider()
    { ENV_GLOBAL, "IsSpiritService", &LuaUnit::IsSpiritService },                 // :IsSpiritService()
    { ENV_GLOBAL, "HealthBelowPct", &LuaUnit::HealthBelowPct },                   // :HealthBelowPct(int32 pct)
    { ENV_GLOBAL, "HealthAbovePct", &LuaUnit::HealthAbovePct },                   // :HealthAbovePct(int32 pct)
    { ENV_GLOBAL, "IsMounted", &LuaUnit::IsMounted },                             // :IsMounted()
    { ENV_GLOBAL, "AttackStop", &LuaUnit::AttackStop },                           // :AttackStop()
    { ENV_GLOBAL, "Attack", &LuaUnit::Attack },                                   // :Attack(who[, meleeAttack])
    // {"IsVisible", &LuaUnit::IsVisible},                          // :IsVisible()
    // {"IsMoving", &LuaUnit::IsMoving},                            // :IsMoving()
    // {"IsFlying", &LuaUnit::IsFlying},                            // :IsFlying()
    { ENV_GLOBAL, "IsStopped", &LuaUnit::IsStopped },                             // :IsStopped()
    { ENV_GLOBAL, "HasUnitState", &LuaUnit::HasUnitState },                       // :HasUnitState(state) - state from UnitState enum
    { ENV_GLOBAL, "IsQuestGiver", &LuaUnit::IsQuestGiver },                       // :IsQuestGiver() - Returns true if the unit is a quest giver, false if not
    { ENV_GLOBAL, "IsWithinDistInMap", &LuaUnit::IsWithinDistInMap },             // :IsWithinDistInMap(worldObject, radius) - Returns if the unit is within distance in map of the worldObject
    { ENV_GLOBAL, "IsInAccessiblePlaceFor", &LuaUnit::IsInAccessiblePlaceFor },   // :IsInAccessiblePlaceFor(creature) - Returns if the unit is in an accessible place for the specified creature
    { ENV_GLOBAL, "IsVendor", &LuaUnit::IsVendor },                               // :IsVendor() - Returns if the unit is a vendor or not
    { ENV_GLOBAL, "IsWithinLoS", &LuaUnit::IsWithinLoS },                         // :IsWithinLoS(x, y, z)
    // {"IsRooted", &LuaUnit::IsRooted},                            // :IsRooted()
    { ENV_GLOBAL, "IsFullHealth", &LuaUnit::IsFullHealth },                       // :IsFullHealth() - Returns if the unit is full health
    { ENV_GLOBAL, "HasAura", &LuaUnit::HasAura },                                 // :HasAura(spellId) - Returns true if the unit has the aura from the spell
    { ENV_GLOBAL, "IsStandState", &LuaUnit::IsStandState },                       // :IsStandState() - Returns true if the unit is standing
#ifndef CLASSIC
    { ENV_GLOBAL, "IsOnVehicle", &LuaUnit::IsOnVehicle },                         // :IsOnVehicle() - Checks if the unit is on a vehicle
#endif

    // Other
    { ENV_GLOBAL, "RegisterEvent", &LuaUnit::RegisterEvent },                     // :RegisterEvent(function, delay, calls)
    { ENV_GLOBAL, "RemoveEventById", &LuaUnit::RemoveEventById },                 // :RemoveEventById(eventID)
    { ENV_GLOBAL, "RemoveEvents", &LuaUnit::RemoveEvents },                       // :RemoveEvents()
    { ENV_GLOBAL, "AddAura", &LuaUnit::AddAura },                                 // :AddAura(spellId, target) - Adds an aura to the specified target
    { ENV_GLOBAL, "RemoveAura", &LuaUnit::RemoveAura },                           // :RemoveAura(spellId[, casterGUID]) - Removes an aura from the unit by the spellId, casterGUID(Original caster) is optional
    { ENV_GLOBAL, "RemoveAllAuras", &LuaUnit::RemoveAllAuras },                   // :RemoveAllAuras() - Removes all the unit's auras
    { ENV_GLOBAL, "ClearInCombat", &LuaUnit::ClearInCombat },                     // :ClearInCombat() - Clears the unit's combat list (unit will be out of combat), resets the timer to 0, etc
    { ENV_GLOBAL, "DeMorph", &LuaUnit::DeMorph },                                 // :DeMorph() - Sets display back to native
    { ENV_GLOBAL, "SendUnitWhisper", &LuaUnit::SendUnitWhisper },                 // :SendUnitWhisper(msg, receiver[, bossWhisper]) - Sends a whisper to the receiver
    { ENV_GLOBAL, "SendUnitEmote", &LuaUnit::SendUnitEmote },                     // :SendUnitEmote(msg[, receiver, bossEmote]) - Sends a text emote
    { ENV_GLOBAL, "SendUnitSay", &LuaUnit::SendUnitSay },                         // :SendUnitSay(msg, language) - Sends a "Say" message with the specified language (all languages: 0)
    { ENV_GLOBAL, "SendUnitYell", &LuaUnit::SendUnitYell },                       // :SendUnitYell(msg, language) - Sends a "Yell" message with the specified language (all languages: 0)
    { ENV_GLOBAL, "CastSpell", &LuaUnit::CastSpell },                             // :CastSpell(target, spellID[, triggered]) - Casts spell on target (player/npc/creature), if triggered is true then instant cast
    { ENV_GLOBAL, "CastSpellAoF", &LuaUnit::CastSpellAoF },                       // :CastSpellAoF(x, y, z, spellID[, triggered]) - Casts the spell on coordinates, if triggered is false has mana cost and cast time
    { ENV_GLOBAL, "PlayDirectSound", &LuaUnit::PlayDirectSound },                 // :PlayDirectSound(soundId, player) - Unit plays soundID to player, or everyone around if no player
    { ENV_GLOBAL, "PlayDistanceSound", &LuaUnit::PlayDistanceSound },             // :PlayDistanceSound(soundId, player) - Unit plays soundID to player, or everyone around if no player. The sound fades the further you are
    // {"Kill", &LuaUnit::Kill},                                    // :Kill(target, durabilityLoss) - Unit kills the target. Durabilityloss is true by default
    { ENV_GLOBAL, "StopSpellCast", &LuaUnit::StopSpellCast },                     // :StopSpellCast(spellId(optional)) - Stops the unit from casting a spell. If a spellId is defined, it will stop that unit from casting that spell
    { ENV_GLOBAL, "InterruptSpell", &LuaUnit::InterruptSpell },                   // :InterruptSpell(spellType, delayed(optional)) - Interrupts the unit's spell by the spellType. If delayed is true it will skip if the spell is delayed.
    { ENV_GLOBAL, "SendChatMessageToPlayer", &LuaUnit::SendChatMessageToPlayer }, // :SendChatMessageToPlayer(type, lang, msg, target) - Unit sends a chat message to the given target player
    { ENV_GLOBAL, "Emote", &LuaUnit::Emote },                                     // :Emote(emote)
    { ENV_GLOBAL, "CountPctFromCurHealth", &LuaUnit::CountPctFromCurHealth },     // :CountPctFromCurHealth(int32 pct)
    { ENV_GLOBAL, "CountPctFromMaxHealth", &LuaUnit::CountPctFromMaxHealth },     // :CountPctFromMaxHealth()
    { ENV_GLOBAL, "Dismount", &LuaUnit::Dismount },                               // :Dismount() - Dismounts the unit.
    { ENV_GLOBAL, "Mount", &LuaUnit::Mount },                                     // :Mount(displayId) - Mounts the unit on the specified displayId.
    // {"RestoreDisplayId", &LuaUnit::RestoreDisplayId},            // :RestoreDisplayId()
    // {"RestoreFaction", &LuaUnit::RestoreFaction},                // :RestoreFaction()
    // {"RemoveBindSightAuras", &LuaUnit::RemoveBindSightAuras},    // :RemoveBindSightAuras()
    // {"RemoveCharmAuras", &LuaUnit::RemoveCharmAuras},            // :RemoveCharmAuras()
    { ENV_GLOBAL, "ClearThreatList", &LuaUnit::ClearThreatList },                 // :ClearThreatList()
    { ENV_GLOBAL, "ClearUnitState", &LuaUnit::ClearUnitState },                   // :ClearUnitState(state)
    { ENV_GLOBAL, "AddUnitState", &LuaUnit::AddUnitState },                       // :AddUnitState(state)
    // {"DisableMelee", &LuaUnit::DisableMelee},                    // :DisableMelee([disable]) - if true, enables
    // {"SummonGuardian", &LuaUnit::SummonGuardian},                // :SummonGuardian(entry, x, y, z, o[, duration]) - summons a guardian to location. Scales with summoner, is friendly to him and guards him.
    { ENV_GLOBAL, "NearTeleport", &LuaUnit::NearTeleport },                       // :NearTeleport(x, y, z, o) - Teleports to give coordinates. Does not leave combat or unsummon pet.
    { ENV_GLOBAL, "MoveIdle", &LuaUnit::MoveIdle },                               // :MoveIdle()
    { ENV_GLOBAL, "MoveRandom", &LuaUnit::MoveRandom },                           // :MoveRandom(radius)
    { ENV_GLOBAL, "MoveHome", &LuaUnit::MoveHome },                               // :MoveHome()
    { ENV_GLOBAL, "MoveFollow", &LuaUnit::MoveFollow },                           // :MoveFollow(target[, dist, angle])
    { ENV_GLOBAL, "MoveChase", &LuaUnit::MoveChase },                             // :MoveChase(target[, dist, angle])
    { ENV_GLOBAL, "MoveConfused", &LuaUnit::MoveConfused },                       // :MoveConfused()
    { ENV_GLOBAL, "MoveFleeing", &LuaUnit::MoveFleeing },                         // :MoveFleeing(enemy[, time])
    { ENV_GLOBAL, "MoveTo", &LuaUnit::MoveTo },                                   // :MoveTo(id, x, y, z[, genPath]) - Moves to point. id is sent to WP reach hook. genPath defaults to true
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "MoveJump", &LuaUnit::MoveJump },                               // :MoveJump(x, y, z, zSpeed, maxHeight, id)
#endif
    { ENV_GLOBAL, "MoveStop", &LuaUnit::MoveStop },                               // :MoveStop()
    { ENV_GLOBAL, "MoveExpire", &LuaUnit::MoveExpire },                           // :MoveExpire([reset])
    { ENV_GLOBAL, "MoveClear", &LuaUnit::MoveClear },                             // :MoveClear([reset])

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Player> PlayerMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetSelection", &LuaPlayer::GetSelection },                                 // :GetSelection()
    { ENV_GLOBAL, "GetGMRank", &LuaPlayer::GetGMRank },                                       // :GetSecurity()
    { ENV_GLOBAL, "GetGuildId", &LuaPlayer::GetGuildId },                                     // :GetGuildId() - nil on no guild
    { ENV_GLOBAL, "GetCoinage", &LuaPlayer::GetCoinage },                                     // :GetCoinage()
    { ENV_GLOBAL, "GetTeam", &LuaPlayer::GetTeam },                                           // :GetTeam() - returns the player's team. 0 for ally, 1 for horde
    { ENV_GLOBAL, "GetItemCount", &LuaPlayer::GetItemCount },                                 // :GetItemCount(item_id[, check_bank])
    { ENV_GLOBAL, "GetGroup", &LuaPlayer::GetGroup },                                         // :GetGroup()
    { ENV_GLOBAL, "GetGuild", &LuaPlayer::GetGuild },                                         // :GetGuild()
    { ENV_GLOBAL, "GetAccountId", &LuaPlayer::GetAccountId },                                 // :GetAccountId()
    { ENV_GLOBAL, "GetAccountName", &LuaPlayer::GetAccountName },                             // :GetAccountName()
#ifndef CATA
#ifndef CLASSIC
    { ENV_GLOBAL, "GetArenaPoints", &LuaPlayer::GetArenaPoints },                             // :GetArenaPoints()
    { ENV_GLOBAL, "GetHonorPoints", &LuaPlayer::GetHonorPoints },                             // :GetHonorPoints()
#endif
#endif
    { ENV_GLOBAL, "GetLifetimeKills", &LuaPlayer::GetLifetimeKills },                         // :GetLifetimeKills() - Returns the player's lifetime (honorable) kills
    { ENV_GLOBAL, "GetPlayerIP", &LuaPlayer::GetPlayerIP },                                   // :GetPlayerIP() - Returns the player's IP Address
    { ENV_GLOBAL, "GetLevelPlayedTime", &LuaPlayer::GetLevelPlayedTime },                     // :GetLevelPlayedTime() - Returns the player's played time at that level
    { ENV_GLOBAL, "GetTotalPlayedTime", &LuaPlayer::GetTotalPlayedTime },                     // :GetTotalPlayedTime() - Returns the total played time of that player
    { ENV_GLOBAL, "GetItemByPos", &LuaPlayer::GetItemByPos },                                 // :GetItemByPos(bag, slot) - Returns item in given slot in a bag (bag: 19-22 slot : 0-35) or inventory (bag: -1 slot : 0-38)
    { ENV_GLOBAL, "GetReputation", &LuaPlayer::GetReputation },                               // :GetReputation(faction) - Gets player's reputation with given faction
    { ENV_GLOBAL, "GetItemByEntry", &LuaPlayer::GetItemByEntry },                             // :GetItemByEntry(entry) - Gets an item if the player has it
    { ENV_GLOBAL, "GetEquippedItemBySlot", &LuaPlayer::GetEquippedItemBySlot },               // :GetEquippedItemBySlot(slotId) - Returns equipped item by slot
    { ENV_GLOBAL, "GetQuestLevel", &LuaPlayer::GetQuestLevel },                               // :GetQuestLevel(quest) - Returns quest's level
    { ENV_GLOBAL, "GetChatTag", &LuaPlayer::GetChatTag },                                     // :GetChatTag() - Returns player chat tag ID
    { ENV_GLOBAL, "GetRestBonus", &LuaPlayer::GetRestBonus },                                 // :GetRestBonus() - Gets player's rest bonus
    { ENV_GLOBAL, "GetRestType", &LuaPlayer::GetRestType },                                   // :GetRestType() - Returns the player's rest type
#ifdef WOTLK
    { ENV_GLOBAL, "GetPhaseMaskForSpawn", &LuaPlayer::GetPhaseMaskForSpawn },                 // :GetPhaseMaskForSpawn() - Gets the real phasemask for spawning things. Used if the player is in GM mode
#endif
    { ENV_GLOBAL, "GetReqKillOrCastCurrentCount", &LuaPlayer::GetReqKillOrCastCurrentCount }, // :GetReqKillOrCastCurrentCount(questId, entry) - Gets the objective (kill or cast) current count done
    { ENV_GLOBAL, "GetQuestStatus", &LuaPlayer::GetQuestStatus },                             // :GetQuestStatus(entry) - Gets the quest's status
    { ENV_GLOBAL, "GetInGameTime", &LuaPlayer::GetInGameTime },                               // :GetInGameTime() - Returns player's ingame time
    { ENV_GLOBAL, "GetComboPoints", &LuaPlayer::GetComboPoints },                             // :GetComboPoints() - Returns player's combo points
    { ENV_GLOBAL, "GetComboTarget", &LuaPlayer::GetComboTarget },                             // :GetComboTarget() - Returns the player's combo target
    { ENV_GLOBAL, "GetGuildName", &LuaPlayer::GetGuildName },                                 // :GetGuildName() - Returns player's guild's name or nil
    { ENV_GLOBAL, "GetFreeTalentPoints", &LuaPlayer::GetFreeTalentPoints },                   // :GetFreeTalentPoints() - Returns the amount of unused talent points
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "GetActiveSpec", &LuaPlayer::GetActiveSpec },                               // :GetActiveSpec() - Returns the active specID
    { ENV_GLOBAL, "GetSpecsCount", &LuaPlayer::GetSpecsCount },                               // :GetSpecsCount() - Returns the player's spec count
#endif
    { ENV_GLOBAL, "GetSpellCooldownDelay", &LuaPlayer::GetSpellCooldownDelay },               // :GetSpellCooldownDelay(spellId) - Returns the spell's cooldown
    { ENV_GLOBAL, "GetGuildRank", &LuaPlayer::GetGuildRank },                                 // :GetGuildRank() - Gets the player's guild rank
    { ENV_GLOBAL, "GetDifficulty", &LuaPlayer::GetDifficulty },                               // :GetDifficulty([isRaid]) - Returns the current difficulty
    { ENV_GLOBAL, "GetHealthBonusFromStamina", &LuaPlayer::GetHealthBonusFromStamina },       // :GetHealthBonusFromStamina() - Returns the HP bonus from stamina
    { ENV_GLOBAL, "GetManaBonusFromIntellect", &LuaPlayer::GetManaBonusFromIntellect },       // :GetManaBonusFromIntellect() - Returns the mana bonus from intellect
    { ENV_GLOBAL, "GetMaxSkillValue", &LuaPlayer::GetMaxSkillValue },                         // :GetMaxSkillValue(skill) - Gets max skill value for the given skill
    { ENV_GLOBAL, "GetPureMaxSkillValue", &LuaPlayer::GetPureMaxSkillValue },                 // :GetPureMaxSkillValue(skill) - Gets max base skill value
    { ENV_GLOBAL, "GetSkillValue", &LuaPlayer::GetSkillValue },                               // :GetSkillValue(skill) - Gets current skill value
    { ENV_GLOBAL, "GetBaseSkillValue", &LuaPlayer::GetBaseSkillValue },                       // :GetBaseSkillValue(skill) - Gets current base skill value (no temp bonus)
    { ENV_GLOBAL, "GetPureSkillValue", &LuaPlayer::GetPureSkillValue },                       // :GetPureSkillValue(skill) - Gets current base skill value (no bonuses)
    { ENV_GLOBAL, "GetSkillPermBonusValue", &LuaPlayer::GetSkillPermBonusValue },             // :GetSkillPermBonusValue(skill) - Returns current permanent bonus
    { ENV_GLOBAL, "GetSkillTempBonusValue", &LuaPlayer::GetSkillTempBonusValue },             // :GetSkillTempBonusValue(skill) - Returns current temp bonus
    { ENV_GLOBAL, "GetReputationRank", &LuaPlayer::GetReputationRank },                       // :GetReputationRank(faction) - Returns the reputation rank with given faction
    { ENV_GLOBAL, "GetSpellCooldowns", &LuaPlayer::GetSpellCooldowns },                       // :GetSpellCooldowns() - Gets a table where spellIDs are the keys and values are cooldowns
    { ENV_GLOBAL, "GetDrunkValue", &LuaPlayer::GetDrunkValue },                               // :GetDrunkValue() - Returns the current drunkness value
    { ENV_GLOBAL, "GetBattlegroundId", &LuaPlayer::GetBattlegroundId },                       // :GetBattlegroundId() - Returns the player's current battleground ID
    { ENV_GLOBAL, "GetBattlegroundTypeId", &LuaPlayer::GetBattlegroundTypeId },               // :GetBattlegroundTypeId() - Returns the player's current battleground type ID
    { ENV_GLOBAL, "GetXPRestBonus", &LuaPlayer::GetXPRestBonus },                             // :GetXPRestBonus(xp) - Returns the rested bonus XP from given XP
    { ENV_GLOBAL, "GetRestTime", &LuaPlayer::GetRestTime },                                   // :GetRestTime() - Returns the timed rested
    { ENV_GLOBAL, "GetGroupInvite", &LuaPlayer::GetGroupInvite },                             // :GetGroupInvite() - Returns the group invited to
    { ENV_GLOBAL, "GetSubGroup", &LuaPlayer::GetSubGroup },                                   // :GetSubGroup() - Gets the player's current subgroup ID
    { ENV_GLOBAL, "GetNextRandomRaidMember", &LuaPlayer::GetNextRandomRaidMember },           // :GetNextRandomRaidMember(radius) - Gets a random raid member in given radius
    { ENV_GLOBAL, "GetOriginalGroup", &LuaPlayer::GetOriginalGroup },                         // :GetOriginalGroup() - Gets the original group object
    { ENV_GLOBAL, "GetOriginalSubGroup", &LuaPlayer::GetOriginalSubGroup },                   // :GetOriginalSubGroup() - Returns the original subgroup ID
#ifndef MANGOS
    { ENV_GLOBAL, "GetChampioningFaction", &LuaPlayer::GetChampioningFaction },               // :GetChampioningFaction() - Returns the player's championing faction
#endif
    { ENV_GLOBAL, "GetLatency", &LuaPlayer::GetLatency },                                     // :GetLatency() - Returns player's latency
    // {"GetRecruiterId", &LuaPlayer::GetRecruiterId},                          // :GetRecruiterId() - Returns player's recruiter's ID
    { ENV_GLOBAL, "GetDbLocaleIndex", &LuaPlayer::GetDbLocaleIndex },                         // :GetDbLocaleIndex() - Returns locale index
    { ENV_GLOBAL, "GetDbcLocale", &LuaPlayer::GetDbcLocale },                                 // :GetDbcLocale() - Returns DBC locale
    { ENV_GLOBAL, "GetCorpse", &LuaPlayer::GetCorpse },                                       // :GetCorpse() - Returns the player's corpse
    { ENV_GLOBAL, "GetGossipTextId", &LuaPlayer::GetGossipTextId },                           // :GetGossipTextId(worldObject) - Returns the WorldObject's gossip textId
    { ENV_GLOBAL, "GetQuestRewardStatus", &LuaPlayer::GetQuestRewardStatus },                 // :GetQuestRewardStatus(questId) - Returns the true/false of the quest reward status
#ifndef CATA
    { ENV_GLOBAL, "GetShieldBlockValue", &LuaPlayer::GetShieldBlockValue },                   // :GetShieldBlockValue() - Returns block value
#endif
#ifdef CLASSIC
    {"GetHonorStoredKills", &LuaPlayer::GetHonorStoredKills},                     // :GetHonorStoredKills(on/off)
    { "GetRankPoints", &LuaPlayer::GetRankPoints },                                 // :GetRankPoints()
    { "GetHonorLastWeekStandingPos", &LuaPlayer::GetHonorLastWeekStandingPos },     // :GetHonorLastWeekStandingPos()
#endif

    // Setters
    { ENV_GLOBAL, "AdvanceSkillsToMax", &LuaPlayer::AdvanceSkillsToMax },     // :AdvanceSkillsToMax() - Advances all currently known skills to the currently known max level
    { ENV_GLOBAL, "AdvanceSkill", &LuaPlayer::AdvanceSkill },                 // :AdvanceSkill(skill_id, step) - Advances skill by ID and the amount(step)
    { ENV_GLOBAL, "AdvanceAllSkills", &LuaPlayer::AdvanceAllSkills },         // :AdvanceAllSkills(value) - Advances all current skills to your input(value)
    { ENV_GLOBAL, "AddLifetimeKills", &LuaPlayer::AddLifetimeKills },         // :AddLifetimeKills(val) - Adds lifetime (honorable) kills to your current lifetime kills
    { ENV_GLOBAL, "SetCoinage", &LuaPlayer::SetCoinage },                     // :SetCoinage(amount) - sets plr's coinage to this
#ifndef CLASSIC
    { ENV_GLOBAL, "SetKnownTitle", &LuaPlayer::SetKnownTitle },               // :SetKnownTitle(id)
    { ENV_GLOBAL, "UnsetKnownTitle", &LuaPlayer::UnsetKnownTitle },           // :UnsetKnownTitle(id)
#endif
    { ENV_GLOBAL, "SetBindPoint", &LuaPlayer::SetBindPoint },                 // :SetBindPoint(x, y, z, map, areaid) - sets home for hearthstone
#ifndef CATA
#ifndef CLASSIC
    { ENV_GLOBAL, "SetArenaPoints", &LuaPlayer::SetArenaPoints },             // :SetArenaPoints(amount)
    { ENV_GLOBAL, "SetHonorPoints", &LuaPlayer::SetHonorPoints },             // :SetHonorPoints(amount)
#endif
#endif
#ifdef CLASSIC
    {"SetHonorStoredKills", &LuaPlayer::SetHonorStoredKills},     // :SetHonorStoredKills(kills, [on/off])
    { "SetRankPoints", &LuaPlayer::SetRankPoints },                 // :SetRankPoints(rankPoints)
    { "SetHonorLastWeekStandingPos", &LuaPlayer::SetHonorLastWeekStandingPos }, // :SetHonorLastWeekStandingPos(standingPos)
#endif
    { ENV_GLOBAL, "SetLifetimeKills", &LuaPlayer::SetLifetimeKills },         // :SetLifetimeKills(val) - Sets the overall lifetime (honorable) kills of the player
    { ENV_GLOBAL, "SetGameMaster", &LuaPlayer::SetGameMaster },               // :SetGameMaster([on]) - Sets GM mode on or off
    { ENV_GLOBAL, "SetGMChat", &LuaPlayer::SetGMChat },                       // :SetGMChat([on]) - Sets GM chat on or off
    { ENV_GLOBAL, "SetTaxiCheat", &LuaPlayer::SetTaxiCheat },                 // :SetTaxiCheat([on]) - Sets taxi cheat on or off
    { ENV_GLOBAL, "SetGMVisible", &LuaPlayer::SetGMVisible },                 // :SetGMVisible([on]) - Sets gm visibility on or off
    { ENV_GLOBAL, "SetPvPDeath", &LuaPlayer::SetPvPDeath },                   // :SetPvPDeath([on]) - Sets PvP death on or off
    { ENV_GLOBAL, "SetAcceptWhispers", &LuaPlayer::SetAcceptWhispers },       // :SetAcceptWhispers([on]) - Sets whisper accepting death on or off
    { ENV_GLOBAL, "SetRestBonus", &LuaPlayer::SetRestBonus },                 // :SetRestBonus(bonusrate) - Sets new restbonus rate
    { ENV_GLOBAL, "SetRestType", &LuaPlayer::SetRestType },                   // :SetRestType() - Sets rest type
    { ENV_GLOBAL, "SetQuestStatus", &LuaPlayer::SetQuestStatus },             // :SetQuestStatus(entry, status) - Sets the quest's status
    { ENV_GLOBAL, "SetReputation", &LuaPlayer::SetReputation },               // :SetReputation(faction, value) - Sets the faction reputation for the player
    { ENV_GLOBAL, "SetFreeTalentPoints", &LuaPlayer::SetFreeTalentPoints },   // :SetFreeTalentPoints(points) - Sets the amount of unused talent points
    { ENV_GLOBAL, "SetGuildRank", &LuaPlayer::SetGuildRank },                 // :SetGuildRank(rank) - Sets player's guild rank
    // {"SetMovement", &LuaPlayer::SetMovement},                // :SetMovement(type) - Sets player's movement type
    { ENV_GLOBAL, "SetSkill", &LuaPlayer::SetSkill },                         // :SetSkill(skill, step, currVal, maxVal) - Sets the skill's boundaries and value
    { ENV_GLOBAL, "SetFactionForRace", &LuaPlayer::SetFactionForRace },       // :SetFactionForRace(race) - Sets the faction by raceID
    { ENV_GLOBAL, "SetDrunkValue", &LuaPlayer::SetDrunkValue },               // :SetDrunkValue(newDrunkValue) - Sets drunkness value
    { ENV_GLOBAL, "SetRestTime", &LuaPlayer::SetRestTime },                   // :SetRestTime(value) - Sets the rested time
    { ENV_GLOBAL, "SetAtLoginFlag", &LuaPlayer::SetAtLoginFlag },             // :SetAtLoginFlag(flag) - Adds an at login flag
    { ENV_GLOBAL, "SetPlayerLock", &LuaPlayer::SetPlayerLock },               // :SetPlayerLock(on/off)
    { ENV_GLOBAL, "SetGender", &LuaPlayer::SetGender },                       // :SetGender(value) - 0 = male 1 = female
    { ENV_GLOBAL, "SetSheath", &LuaPlayer::SetSheath },                       // :SetSheath(SheathState) - Sets player's sheathstate
#ifdef MANGOS
    { ENV_GLOBAL, "SetFFA", &LuaPlayer::SetFFA },                             // :SetFFA([apply]) - Sets the units FFA tag on or off
#endif

    // Boolean
    { ENV_GLOBAL, "IsInGroup", &LuaPlayer::IsInGroup },                                               // :IsInGroup()
    { ENV_GLOBAL, "IsInGuild", &LuaPlayer::IsInGuild },                                               // :IsInGuild()
    { ENV_GLOBAL, "IsGM", &LuaPlayer::IsGM },                                                         // :IsGM()
    { ENV_GLOBAL, "IsAlliance", &LuaPlayer::IsAlliance },                                             // :IsAlliance()
    { ENV_GLOBAL, "IsHorde", &LuaPlayer::IsHorde },                                                   // :IsHorde()
#ifndef CLASSIC
    { ENV_GLOBAL, "HasTitle", &LuaPlayer::HasTitle },                                                 // :HasTitle(id)
#endif
    { ENV_GLOBAL, "HasItem", &LuaPlayer::HasItem },                                                   // :HasItem(itemId[, count, check_bank]) - Returns true if the player has the item(itemId) and specified count, else it will return false
    { ENV_GLOBAL, "Teleport", &LuaPlayer::Teleport },                                                 // :Teleport(Map, X, Y, Z, O) - Teleports player to specified co - ordinates. Returns true if success and false if not
    { ENV_GLOBAL, "AddItem", &LuaPlayer::AddItem },                                                   // :AddItem(id, amount) - Adds amount of item to player. Returns true if success and false if not
#ifndef CLASSIC
    { ENV_GLOBAL, "IsInArenaTeam", &LuaPlayer::IsInArenaTeam },                                       // :IsInArenaTeam(type) - type : 0 = 2v2, 1 = 3v3, 2 = 5v5
#endif
    { ENV_GLOBAL, "CanEquipItem", &LuaPlayer::CanEquipItem },                                         // :CanEquipItem(entry/item, slot) - Returns true if the player can equip given item/item entry
    { ENV_GLOBAL, "IsFalling", &LuaPlayer::IsFalling },                                               // :IsFalling() - Returns true if the unit is falling
    { ENV_GLOBAL, "ToggleAFK", &LuaPlayer::ToggleAFK },                                               // :ToggleAFK() - Toggles AFK state for player
    { ENV_GLOBAL, "ToggleDND", &LuaPlayer::ToggleDND },                                               // :ToggleDND() - Toggles DND state for player
    { ENV_GLOBAL, "IsAFK", &LuaPlayer::IsAFK },                                                       // :IsAFK() - Returns true if the player is afk
    { ENV_GLOBAL, "IsDND", &LuaPlayer::IsDND },                                                       // :IsDND() - Returns true if the player is in dnd mode
    { ENV_GLOBAL, "IsAcceptingWhispers", &LuaPlayer::IsAcceptingWhispers },                           // :IsAcceptWhispers() - Returns true if the player accepts whispers
    { ENV_GLOBAL, "IsGMChat", &LuaPlayer::IsGMChat },                                                 // :IsGMChat() - Returns true if the player has GM chat on
    { ENV_GLOBAL, "IsTaxiCheater", &LuaPlayer::IsTaxiCheater },                                       // :IsTaxiCheater() - Returns true if the player has taxi cheat on
    { ENV_GLOBAL, "IsGMVisible", &LuaPlayer::IsGMVisible },                                           // :IsGMVisible() - Returns true if the player is GM visible
    { ENV_GLOBAL, "HasQuest", &LuaPlayer::HasQuest },                                                 // :HasQuest(entry) - Returns true if player has the quest
    { ENV_GLOBAL, "InBattlegroundQueue", &LuaPlayer::InBattlegroundQueue },                           // :InBattlegroundQueue() - Returns true if the player is in a battleground queue
    // {"IsImmuneToEnvironmentalDamage", &LuaPlayer::IsImmuneToEnvironmentalDamage},    // :IsImmuneToEnvironmentalDamage() - Returns true if the player is immune to enviromental damage
    { ENV_GLOBAL, "CanSpeak", &LuaPlayer::CanSpeak },                                                 // :CanSpeak() - Returns true if the player can speak
    { ENV_GLOBAL, "HasAtLoginFlag", &LuaPlayer::HasAtLoginFlag },                                     // :HasAtLoginFlag(flag) - returns true if the player has the login flag
    // {"InRandomLfgDungeon", &LuaPlayer::InRandomLfgDungeon},                          // :InRandomLfgDungeon() - Returns true if the player is in a random LFG dungeon
    // {"HasPendingBind", &LuaPlayer::HasPendingBind},                                  // :HasPendingBind() - Returns true if the player has a pending instance bind
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "HasAchieved", &LuaPlayer::HasAchieved },                                           // :HasAchieved(achievementID) - Returns true if the player has achieved the achievement
#endif
    { ENV_GLOBAL, "CanUninviteFromGroup", &LuaPlayer::CanUninviteFromGroup },                         // :CanUninviteFromGroup() - Returns true if the player can uninvite from group
    { ENV_GLOBAL, "IsRested", &LuaPlayer::IsRested },                                                 // :IsRested() - Returns true if the player is rested
    // {"CanFlyInZone", &LuaPlayer::CanFlyInZone},                                      // :CanFlyInZone(mapid, zone) - Returns true if the player can fly in the area
    // {"IsNeverVisible", &LuaPlayer::IsNeverVisible},                                  // :IsNeverVisible() - Returns true if the player is never visible
    { ENV_GLOBAL, "IsVisibleForPlayer", &LuaPlayer::IsVisibleForPlayer },                             // :IsVisibleForPlayer(player) - Returns true if the player is visible for the target player
    // {"IsUsingLfg", &LuaPlayer::IsUsingLfg},                                          // :IsUsingLfg() - Returns true if the player is using LFG
    { ENV_GLOBAL, "HasQuestForItem", &LuaPlayer::HasQuestForItem },                                   // :HasQuestForItem(entry) - Returns true if the player has the quest for the item
    { ENV_GLOBAL, "HasQuestForGO", &LuaPlayer::HasQuestForGO },                                       // :HasQuestForGO(entry) - Returns true if the player has the quest for the gameobject
    { ENV_GLOBAL, "CanShareQuest", &LuaPlayer::CanShareQuest },                                       // :CanShareQuest(entry) - Returns true if the quest entry is shareable by the player
    // {"HasReceivedQuestReward", &LuaPlayer::HasReceivedQuestReward},                  // :HasReceivedQuestReward(entry) - Returns true if the player has recieved the quest's reward
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "HasTalent", &LuaPlayer::HasTalent },                                               // :HasTalent(talentId, spec) - Returns true if the player has the talent in given spec
#endif
    { ENV_GLOBAL, "IsInSameGroupWith", &LuaPlayer::IsInSameGroupWith },                               // :IsInSameGroupWith(player) - Returns true if the players are in the same group
    { ENV_GLOBAL, "IsInSameRaidWith", &LuaPlayer::IsInSameRaidWith },                                 // :IsInSameRaidWith(player) - Returns true if the players are in the same raid
    { ENV_GLOBAL, "IsGroupVisibleFor", &LuaPlayer::IsGroupVisibleFor },                               // :IsGroupVisibleFor(player) - Player is group visible for the target
    { ENV_GLOBAL, "HasSkill", &LuaPlayer::HasSkill },                                                 // :HasSkill(skill) - Returns true if the player has the skill
    { ENV_GLOBAL, "IsHonorOrXPTarget", &LuaPlayer::IsHonorOrXPTarget },                               // :IsHonorOrXPTarget(victim) - Returns true if the victim gives honor or XP
    { ENV_GLOBAL, "CanParry", &LuaPlayer::CanParry },                                                 // :CanParry() - Returns true if the player can parry
    { ENV_GLOBAL, "CanBlock", &LuaPlayer::CanBlock },                                                 // :CanBlock() - Returns true if the player can block
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "CanTitanGrip", &LuaPlayer::CanTitanGrip },                                         // :CanTitanGrip() - Returns true if the player has titan grip
#endif
    { ENV_GLOBAL, "InBattleground", &LuaPlayer::InBattleground },                                     // :InBattleground() - Returns true if the player is in a battleground
#ifndef CLASSIC
    { ENV_GLOBAL, "InArena", &LuaPlayer::InArena },                                                   // :InArena() - Returns true if the player is in an arena
#endif
    // {"IsOutdoorPvPActive", &LuaPlayer::IsOutdoorPvPActive},                          // :IsOutdoorPvPActive() - Returns true if the player is outdoor pvp active
    // {"IsARecruiter", &LuaPlayer::IsARecruiter},                                      // :IsARecruiter() - Returns true if the player is a recruiter
    { ENV_GLOBAL, "CanUseItem", &LuaPlayer::CanUseItem },                                             // :CanUseItem(item/entry) - Returns true if the player can use the item or item entry
    { ENV_GLOBAL, "HasSpell", &LuaPlayer::HasSpell },                                                 // :HasSpell(id)
    { ENV_GLOBAL, "HasSpellCooldown", &LuaPlayer::HasSpellCooldown },                                 // :HasSpellCooldown(spellId) - Returns true if the spell is on cooldown
    { ENV_GLOBAL, "IsInWater", &LuaPlayer::IsInWater },                                               // :IsInWater() - Returns true if the player is in water
#ifndef CLASSIC
    { ENV_GLOBAL, "CanFly", &LuaPlayer::CanFly },                                                     // :CanFly() - Returns true if the player can fly
#endif
    { ENV_GLOBAL, "IsMoving", &LuaPlayer::IsMoving },                                                 // :IsMoving()
#ifndef CLASSIC
    { ENV_GLOBAL, "IsFlying", &LuaPlayer::IsFlying },                                                 // :IsFlying()
#endif

    // Gossip
    { ENV_GLOBAL, "GossipMenuAddItem", &LuaPlayer::GossipMenuAddItem },                               // :GossipMenuAddItem(icon, msg, sender, intid[, code, popup, money])
    { ENV_GLOBAL, "GossipSendMenu", &LuaPlayer::GossipSendMenu },                                     // :GossipSendMenu(npc_text, unit[, menu_id]) - If unit is a player, you need to use a menu_id. menu_id is used to hook the gossip select function to the menu
    { ENV_GLOBAL, "GossipComplete", &LuaPlayer::GossipComplete },                                     // :GossipComplete()
    { ENV_GLOBAL, "GossipClearMenu", &LuaPlayer::GossipClearMenu },                                   // :GossipClearMenu() - Clears the gossip menu of options. Pretty much only useful with player gossip. Need to use before creating a new menu for the player

    // Other
    { ENV_GLOBAL, "SendClearCooldowns", &LuaPlayer::SendClearCooldowns },                            // :SendClearCooldowns(spellId, (unit)target) - Clears the cooldown of the target with a specified spellId
    { ENV_GLOBAL, "SendBroadcastMessage", &LuaPlayer::SendBroadcastMessage },                         // :SendBroadcastMessage(message)
    { ENV_GLOBAL, "SendAreaTriggerMessage", &LuaPlayer::SendAreaTriggerMessage },                     // :SendAreaTriggerMessage(message) - Sends a yellow message in the middle of your screen
    { ENV_GLOBAL, "SendNotification", &LuaPlayer::SendNotification },                                 // :SendNotification(message) - Sends a red message in the middle of your screen
    { ENV_GLOBAL, "SendPacket", &LuaPlayer::SendPacket },                                             // :SendPacket(packet, selfOnly) - Sends a packet to player or everyone around also if selfOnly is false
    { ENV_GLOBAL, "SendAddonMessage", &LuaPlayer::SendAddonMessage },                                 // :SendAddonMessage(prefix, message, channel, receiver) - Sends an addon message to the player. 
    { ENV_GLOBAL, "SendVendorWindow", &LuaPlayer::SendVendorWindow },                                 // :SendVendorWindow(unit) - Sends the unit's vendor window to the player
    { ENV_GLOBAL, "ModifyMoney", &LuaPlayer::ModifyMoney },                                           // :ModifyMoney(amount[, sendError]) - Modifies (does not set) money (copper count) of the player. Amount can be negative to remove copper
    { ENV_GLOBAL, "LearnSpell", &LuaPlayer::LearnSpell },                                             // :LearnSpell(id) - learns the given spell
    { ENV_GLOBAL, "RemoveItem", &LuaPlayer::RemoveItem },                                             // :RemoveItem(item/entry, amount) - Removes amount of item from player
    { ENV_GLOBAL, "RemoveLifetimeKills", &LuaPlayer::RemoveLifetimeKills },                           // :RemoveLifetimeKills(val) - Removes a specified amount(val) of the player's lifetime (honorable) kills
    { ENV_GLOBAL, "ResurrectPlayer", &LuaPlayer::ResurrectPlayer },                                   // :ResurrectPlayer([percent[, sickness(bool)]]) - Resurrects the player at percentage, player gets resurrection sickness if sickness set to true
    { ENV_GLOBAL, "PlaySoundToPlayer", &LuaPlayer::PlaySoundToPlayer },                               // :PlaySoundToPlayer(soundId) - Plays the specified sound to the player
    { ENV_GLOBAL, "EquipItem", &LuaPlayer::EquipItem },                                               // :EquipItem(entry/item, slot) - Equips given item or item entry for player to given slot. Returns the equipped item or nil
    { ENV_GLOBAL, "ResetSpellCooldown", &LuaPlayer::ResetSpellCooldown },                             // :ResetSpellCooldown(spellId, update(bool~optional)) - Resets cooldown of the specified spellId. If update is true, it will send WorldPacket SMSG_CLEAR_COOLDOWN to the player, else it will just clear the spellId from m_spellCooldowns. This is true by default
    { ENV_GLOBAL, "ResetTypeCooldowns", &LuaPlayer::ResetTypeCooldowns },                             // :ResetTypeCooldowns(category, update(bool~optional)) - Resets all cooldowns for the spell category(type). If update is true, it will send WorldPacket SMSG_CLEAR_COOLDOWN to the player, else it will just clear the spellId from m_spellCooldowns. This is true by default
    { ENV_GLOBAL, "ResetAllCooldowns", &LuaPlayer::ResetAllCooldowns },                               // :ResetAllCooldowns() - Resets all spell cooldowns
    { ENV_GLOBAL, "GiveLevel", &LuaPlayer::GiveLevel },                                               // :GiveLevel(level) - Gives levels to the player
    { ENV_GLOBAL, "GiveXP", &LuaPlayer::GiveXP },                                                     // :GiveXP(xp[, victim, pureXP, triggerHook]) - Gives XP to the player. If pure is false, bonuses are count in. If triggerHook is false, GiveXp hook is not triggered.
    // {"RemovePet", &LuaPlayer::RemovePet},                                            // :RemovePet([mode, returnreagent]) - Removes the player's pet. Mode determines if the pet is saved and how
    // {"SummonPet", &LuaPlayer::SummonPet},                                            // :SummonPet(entry, x, y, z, o, petType, despwtime) - Summons a pet for the player
    { ENV_GLOBAL, "Say", &LuaPlayer::Say },                                                           // :Say(text, lang) - The player says the text
    { ENV_GLOBAL, "Yell", &LuaPlayer::Yell },                                                         // :Yell(text, lang) - The player yells the text
    { ENV_GLOBAL, "TextEmote", &LuaPlayer::TextEmote },                                               // :TextEmote(text) - The player does a textemote with the text
    { ENV_GLOBAL, "Whisper", &LuaPlayer::Whisper },                                                   // :Whisper(text, lang, receiverGuid) - The player whispers the text to the guid
    { ENV_GLOBAL, "CompleteQuest", &LuaPlayer::CompleteQuest },                                       // :CompleteQuest(entry) - Completes a quest by entry
    { ENV_GLOBAL, "IncompleteQuest", &LuaPlayer::IncompleteQuest },                                   // :IncompleteQuest(entry) - Uncompletes the quest by entry for the player
    { ENV_GLOBAL, "FailQuest", &LuaPlayer::FailQuest },                                               // :FailQuest(entry) - Player fails the quest entry
    // {"RemoveActiveQuest", &LuaPlayer::RemoveActiveQuest},                            // :RemoveActiveQuest(entry) - Removes an active quest
    // {"RemoveRewardedQuest", &LuaPlayer::RemoveRewardedQuest},                        // :RemoveRewardedQuest(entry) - Removes a rewarded quest
    { ENV_GLOBAL, "AreaExploredOrEventHappens", &LuaPlayer::AreaExploredOrEventHappens },             // :AreaExploredOrEventHappens(questId) - Satisfies an area or event requrement for the questId
    { ENV_GLOBAL, "GroupEventHappens", &LuaPlayer::GroupEventHappens },                               // :GroupEventHappens(questId, worldObject) - Satisfies a group event for the questId with the world object
    { ENV_GLOBAL, "KilledMonsterCredit", &LuaPlayer::KilledMonsterCredit },                           // :KilledMonsterCredit(entry) - Satisfies a monsterkill for the player
    // {"KilledPlayerCredit", &LuaPlayer::KilledPlayerCredit},                          // :KilledPlayerCredit() - Satisfies a player kill for the player
    // {"KillGOCredit", &LuaPlayer::KillGOCredit},                                      // :KillGOCredit(GOEntry[, GUID]) - Credits the player for destroying a GO, guid is optional
    { ENV_GLOBAL, "TalkedToCreature", &LuaPlayer::TalkedToCreature },                                 // :TalkedToCreature(npcEntry, creature) - Satisfies creature talk objective for the player
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "ResetPetTalents", &LuaPlayer::ResetPetTalents },                                   // :ResetPetTalents() - Resets player's pet's talents
#endif
    { ENV_GLOBAL, "AddComboPoints", &LuaPlayer::AddComboPoints },                                     // :AddComboPoints(target, count[, spell]) - Adds combo points to the target for the player
    // {"GainSpellComboPoints", &LuaPlayer::GainSpellComboPoints},                      // :GainSpellComboPoints(amount) - Player gains spell combo points
    { ENV_GLOBAL, "ClearComboPoints", &LuaPlayer::ClearComboPoints },                                 // :ClearComboPoints() - Clears player's combo points
    { ENV_GLOBAL, "RemoveSpell", &LuaPlayer::RemoveSpell },                                           // :RemoveSpell(entry[, disabled, learn_low_rank]) - Removes (unlearn) the given spell
    { ENV_GLOBAL, "ResetTalents", &LuaPlayer::ResetTalents },                                         // :ResetTalents([no_cost]) - Resets player's talents
    { ENV_GLOBAL, "ResetTalentsCost", &LuaPlayer::ResetTalentsCost },                                 // :ResetTalentsCost() - Returns the reset talents cost
    // {"AddTalent", &LuaPlayer::AddTalent},                                            // :AddTalent(spellid, spec, learning) - Adds a talent spell for the player to given spec
    { ENV_GLOBAL, "RemoveFromGroup", &LuaPlayer::RemoveFromGroup },                                   // :RemoveFromGroup() - Removes the player from his group
    { ENV_GLOBAL, "KillPlayer", &LuaPlayer::KillPlayer },                                             // :KillPlayer() - Kills the player
    { ENV_GLOBAL, "DurabilityLossAll", &LuaPlayer::DurabilityLossAll },                               // :DurabilityLossAll(percent[, inventory]) - The player's items lose durability. Inventory true by default
    { ENV_GLOBAL, "DurabilityLoss", &LuaPlayer::DurabilityLoss },                                     // :DurabilityLoss(item, percent) - The given item loses durability
    { ENV_GLOBAL, "DurabilityPointsLoss", &LuaPlayer::DurabilityPointsLoss },                         // :DurabilityPointsLoss(item, points) - The given item loses durability
    { ENV_GLOBAL, "DurabilityPointsLossAll", &LuaPlayer::DurabilityPointsLossAll },                   // :DurabilityPointsLossAll(points, inventory) - Player's items lose durability
    { ENV_GLOBAL, "DurabilityPointLossForEquipSlot", &LuaPlayer::DurabilityPointLossForEquipSlot },   // :DurabilityPointLossForEquipSlot(slot) - Causes durability loss for the item in the given slot
    { ENV_GLOBAL, "DurabilityRepairAll", &LuaPlayer::DurabilityRepairAll },                           // :DurabilityRepairAll([has_cost, discount, guildBank]) - Repairs all durability
    { ENV_GLOBAL, "DurabilityRepair", &LuaPlayer::DurabilityRepair },                                 // :DurabilityRepair(position[, has_cost, discount, guildBank]) - Repairs item durability of item in given position
#ifndef CATA
#ifndef CLASSIC
    { ENV_GLOBAL, "ModifyHonorPoints", &LuaPlayer::ModifyHonorPoints },                               // :ModifyHonorPoints(amount) - Modifies the player's honor points
    { ENV_GLOBAL, "ModifyArenaPoints", &LuaPlayer::ModifyArenaPoints },                               // :ModifyArenaPoints(amount) - Modifies the player's arena points
#endif
#endif
    { ENV_GLOBAL, "LeaveBattleground", &LuaPlayer::LeaveBattleground },                               // :LeaveBattleground([teleToEntryPoint]) - The player leaves the battleground
    // {"BindToInstance", &LuaPlayer::BindToInstance},                                  // :BindToInstance() - Binds the player to the current instance
    { ENV_GLOBAL, "UnbindInstance", &LuaPlayer::UnbindInstance },                                     // :UnbindInstance(map, difficulty) - Unbinds the player from an instance
    { ENV_GLOBAL, "RemoveFromBattlegroundRaid", &LuaPlayer::RemoveFromBattlegroundRaid },             // :RemoveFromBattlegroundRaid() - Removes the player from a battleground or battlefield raid
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "ResetAchievements", &LuaPlayer::ResetAchievements },                               // :ResetAchievements() - Resets playeräs achievements
#endif
    { ENV_GLOBAL, "KickPlayer", &LuaPlayer::KickPlayer },                                             // :KickPlayer() - Kicks player from server
    { ENV_GLOBAL, "LogoutPlayer", &LuaPlayer::LogoutPlayer },                                         // :LogoutPlayer([save]) - Logs the player out and saves if true
    { ENV_GLOBAL, "SendTrainerList", &LuaPlayer::SendTrainerList },                                   // :SendTrainerList(WorldObject) - Sends trainer list from object to player
    { ENV_GLOBAL, "SendListInventory", &LuaPlayer::SendListInventory },                               // :SendListInventory(WorldObject) - Sends vendor list from object to player
    { ENV_GLOBAL, "SendShowBank", &LuaPlayer::SendShowBank },                                         // :SendShowBank(WorldObject) - Sends bank window from object to player
    { ENV_GLOBAL, "SendTabardVendorActivate", &LuaPlayer::SendTabardVendorActivate },                 // :SendTabardVendorActivate(WorldObject) - Sends tabard vendor window from object to player
    { ENV_GLOBAL, "SendSpiritResurrect", &LuaPlayer::SendSpiritResurrect },                           // :SendSpiritResurrect() - Sends resurrect window to player
    { ENV_GLOBAL, "SendTaxiMenu", &LuaPlayer::SendTaxiMenu },                                         // :SendTaxiMenu(creature) - Sends flight window to player from creature
    { ENV_GLOBAL, "RewardQuest", &LuaPlayer::RewardQuest },                                           // :RewardQuest(entry) - Gives quest rewards for the player
    { ENV_GLOBAL, "SendAuctionMenu", &LuaPlayer::SendAuctionMenu },                                   // :SendAuctionMenu(unit) - Sends auction window to player. Auction house is sent by object.
    { ENV_GLOBAL, "SendShowMailBox", &LuaPlayer::SendShowMailBox },                                   // :SendShowMailBox([guid]) - Sends mail window to player. Sent by guid if provided (valid mailbox required for < wotlk)
    { ENV_GLOBAL, "StartTaxi", &LuaPlayer::StartTaxi },                                               // :StartTaxi(pathId) - player starts the given flight path
    { ENV_GLOBAL, "GossipSendPOI", &LuaPlayer::GossipSendPOI },                                       // :GossipSendPOI(X, Y, Icon, Flags, Data, Name) - Sends a point of interest to the player
    { ENV_GLOBAL, "GossipAddQuests", &LuaPlayer::GossipAddQuests },                                   // :GossipAddQuests(unit) - Adds unit's quests to player's gossip menu
    { ENV_GLOBAL, "SendQuestTemplate", &LuaPlayer::SendQuestTemplate },                               // :SendQuestTemplate(questId, activeAccept) -- Sends quest template to player
    { ENV_GLOBAL, "SpawnBones", &LuaPlayer::SpawnBones },                                             // :SpawnBones() - Removes the player's corpse and spawns bones
    { ENV_GLOBAL, "RemovedInsignia", &LuaPlayer::RemovedInsignia },                                   // :RemovedInsignia(looter) - Looter removes the player's corpse, looting the player and replacing with lootable bones
    { ENV_GLOBAL, "SendGuildInvite", &LuaPlayer::SendGuildInvite },                                   // :SendGuildInvite(player) - Sends a guild invite to the specific player
    { ENV_GLOBAL, "CreateCorpse", &LuaPlayer::CreateCorpse },                                         // :CreateCorpse() - Creates the player's corpse
    { ENV_GLOBAL, "Mute", &LuaPlayer::Mute },                                                         // :Mute(time[, reason]) - Mutes the player for given time in seconds.
    { ENV_GLOBAL, "SummonPlayer", &LuaPlayer::SummonPlayer },                                         // :SummonPlayer(player, map, x, y, z, zoneId[, delay]) - Sends a popup to the player asking if he wants to be summoned if yes, teleported to coords. ZoneID defines the location name shown in the popup Delay is the time until the popup closes automatically.
    { ENV_GLOBAL, "SaveToDB", &LuaPlayer::SaveToDB },                                                 // :SaveToDB() - Saves to database
#ifdef CLASSIC
    {"UpdateHonor", &LuaPlayer::UpdateHonor},                                             // :UpdateHonor() - Updates Player Honor
    { "ResetHonor", &LuaPlayer::ResetHonor },                                               // :ResetHonor() - Resets Player Honor
    { "ClearHonorInfo", &LuaPlayer::ClearHonorInfo },                                       // :ClearHonorInfo() - Clear Player Honor Info
#endif

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Creature> CreatureMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetAITarget", &LuaCreature::GetAITarget },                                     // :GetAITarget(type[, playeronly, position, distance, aura]) - Get an unit in threat list
    { ENV_GLOBAL, "GetAITargets", &LuaCreature::GetAITargets },                                   // :GetAITargets() - Get units in threat list
    { ENV_GLOBAL, "GetAITargetsCount", &LuaCreature::GetAITargetsCount },                         // :GetAITargetsCount() - Get threat list size
    { ENV_GLOBAL, "GetHomePosition", &LuaCreature::GetHomePosition },                             // :GetHomePosition() - Returns x,y,z,o of spawn position
    { ENV_GLOBAL, "GetCorpseDelay", &LuaCreature::GetCorpseDelay },                               // :GetCorpseDelay() - Returns corpse delay
    { ENV_GLOBAL, "GetCreatureSpellCooldownDelay", &LuaCreature::GetCreatureSpellCooldownDelay }, // :GetCreatureSpellCooldownDelay(spellId) - Returns spell cooldown delay
    { ENV_GLOBAL, "GetScriptId", &LuaCreature::GetScriptId },                                     // :GetScriptId() - Returns creature's script ID
    { ENV_GLOBAL, "GetAIName", &LuaCreature::GetAIName },                                         // :GetAIName() - Returns creature's AI name
    { ENV_GLOBAL, "GetScriptName", &LuaCreature::GetScriptName },                                 // :GetScriptName() - Returns creature's script name
    { ENV_GLOBAL, "GetReactState", &LuaCreature::GetReactState },                                 // :GetReactState() - Returns creature's react state
    { ENV_GLOBAL, "GetAttackDistance", &LuaCreature::GetAttackDistance },                         // :GetAttackDistance(unit) - Returns attack distance to unit
    { ENV_GLOBAL, "GetAggroRange", &LuaCreature::GetAggroRange },                                 // :GetAggroRange(unit) - Returns aggro distance to unit
    { ENV_GLOBAL, "GetDefaultMovementType", &LuaCreature::GetDefaultMovementType },               // :GetDefaultMovementType() - Returns default movement type
    { ENV_GLOBAL, "GetRespawnDelay", &LuaCreature::GetRespawnDelay },                             // :GetRespawnDelay() - Returns respawn delay
    { ENV_GLOBAL, "GetRespawnRadius", &LuaCreature::GetRespawnRadius },                           // :GetRespawnRadius() - Returns respawn radius
    // {"GetWaypointPath", &LuaCreature::GetWaypointPath},                          // :GetWaypointPath() - Returns waypoint path ID
    // {"GetCurrentWaypointId", &LuaCreature::GetCurrentWaypointId},                // :GetCurrentWaypointId() - Returns waypoint ID
    // {"GetLootMode", &LuaCreature::GetLootMode},                                  // :GetLootMode() - Returns loot mode
    { ENV_GLOBAL, "GetLootRecipient", &LuaCreature::GetLootRecipient },                           // :GetLootRecipient() - Returns loot receiver
    { ENV_GLOBAL, "GetLootRecipientGroup", &LuaCreature::GetLootRecipientGroup },                 // :GetLootRecipientGroup() - Returns loot receiver group
    { ENV_GLOBAL, "GetNPCFlags", &LuaCreature::GetNPCFlags },                                     // :GetNPCFlags() - Returns NPC flags
#ifndef CATA
    { ENV_GLOBAL, "GetShieldBlockValue", &LuaCreature::GetShieldBlockValue },                     // :GetShieldBlockValue() - Returns block value
#endif

    // Setters
    { ENV_GLOBAL, "SetHover", &LuaCreature::SetHover },                                   // :SetHover([enable]) - Sets hover on or off
    // {"SetDisableGravity", &LuaCreature::SetDisableGravity},              // :SetDisableGravity([disable, packetOnly]) - Disables or enables gravity
    { ENV_GLOBAL, "SetReactState", &LuaCreature::SetReactState },                         // :SetReactState(state) - Sets react state
    { ENV_GLOBAL, "SetNoCallAssistance", &LuaCreature::SetNoCallAssistance },             // :SetNoCallAssistance([noCall]) - Sets call assistance to false or true
    { ENV_GLOBAL, "SetNoSearchAssistance", &LuaCreature::SetNoSearchAssistance },         // :SetNoSearchAssistance([noSearch]) - Sets assistance searhing to false or true
    { ENV_GLOBAL, "SetDefaultMovementType", &LuaCreature::SetDefaultMovementType },       // :SetDefaultMovementType(type) - Sets default movement type
    { ENV_GLOBAL, "SetRespawnDelay", &LuaCreature::SetRespawnDelay },                     // :SetRespawnDelay(delay) - Sets the respawn delay
    { ENV_GLOBAL, "SetRespawnRadius", &LuaCreature::SetRespawnRadius },                   // :SetRespawnRadius(dist) - Sets the respawn radius
    { ENV_GLOBAL, "SetInCombatWithZone", &LuaCreature::SetInCombatWithZone },             // :SetInCombatWithZone() - Sets the creature in combat with everyone in zone
    { ENV_GLOBAL, "SetDisableReputationGain", &LuaCreature::SetDisableReputationGain },   // :SetDisableReputationGain([disable]) - Disables or enables reputation gain from creature
    // {"SetLootMode", &LuaCreature::SetLootMode},                          // :SetLootMode(lootMode) - Sets the lootmode
    { ENV_GLOBAL, "SetNPCFlags", &LuaCreature::SetNPCFlags },                             // :SetNPCFlags(flags) - Sets NPC flags
    { ENV_GLOBAL, "SetDeathState", &LuaCreature::SetDeathState },                         // :SetDeathState(value) - 0 = alive 1 = just died 2 = corpse 3 = dead
    { ENV_GLOBAL, "SetWalk", &LuaCreature::SetWalk },                                     // :SetWalk([enable]) - If false, creature runs, otherwise walks

    // Booleans
    { ENV_GLOBAL, "IsWorldBoss", &LuaCreature::IsWorldBoss },                                                 // :IsWorldBoss() - Returns true if the creature is a WorldBoss, false if not
    { ENV_GLOBAL, "IsRacialLeader", &LuaCreature::IsRacialLeader },                                           // :IsRacialLeader() - Returns true if the creature is a racial leader, false if not
    { ENV_GLOBAL, "IsCivilian", &LuaCreature::IsCivilian },                                                   // :IsCivilian() - Returns true if the creature is a civilian, false if not
    // {"IsTrigger", &LuaCreature::IsTrigger},                                                  // :IsTrigger() - Returns true if the creature is a trigger, false if not
    { ENV_GLOBAL, "IsGuard", &LuaCreature::IsGuard },                                                         // :IsGuard() - Returns true if the creature is a guard, false if not
    { ENV_GLOBAL, "IsElite", &LuaCreature::IsElite },                                                         // :IsElite() - Returns true if the creature is an elite, false if not
    { ENV_GLOBAL, "IsInEvadeMode", &LuaCreature::IsInEvadeMode },                                             // :IsInEvadeMode() - Returns true if the creature is in evade mode, false if not
    { ENV_GLOBAL, "HasCategoryCooldown", &LuaCreature::HasCategoryCooldown },                                 // :HasCategoryCooldown(spellId) - Returns true if the creature has a cooldown for the spell's category
    { ENV_GLOBAL, "CanWalk", &LuaCreature::CanWalk },                                                         // :CanWalk() - Returns true if the creature can walk
    { ENV_GLOBAL, "CanSwim", &LuaCreature::CanSwim },                                                         // :CanSwim() - Returns true if the creature can swim
    { ENV_GLOBAL, "HasReactState", &LuaCreature::HasReactState },                                             // :HasReactState(state) - Returns true if the creature has react state
    // {"CanStartAttack", &LuaCreature::CanStartAttack},                                        // :CanStartAttack(unit[, force]) - Returns true if the creature can attack the unit
    { ENV_GLOBAL, "HasSearchedAssistance", &LuaCreature::HasSearchedAssistance },                             // :HasSearchedAssistance() - Returns true if the creature has searched assistance
    { ENV_GLOBAL, "CanAssistTo", &LuaCreature::CanAssistTo },                                                 // :CanAssistTo(unit, enemy[, checkfaction]) - Returns true if the creature can assist unit with enemy
    { ENV_GLOBAL, "IsTargetAcceptable", &LuaCreature::IsTargetAcceptable },                                   // :IsTargetAcceptable(unit) - Returns true if the creature can target unit
    { ENV_GLOBAL, "HasInvolvedQuest", &LuaCreature::HasInvolvedQuest },                                       // :HasInvolvedQuest(questId) - Returns true if the creature can finish the quest for players
    { ENV_GLOBAL, "IsRegeneratingHealth", &LuaCreature::IsRegeneratingHealth },                               // :IsRegeneratingHealth() - Returns true if the creature is regenerating health
    { ENV_GLOBAL, "IsReputationGainDisabled", &LuaCreature::IsReputationGainDisabled },                       // :IsReputationGainDisabled() - Returns true if the creature has reputation gain disabled
    // {"IsDamageEnoughForLootingAndReward", &LuaCreature::IsDamageEnoughForLootingAndReward},  // :IsDamageEnoughForLootingAndReward()
    // {"HasLootMode", &LuaCreature::HasLootMode},
    { ENV_GLOBAL, "HasSpell", &LuaCreature::HasSpell },                                                       // :HasSpell(id)
    { ENV_GLOBAL, "HasQuest", &LuaCreature::HasQuest },                                                       // :HasQuest(id)
    { ENV_GLOBAL, "HasSpellCooldown", &LuaCreature::HasSpellCooldown },                                       // :HasSpellCooldown(spellId) - Returns true if the spell is on cooldown
    { ENV_GLOBAL, "CanFly", &LuaCreature::CanFly },                                                           // :CanFly() - Returns true if the creature can fly

    // Other
    // {"Despawn", &LuaCreature::Despawn},                          // :Despawn([despawnDelay]) - Creature despawns after given time
    { ENV_GLOBAL, "FleeToGetAssistance", &LuaCreature::FleeToGetAssistance },     // :FleeToGetAssistance() - Creature flees for assistance
    { ENV_GLOBAL, "CallForHelp", &LuaCreature::CallForHelp },                     // :CallForHelp(radius) - Creature calls for help from units in radius
    { ENV_GLOBAL, "CallAssistance", &LuaCreature::CallAssistance },               // :CallAssistance() - Creature calls for assistance
    { ENV_GLOBAL, "RemoveCorpse", &LuaCreature::RemoveCorpse },                   // :RemoveCorpse([setSpawnTime]) - Removes corpse
    { ENV_GLOBAL, "DespawnOrUnsummon", &LuaCreature::DespawnOrUnsummon },         // :DespawnOrUnsummon([Delay]) - Despawns the creature after delay if given
    { ENV_GLOBAL, "Respawn", &LuaCreature::Respawn },                             // :Respawn([force]) - Respawns the creature
    // {"AddLootMode", &LuaCreature::AddLootMode},                  // :AddLootMode(lootMode)
    // {"DealDamage", &LuaCreature::DealDamage},                    // :DealDamage(target, amount) - Deals damage to target (if target) : if no target, unit will damage self
    // {"SendCreatureTalk", &LuaCreature::SendCreatureTalk},        // :SendCreatureTalk(id, playerGUID) - Sends a chat message to a playerGUID (player) by id. Id can be found in creature_text under the 'group_id' column
    { ENV_GLOBAL, "AttackStart", &LuaCreature::AttackStart },                     // :AttackStart(target) - Creature attacks the specified target
    // {"ResetLootMode", &LuaCreature::ResetLootMode},
    // {"RemoveLootMode", &LuaCreature::RemoveLootMode},
    { ENV_GLOBAL, "SaveToDB", &LuaCreature::SaveToDB },                           // :SaveToDB() - Saves to database
    { ENV_GLOBAL, "SelectVictim", &LuaCreature::SelectVictim },                   // :SelectVictim() - Selects a victim
    { ENV_GLOBAL, "MoveWaypoint", &LuaCreature::MoveWaypoint },                   // :MoveWaypoint()

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<GameObject> GameObjectMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetDisplayId", &LuaGameObject::GetDisplayId },         // :GetDisplayId()
    { ENV_GLOBAL, "GetGoState", &LuaGameObject::GetGoState },             // :GetGoState() - Returns state
    { ENV_GLOBAL, "GetLootState", &LuaGameObject::GetLootState },         // :GetLootState() - Returns loot state

    // Setters
    { ENV_GLOBAL, "SetGoState", &LuaGameObject::SetGoState },
    { ENV_GLOBAL, "SetLootState", &LuaGameObject::SetLootState },

    // Boolean
    { ENV_GLOBAL, "IsTransport", &LuaGameObject::IsTransport },           // :IsTransport()
    // {"IsDestructible", &LuaGameObject::IsDestructible},  // :IsDestructible()
    { ENV_GLOBAL, "IsActive", &LuaGameObject::IsActive },                 // :IsActive()
    { ENV_GLOBAL, "HasQuest", &LuaGameObject::HasQuest },                 // :HasQuest(questId)
    { ENV_GLOBAL, "IsSpawned", &LuaGameObject::IsSpawned },               // :IsSpawned()

    // Other
    { ENV_GLOBAL, "RegisterEvent", &LuaGameObject::RegisterEvent },       // :RegisterEvent(function, delay, calls)
    { ENV_GLOBAL, "RemoveEventById", &LuaGameObject::RemoveEventById },   // :RemoveEventById(eventID)
    { ENV_GLOBAL, "RemoveEvents", &LuaGameObject::RemoveEvents },         // :RemoveEvents()
    { ENV_GLOBAL, "RemoveFromWorld", &LuaGameObject::RemoveFromWorld },   // :RemoveFromWorld(del)
    { ENV_GLOBAL, "UseDoorOrButton", &LuaGameObject::UseDoorOrButton },   // :UseDoorOrButton(delay) - Activates/closes/opens after X delay UNDOCUMENTED
    { ENV_GLOBAL, "Despawn", &LuaGameObject::Despawn },                   // :Despawn([delay]) - Despawns the object after delay
    { ENV_GLOBAL, "Respawn", &LuaGameObject::Respawn },                   // :Respawn([delay]) - respawns the object after delay
    { ENV_GLOBAL, "SaveToDB", &LuaGameObject::SaveToDB },                 // :SaveToDB() - Saves to database

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Item> ItemMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetOwnerGUID", &LuaItem::GetOwnerGUID },                   // :GetOwnerGUID() - Returns the owner's guid
    { ENV_GLOBAL, "GetOwner", &LuaItem::GetOwner },                           // :GetOwner() - Returns the owner object (player)
    { ENV_GLOBAL, "GetCount", &LuaItem::GetCount },                           // :GetCount() - Returns item stack count
    { ENV_GLOBAL, "GetMaxStackCount", &LuaItem::GetMaxStackCount },           // :GetMaxStackCount() - Returns item max stack count
    { ENV_GLOBAL, "GetSlot", &LuaItem::GetSlot },                             // :GetSlot() - returns the slot the item is in
    { ENV_GLOBAL, "GetBagSlot", &LuaItem::GetBagSlot },                       // :GetBagSlot() - returns the bagslot of the bag the item is in
    { ENV_GLOBAL, "GetEnchantmentId", &LuaItem::GetEnchantmentId },           // :GetEnchantmentId(enchant_slot) - Returns the enchantment in given slot. (permanent = 0)
    { ENV_GLOBAL, "GetSpellId", &LuaItem::GetSpellId },                       // :GetSpellId(index) - Returns spellID at given index (0 - 4)
    { ENV_GLOBAL, "GetSpellTrigger", &LuaItem::GetSpellTrigger },             // :GetSpellTrigger(index) - Returns spell trigger at given index (0 - 4)
    { ENV_GLOBAL, "GetItemLink", &LuaItem::GetItemLink },                     // :GetItemLink([localeID]) - Returns the shift clickable link of the item. Name translated if locale given and exists
    { ENV_GLOBAL, "GetClass", &LuaItem::GetClass },                           // :GetClass()
    { ENV_GLOBAL, "GetSubClass", &LuaItem::GetSubClass },                     // :GetSubClass()
    { ENV_GLOBAL, "GetName", &LuaItem::GetName },                             // :GetName()
    { ENV_GLOBAL, "GetDisplayId", &LuaItem::GetDisplayId },                   // :GetDisplayId()
    { ENV_GLOBAL, "GetQuality", &LuaItem::GetQuality },                       // :GetQuality()
    { ENV_GLOBAL, "GetBuyCount", &LuaItem::GetBuyCount },                     // :GetBuyCount()
    { ENV_GLOBAL, "GetBuyPrice", &LuaItem::GetBuyPrice },                     // :GetBuyPrice()
    { ENV_GLOBAL, "GetSellPrice", &LuaItem::GetSellPrice },                   // :GetSellPrice()
    { ENV_GLOBAL, "GetInventoryType", &LuaItem::GetInventoryType },           // :GetInventoryType()
    { ENV_GLOBAL, "GetAllowableClass", &LuaItem::GetAllowableClass },         // :GetAllowableClass()
    { ENV_GLOBAL, "GetAllowableRace", &LuaItem::GetAllowableRace },           // :GetAllowableRace()
    { ENV_GLOBAL, "GetItemLevel", &LuaItem::GetItemLevel },                   // :GetItemLevel()
    { ENV_GLOBAL, "GetRequiredLevel", &LuaItem::GetRequiredLevel },           // :GetRequiredLevel()
#ifdef WOTLK
    { ENV_GLOBAL, "GetStatsCount", &LuaItem::GetStatsCount },                 // :GetStatsCount()
#endif
    { ENV_GLOBAL, "GetRandomProperty", &LuaItem::GetRandomProperty },         // :GetRandomProperty()
#ifndef CLASSIC
    { ENV_GLOBAL, "GetRandomSuffix", &LuaItem::GetRandomSuffix },             // :GetRandomSuffix()
#endif
    { ENV_GLOBAL, "GetItemSet", &LuaItem::GetItemSet },                       // :GetItemSet()
    { ENV_GLOBAL, "GetBagSize", &LuaItem::GetBagSize },                       // :GetBagSize()

    // Setters
    { ENV_GLOBAL, "SetOwner", &LuaItem::SetOwner },                           // :SetOwner(player) - Sets the owner of the item
    { ENV_GLOBAL, "SetBinding", &LuaItem::SetBinding },                       // :SetBinding(bound) - Sets the item binding to true or false
    { ENV_GLOBAL, "SetCount", &LuaItem::SetCount },                           // :SetCount(count) - Sets the item count

    // Boolean
    { ENV_GLOBAL, "IsSoulBound", &LuaItem::IsSoulBound },                     // :IsSoulBound() - Returns true if the item is soulbound
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_GLOBAL, "IsBoundAccountWide", &LuaItem::IsBoundAccountWide },       // :IsBoundAccountWide() - Returns true if the item is account bound
#endif
    { ENV_GLOBAL, "IsBoundByEnchant", &LuaItem::IsBoundByEnchant },           // :IsBoundByEnchant() - Returns true if the item is bound with an enchant
    { ENV_GLOBAL, "IsNotBoundToPlayer", &LuaItem::IsNotBoundToPlayer },       // :IsNotBoundToPlayer(player) - Returns true if the item is not bound with player
    { ENV_GLOBAL, "IsLocked", &LuaItem::IsLocked },                           // :IsLocked() - Returns true if the item is locked
    { ENV_GLOBAL, "IsBag", &LuaItem::IsBag },                                 // :IsBag() - Returns true if the item is a bag
#ifndef CLASSIC
    { ENV_GLOBAL, "IsCurrencyToken", &LuaItem::IsCurrencyToken },             // :IsCurrencyToken() - Returns true if the item is a currency token
#endif
    { ENV_GLOBAL, "IsNotEmptyBag", &LuaItem::IsNotEmptyBag },                 // :IsNotEmptyBag() - Returns true if the item is not an empty bag
    { ENV_GLOBAL, "IsBroken", &LuaItem::IsBroken },                           // :IsBroken() - Returns true if the item is broken
    { ENV_GLOBAL, "CanBeTraded", &LuaItem::CanBeTraded },                     // :CanBeTraded() - Returns true if the item can be traded
    { ENV_GLOBAL, "IsInTrade", &LuaItem::IsInTrade },                         // :IsInTrade() - Returns true if the item is in trade
    { ENV_GLOBAL, "IsInBag", &LuaItem::IsInBag },                             // :IsInBag() - Returns true if the item is in a bag
    { ENV_GLOBAL, "IsEquipped", &LuaItem::IsEquipped },                       // :IsEquipped() - Returns true if the item is equipped
    { ENV_GLOBAL, "HasQuest", &LuaItem::HasQuest },                           // :HasQuest(questId) - Returns true if the item starts the quest
    { ENV_GLOBAL, "IsPotion", &LuaItem::IsPotion },                           // :IsPotion() - Returns true if the item is a potion
#ifndef CATA
    { ENV_GLOBAL, "IsWeaponVellum", &LuaItem::IsWeaponVellum },               // :IsWeaponVellum() - Returns true if the item is a weapon vellum
    { ENV_GLOBAL, "IsArmorVellum", &LuaItem::IsArmorVellum },                 // :IsArmorVellum() - Returns true if the item is an armor vellum
#endif
    { ENV_GLOBAL, "IsConjuredConsumable", &LuaItem::IsConjuredConsumable },   // :IsConjuredConsumable() - Returns true if the item is a conjured consumable
    // {"IsRefundExpired", &LuaItem::IsRefundExpired},          // :IsRefundExpired() - Returns true if the item's refund time has expired
    { ENV_GLOBAL, "SetEnchantment", &LuaItem::SetEnchantment },               // :SetEnchantment(enchantid, enchantmentslot) - Sets a new enchantment for the item. Returns true on success
    { ENV_GLOBAL, "ClearEnchantment", &LuaItem::ClearEnchantment },           // :ClearEnchantment(enchantmentslot) - Removes the enchantment from the item if one exists. Returns true on success

    // Other
    { ENV_GLOBAL, "SaveToDB", &LuaItem::SaveToDB },                           // :SaveToDB() - Saves to database

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Aura> AuraMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetCaster", &LuaAura::GetCaster },                 // :GetCaster() - Returns caster as object
    { ENV_GLOBAL, "GetCasterGUID", &LuaAura::GetCasterGUID },         // :GetCasterGUID() - Returns caster as GUID
    { ENV_GLOBAL, "GetCasterLevel", &LuaAura::GetCasterLevel },       // :GetCasterLevel() - Returns casters level
    { ENV_GLOBAL, "GetDuration", &LuaAura::GetDuration },             // :GetDuration() - Returns remaining duration
    { ENV_GLOBAL, "GetMaxDuration", &LuaAura::GetMaxDuration },       // :GetMaxDuration() - Returns maximum duration
    { ENV_GLOBAL, "GetCharges", &LuaAura::GetCharges },               // :GetCharges() - Returns remaining charges
    { ENV_GLOBAL, "GetAuraId", &LuaAura::GetAuraId },                 // :GetAuraId() - Returns aura ID
    { ENV_GLOBAL, "GetStackAmount", &LuaAura::GetStackAmount },       // :GetStackAmount() - Returns current stack amount
    { ENV_GLOBAL, "GetOwner", &LuaAura::GetOwner },                   // :GetOwner() - Gets the unit wearing the aura

    // Setters
    { ENV_GLOBAL, "SetDuration", &LuaAura::SetDuration },             // :SetDuration(duration) - Sets remaining duration
    { ENV_GLOBAL, "SetMaxDuration", &LuaAura::SetMaxDuration },       // :SetMaxDuration(duration) - Sets maximum duration
    { ENV_GLOBAL, "SetStackAmount", &LuaAura::SetStackAmount },       // :SetStackAmount(amount) - Sets current stack amount

    // Other
    { ENV_GLOBAL, "Remove", &LuaAura::Remove },                       // :Remove() - Removes the aura

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Spell> SpellMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetCaster", &LuaSpell::GetCaster },                // :GetCaster() - Returns the spell's caster (UNIT)
    { ENV_GLOBAL, "GetCastTime", &LuaSpell::GetCastTime },            // :GetCastTime() - Returns the spell cast time
    { ENV_GLOBAL, "GetEntry", &LuaSpell::GetId },                     // :GetEntry() - Returns the spell's ID
    { ENV_GLOBAL, "GetDuration", &LuaSpell::GetDuration },            // :GetDuration() - Returns the spell's duration
    { ENV_GLOBAL, "GetPowerCost", &LuaSpell::GetPowerCost },          // :GetPowerCost() - Returns the spell's power cost (mana, energy, rage, etc)
    { ENV_GLOBAL, "GetTargetDest", &LuaSpell::GetTargetDest },        // :GetTargetDest() - Returns the target destination (x,y,z,o,map) or nil. Orientation and map may be 0.
    { ENV_GLOBAL, "GetTarget", &LuaSpell::GetTarget },                // :GetTarget() - Returns spell cast target (item, worldobject)

    // Setters
    { ENV_GLOBAL, "SetAutoRepeat", &LuaSpell::SetAutoRepeat },        // :SetAutoRepeat(boolean)

    // Boolean
    { ENV_GLOBAL, "IsAutoRepeat", &LuaSpell::IsAutoRepeat },          // :IsAutoRepeat()

    // Other
    { ENV_GLOBAL, "Cancel", &LuaSpell::cancel },                      // :Cancel() - Cancels the spell casting
    { ENV_GLOBAL, "Cast", &LuaSpell::Cast },                          // :Cast(skipCheck) - Casts the spell (if true, removes the check for instant spells, etc)
    { ENV_GLOBAL, "Finish", &LuaSpell::Finish },                      // :Finish() - Finishes the spell (SPELL_STATE_FINISH)

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Quest> QuestMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetId", &LuaQuest::GetId },                                // :GetId() - Returns the quest's Id
    { ENV_GLOBAL, "GetLevel", &LuaQuest::GetLevel },                          // :GetLevel() - Returns the quest's level
    // {"GetMaxLevel", &LuaQuest::GetMaxLevel},                 // :GetMaxLevel() - Returns the quest's max level
    { ENV_GLOBAL, "GetMinLevel", &LuaQuest::GetMinLevel },                    // :GetMinLevel() - Returns the quest's min level
    { ENV_GLOBAL, "GetNextQuestId", &LuaQuest::GetNextQuestId },              // :GetNextQuestId() - Returns the quest's next quest ID
    { ENV_GLOBAL, "GetPrevQuestId", &LuaQuest::GetPrevQuestId },              // :GetPrevQuestId() - Returns the quest's previous quest ID
    { ENV_GLOBAL, "GetNextQuestInChain", &LuaQuest::GetNextQuestInChain },    // :GetNexQuestInChain() - Returns the next quest in its chain
    { ENV_GLOBAL, "GetFlags", &LuaQuest::GetFlags },                          // :GetFlags() - Returns the quest's flags
    { ENV_GLOBAL, "GetType", &LuaQuest::GetType },                            // :GetType() - Returns the quest's type

    // Boolean
    { ENV_GLOBAL, "HasFlag", &LuaQuest::HasFlag },                            // :HasFlag(flag) - Returns true or false if the quest has the specified flag
#ifndef CLASSIC
    { ENV_GLOBAL, "IsDaily", &LuaQuest::IsDaily },                            // :IsDaily() - Returns true or false if the quest is a daily
#endif
    { ENV_GLOBAL, "IsRepeatable", &LuaQuest::IsRepeatable },                  // :IsRepeatable() - Returns true or false if the quest is repeatable

    // Setters
    { ENV_GLOBAL, "SetFlag", &LuaQuest::SetFlag },                            // :SetFlag(flag) - Sets the flag of the quest by the specified flag

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Group> GroupMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetMembers", &LuaGroup::GetMembers },                      // :GetMembers() - returns a table the players in this group. (Online?)
    { ENV_GLOBAL, "GetLeaderGUID", &LuaGroup::GetLeaderGUID },
    { ENV_GLOBAL, "GetLeader", &LuaGroup::GetLeader },
    { ENV_GLOBAL, "GetGUID", &LuaGroup::GetGUID },
    { ENV_GLOBAL, "GetMemberGroup", &LuaGroup::GetMemberGroup },              // :GetMemberGroup(player) - Returns the player's subgroup ID
    { ENV_GLOBAL, "GetMemberGUID", &LuaGroup::GetMemberGUID },                // :GetMemberGUID("name") - Returns the member's GUID
    { ENV_GLOBAL, "GetMembersCount", &LuaGroup::GetMembersCount },            // :GetMembersCount() - Returns the member count of the group

    // Setters
    { ENV_GLOBAL, "SetLeader", &LuaGroup::ChangeLeader },                     // :SetLeader(Player) - Sets the player as the new leader
    { ENV_GLOBAL, "SetMembersGroup", &LuaGroup::ChangeMembersGroup },         // :ChangeMembersGroup(player, subGroup) - Changes the member's subgroup
    { ENV_GLOBAL, "SetTargetIcon", &LuaGroup::SetTargetIcon },                // :SetTargetIcon(icon, targetguid[, setterguid]) - Sets target's icon for group. target 0 to clear.

    // Boolean
    { ENV_GLOBAL, "IsLeader", &LuaGroup::IsLeader },                          // :IsLeader(GUID)
    { ENV_GLOBAL, "AddInvite", &LuaGroup::AddInvite },                        // :AddInvite(player) - Adds a an invite to player. Returns true if succesful
    { ENV_GLOBAL, "RemoveMember", &LuaGroup::RemoveMember },                  // :RemoveMember(player) - Removes player from group. Returns true on success
    { ENV_GLOBAL, "Disband", &LuaGroup::Disband },                            // :Disband() - Disbands the group
    { ENV_GLOBAL, "IsFull", &LuaGroup::IsFull },                              // :IsFull() - Returns true if the group is full
    // {"IsLFGGroup", &LuaGroup::isLFGGroup},                   // :IsLFGGroup() - Returns true if the group is an LFG group
    { ENV_GLOBAL, "IsRaidGroup", &LuaGroup::isRaidGroup },                    // :IsRaidGroup() - Returns true if the group is a raid group
    { ENV_GLOBAL, "IsBGGroup", &LuaGroup::isBGGroup },                        // :IsBGGroup() - Returns true if the group is a battleground group
    // {"IsBFGroup", &LuaGroup::isBFGroup},                     // :IsBFGroup() - Returns true if the group is a battlefield group
    { ENV_GLOBAL, "IsMember", &LuaGroup::IsMember },                          // :IsMember(player) - Returns true if the player is a member of the group
    { ENV_GLOBAL, "IsAssistant", &LuaGroup::IsAssistant },                    // :IsAssistant(player) - returns true if the player is an assistant in the group
    { ENV_GLOBAL, "SameSubGroup", &LuaGroup::SameSubGroup },                  // :SameSubGroup(player1, player2) - Returns true if the players are in the same subgroup in the group
    { ENV_GLOBAL, "HasFreeSlotSubGroup", &LuaGroup::HasFreeSlotSubGroup },    // :HasFreeSlotSubGroup(subGroup) - Returns true if the subgroupID has free slots

    // Other
    { ENV_GLOBAL, "SendPacket", &LuaGroup::SendPacket },                      // :SendPacket(packet, sendToPlayersInBattleground[, ignoreguid]) - Sends a specified packet to the group with the choice (true/false) to send it to players in a battleground. Optionally ignores given player guid
    // {"ConvertToLFG", &LuaGroup::ConvertToLFG},               // :ConvertToLFG() - Converts the group to an LFG group
    { ENV_GLOBAL, "ConvertToRaid", &LuaGroup::ConvertToRaid },                // :ConvertToRaid() - Converts the group to a raid group

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Guild> GuildMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetMembers", &LuaGuild::GetMembers },                  // :GetMembers() - returns a table containing the players in this guild. (Online?)
    { ENV_GLOBAL, "GetLeader", &LuaGuild::GetLeader },                    // :GetLeader() - Returns the guild learder's object
    { ENV_GLOBAL, "GetLeaderGUID", &LuaGuild::GetLeaderGUID },            // :GetLeaderGUID() - Returns the guild learder's guid
    { ENV_GLOBAL, "GetId", &LuaGuild::GetId },                            // :GetId() - Gets the guild's ID
    { ENV_GLOBAL, "GetName", &LuaGuild::GetName },                        // :GetName() - Gets the guild name
    { ENV_GLOBAL, "GetMOTD", &LuaGuild::GetMOTD },                        // :GetMOTD() - Gets the guild MOTD string
    { ENV_GLOBAL, "GetInfo", &LuaGuild::GetInfo },                        // :GetInfo() - Gets the guild info string
    { ENV_GLOBAL, "GetMemberCount", &LuaGuild::GetMemberCount },          // :GetMemberCount() - Returns the amount of players in the guild

    // Setters
#ifndef CLASSIC
    { ENV_GLOBAL, "SetBankTabText", &LuaGuild::SetBankTabText },          // :SetBankTabText(tabId, text)
#endif
    { ENV_GLOBAL, "SetMemberRank", &LuaGuild::ChangeMemberRank },         // :SetMemberRank(player, newRank) - Sets the player rank in the guild to the new rank
#ifndef CATA
    { ENV_GLOBAL, "SetLeader", &LuaGuild::SetLeader },                    // :SetLeader() - Sets the guild's leader
#endif

    // Boolean

    // Other
    { ENV_GLOBAL, "ChangeMemberRank", &LuaGuild::ChangeMemberRank },      // :ChangeMemberRank(player, rankId) - Changes players rank to rank specified
    { ENV_GLOBAL, "SendPacket", &LuaGuild::SendPacket },                  // :SendPacket(packet) - sends packet to guild
    { ENV_GLOBAL, "SendPacketToRanked", &LuaGuild::SendPacketToRanked },  // :SendPacketToRanked(packet, rankId) - sends packet to guild, specifying a rankId will only send the packet to your ranked members
    { ENV_GLOBAL, "Disband", &LuaGuild::Disband },                        // :Disband() - Disbands the guild
    { ENV_GLOBAL, "AddMember", &LuaGuild::AddMember },                    // :AddMember(player, rank) - adds the player to the guild. Rank is optional
    { ENV_GLOBAL, "DeleteMember", &LuaGuild::DeleteMember },              // :DeleteMember(player, disbanding, kicked) - Deletes the player from the guild. Disbanding and kicked are optional bools
#ifndef CLASSIC
    { ENV_GLOBAL, "DepositBankMoney", &LuaGuild::DepositBankMoney },      // :DepositBankMoney(money) - Deposits money into the guild bank
    { ENV_GLOBAL, "WithdrawBankMoney", &LuaGuild::WithdrawBankMoney },    // :WithdrawBankMoney(money) - Withdraws money from the guild bank
#endif

    { ENV_NONE, NULL, NULL }
};

#ifndef CLASSIC
#ifndef TBC
ElunaMethods<Vehicle> VehicleMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetOwner", &LuaVehicle::GetOwner },                    // :GetOwner() - Returns the the vehicle unit
    { ENV_GLOBAL, "GetEntry", &LuaVehicle::GetEntry },                    // :GetEntry() - Returns vehicle ID
    { ENV_GLOBAL, "GetPassenger", &LuaVehicle::GetPassenger },            // :GetPassenger(seatId) - Returns the passenger by seatId

    // Boolean
    { ENV_GLOBAL, "IsOnBoard", &LuaVehicle::IsOnBoard },                  // :IsOnBoard(unit) - Returns true if the unit is on board

    // Other
    { ENV_GLOBAL, "AddPassenger", &LuaVehicle::AddPassenger },            // :AddPassenger(passenger, seatId) - Adds a vehicle passenger
    { ENV_GLOBAL, "RemovePassenger", &LuaVehicle::RemovePassenger },      // :RemovePassenger(passenger) - Removes the passenger from the vehicle

    { ENV_NONE, NULL, NULL }
};
#endif
#endif

ElunaMethods<QueryResult> QueryMethods[] =
{
    { ENV_GLOBAL, "NextRow", &LuaQuery::NextRow },                        // :NextRow() - Advances to next rown in the query. Returns true if there is a next row, otherwise false
    { ENV_GLOBAL, "GetColumnCount", &LuaQuery::GetColumnCount },          // :GetColumnCount() - Gets the column count of the query
    { ENV_GLOBAL, "GetRowCount", &LuaQuery::GetRowCount },                // :GetRowCount() - Gets the row count of the query

    { ENV_GLOBAL, "GetBool", &LuaQuery::GetBool },                        // :GetBool(column) - returns a bool from a number column (for example tinyint)
    { ENV_GLOBAL, "GetUInt8", &LuaQuery::GetUInt8 },                      // :GetUInt8(column) - returns the value of an unsigned tinyint column
    { ENV_GLOBAL, "GetUInt16", &LuaQuery::GetUInt16 },                    // :GetUInt16(column) - returns the value of a unsigned smallint column
    { ENV_GLOBAL, "GetUInt32", &LuaQuery::GetUInt32 },                    // :GetUInt32(column) - returns the value of an unsigned int or mediumint column
    { ENV_GLOBAL, "GetUInt64", &LuaQuery::GetUInt64 },                    // :GetUInt64(column) - returns the value of an unsigned bigint column as string
    { ENV_GLOBAL, "GetInt8", &LuaQuery::GetInt8 },                        // :GetInt8(column) - returns the value of an tinyint column
    { ENV_GLOBAL, "GetInt16", &LuaQuery::GetInt16 },                      // :GetInt16(column) - returns the value of a smallint column
    { ENV_GLOBAL, "GetInt32", &LuaQuery::GetInt32 },                      // :GetInt32(column) - returns the value of an int or mediumint column
    { ENV_GLOBAL, "GetInt64", &LuaQuery::GetInt64 },                      // :GetInt64(column) - returns the value of a bigint column as string
    { ENV_GLOBAL, "GetFloat", &LuaQuery::GetFloat },                      // :GetFloat(column) - returns the value of a float column
    { ENV_GLOBAL, "GetDouble", &LuaQuery::GetDouble },                    // :GetDouble(column) - returns the value of a double column
    { ENV_GLOBAL, "GetString", &LuaQuery::GetString },                    // :GetString(column) - returns the value of a string column
    { ENV_GLOBAL, "IsNull", &LuaQuery::IsNull },                          // :IsNull(column) - returns true if the column is null

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<WorldPacket> PacketMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetOpcode", &LuaPacket::GetOpcode },                   // :GetOpcode() - Returns an opcode
    { ENV_GLOBAL, "GetSize", &LuaPacket::GetSize },                       // :GetSize() - Returns the packet size

    // Setters
    { ENV_GLOBAL, "SetOpcode", &LuaPacket::SetOpcode },                   // :SetOpcode(opcode) - Sets the opcode by specifying an opcode

    // Readers
    { ENV_GLOBAL, "ReadByte", &LuaPacket::ReadByte },                     // :ReadByte() - Reads an int8 value
    { ENV_GLOBAL, "ReadUByte", &LuaPacket::ReadUByte },                   // :ReadUByte() - Reads an uint8 value
    { ENV_GLOBAL, "ReadShort", &LuaPacket::ReadShort },                   // :ReadShort() - Reads an int16 value
    { ENV_GLOBAL, "ReadUShort", &LuaPacket::ReadUShort },                 // :ReadUShort() - Reads an uint16 value
    { ENV_GLOBAL, "ReadLong", &LuaPacket::ReadLong },                     // :ReadLong() - Reads an int32 value
    { ENV_GLOBAL, "ReadULong", &LuaPacket::ReadULong },                   // :ReadULong() - Reads an uint32 value
    { ENV_GLOBAL, "ReadGUID", &LuaPacket::ReadGUID },                     // :ReadGUID() - Reads an uint64 value
    { ENV_GLOBAL, "ReadString", &LuaPacket::ReadString },                 // :ReadString() - Reads a string value
    { ENV_GLOBAL, "ReadFloat", &LuaPacket::ReadFloat },                   // :ReadFloat() - Reads a float value
    { ENV_GLOBAL, "ReadDouble", &LuaPacket::ReadDouble },                 // :ReadDouble() - Reads a double value

    // Writers
    { ENV_GLOBAL, "WriteByte", &LuaPacket::WriteByte },                   // :WriteByte(val) - Writes an int8 value
    { ENV_GLOBAL, "WriteUByte", &LuaPacket::WriteUByte },                 // :WriteUByte(val) - Writes a uint8 value
    { ENV_GLOBAL, "WriteShort", &LuaPacket::WriteShort },                 // :WriteShort(val) - Writes an int16 value
    { ENV_GLOBAL, "WriteUShort", &LuaPacket::WriteUShort },               // :WriteUShort(val) - Writes a uint16 value
    { ENV_GLOBAL, "WriteLong", &LuaPacket::WriteLong },                   // :WriteLong(val) - Writes an int32 value
    { ENV_GLOBAL, "WriteULong", &LuaPacket::WriteULong },                 // :WriteULong(val) - Writes a uint32 value
    { ENV_GLOBAL, "WriteGUID", &LuaPacket::WriteGUID },                   // :WriteGUID(guid) - Writes a uint64 value
    { ENV_GLOBAL, "WriteString", &LuaPacket::WriteString },               // :WriteString(val) - Writes a string value
    { ENV_GLOBAL, "WriteFloat", &LuaPacket::WriteFloat },                 // :WriteFloat(val) - Writes a float value
    { ENV_GLOBAL, "WriteDouble", &LuaPacket::WriteDouble },               // :WriteDouble(val) - Writes a double value

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Map> MapMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetName", &LuaMap::GetName },                          // :GetName() - Returns the map's name UNDOCUMENTED
    { ENV_GLOBAL, "GetDifficulty", &LuaMap::GetDifficulty },              // :GetDifficulty() - Returns the map's difficulty UNDOCUMENTED
    { ENV_GLOBAL, "GetInstanceId", &LuaMap::GetInstanceId },              // :GetInstanceId() - Returns the map's instance ID UNDOCUMENTED
    { ENV_GLOBAL, "GetPlayerCount", &LuaMap::GetPlayerCount },            // :GetPlayerCount() - Returns the amount of players on map except GM's UNDOCUMENTED
    { ENV_GLOBAL, "GetMapId", &LuaMap::GetMapId },                        // :GetMapId() - Returns the map's ID UNDOCUMENTED
    { ENV_GLOBAL, "GetAreaId", &LuaMap::GetAreaId },                      // :GetAreaId(x, y, z) - Returns the map's area ID based on coords UNDOCUMENTED
    { ENV_GLOBAL, "GetHeight", &LuaMap::GetHeight },                      // :GetHeight(x, y[, phasemask]) - Returns ground Z coordinate. UNDOCUMENTED

    // Booleans
#ifndef CLASSIC
    { ENV_GLOBAL, "IsArena", &LuaMap::IsArena },                          // :IsArena() - Returns the true if the map is an arena, else false UNDOCUMENTED
#endif
    { ENV_GLOBAL, "IsBattleground", &LuaMap::IsBattleground },            // :IsBattleground() - Returns the true if the map is a battleground, else false UNDOCUMENTED
    { ENV_GLOBAL, "IsDungeon", &LuaMap::IsDungeon },                      // :IsDungeon() - Returns the true if the map is a dungeon , else false UNDOCUMENTED
    { ENV_GLOBAL, "IsEmpty", &LuaMap::IsEmpty },                          // :IsEmpty() - Returns the true if the map is empty, else false UNDOCUMENTED
#ifndef CLASSIC
    { ENV_GLOBAL, "IsHeroic", &LuaMap::IsHeroic },                        // :IsHeroic() - Returns the true if the map is a heroic dungeon, else false UNDOCUMENTED
#endif
    { ENV_GLOBAL, "IsRaid", &LuaMap::IsRaid },                            // :IsRaid() - Returns the true if the map is a raid map, else false UNDOCUMENTED

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Corpse> CorpseMethods[] =
{
    { ENV_GLOBAL, "GetOwnerGUID", &LuaCorpse::GetOwnerGUID },                     // :GetOwnerGUID() - Returns the corpse owner GUID
    { ENV_GLOBAL, "GetGhostTime", &LuaCorpse::GetGhostTime },                     // :GetGhostTime() - Returns the ghost time of a corpse
    { ENV_GLOBAL, "GetType", &LuaCorpse::GetType },                               // :GetType() - Returns the (CorpseType) of a corpse
    { ENV_GLOBAL, "ResetGhostTime", &LuaCorpse::ResetGhostTime },                 // :ResetGhostTime() - Resets the corpse's ghost time
    { ENV_GLOBAL, "SaveToDB", &LuaCorpse::SaveToDB },                             // :SaveToDB() - Saves to database
    { ENV_GLOBAL, "DeleteBonesFromWorld", &LuaCorpse::DeleteBonesFromWorld },     // :DeleteBonesFromWorld() - Deletes all bones from the world

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<Weather> WeatherMethods[] =
{
    // Getters
    { ENV_GLOBAL, "GetZoneId", &LuaWeather::GetZoneId },                                  // :GetZoneId() - Returns the weather's zoneId

    // Setters
    { ENV_GLOBAL, "SetWeather", &LuaWeather::SetWeather },                                // :SetWeather(weatherType, grade) - Sets the weather by weather type and grade

    // Boolean
    { ENV_GLOBAL, "Regenerate", &LuaWeather::Regenerate },                                // :Regenerate() - Calculates weather, returns true if the weather changed
    { ENV_GLOBAL, "UpdateWeather", &LuaWeather::UpdateWeather },                          // :UpdateWeather() - Updates the weather in a zone that has players in it, returns false if players aren't found

    // Other
    { ENV_GLOBAL, "SendWeatherUpdateToPlayer", &LuaWeather::SendWeatherUpdateToPlayer },  // :SendWeatherUpdateToPlayer(player) - Sends weather update to the player

    { ENV_NONE, NULL, NULL }
};

ElunaMethods<AuctionHouseObject> AuctionMethods[] =
{
    { ENV_NONE, NULL, NULL }
};

void Eluna::RegisterGlobals()
{
    ElunaFunctions* function = GlobalFunctions;
    for (; function->name; ++function)
    {
        if (function->env == ENV_MAP && !GMap)
            continue;
        else if (function->env == ENV_WORLD && GMap)
            continue;
        else if (function->env == ENV_NONE)
            continue;
        else
            ASSERT(function->env >= ENV_NONE && function->env < ENV_MAX); // none of defined types
        lua_register(L, function->name, function->mfunc);
    }
}

void Eluna::RegisterFunctions()
{
    lua_settop(L, 0); // clean stack

    RegisterGlobals();

    ElunaTemplate<Object>::Register(L, "Object");
    ElunaTemplate<Object>::SetMethods(this, ObjectMethods);

    ElunaTemplate<WorldObject>::Register(L, "WorldObject");
    ElunaTemplate<WorldObject>::SetMethods(this, ObjectMethods);
    ElunaTemplate<WorldObject>::SetMethods(this, WorldObjectMethods);

    ElunaTemplate<Unit>::Register(L, "Unit");
    ElunaTemplate<Unit>::SetMethods(this, ObjectMethods);
    ElunaTemplate<Unit>::SetMethods(this, WorldObjectMethods);
    ElunaTemplate<Unit>::SetMethods(this, UnitMethods);

    ElunaTemplate<Player>::Register(L, "Player");
    ElunaTemplate<Player>::SetMethods(this, ObjectMethods);
    ElunaTemplate<Player>::SetMethods(this, WorldObjectMethods);
    ElunaTemplate<Player>::SetMethods(this, UnitMethods);
    ElunaTemplate<Player>::SetMethods(this, PlayerMethods);

    ElunaTemplate<Creature>::Register(L, "Creature");
    ElunaTemplate<Creature>::SetMethods(this, ObjectMethods);
    ElunaTemplate<Creature>::SetMethods(this, WorldObjectMethods);
    ElunaTemplate<Creature>::SetMethods(this, UnitMethods);
    ElunaTemplate<Creature>::SetMethods(this, CreatureMethods);

    ElunaTemplate<GameObject>::Register(L, "GameObject");
    ElunaTemplate<GameObject>::SetMethods(this, ObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(this, WorldObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(this, GameObjectMethods);

    ElunaTemplate<Corpse>::Register(L, "Corpse");
    ElunaTemplate<Corpse>::SetMethods(this, ObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(this, WorldObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(this, CorpseMethods);

    ElunaTemplate<Item>::Register(L, "Item");
    ElunaTemplate<Item>::SetMethods(this, ObjectMethods);
    ElunaTemplate<Item>::SetMethods(this, ItemMethods);

#ifndef CLASSIC
#ifndef TBC
    ElunaTemplate<Vehicle>::Register(L, "Vehicle");
    ElunaTemplate<Vehicle>::SetMethods(this, VehicleMethods);
#endif
#endif

    ElunaTemplate<Group>::Register(L, "Group");
    ElunaTemplate<Group>::SetMethods(this, GroupMethods);

    ElunaTemplate<Guild>::Register(L, "Guild");
    ElunaTemplate<Guild>::SetMethods(this, GuildMethods);

    ElunaTemplate<Aura>::Register(L, "Aura");
    ElunaTemplate<Aura>::SetMethods(this, AuraMethods);

    ElunaTemplate<Spell>::Register(L, "Spell");
    ElunaTemplate<Spell>::SetMethods(this, SpellMethods);

    ElunaTemplate<Quest>::Register(L, "Quest");
    ElunaTemplate<Quest>::SetMethods(this, QuestMethods);

    ElunaTemplate<Map>::Register(L, "Map");
    ElunaTemplate<Map>::SetMethods(this, MapMethods);

    ElunaTemplate<Weather>::Register(L, "Weather");
    ElunaTemplate<Weather>::SetMethods(this, WeatherMethods);

    ElunaTemplate<AuctionHouseObject>::Register(L, "AuctionHouseObject");
    ElunaTemplate<AuctionHouseObject>::SetMethods(this, AuctionMethods);

    ElunaTemplate<WorldPacket>::Register(L, "WorldPacket", true);
    ElunaTemplate<WorldPacket>::SetMethods(this, PacketMethods);

    ElunaTemplate<QueryResult>::Register(L, "QueryResult", true);
    ElunaTemplate<QueryResult>::SetMethods(this, QueryMethods);

    lua_settop(L, 0); // clean stack
}
