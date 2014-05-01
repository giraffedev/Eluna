/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "HookMgr.h"
#include "LuaEngine.h"
#include "Includes.h"

void Eluna::OnStateMessage(StateMsg* msg)
{
    if (!GlobalEventBindings.HasEvents(ELUNA_EVENT_ON_STATE_MSG))
        return;
    GlobalEventBindings.BeginCall(ELUNA_EVENT_ON_STATE_MSG);
    msg->Push(L);
    GlobalEventBindings.ExecuteCall();
    GlobalEventBindings.EndCall();
}

void Eluna::OnCloseLua()
{
    if (!GlobalEventBindings.HasEvents(ELUNA_EVENT_ON_LUA_STATE_CLOSE))
        return;
    GlobalEventBindings.BeginCall(ELUNA_EVENT_ON_LUA_STATE_CLOSE);
    GlobalEventBindings.ExecuteCall();
    GlobalEventBindings.EndCall();
}

// areatrigger
bool Eluna::OnAreaTrigger(Player* pPlayer, AreaTriggerEntry const* pTrigger)
{
    if (!MapEventBindings.HasEvents(MAP_EVENT_ON_AREATRIGGER))
        return false;
    MapEventBindings.BeginCall(MAP_EVENT_ON_AREATRIGGER);
    Push(L, pPlayer);
    Push(L, pTrigger->id);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
    return false;
}
// weather
void Eluna::OnChange(Weather* weather, WeatherState state, float grade)
{
    if (!ServerEventBindings.HasEvents(WEATHER_EVENT_ON_CHANGE))
        return;
    ServerEventBindings.BeginCall(WEATHER_EVENT_ON_CHANGE);
    Push(L, (weather->GetZone()));
    Push(L, state);
    Push(L, grade);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}
// Auction House
void Eluna::OnAdd(AuctionHouseObject* ah)
{
    if (!ServerEventBindings.HasEvents(AUCTION_EVENT_ON_ADD))
        return;
    ServerEventBindings.BeginCall(AUCTION_EVENT_ON_ADD);
    Push(L, (ah));
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnRemove(AuctionHouseObject* ah)
{
    if (!ServerEventBindings.HasEvents(AUCTION_EVENT_ON_REMOVE))
        return;
    ServerEventBindings.BeginCall(AUCTION_EVENT_ON_REMOVE);
    Push(L, (ah));
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnSuccessful(AuctionHouseObject* ah)
{
    if (!ServerEventBindings.HasEvents(AUCTION_EVENT_ON_SUCCESSFUL))
        return;
    ServerEventBindings.BeginCall(AUCTION_EVENT_ON_SUCCESSFUL);
    Push(L, (ah));
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnExpire(AuctionHouseObject* ah)
{
    if (!ServerEventBindings.HasEvents(AUCTION_EVENT_ON_EXPIRE))
        return;
    ServerEventBindings.BeginCall(AUCTION_EVENT_ON_EXPIRE);
    Push(L, (ah));
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

// Packet
bool Eluna::OnPacketSend(WorldSession* session, WorldPacket& packet)
{
    bool result = true;
    Player* player = NULL;
    if (session)
        player = session->GetPlayer();
    if (ServerEventBindings.HasEvents(SERVER_EVENT_ON_PACKET_SEND))
    {
        ServerEventBindings.BeginCall(SERVER_EVENT_ON_PACKET_SEND);
        Push(L, new WorldPacket(packet));
        Push(L, player);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(L, i, false))
                packet = *data;
            if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    if (PacketEventBindings.HasEvents(Opcodes(packet.GetOpcode())))
    {
        PacketEventBindings.BeginCall(Opcodes(packet.GetOpcode()));
        Push(L, new WorldPacket(packet));
        Push(L, player);
        PacketEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(L, i, false))
                packet = *data;
            if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        PacketEventBindings.EndCall();
    }
    return result;
}
bool Eluna::OnPacketReceive(WorldSession* session, WorldPacket& packet)
{
    bool result = true;
    Player* player = NULL;
    if (session)
        player = session->GetPlayer();
    if (ServerEventBindings.HasEvents(SERVER_EVENT_ON_PACKET_RECEIVE))
    {
        ServerEventBindings.BeginCall(SERVER_EVENT_ON_PACKET_RECEIVE);
        Push(L, new WorldPacket(packet));
        Push(L, player);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(L, i, false))
                packet = *data;
            if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    if (PacketEventBindings.HasEvents(Opcodes(packet.GetOpcode())))
    {
        PacketEventBindings.BeginCall(Opcodes(packet.GetOpcode()));
        Push(L, new WorldPacket(packet));
        Push(L, player);
        PacketEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(L, i, false))
                packet = *data;
            if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        PacketEventBindings.EndCall();
    }
    return result;
}
// AddOns
bool Eluna::OnAddonMessage(Player* sender, uint32 type, std::string& msg, Player* receiver, Guild* guild, Group* group, Channel* channel)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_ADDON_MESSAGE))
        return false;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_ADDON_MESSAGE);
    Push(L, sender);
    Push(L, type);
    const char* c_msg = msg.c_str();
    Push(L, strtok((char*)c_msg, "\t")); // prefix
    Push(L, strtok(NULL, "")); // msg
    if (receiver)
        Push(L, receiver);
    else if (guild)
        Push(L, guild);
    else if (group)
        Push(L, group);
    else if (channel)
        Push(L, channel->GetChannelId());
    else
        Push(L);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
    return true;
}

void Eluna::OnOpenStateChange(bool open)
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_OPEN_STATE_CHANGE))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_OPEN_STATE_CHANGE);
    Push(L, open);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnConfigLoad(bool reload)
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_CONFIG_LOAD))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_CONFIG_LOAD);
    Push(L, reload);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask)
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_SHUTDOWN_INIT))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_SHUTDOWN_INIT);
    Push(L, code);
    Push(L, mask);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnShutdownCancel()
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_SHUTDOWN_CANCEL))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_SHUTDOWN_CANCEL);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnWorldUpdate(uint32 diff)
{
    // process sent messages from map states
    StateMsg* msg = NULL;
    while (StateMsgQue.next(msg))
    {
        if (Eluna* E = msg->GetTarget())
            E->OnStateMessage(msg);
        delete msg;
    }

    // process world tick
    m_EventMgr.Update(diff);

    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_UPDATE))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_UPDATE);
    Push(L, diff);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnStartup()
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_STARTUP))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_STARTUP);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnShutdown()
{
    if (!ServerEventBindings.HasEvents(WORLD_EVENT_ON_SHUTDOWN))
        return;
    ServerEventBindings.BeginCall(WORLD_EVENT_ON_SHUTDOWN);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::HandleGossipSelectOption(Player* pPlayer, Item* item, uint32 sender, uint32 action, std::string code)
{
    int bind = ItemGossipBindings.GetBind(item->GetEntry(), GOSSIP_EVENT_ON_SELECT);
    if (bind)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        BeginCall(bind);
        Push(L, GOSSIP_EVENT_ON_SELECT);
        Push(L, pPlayer);
        Push(L, item);
        Push(L, sender);
        Push(L, action);
        if (code.empty())
            Push(L);
        else
            Push(L, code);
        ExecuteCall(6, 0);
    }
}

void Eluna::HandleGossipSelectOption(Player* pPlayer, uint32 menuId, uint32 sender, uint32 action, std::string code)
{
    int bind = playerGossipBindings.GetBind(menuId, GOSSIP_EVENT_ON_SELECT);
    if (bind)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        BeginCall(bind);
        Push(L, GOSSIP_EVENT_ON_SELECT);
        Push(L, pPlayer); // receiver
        Push(L, pPlayer); // sender, just not to mess up the amount of args.
        Push(L, sender);
        Push(L, action);
        if (code.empty())
            Push(L);
        else
            Push(L, code);
        Push(L, menuId);
        ExecuteCall(7, 0);
    }
}

// item
bool Eluna::OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, Item* pTarget)
{
    int bind = ItemEventBindings.GetBind(pTarget->GetEntry(), ITEM_EVENT_ON_DUMMY_EFFECT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, ITEM_EVENT_ON_DUMMY_EFFECT);
    Push(L, pCaster);
    Push(L, spellId);
    Push(L, effIndex);
    Push(L, pTarget);
    ExecuteCall(5, 0);
    return true;
}

bool Eluna::OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest)
{
    int bind = ItemEventBindings.GetBind(pItem->GetEntry(), ITEM_EVENT_ON_QUEST_ACCEPT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, ITEM_EVENT_ON_QUEST_ACCEPT);
    Push(L, pPlayer);
    Push(L, pItem);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

bool Eluna::OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    int bind1 = ItemGossipBindings.GetBind(pItem->GetEntry(), GOSSIP_EVENT_ON_HELLO);
    int bind2 = ItemEventBindings.GetBind(pItem->GetEntry(), ITEM_EVENT_ON_USE);
    if (!bind1 && !bind2)
        return false;
    if (bind1)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        BeginCall(bind1);
        Push(L, GOSSIP_EVENT_ON_HELLO);
        Push(L, pPlayer);
        Push(L, pItem);
        ExecuteCall(3, 0);
    }
    if (bind2)
    {
        BeginCall(bind2);
        Push(L, ITEM_EVENT_ON_USE);
        Push(L, pPlayer);
        Push(L, pItem);
#ifdef MANGOS
        if (GameObject* target = targets.getGOTarget())
            Push(L, target);
        else if (Item* target = targets.getItemTarget())
            Push(L, target);
        else if (Corpse* target = pPlayer->GetMap()->GetCorpse(targets.getCorpseTargetGuid()))
            Push(L, target);
        else if (Unit* target = targets.getUnitTarget())
            Push(L, target);
        else
            Push(L);
#else
        if (GameObject* target = targets.GetGOTarget())
            Push(L, target);
        else if (Item* target = targets.GetItemTarget())
            Push(L, target);
        else if (Corpse* target = targets.GetCorpseTarget())
            Push(L, target);
        else if (Unit* target = targets.GetUnitTarget())
            Push(L, target);
        else if (WorldObject* target = targets.GetObjectTarget())
            Push(L, target);
        else
            Push(L);
#endif
        ExecuteCall(4, 0);
    }
    // pPlayer->SendEquipError((InventoryResult)83, pItem, NULL);
    return true;
}

bool Eluna::OnExpire(Player* pPlayer, ItemTemplate const* pProto)
{
    int bind = ItemEventBindings.GetBind(pProto->ItemId, ITEM_EVENT_ON_EXPIRE);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, ITEM_EVENT_ON_EXPIRE);
    Push(L, pPlayer);
    Push(L, pProto->ItemId);
    ExecuteCall(3, 0);
    return true;
}

bool Eluna::OnRemove(Player* pPlayer, Item* item)
{
    int bind = ItemEventBindings.GetBind(item->GetEntry(), ITEM_EVENT_ON_REMOVE);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, ITEM_EVENT_ON_REMOVE);
    Push(L, pPlayer);
    Push(L, item);
    ExecuteCall(3, 0);
    return true;
}

// Player
bool Eluna::OnCommand(Player* player, const char* text)
{
    std::string fullcmd(text);
    char* creload = strtok((char*)text, " ");
    char* celuna = strtok(NULL, "");
    if (creload && celuna)
    {
        std::string reload(creload);
        std::string eluna(celuna);
        std::transform(reload.begin(), reload.end(), reload.begin(), ::tolower);
        if (reload == "reload")
        {
            std::transform(eluna.begin(), eluna.end(), eluna.begin(), ::tolower);
            if (std::string("eluna").find(eluna) == 0)
            {
                ReloadLuaStates();
                sWorld->SendServerMessage(SERVER_MSG_STRING, "Eluna reloaded.");
                return false;
            }
        }
    }
    bool result = true;
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_COMMAND))
        return result;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_COMMAND);
    Push(L, player);
    Push(L, fullcmd);
    ServerEventBindings.ExecuteCall();
    for (int i = 1; i <= lua_gettop(L); ++i)
    {
        if (lua_isnoneornil(L, i))
            continue;
        result = CHECKVAL<bool>(L, i, result);
    }
    ServerEventBindings.EndCall();
    return result;
}

void Eluna::OnLootItem(Player* pPlayer, Item* pItem, uint32 count, uint64 guid)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_LOOT_ITEM))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_LOOT_ITEM);
    Push(L, pPlayer);
    Push(L, pItem);
    Push(L, count);
    Push(L, guid);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnLootMoney(Player* pPlayer, uint32 amount)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_LOOT_MONEY))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_LOOT_MONEY);
    Push(L, pPlayer);
    Push(L, amount);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnFirstLogin(Player* pPlayer)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_FIRST_LOGIN))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_FIRST_LOGIN);
    Push(L, pPlayer);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnRepop(Player* pPlayer)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_REPOP))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_REPOP);
    Push(L, pPlayer);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnResurrect(Player* pPlayer)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_RESURRECT))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_RESURRECT);
    Push(L, pPlayer);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnQuestAbandon(Player* pPlayer, uint32 questId)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_QUEST_ABANDON))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_QUEST_ABANDON);
    Push(L, pPlayer);
    Push(L, questId);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnEquip(Player* pPlayer, Item* pItem, uint8 bag, uint8 slot)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_EQUIP))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_EQUIP);
    Push(L, pPlayer);
    Push(L, pItem);
    Push(L, bag);
    Push(L, slot);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

InventoryResult Eluna::OnCanUseItem(const Player* pPlayer, uint32 itemEntry)
{
    InventoryResult result = EQUIP_ERR_OK;
    if (PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_CAN_USE_ITEM))
    {
        PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_CAN_USE_ITEM);
        Push(L, pPlayer);
        Push(L, itemEntry);
        PlayerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            uint32 res = lua_tounsigned(L, i);
            if (res != EQUIP_ERR_OK)
                result = (InventoryResult)res;
        }
        PlayerEventBindings.EndCall();
    }
    return result;
}
void Eluna::OnPlayerEnterCombat(Player* pPlayer, Unit* pEnemy)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_ENTER_COMBAT))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_ENTER_COMBAT);
    Push(L, pPlayer);
    Push(L, pEnemy);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnPlayerLeaveCombat(Player* pPlayer)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_LEAVE_COMBAT))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_LEAVE_COMBAT);
    Push(L, pPlayer);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnPVPKill(Player* pKiller, Player* pKilled)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_KILL_PLAYER))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_KILL_PLAYER);
    Push(L, pKiller);
    Push(L, pKilled);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnCreatureKill(Player* pKiller, Creature* pKilled)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_KILL_CREATURE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_KILL_CREATURE);
    Push(L, pKiller);
    Push(L, pKilled);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnPlayerKilledByCreature(Creature* pKiller, Player* pKilled)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_KILLED_BY_CREATURE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_KILLED_BY_CREATURE);
    Push(L, pKiller);
    Push(L, pKilled);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnLevelChanged(Player* pPlayer, uint8 oldLevel)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_LEVEL_CHANGE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_LEVEL_CHANGE);
    Push(L, pPlayer);
    Push(L, oldLevel);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnFreeTalentPointsChanged(Player* pPlayer, uint32 newPoints)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_TALENTS_CHANGE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_TALENTS_CHANGE);
    Push(L, pPlayer);
    Push(L, newPoints);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnTalentsReset(Player* pPlayer, bool noCost)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_TALENTS_RESET))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_TALENTS_RESET);
    Push(L, pPlayer);
    Push(L, noCost);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnMoneyChanged(Player* pPlayer, int32& amount)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_MONEY_CHANGE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_MONEY_CHANGE);
    Push(L, pPlayer);
    Push(L, amount);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnGiveXP(Player* pPlayer, uint32& amount, Unit* pVictim)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_GIVE_XP))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_GIVE_XP);
    Push(L, pPlayer);
    Push(L, amount);
    Push(L, pVictim);
    PlayerEventBindings.ExecuteCall();
    for (int i = 1; i <= lua_gettop(L); ++i)
    {
        if (lua_isnoneornil(L, i))
            continue;
        amount = CHECKVAL<uint32>(L, i, 0);
    }
    PlayerEventBindings.EndCall();
}

void Eluna::OnReputationChange(Player* pPlayer, uint32 factionID, int32& standing, bool incremental)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_REPUTATION_CHANGE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_REPUTATION_CHANGE);
    Push(L, pPlayer);
    Push(L, factionID);
    Push(L, standing);
    Push(L, incremental);
    PlayerEventBindings.ExecuteCall();
    for (int i = 1; i <= lua_gettop(L); ++i)
    {
        if (lua_isnoneornil(L, i))
            continue;
        standing = CHECKVAL<uint32>(L, i, 0);
    }
    PlayerEventBindings.EndCall();
}

void Eluna::OnDuelRequest(Player* pTarget, Player* pChallenger)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_DUEL_REQUEST))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_DUEL_REQUEST);
    Push(L, pTarget);
    Push(L, pChallenger);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnDuelStart(Player* pStarter, Player* pChallenger)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_DUEL_START))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_DUEL_START);
    Push(L, pStarter);
    Push(L, pChallenger);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnDuelEnd(Player* pWinner, Player* pLoser, DuelCompleteType type)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_DUEL_END))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_DUEL_END);
    Push(L, pWinner);
    Push(L, pLoser);
    Push(L, type);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnEmote(Player* pPlayer, uint32 emote)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_EMOTE))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_EMOTE);
    Push(L, pPlayer);
    Push(L, emote);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnTextEmote(Player* pPlayer, uint32 textEmote, uint32 emoteNum, uint64 guid)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_TEXT_EMOTE))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_TEXT_EMOTE);
    Push(L, pPlayer);
    Push(L, textEmote);
    Push(L, emoteNum);
    Push(L, guid);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnSpellCast(Player* pPlayer, Spell* pSpell, bool skipCheck)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_SPELL_CAST))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_SPELL_CAST);
    Push(L, pPlayer);
    Push(L, pSpell);
    Push(L, skipCheck);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnLogin(Player* pPlayer)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_LOGIN))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_LOGIN);
    Push(L, pPlayer);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnLogout(Player* pPlayer)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_LOGOUT))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_LOGOUT);
    Push(L, pPlayer);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnCreate(Player* pPlayer)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_CHARACTER_CREATE))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_CHARACTER_CREATE);
    Push(L, pPlayer);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnDelete(uint32 guidlow)
{
    if (!ServerEventBindings.HasEvents(PLAYER_EVENT_ON_CHARACTER_DELETE))
        return;
    ServerEventBindings.BeginCall(PLAYER_EVENT_ON_CHARACTER_DELETE);
    Push(L, guidlow);
    ServerEventBindings.ExecuteCall();
    ServerEventBindings.EndCall();
}

void Eluna::OnSave(Player* pPlayer)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_SAVE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_SAVE);
    Push(L, pPlayer);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnBindToInstance(Player* pPlayer, Difficulty difficulty, uint32 mapid, bool permanent)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_BIND_TO_INSTANCE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_BIND_TO_INSTANCE);
    Push(L, pPlayer);
    Push(L, difficulty);
    Push(L, mapid);
    Push(L, permanent);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

void Eluna::OnUpdateZone(Player* pPlayer, uint32 newZone, uint32 newArea)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_UPDATE_ZONE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_UPDATE_ZONE);
    Push(L, pPlayer);
    Push(L, newZone);
    Push(L, newArea);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}

bool Eluna::OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg)
{
    if (lang == LANG_ADDON && OnAddonMessage(pPlayer, type, msg, NULL, NULL, NULL, NULL))
        return true;
    bool result = true;
    if (ServerEventBindings.HasEvents(PLAYER_EVENT_ON_CHAT))
    {
        ServerEventBindings.BeginCall(PLAYER_EVENT_ON_CHAT);
        Push(L, pPlayer);
        Push(L, msg);
        Push(L, type);
        Push(L, lang);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (const char* c_str = CHECKVAL<const char*>(L, i, NULL))
                msg = std::string(c_str);
            else if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    return result;
}

bool Eluna::OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Group* pGroup)
{
    if (lang == LANG_ADDON && OnAddonMessage(pPlayer, type, msg, NULL, NULL, pGroup, NULL))
        return true;
    bool result = true;
    if (ServerEventBindings.HasEvents(PLAYER_EVENT_ON_GROUP_CHAT))
    {
        ServerEventBindings.BeginCall(PLAYER_EVENT_ON_GROUP_CHAT);
        Push(L, pPlayer);
        Push(L, msg);
        Push(L, type);
        Push(L, lang);
        Push(L, pGroup);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (const char* c_str = CHECKVAL<const char*>(L, i, NULL))
                msg = std::string(c_str);
            else if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    return result;
}

bool Eluna::OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Guild* pGuild)
{
    if (lang == LANG_ADDON && OnAddonMessage(pPlayer, type, msg, NULL, pGuild, NULL, NULL))
        return true;
    bool result = true;
    if (ServerEventBindings.HasEvents(PLAYER_EVENT_ON_GUILD_CHAT))
    {
        ServerEventBindings.BeginCall(PLAYER_EVENT_ON_GUILD_CHAT);
        Push(L, pPlayer);
        Push(L, msg);
        Push(L, type);
        Push(L, lang);
        Push(L, pGuild);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (const char* c_str = CHECKVAL<const char*>(L, i, NULL))
                msg = std::string(c_str);
            else if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    return result;
}

bool Eluna::OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Channel* pChannel)
{
    if (lang == LANG_ADDON && OnAddonMessage(pPlayer, type, msg, NULL, NULL, NULL, pChannel))
        return true;
    bool result = true;
    if (ServerEventBindings.HasEvents(PLAYER_EVENT_ON_CHANNEL_CHAT))
    {
        ServerEventBindings.BeginCall(PLAYER_EVENT_ON_CHANNEL_CHAT);
        Push(L, pPlayer);
        Push(L, msg);
        Push(L, type);
        Push(L, lang);
        Push(L, pChannel->GetChannelId());
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (const char* c_str = CHECKVAL<const char*>(L, i, NULL))
                msg = std::string(c_str);
            else if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    return result;
}

bool Eluna::OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Player* pReceiver)
{
    if (lang == LANG_ADDON && OnAddonMessage(pPlayer, type, msg, pReceiver, NULL, NULL, NULL))
        return true;
    bool result = true;
    if (ServerEventBindings.HasEvents(PLAYER_EVENT_ON_WHISPER))
    {
        ServerEventBindings.BeginCall(PLAYER_EVENT_ON_WHISPER);
        Push(L, pPlayer);
        Push(L, msg);
        Push(L, type);
        Push(L, lang);
        Push(L, pReceiver);
        ServerEventBindings.ExecuteCall();
        for (int i = 1; i <= lua_gettop(L); ++i)
        {
            if (lua_isnoneornil(L, i))
                continue;
            if (const char* c_str = CHECKVAL<const char*>(L, i, NULL))
                msg = std::string(c_str);
            else if (!CHECKVAL<bool>(L, i, true))
            {
                result = false;
                break;
            }
        }
        ServerEventBindings.EndCall();
    }
    return result;
}

#ifndef CLASSIC
#ifndef TBC
// Vehicle
void Eluna::OnInstall(Vehicle* vehicle)
{
    if (!MapEventBindings.HasEvents(VEHICLE_EVENT_ON_INSTALL))
        return;
    MapEventBindings.BeginCall(VEHICLE_EVENT_ON_INSTALL);
    Push(L, vehicle);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}

void Eluna::OnUninstall(Vehicle* vehicle)
{
    if (!MapEventBindings.HasEvents(VEHICLE_EVENT_ON_UNINSTALL))
        return;
    MapEventBindings.BeginCall(VEHICLE_EVENT_ON_UNINSTALL);
    Push(L, vehicle);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}

void Eluna::OnInstallAccessory(Vehicle* vehicle, Creature* accessory)
{
    if (!MapEventBindings.HasEvents(VEHICLE_EVENT_ON_INSTALL_ACCESSORY))
        return;
    MapEventBindings.BeginCall(VEHICLE_EVENT_ON_INSTALL_ACCESSORY);
    Push(L, vehicle);
    Push(L, accessory);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}

void Eluna::OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 seatId)
{
    if (!MapEventBindings.HasEvents(VEHICLE_EVENT_ON_ADD_PASSENGER))
        return;
    MapEventBindings.BeginCall(VEHICLE_EVENT_ON_ADD_PASSENGER);
    Push(L, vehicle);
    Push(L, passenger);
    Push(L, seatId);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}

void Eluna::OnRemovePassenger(Vehicle* vehicle, Unit* passenger)
{
    if (!MapEventBindings.HasEvents(VEHICLE_EVENT_ON_REMOVE_PASSENGER))
        return;
    MapEventBindings.BeginCall(VEHICLE_EVENT_ON_REMOVE_PASSENGER);
    Push(L, vehicle);
    Push(L, passenger);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}
#endif
#endif

void Eluna::OnAddMember(Guild* guild, Player* player, uint32 plRank)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_ADD_MEMBER))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_ADD_MEMBER);
    Push(L, guild);
    Push(L, player);
    Push(L, plRank);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnRemoveMember(Guild* guild, Player* player, bool isDisbanding)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_REMOVE_MEMBER))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_REMOVE_MEMBER);
    Push(L, guild);
    Push(L, player);
    Push(L, isDisbanding);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnMOTDChanged(Guild* guild, const std::string& newMotd)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_MOTD_CHANGE))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_MOTD_CHANGE);
    Push(L, guild);
    Push(L, newMotd);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnInfoChanged(Guild* guild, const std::string& newInfo)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_INFO_CHANGE))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_INFO_CHANGE);
    Push(L, guild);
    Push(L, newInfo);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnCreate(Guild* guild, Player* leader, const std::string& name)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_CREATE))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_CREATE);
    Push(L, guild);
    Push(L, leader);
    Push(L, name);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnDisband(Guild* guild)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_DISBAND))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_DISBAND);
    Push(L, guild);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnMemberWitdrawMoney(Guild* guild, Player* player, uint32 &amount, bool isRepair) // isRepair not a part of Mangos, implement?
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_MONEY_WITHDRAW))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_MONEY_WITHDRAW);
    Push(L, guild);
    Push(L, player);
    Push(L, amount);
    Push(L, isRepair); // isRepair not a part of Mangos, implement?
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnMemberDepositMoney(Guild* guild, Player* player, uint32 &amount)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_MONEY_DEPOSIT))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_MONEY_DEPOSIT);
    Push(L, guild);
    Push(L, player);
    Push(L, amount);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId,
    bool isDestBank, uint8 destContainer, uint8 destSlotId)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_ITEM_MOVE))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_ITEM_MOVE);
    Push(L, guild);
    Push(L, player);
    Push(L, pItem);
    Push(L, isSrcBank);
    Push(L, srcContainer);
    Push(L, srcSlotId);
    Push(L, isDestBank);
    Push(L, destContainer);
    Push(L, destSlotId);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnEvent(Guild* guild, uint8 eventType, uint32 playerGuid1, uint32 playerGuid2, uint8 newRank)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_EVENT))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_EVENT);
    Push(L, guild);
    Push(L, eventType);
    Push(L, playerGuid1);
    Push(L, playerGuid2);
    Push(L, newRank);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}

void Eluna::OnBankEvent(Guild* guild, uint8 eventType, uint8 tabId, uint32 playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId)
{
    if (!GuildEventBindings.HasEvents(GUILD_EVENT_ON_BANK_EVENT))
        return;
    GuildEventBindings.BeginCall(GUILD_EVENT_ON_BANK_EVENT);
    Push(L, guild);
    Push(L, eventType);
    Push(L, tabId);
    Push(L, playerGuid);
    Push(L, itemOrMoney);
    Push(L, itemStackCount);
    Push(L, destTabId);
    GuildEventBindings.ExecuteCall();
    GuildEventBindings.EndCall();
}
// Group
void Eluna::OnAddMember(Group* group, uint64 guid)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_MEMBER_ADD))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_MEMBER_ADD);
    Push(L, group);
    Push(L, guid);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

void Eluna::OnInviteMember(Group* group, uint64 guid)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_MEMBER_INVITE))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_MEMBER_INVITE);
    Push(L, group);
    Push(L, guid);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

void Eluna::OnRemoveMember(Group* group, uint64 guid, uint8 method)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_MEMBER_REMOVE))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_MEMBER_REMOVE);
    Push(L, group);
    Push(L, guid);
    Push(L, method);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

void Eluna::OnChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_LEADER_CHANGE))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_LEADER_CHANGE);
    Push(L, group);
    Push(L, newLeaderGuid);
    Push(L, oldLeaderGuid);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

void Eluna::OnDisband(Group* group)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_DISBAND))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_DISBAND);
    Push(L, group);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

void Eluna::OnCreate(Group* group, uint64 leaderGuid, GroupType groupType)
{
    if (!GroupEventBindings.HasEvents(GROUP_EVENT_ON_CREATE))
        return;
    GroupEventBindings.BeginCall(GROUP_EVENT_ON_CREATE);
    Push(L, group);
    Push(L, leaderGuid);
    Push(L, groupType);
    GroupEventBindings.ExecuteCall();
    GroupEventBindings.EndCall();
}

/* Map */
void Eluna::OnCreate()
{
    if (!MapEventBindings.HasEvents(MAP_EVENT_ON_CREATE))
        return;
    MapEventBindings.BeginCall(MAP_EVENT_ON_CREATE);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}
void Eluna::OnDestroy()
{
    if (!MapEventBindings.HasEvents(MAP_EVENT_ON_DESTROY))
        return;
    MapEventBindings.BeginCall(MAP_EVENT_ON_DESTROY);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}
void Eluna::OnPlayerEnter(Map* map, Player* player)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_MAP_ENTER))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_MAP_ENTER);
    Push(L, player);
    Push(L, map);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();
}
void Eluna::OnPlayerLeave(Map* map, Player* player)
{
    if (!PlayerEventBindings.HasEvents(PLAYER_EVENT_ON_MAP_LEAVE))
        return;
    PlayerEventBindings.BeginCall(PLAYER_EVENT_ON_MAP_LEAVE);
    Push(L, player);
    Push(L, map);
    PlayerEventBindings.ExecuteCall();
    PlayerEventBindings.EndCall();

    // Player's timed events must not trigger from another map in any way
    m_EventMgr.RemoveEvents(&player->m_Events);
}
void Eluna::OnUpdate(uint32 diff)
{
    m_EventMgr.Update(diff);
    if (!MapEventBindings.HasEvents(MAP_EVENT_ON_UPDATE))
        return;
    MapEventBindings.BeginCall(MAP_EVENT_ON_UPDATE);
    Push(L, diff);
    MapEventBindings.ExecuteCall();
    MapEventBindings.EndCall();
}

// creature
bool Eluna::OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, Creature* pTarget)
{
    int bind = CreatureEventBindings.GetBind(pTarget->GetEntry(), CREATURE_EVENT_ON_DUMMY_EFFECT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_DUMMY_EFFECT);
    Push(L, pCaster);
    Push(L, spellId);
    Push(L, effIndex);
    Push(L, pTarget);
    ExecuteCall(5, 0);
    return true;
}

bool Eluna::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    int bind = CreatureGossipBindings.GetBind(pCreature->GetEntry(), GOSSIP_EVENT_ON_HELLO);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_HELLO);
    Push(L, pPlayer);
    Push(L, pCreature);
    ExecuteCall(3, 0);
    return true;
}

bool Eluna::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    int bind = CreatureGossipBindings.GetBind(pCreature->GetEntry(), GOSSIP_EVENT_ON_SELECT);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_SELECT);
    Push(L, pPlayer);
    Push(L, pCreature);
    Push(L, sender);
    Push(L, action);
    ExecuteCall(5, 0);
    return true;
}

bool Eluna::OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, const char* code)
{
    int bind = CreatureGossipBindings.GetBind(pCreature->GetEntry(), GOSSIP_EVENT_ON_SELECT);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_SELECT);
    Push(L, pPlayer);
    Push(L, pCreature);
    Push(L, sender);
    Push(L, action);
    Push(L, code);
    ExecuteCall(6, 0);
    return true;
}

bool Eluna::OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    int bind = CreatureEventBindings.GetBind(pCreature->GetEntry(), CREATURE_EVENT_ON_QUEST_ACCEPT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_QUEST_ACCEPT);
    Push(L, pPlayer);
    Push(L, pCreature);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

bool Eluna::OnQuestComplete(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    int bind = CreatureEventBindings.GetBind(pCreature->GetEntry(), CREATURE_EVENT_ON_QUEST_COMPLETE);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_QUEST_COMPLETE);
    Push(L, pPlayer);
    Push(L, pCreature);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

bool Eluna::OnQuestReward(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    int bind = CreatureEventBindings.GetBind(pCreature->GetEntry(), CREATURE_EVENT_ON_QUEST_REWARD);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_QUEST_REWARD);
    Push(L, pPlayer);
    Push(L, pCreature);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

uint32 Eluna::GetDialogStatus(Player* pPlayer, Creature* pCreature)
{
    int bind = CreatureEventBindings.GetBind(pCreature->GetEntry(), CREATURE_EVENT_ON_DIALOG_STATUS);
    if (!bind)
        return 0;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_DIALOG_STATUS);
    Push(L, pPlayer);
    Push(L, pCreature);
    ExecuteCall(3, 0);
    return DIALOG_STATUS_SCRIPTED_NO_STATUS;
}

void Eluna::OnSummoned(Creature* pCreature, Unit* pSummoner)
{
    int bind = CreatureEventBindings.GetBind(pCreature->GetEntry(), CREATURE_EVENT_ON_SUMMONED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_SUMMONED);
    Push(L, pCreature);
    Push(L, pSummoner);
    ExecuteCall(3, 0);
}

struct ElunaCreatureAI : ScriptedAI
{
#ifdef MANGOS
#define me  m_creature
#endif

    ElunaCreatureAI(Creature* creature): ScriptedAI(creature)
    {
        me->GetMap()->GetEluna()->JustRespawned(me);
    }
    ~ElunaCreatureAI() {}

    //Called at World update tick
#ifdef MANGOS
    void UpdateAI(const uint32 diff) OVERRIDE
#else
    void UpdateAI(uint32 diff) OVERRIDE
#endif
    {
        ScriptedAI::UpdateAI(diff);
        me->GetMap()->GetEluna()->UpdateAI(me, diff);
    }

        //Called for reaction at enter to combat if not in combat yet (enemy can be NULL)
        //Called at creature aggro either by MoveInLOS or Attack Start
        void EnterCombat(Unit* target) OVERRIDE
    {
        ScriptedAI::EnterCombat(target);
        me->GetMap()->GetEluna()->EnterCombat(me, target);
    }

        // Called at any Damage from any attacker (before damage apply)
        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        ScriptedAI::DamageTaken(attacker, damage);
        me->GetMap()->GetEluna()->DamageTaken(me, attacker, damage);
    }

        //Called at creature death
        void JustDied(Unit* killer) OVERRIDE
    {
        ScriptedAI::JustDied(killer);
        me->GetMap()->GetEluna()->JustDied(me, killer);
    }

        //Called at creature killing another unit
        void KilledUnit(Unit* victim) OVERRIDE
    {
        ScriptedAI::KilledUnit(victim);
        me->GetMap()->GetEluna()->KilledUnit(me, victim);
    }

        // Called when the creature summon successfully other creature
        void JustSummoned(Creature* summon) OVERRIDE
    {
        ScriptedAI::JustSummoned(summon);
        me->GetMap()->GetEluna()->JustSummoned(me, summon);
    }

        // Called when a summoned creature is despawned
        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
    {
        ScriptedAI::SummonedCreatureDespawn(summon);
        me->GetMap()->GetEluna()->SummonedCreatureDespawn(me, summon);
    }

        //Called at waypoint reached or PointMovement end
        void MovementInform(uint32 type, uint32 id) OVERRIDE
    {
        ScriptedAI::MovementInform(type, id);
        me->GetMap()->GetEluna()->MovementInform(me, type, id);
    }

        // Called before EnterCombat even before the creature is in combat.
        void AttackStart(Unit* target) OVERRIDE
    {
        ScriptedAI::AttackStart(target);
        me->GetMap()->GetEluna()->AttackStart(me, target);
    }

        // Called for reaction at stopping attack at no attackers or targets
        void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        me->GetMap()->GetEluna()->EnterEvadeMode(me);
    }

        // Called when the creature is target of hostile action: swing, hostile spell landed, fear/etc)
        void AttackedBy(Unit* attacker) OVERRIDE
    {
        ScriptedAI::AttackedBy(attacker);
        me->GetMap()->GetEluna()->AttackedBy(me, attacker);
    }

        // Called when creature is spawned or respawned (for reseting variables)
        void JustRespawned() OVERRIDE
    {
        ScriptedAI::JustRespawned();
        me->GetMap()->GetEluna()->JustRespawned(me);
    }

        // Called at reaching home after evade
        void JustReachedHome() OVERRIDE
    {
        ScriptedAI::JustReachedHome();
        me->GetMap()->GetEluna()->JustReachedHome(me);
    }

        // Called at text emote receive from player
        void ReceiveEmote(Player* player, uint32 emoteId) OVERRIDE
    {
        ScriptedAI::ReceiveEmote(player, emoteId);
        me->GetMap()->GetEluna()->ReceiveEmote(me, player, emoteId);
    }

        // called when the corpse of this creature gets removed
        void CorpseRemoved(uint32& respawnDelay) OVERRIDE
    {
        ScriptedAI::CorpseRemoved(respawnDelay);
        me->GetMap()->GetEluna()->CorpseRemoved(me, respawnDelay);
    }

#ifdef MANGOS
        // Enables use of MoveInLineOfSight
        bool IsVisible(Unit* who) const OVERRIDE
    {
        return true;
    }
#endif

        void MoveInLineOfSight(Unit* who) OVERRIDE
    {
        ScriptedAI::MoveInLineOfSight(who);
        me->GetMap()->GetEluna()->MoveInLineOfSight(me, who);
    }

        // Called when hit by a spell
        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
    {
        ScriptedAI::SpellHit(caster, spell);
        me->GetMap()->GetEluna()->SpellHit(me, caster, spell);
    }

        // Called when spell hits a target
        void SpellHitTarget(Unit* target, SpellInfo const* spell) OVERRIDE
    {
        ScriptedAI::SpellHitTarget(target, spell);
        me->GetMap()->GetEluna()->SpellHitTarget(me, target, spell);
    }

#ifndef MANGOS

        // Called when the creature is summoned successfully by other creature
        void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        ScriptedAI::IsSummonedBy(summoner);
        me->GetMap()->GetEluna()->IsSummonedBy(me, summoner);
    }

        void SummonedCreatureDies(Creature* summon, Unit* killer) OVERRIDE
    {
        ScriptedAI::SummonedCreatureDies(summon, killer);
        me->GetMap()->GetEluna()->SummonedCreatureDies(me, summon, killer);
    }

        // Called when owner takes damage
        void OwnerAttackedBy(Unit* attacker) OVERRIDE
    {
        ScriptedAI::OwnerAttackedBy(attacker);
        me->GetMap()->GetEluna()->OwnerAttackedBy(me, attacker);
    }

        // Called when owner attacks something
        void OwnerAttacked(Unit* target) OVERRIDE
    {
        ScriptedAI::OwnerAttacked(target);
        me->GetMap()->GetEluna()->OwnerAttacked(me, target);
    }
#endif

#ifdef MANGOS
#undef me
#endif
};



//Called at World update tick
#ifdef MANGOS
void Eluna::UpdateAI(Creature* me, const uint32 diff)
#else
void Eluna::UpdateAI(Creature* me, uint32 diff)
#endif
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_AIUPDATE);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_AIUPDATE);
    Push(L, me);
    Push(L, diff);
    ExecuteCall(3, 0);
}

//Called for reaction at enter to combat if not in combat yet (enemy can be NULL)
//Called at creature aggro either by MoveInLOS or Attack Start
void Eluna::EnterCombat(Creature* me, Unit* target)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_ENTER_COMBAT);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_ENTER_COMBAT);
    Push(L, me);
    Push(L, target);
    ExecuteCall(3, 0);
}

// Called at any Damage from any attacker (before damage apply)
void Eluna::DamageTaken(Creature* me, Unit* attacker, uint32& damage)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_DAMAGE_TAKEN);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_DAMAGE_TAKEN);
    Push(L, me);
    Push(L, attacker);
    Push(L, damage);
    ExecuteCall(4, 0);
}

//Called at creature death
void Eluna::JustDied(Creature* me, Unit* killer)
{
    On_Reset(me);
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_DIED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_DIED);
    Push(L, me);
    Push(L, killer);
    ExecuteCall(3, 0);
}

//Called at creature killing another unit
void Eluna::KilledUnit(Creature* me, Unit* victim)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_TARGET_DIED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_TARGET_DIED);
    Push(L, me);
    Push(L, victim);
    ExecuteCall(3, 0);
}

// Called when the creature summon successfully other creature
void Eluna::JustSummoned(Creature* me, Creature* summon)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_JUST_SUMMONED_CREATURE);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_JUST_SUMMONED_CREATURE);
    Push(L, me);
    Push(L, summon);
    ExecuteCall(3, 0);
}

// Called when a summoned creature is despawned
void Eluna::SummonedCreatureDespawn(Creature* me, Creature* summon)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_SUMMONED_CREATURE_DESPAWN);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_SUMMONED_CREATURE_DESPAWN);
    Push(L, me);
    Push(L, summon);
    ExecuteCall(3, 0);
}

//Called at waypoint reached or PointMovement end
void Eluna::MovementInform(Creature* me, uint32 type, uint32 id)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_REACH_WP);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_REACH_WP);
    Push(L, me);
    Push(L, type);
    Push(L, id);
    ExecuteCall(4, 0);
}

// Called before EnterCombat even before the creature is in combat.
void Eluna::AttackStart(Creature* me, Unit* target)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_PRE_COMBAT);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_PRE_COMBAT);
    Push(L, me);
    Push(L, target);
    ExecuteCall(3, 0);
}

// Called for reaction at stopping attack at no attackers or targets
void Eluna::EnterEvadeMode(Creature* me)
{
    On_Reset(me);
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_LEAVE_COMBAT);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_LEAVE_COMBAT);
    Push(L, me);
    ExecuteCall(2, 0);
}

// Called when the creature is target of hostile action: swing, hostile spell landed, fear/etc)
void Eluna::AttackedBy(Creature* me, Unit* attacker)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_ATTACKED_AT);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_ATTACKED_AT);
    Push(L, me);
    Push(L, attacker);
    ExecuteCall(3, 0);
}

// Called when creature is spawned or respawned (for reseting variables)
void Eluna::JustRespawned(Creature* me)
{
    On_Reset(me);
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_SPAWN);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_SPAWN);
    Push(L, me);
    ExecuteCall(2, 0);
}

// Called at reaching home after evade
void Eluna::JustReachedHome(Creature* me)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_REACH_HOME);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_REACH_HOME);
    Push(L, me);
    ExecuteCall(2, 0);
}

// Called at text emote receive from player
void Eluna::ReceiveEmote(Creature* me, Player* player, uint32 emoteId)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_RECEIVE_EMOTE);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_RECEIVE_EMOTE);
    Push(L, me);
    Push(L, player);
    Push(L, emoteId);
    ExecuteCall(4, 0);
}

// called when the corpse of this creature gets removed
void Eluna::CorpseRemoved(Creature* me, uint32& respawnDelay)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_CORPSE_REMOVED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_CORPSE_REMOVED);
    Push(L, me);
    Push(L, respawnDelay);
    ExecuteCall(3, 0);
}

void Eluna::MoveInLineOfSight(Creature* me, Unit* who)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_MOVE_IN_LOS);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_MOVE_IN_LOS);
    Push(L, me);
    Push(L, who);
    ExecuteCall(3, 0);
}

// Called on creature initial spawn, respawn, death, evade (leave combat)
void Eluna::On_Reset(Creature* me) // custom
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_RESET);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_RESET);
    Push(L, me);
    ExecuteCall(2, 0);
}

// Called when hit by a spell
void Eluna::SpellHit(Creature* me, Unit* caster, SpellInfo const* spell)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_HIT_BY_SPELL);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_HIT_BY_SPELL);
    Push(L, me);
    Push(L, caster);
    Push(L, spell->Id); // Pass spell object?
    ExecuteCall(4, 0);
}

// Called when spell hits a target
void Eluna::SpellHitTarget(Creature* me, Unit* target, SpellInfo const* spell)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_SPELL_HIT_TARGET);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_SPELL_HIT_TARGET);
    Push(L, me);
    Push(L, target);
    Push(L, spell->Id); // Pass spell object?
    ExecuteCall(4, 0);
}

#ifndef MANGOS

// Called when the creature is summoned successfully by other creature
void Eluna::IsSummonedBy(Creature* me, Unit* summoner)
{
    OnSummoned(me, summoner);
}

void Eluna::SummonedCreatureDies(Creature* me, Creature* summon, Unit* killer)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_SUMMONED_CREATURE_DIED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_SUMMONED_CREATURE_DIED);
    Push(L, me);
    Push(L, summon);
    Push(L, killer);
    ExecuteCall(4, 0);
}

// Called when owner takes damage
void Eluna::OwnerAttackedBy(Creature* me, Unit* attacker)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_OWNER_ATTACKED_AT);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_OWNER_ATTACKED_AT);
    Push(L, me);
    Push(L, attacker);
    ExecuteCall(3, 0);
}

// Called when owner attacks something
void Eluna::OwnerAttacked(Creature* me, Unit* target)
{
    int bind = CreatureEventBindings.GetBind(me->GetEntry(), CREATURE_EVENT_ON_OWNER_ATTACKED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, CREATURE_EVENT_ON_OWNER_ATTACKED);
    Push(L, me);
    Push(L, target);
    ExecuteCall(3, 0);
}
#endif

// gameobject
bool Eluna::OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, GameObject* pTarget)
{
    int bind = GameObjectEventBindings.GetBind(pTarget->GetEntry(), GAMEOBJECT_EVENT_ON_DUMMY_EFFECT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_DUMMY_EFFECT);
    Push(L, pCaster);
    Push(L, spellId);
    Push(L, effIndex);
    Push(L, pTarget);
    ExecuteCall(5, 0);
    return true;
}

bool Eluna::OnGossipHello(Player* pPlayer, GameObject* pGameObject)
{
    int bind = GameObjectGossipBindings.GetBind(pGameObject->GetEntry(), GOSSIP_EVENT_ON_HELLO);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_HELLO);
    Push(L, pPlayer);
    Push(L, pGameObject);
    ExecuteCall(3, 0);
    return true;
}

bool Eluna::OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action)
{
    int bind = GameObjectGossipBindings.GetBind(pGameObject->GetEntry(), GOSSIP_EVENT_ON_SELECT);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_SELECT);
    Push(L, pPlayer);
    Push(L, pGameObject);
    Push(L, sender);
    Push(L, action);
    ExecuteCall(5, 0);
    return true;
}

bool Eluna::OnGossipSelectCode(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, const char* code)
{
    int bind = GameObjectGossipBindings.GetBind(pGameObject->GetEntry(), GOSSIP_EVENT_ON_SELECT);
    if (!bind)
        return false;
    pPlayer->PlayerTalkClass->ClearMenus();
    BeginCall(bind);
    Push(L, GOSSIP_EVENT_ON_SELECT);
    Push(L, pPlayer);
    Push(L, pGameObject);
    Push(L, sender);
    Push(L, action);
    Push(L, code);
    ExecuteCall(6, 0);
    return true;
}

bool Eluna::OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_QUEST_ACCEPT);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_QUEST_ACCEPT);
    Push(L, pPlayer);
    Push(L, pGameObject);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

bool Eluna::OnQuestComplete(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_QUEST_COMPLETE);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_QUEST_COMPLETE);
    Push(L, pPlayer);
    Push(L, pGameObject);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

void Eluna::UpdateAI(GameObject* pGameObject, uint32 diff)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_AIUPDATE);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_AIUPDATE);
    Push(L, pGameObject);
    Push(L, diff);
    ExecuteCall(3, 0);
}

bool Eluna::OnQuestReward(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_QUEST_REWARD);
    if (!bind)
        return false;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_QUEST_REWARD);
    Push(L, pPlayer);
    Push(L, pGameObject);
    Push(L, pQuest);
    ExecuteCall(4, 0);
    return true;
}

uint32 Eluna::GetDialogStatus(Player* pPlayer, GameObject* pGameObject)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_DIALOG_STATUS);
    if (!bind)
        return 0;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_DIALOG_STATUS);
    Push(L, pPlayer);
    Push(L, pGameObject);
    ExecuteCall(3, 0);
    return DIALOG_STATUS_SCRIPTED_NO_STATUS; // DIALOG_STATUS_UNDEFINED
}

#ifndef CLASSIC
#ifndef TBC
void Eluna::OnDestroyed(GameObject* pGameObject, Player* pPlayer)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_DESTROYED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_DESTROYED);
    Push(L, pGameObject);
    Push(L, pPlayer);
    ExecuteCall(3, 0);
}

void Eluna::OnDamaged(GameObject* pGameObject, Player* pPlayer)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_DAMAGED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_DAMAGED);
    Push(L, pGameObject);
    Push(L, pPlayer);
    ExecuteCall(3, 0);
}
#endif
#endif

void Eluna::OnLootStateChanged(GameObject* pGameObject, uint32 state)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_LOOT_STATE_CHANGE);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_LOOT_STATE_CHANGE);
    Push(L, pGameObject);
    Push(L, state);
    ExecuteCall(4, 0);
}

void Eluna::OnGameObjectStateChanged(GameObject* pGameObject, uint32 state)
{
    int bind = GameObjectEventBindings.GetBind(pGameObject->GetEntry(), GAMEOBJECT_EVENT_ON_GO_STATE_CHANGED);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_GO_STATE_CHANGED);
    Push(L, pGameObject);
    Push(L, state);
    ExecuteCall(3, 0);
}

CreatureAI* Eluna::GetAI(Creature* creature)
{
    if (!CreatureEventBindings.GetBindMap(creature->GetEntry()))
        return NULL;
    return new ElunaCreatureAI(creature);
}

void Eluna::OnSpawn(GameObject* gameobject)
{
    int bind = GameObjectEventBindings.GetBind(gameobject->GetEntry(), GAMEOBJECT_EVENT_ON_SPAWN);
    if (!bind)
        return;
    BeginCall(bind);
    Push(L, GAMEOBJECT_EVENT_ON_SPAWN);
    Push(L, gameobject);
    ExecuteCall(2, 0);
}
