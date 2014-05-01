/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef LUAHOOKS_H
#define LUAHOOKS_H

enum RegisterTypes
{
    REGTYPE_GLOBAL,
    REGTYPE_SERVER,
    REGTYPE_PACKET,
    REGTYPE_GUILD,
    REGTYPE_GROUP,
    REGTYPE_MAP,
    REGTYPE_PLAYER,
    REGTYPE_CREATURE,
    REGTYPE_GAMEOBJECT,
    REGTYPE_ITEM,
    REGTYPE_CREATURE_GOSSIP,
    REGTYPE_GAMEOBJECT_GOSSIP,
    REGTYPE_ITEM_GOSSIP,
    REGTYPE_PLAYER_GOSSIP,
    REGTYPE_COUNT
};

// RegisterPacketEvent(Opcode, function)
//  SERVER_EVENT_ON_PACKET_RECEIVE                  =   1,      // (event, packet, player) - Player only if accessible. Can return false or a new packet
//  SERVER_EVENT_ON_PACKET_SEND                     =   2,      // (event, packet, player) - Player only if accessible. Can return false or a new packet

// RegisterServerEvent(EventId, function)
enum ServerEvents
{
    SERVER_EVENT_ON_PACKET_RECEIVE                  =   1,      // (event, packet, player) - Player only if accessible. Can return false or a new packet
    SERVER_EVENT_ON_PACKET_SEND                     =   2,      // (event, packet, player) - Player only if accessible. Can return false or a new packet

    // Not implemented on mangos
    WORLD_EVENT_ON_OPEN_STATE_CHANGE                =   3,      // (event, open)
    WORLD_EVENT_ON_CONFIG_LOAD                      =   4,      // (event, reload)
    WORLD_EVENT_ON_SHUTDOWN_INIT                    =   5,      // (event, code, mask)
    WORLD_EVENT_ON_SHUTDOWN_CANCEL                  =   6,      // (event)
    WORLD_EVENT_ON_UPDATE                           =   7,      // (event, diff)
    WORLD_EVENT_ON_STARTUP                          =   8,      // (event)
    WORLD_EVENT_ON_SHUTDOWN                         =   9,      // (event)

    WEATHER_EVENT_ON_CHANGE                         =   10,     // (event, weather, state, grade)

    AUCTION_EVENT_ON_ADD                            =   11,     // (event, AHObject)
    AUCTION_EVENT_ON_REMOVE                         =   12,     // (event, AHObject)
    AUCTION_EVENT_ON_SUCCESSFUL                     =   13,     // (event, AHObject) // Not Implemented
    AUCTION_EVENT_ON_EXPIRE                         =   14,     // (event, AHObject) // Not Implemented

    PLAYER_EVENT_ON_CHARACTER_CREATE                =   15,     // (event, player)
    PLAYER_EVENT_ON_CHARACTER_DELETE                =   16,     // (event, guid)
    PLAYER_EVENT_ON_LOGIN                           =   17,     // (event, player)
    PLAYER_EVENT_ON_LOGOUT                          =   18,     // (event, player)
    PLAYER_EVENT_ON_CHAT                            =   19,     // (event, player, msg, Type, lang) - Can return false or new msg
    PLAYER_EVENT_ON_WHISPER                         =   20,     // (event, player, msg, Type, lang, receiver) - Can return false or new msg
    PLAYER_EVENT_ON_GROUP_CHAT                      =   21,     // (event, player, msg, Type, lang, group) - Can return false or new msg
    PLAYER_EVENT_ON_GUILD_CHAT                      =   22,     // (event, player, msg, Type, lang, guild) - Can return false or new msg
    PLAYER_EVENT_ON_CHANNEL_CHAT                    =   23,     // (event, player, msg, Type, lang, channel) - Can return false or new msg
    PLAYER_EVENT_ON_EMOTE                           =   24,     // (event, player, emote) - Not triggered on any known emote
    PLAYER_EVENT_ON_TEXT_EMOTE                      =   25,     // (event, player, textEmote, emoteNum, guid)
    PLAYER_EVENT_ON_COMMAND                         =   26,     // (event, player, command) - Can return false
    PLAYER_EVENT_ON_ADDON_MESSAGE                   =   27,     // (event, sender, type, prefix, msg, target) - target can be nil/whisper_target/guid/group/channel

    SERVER_EVENT_COUNT
};

// RegisterMapEvent(eventId, function)
enum MapEvents
{
    MAP_EVENT_ON_UPDATE                             =   1,      // (event, diff)
    MAP_EVENT_ON_CREATE                             =   2,      // (event)
    MAP_EVENT_ON_DESTROY                            =   3,      // (event)
    MAP_EVENT_ON_AREATRIGGER                            =   4,      // (event, player, triggerId)

    VEHICLE_EVENT_ON_INSTALL                        =   5,      // (event, vehicle)
    VEHICLE_EVENT_ON_UNINSTALL                      =   6,      // (event, vehicle)
    VEHICLE_EVENT_ON_ADD_PASSENGER                  =   7,      // (event, vehicle, unit, seatId)
    VEHICLE_EVENT_ON_REMOVE_PASSENGER               =   8,      // (event, vehicle, unit)
    VEHICLE_EVENT_ON_INSTALL_ACCESSORY              =   9,      // (event, vehicle, creature)

    MAP_EVENT_COUNT
};

// RegisterGlobalEvent(eventId, function)
enum GlobalEvents
{
    ELUNA_EVENT_ON_STATE_MSG                        =   1,      // (event, value1, value2...) - value is nil/bool/string/number //RECHECK
    ELUNA_EVENT_ON_LUA_STATE_CLOSE                  =   2,      // (event)                                                     //RECHECK

    GLOBAL_EVENT_COUNT
};

// RegisterPlayerEvent(eventId, function)
enum PlayerEvents
{
    PLAYER_EVENT_ON_MAP_ENTER                       =   1,      // (event, plpayer, map)
    PLAYER_EVENT_ON_MAP_LEAVE                       =   2,      // (event, plpayer, map)
    PLAYER_EVENT_ON_SPELL_CAST                      =   3,      // (event, player, spell, skipCheck)
    PLAYER_EVENT_ON_KILL_PLAYER                     =   4,      // (event, killer, killed)
    PLAYER_EVENT_ON_KILL_CREATURE                   =   5,      // (event, killer, killed)
    PLAYER_EVENT_ON_KILLED_BY_CREATURE              =   6,      // (event, killer, killed)
    PLAYER_EVENT_ON_DUEL_REQUEST                    =   7,      // (event, target, challenger)
    PLAYER_EVENT_ON_DUEL_START                      =   8,      // (event, player1, player2)
    PLAYER_EVENT_ON_DUEL_END                        =   9,      // (event, winner, loser, type)
    PLAYER_EVENT_ON_GIVE_XP                         =   10,     // (event, player, amount, victim) - Can return new XP amount
    PLAYER_EVENT_ON_LEVEL_CHANGE                    =   11,     // (event, player, oldLevel)
    PLAYER_EVENT_ON_MONEY_CHANGE                    =   12,     // (event, player, amount)
    PLAYER_EVENT_ON_REPUTATION_CHANGE               =   13,     // (event, player, factionId, standing, incremental) - Can return new standing
    PLAYER_EVENT_ON_TALENTS_CHANGE                  =   14,     // (event, player, points)
    PLAYER_EVENT_ON_TALENTS_RESET                   =   15,     // (event, player, noCost)
    PLAYER_EVENT_ON_SAVE                            =   16,     // (event, player)
    PLAYER_EVENT_ON_BIND_TO_INSTANCE                =   17,     // (event, player, difficulty, mapid, permanent)
    PLAYER_EVENT_ON_UPDATE_ZONE                     =   18,     // (event, player, newZone, newArea)

    // Custom
    PLAYER_EVENT_ON_EQUIP                           =   19,     // (event, player, item, bag, slot)
    PLAYER_EVENT_ON_FIRST_LOGIN                     =   20,     // (event, player)
    PLAYER_EVENT_ON_CAN_USE_ITEM                    =   21,     // (event, player, itemEntry)
    PLAYER_EVENT_ON_LOOT_ITEM                       =   22,     // (event, player, item, count)
    PLAYER_EVENT_ON_ENTER_COMBAT                    =   23,     // (event, player, enemy)
    PLAYER_EVENT_ON_LEAVE_COMBAT                    =   24,     // (event, player)
    PLAYER_EVENT_ON_REPOP                           =   25,     // (event, player)
    PLAYER_EVENT_ON_RESURRECT                       =   26,     // (event, player)
    PLAYER_EVENT_ON_LOOT_MONEY                      =   27,     // (event, player, amount)
    PLAYER_EVENT_ON_QUEST_ABANDON                   =   28,     // (event, player, questId)

    PLAYER_EVENT_COUNT
};

// RegisterGuildEvent(eventId, function)
enum GuildEventTypes
{
    // Guild
    GUILD_EVENT_ON_ADD_MEMBER                       =   1,      // (event, guild, player, rank)
    GUILD_EVENT_ON_REMOVE_MEMBER                    =   2,      // (event, guild, isDisbanding)
    GUILD_EVENT_ON_MOTD_CHANGE                      =   3,      // (event, guild, newMotd)
    GUILD_EVENT_ON_INFO_CHANGE                      =   4,      // (event, guild, newInfo)
    GUILD_EVENT_ON_CREATE                           =   5,      // (event, guild, leader, name)  // check existence on tc and mangos
    GUILD_EVENT_ON_DISBAND                          =   6,      // (event, guild)
    GUILD_EVENT_ON_MONEY_WITHDRAW                   =   7,      // (event, guild, player, amount, isRepair)
    GUILD_EVENT_ON_MONEY_DEPOSIT                    =   8,      // (event, guild, player, amount)
    GUILD_EVENT_ON_ITEM_MOVE                        =   9,      // (event, guild, player, item, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId)   // TODO
    GUILD_EVENT_ON_EVENT                            =   10,     // (event, guild, eventType, plrGUIDLow1, plrGUIDLow2, newRank)  // TODO
    GUILD_EVENT_ON_BANK_EVENT                       =   11,     // (event, guild, eventType, tabId, playerGUIDLow, itemOrMoney, itemStackCount, destTabId)

    GUILD_EVENT_COUNT
};

// RegisterGroupEvent(eventId, function)
enum GroupEvents
{
    // Group
    GROUP_EVENT_ON_MEMBER_ADD                       =   1,      // (event, group, guid)
    GROUP_EVENT_ON_MEMBER_INVITE                    =   2,      // (event, group, guid)
    GROUP_EVENT_ON_MEMBER_REMOVE                    =   3,      // (event, group, guid, method, kicker, reason)
    GROUP_EVENT_ON_LEADER_CHANGE                    =   4,      // (event, group, newLeaderGuid, oldLeaderGuid)
    GROUP_EVENT_ON_DISBAND                          =   5,      // (event, group)
    GROUP_EVENT_ON_CREATE                           =   6,      // (event, group, leaderGuid, groupType)

    GROUP_EVENT_COUNT
};

// RegisterCreatureEvent(entry, EventId, function)
enum CreatureEvents
{
    CREATURE_EVENT_ON_ENTER_COMBAT                  =   1,      // (event, creature, target)
    CREATURE_EVENT_ON_LEAVE_COMBAT                  =   2,      // (event, creature)
    CREATURE_EVENT_ON_TARGET_DIED                   =   3,      // (event, creature, victim)
    CREATURE_EVENT_ON_DIED                          =   4,      // (event, creature, killer)
    CREATURE_EVENT_ON_SPAWN                         =   5,      // (event, creature)
    CREATURE_EVENT_ON_RESET                         =   6,      // (event, creature)
    CREATURE_EVENT_ON_REACH_WP                      =   7,      // (event, creature, type, id)
    CREATURE_EVENT_ON_AIUPDATE                      =   8,      // (event, creature, diff)
    CREATURE_EVENT_ON_RECEIVE_EMOTE                 =   9,      // (event, creature, player, emoteid)
    CREATURE_EVENT_ON_DAMAGE_TAKEN                  =   10,     // (event, creature, attacker, damage)
    CREATURE_EVENT_ON_PRE_COMBAT                    =   11,     // (event, creature, target)
    CREATURE_EVENT_ON_ATTACKED_AT                   =   12,     // (event, creature, attacker)
    CREATURE_EVENT_ON_OWNER_ATTACKED                =   13,     // (event, creature, target)    // Not on mangos
    CREATURE_EVENT_ON_OWNER_ATTACKED_AT             =   14,     // (event, creature, attacker)  // Not on mangos
    CREATURE_EVENT_ON_HIT_BY_SPELL                  =   15,     // (event, creature, caster, spellid)
    CREATURE_EVENT_ON_SPELL_HIT_TARGET              =   16,     // (event, creature, target, spellid)
    CREATURE_EVENT_ON_JUST_SUMMONED_CREATURE        =   17,     // (event, creature, summon)
    CREATURE_EVENT_ON_SUMMONED_CREATURE_DESPAWN     =   18,     // (event, creature, summon)
    CREATURE_EVENT_ON_SUMMONED_CREATURE_DIED        =   19,     // (event, creature, summon, killer)    // Not on mangos
    CREATURE_EVENT_ON_SUMMONED                      =   20,     // (event, creature, summoner)
    CREATURE_EVENT_ON_REACH_HOME                    =   21,     // (event, creature)
    CREATURE_EVENT_ON_CORPSE_REMOVED                =   22,     // (event, creature, respawndelay)
    CREATURE_EVENT_ON_MOVE_IN_LOS                   =   23,     // (event, creature, unit) // Doesnt actually check LOS
    CREATURE_EVENT_ON_DUMMY_EFFECT                  =   24,     // (event, caster, spellid, effindex, creature)
    CREATURE_EVENT_ON_QUEST_ACCEPT                  =   25,     // (event, player, creature, quest)
    CREATURE_EVENT_ON_QUEST_COMPLETE                =   26,     // (event, player, creature, quest)
    CREATURE_EVENT_ON_QUEST_REWARD                  =   27,     // (event, player, creature, quest, opt)
    CREATURE_EVENT_ON_DIALOG_STATUS                 =   28,     // (event, player, creature)

    CREATURE_EVENT_COUNT
};

// RegisterGameObjectEvent(entry, EventId, function)
enum GameObjectEvents
{
    GAMEOBJECT_EVENT_ON_AIUPDATE                    =   1,      // (event, go, diff)
    GAMEOBJECT_EVENT_ON_SPAWN                       =   2,      // (event, go)
    GAMEOBJECT_EVENT_ON_DUMMY_EFFECT                =   3,      // (event, caster, spellid, effindex, go)
    GAMEOBJECT_EVENT_ON_QUEST_ACCEPT                =   4,      // (event, player, go, quest)
    GAMEOBJECT_EVENT_ON_QUEST_REWARD                =   5,      // (event, player, go, quest, opt)
    GAMEOBJECT_EVENT_ON_DIALOG_STATUS               =   6,      // (event, player, go)
    GAMEOBJECT_EVENT_ON_DESTROYED                   =   7,      // (event, go, player)
    GAMEOBJECT_EVENT_ON_DAMAGED                     =   8,      // (event, go, player)
    GAMEOBJECT_EVENT_ON_LOOT_STATE_CHANGE           =   9,      // (event, go, state)
    GAMEOBJECT_EVENT_ON_GO_STATE_CHANGED            =   10,     // (event, go, state)
    GAMEOBJECT_EVENT_ON_QUEST_COMPLETE              =   11,     // (event, player, go, quest)

    GAMEOBJECT_EVENT_COUNT
};

// RegisterItemEvent(entry, EventId, function)
enum ItemEvents
{
    ITEM_EVENT_ON_DUMMY_EFFECT                      =   1,      // (event, caster, spellid, effindex, item)
    ITEM_EVENT_ON_USE                               =   2,      // (event, player, item, target)
    ITEM_EVENT_ON_QUEST_ACCEPT                      =   3,      // (event, player, item, quest)
    ITEM_EVENT_ON_EXPIRE                            =   4,      // (event, player, itemid)
    ITEM_EVENT_ON_REMOVE                            =   5,      // (event, player, item)

    ITEM_EVENT_COUNT
};

// RegisterCreatureGossipEvent(entry, EventId, function)
// RegisterGameObjectGossipEvent(entry, EventId, function)
// RegisterItemGossipEvent(entry, EventId, function)
// RegisterPlayerGossipEvent(menu_id, EventId, function)
enum GossipEvents
{
    GOSSIP_EVENT_ON_HELLO                           =   1,      // (event, player, object) - Object is the Creature/GameObject/Item
    GOSSIP_EVENT_ON_SELECT                          =   2,      // (event, player, object, sender, intid, code, menu_id) - Object is the Creature/GameObject/Item/Player, menu_id is only for player gossip

    GOSSIP_EVENT_COUNT
};

#endif
