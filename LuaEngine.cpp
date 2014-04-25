/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include <ace/Dirent.h>
#include <ace/OS_NS_sys_stat.h>
#include "LuaEngine.h"

extern void RegisterFunctions(lua_State* L);
extern void AddElunaScripts();

Eluna Eluna::World(NULL);

Eluna::Eluna(Map* _map):
map(_map),
L(luaL_newstate()),
PacketEventBindings(*this),
ServerEventBindings(*this),
PlayerEventBindings(*this),
GuildEventBindings(*this),
GroupEventBindings(*this),
VehicleEventBindings(*this),

CreatureEventBindings(*this),
CreatureGossipBindings(*this),
GameObjectEventBindings(*this),
GameObjectGossipBindings(*this),
ItemEventBindings(*this),
ItemGossipBindings(*this),
playerGossipBindings(*this)
{
    ELUNA_LOG_DEBUG("[Eluna]: Creating new lua state");
    if (ElunaMap.find(L) != ElunaMap.end())
        delete ElunaMap[L];
    ElunaMap[L] = this;
    luaL_openlibs(L);
    RegisterFunctions(L);

    ScriptPaths scripts;
    Eluna::GetScripts("lua_scripts", scripts);
    Eluna::GetScripts("lua_scripts/extensions", scripts);
    Eluna::RunScripts(scripts);

    /*
    if (restart)
    {
    //! Iterate over every supported source type (creature and gameobject)
    //! Not entirely sure how this will affect units in non-loaded grids.
    {
    HashMapHolder<Creature>::ReadGuard g(HashMapHolder<Creature>::GetLock());
    HashMapHolder<Creature>::MapType& m = HashMapHolder<Creature>::GetContainer();
    for (HashMapHolder<Creature>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
    {
    if (itr->second->IsInWorld()) // must check?
    // if(Eluna::CreatureEventBindings->GetBindMap(iter->second->GetEntry())) // update all AI or just Eluna?
    itr->second->AIM_Initialize();
    }
    }

    {
    HashMapHolder<GameObject>::ReadGuard g(HashMapHolder<GameObject>::GetLock());
    HashMapHolder<GameObject>::MapType& m = HashMapHolder<GameObject>::GetContainer();
    for (HashMapHolder<GameObject>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
    {
    if (itr->second->IsInWorld()) // must check?
    // if(Eluna::GameObjectEventBindings->GetBindMap(iter->second->GetEntry())) // update all AI or just Eluna?
    itr->second->AIM_Initialize();
    }
    }
    }
    */
}

Eluna::~Eluna()
{
    ELUNA_LOG_DEBUG("[Eluna]: Closing lua state");
    if (ElunaMap.find(L) != ElunaMap.end())
        ElunaMap.erase(L);

    // Unregisters and stops all timed events
    m_EventMgr.RemoveEvents();

    // Remove bindings
    PacketEventBindings.Clear();
    ServerEventBindings.Clear();
    PlayerEventBindings.Clear();
    GuildEventBindings.Clear();
    GroupEventBindings.Clear();

    CreatureEventBindings.Clear();
    CreatureGossipBindings.Clear();
    GameObjectEventBindings.Clear();
    GameObjectGossipBindings.Clear();
    ItemEventBindings.Clear();
    ItemGossipBindings.Clear();
    playerGossipBindings.Clear();
    VehicleEventBindings.Clear();

    lua_close(L);
}

// Start or restart eluna. Returns true if started
bool StartEluna()
{
#ifndef ELUNA
#ifndef MANGOS
    {
        ELUNA_LOG_ERROR("[Eluna]: LuaEngine is Disabled. (If you want to use it please enable in cmake)");
        return false;
    }
#endif
#endif

    //ELUNA_GUARD();
    //bool restart = false;
    //if (eluna.L)
    //{
    //    restart = true;
    //    Eluna::OnEngineRestart();

    //}
    //else
    //    AddElunaScripts();

#ifdef MANGOS
    // Check config file for eluna is enabled or disabled
    if (!sWorld->getConfig(CONFIG_BOOL_ELUNA_ENABLED))
    {
        ELUNA_LOG_ERROR("[Eluna]: LuaEngine is Disabled. (If you want to use it please set config in 'mangosd.conf')");
        return false;
    }
#endif

    return true;
}

// Finds lua script files from given path (including subdirectories) and pushes them to scripts
void Eluna::GetScripts(std::string path, ScriptPaths& scripts)
{
    ELUNA_LOG_DEBUG("Eluna::GetScripts from path `%s`", path.c_str());

    ACE_Dirent dir;
    if (dir.open(path.c_str()) == -1)
    {
        ELUNA_LOG_ERROR("[Eluna]: Error No `%s` directory found, creating it", path.c_str());
        ACE_OS::mkdir(path.c_str());
        return;
    }

    ACE_DIRENT *directory = 0;
    while (directory = dir.read())
    {
        // Skip the ".." and "." files.
        if (ACE::isdotdir(directory->d_name))
            continue;

        std::string fullpath = path + "\\" + directory->d_name;

        ACE_stat stat_buf;
        if (ACE_OS::lstat(fullpath.c_str(), &stat_buf) == -1)
            continue;

        // load subfolder
        if ((stat_buf.st_mode & S_IFMT) == (S_IFDIR))
        {
            GetScripts(fullpath, scripts);
            continue;
        }

        // was file, check extension
        ELUNA_LOG_DEBUG("[Eluna]: GetScripts Checking file `%s`", fullpath.c_str());
        std::string ext = fullpath.substr(fullpath.length() - 4, 4);
        if (ext != ".lua" && ext != ".dll")
            continue;

        // was correct, add path to scripts to load
        ELUNA_LOG_DEBUG("[Eluna]: GetScripts add path `%s`", fullpath.c_str());
        scripts.erase(fullpath);
        scripts.insert(fullpath);
    }
}

void Eluna::RunScripts(ScriptPaths& scripts)
{
    uint32 count = 0;
    // load last first to load extensions first
    for (ScriptPaths::const_reverse_iterator it = scripts.rbegin(); it != scripts.rend(); ++it)
    {
        if (!luaL_loadfile(L, it->c_str()) && !lua_pcall(L, 0, 0, 0))
        {
            // successfully loaded and ran file
            ELUNA_LOG_DEBUG("[Eluna]: Successfully loaded `%s`", it->c_str());
            ++count;
            continue;
        }
        ELUNA_LOG_ERROR("[Eluna]: Error loading file `%s`", it->c_str());
        report(L);
    }
    ELUNA_LOG_INFO("[Eluna]: Loaded %u Lua scripts", count);
}

void Eluna::report(lua_State* L)
{
    const char* msg = lua_tostring(L, -1);
    while (msg)
    {
        lua_pop(L, -1);
        ELUNA_LOG_ERROR("%s", msg);
        msg = lua_tostring(L, -1);
    }
}

void Eluna::BeginCall(int fReference)
{
    lua_settop(L, 0); // stack should be empty
    lua_rawgeti(L, LUA_REGISTRYINDEX, (fReference));
}

bool Eluna::ExecuteCall(int params, int res)
{
    bool ret = true;
    int top = lua_gettop(L);

    if (lua_type(L, top - params) == LUA_TFUNCTION) // is function
    {
        if (lua_pcall(L, params, res, 0))
        {
            report(L);
            ret = false;
        }
    }
    else
    {
        ret = false;
        if (params > 0)
        {
            for (int i = top; i >= (top - params); i--)
            {
                if (!lua_isnone(L, i))
                    lua_remove(L, i);
            }
        }
    }
    return ret;
}

void Eluna::EndCall(int res)
{
    for (int i = res; i > 0; i--)
    {
        if (!lua_isnone(L, res))
            lua_remove(L, res);
    }
}

void Eluna::Push(lua_State* L)
{
    lua_pushnil(L);
}
void Eluna::Push(lua_State* L, const uint64 l)
{
    std::ostringstream ss;
    ss << l;
    Push(L, ss.str());
}
void Eluna::Push(lua_State* L, const int64 l)
{
    std::ostringstream ss;
    ss << l;
    Push(L, ss.str());
}
void Eluna::Push(lua_State* L, const uint32 u)
{
    lua_pushunsigned(L, u);
}
void Eluna::Push(lua_State* L, const int32 i)
{
    lua_pushinteger(L, i);
}
void Eluna::Push(lua_State* L, const double d)
{
    lua_pushnumber(L, d);
}
void Eluna::Push(lua_State* L, const float f)
{
    lua_pushnumber(L, f);
}
void Eluna::Push(lua_State* L, const bool b)
{
    lua_pushboolean(L, b);
}
void Eluna::Push(lua_State* L, const std::string str)
{
    lua_pushstring(L, str.c_str());
}
void Eluna::Push(lua_State* L, const char* str)
{
    lua_pushstring(L, str);
}
void Eluna::Push(lua_State* L, Pet const* pet)
{
    Push(L, pet->ToCreature());
}
void Eluna::Push(lua_State* L, TempSummon const* summon)
{
    Push(L, summon->ToCreature());
}
void Eluna::Push(lua_State* L, Unit const* unit)
{
    if (!unit)
    {
        Push(L);
        return;
    }
    switch (unit->GetTypeId())
    {
    case TYPEID_UNIT:
        Push(L, unit->ToCreature());
        break;
    case TYPEID_PLAYER:
        Push(L, unit->ToPlayer());
        break;
    default:
        ElunaTemplate<Unit>::push(L, unit);
    }
}
void Eluna::Push(lua_State* L, WorldObject const* obj)
{
    if (!obj)
    {
        Push(L);
        return;
    }
    switch (obj->GetTypeId())
    {
    case TYPEID_UNIT:
        Push(L, obj->ToCreature());
        break;
    case TYPEID_PLAYER:
        Push(L, obj->ToPlayer());
        break;
    case TYPEID_GAMEOBJECT:
        Push(L, obj->ToGameObject());
        break;
    case TYPEID_CORPSE:
        Push(L, obj->ToCorpse());
        break;
    default:
        ElunaTemplate<WorldObject>::push(L, obj);
    }
}
void Eluna::Push(lua_State* L, Object const* obj)
{
    if (!obj)
    {
        Push(L);
        return;
    }
    switch (obj->GetTypeId())
    {
    case TYPEID_UNIT:
        Push(L, obj->ToCreature());
        break;
    case TYPEID_PLAYER:
        Push(L, obj->ToPlayer());
        break;
    case TYPEID_GAMEOBJECT:
        Push(L, obj->ToGameObject());
        break;
    case TYPEID_CORPSE:
        Push(L, obj->ToCorpse());
        break;
    default:
        ElunaTemplate<Object>::push(L, obj);
    }
}
template<> bool Eluna::CHECKVAL<bool>(lua_State* L, int narg)
{
    return lua_isnumber(L, narg) ? luaL_optnumber(L, narg, 1) ? true : false : lua_toboolean(L, narg);
}
template<> bool Eluna::CHECKVAL<bool>(lua_State* L, int narg, bool def)
{
    return lua_isnone(L, narg) ? def : lua_isnumber(L, narg) ? luaL_optnumber(L, narg, 1) ? true : false : lua_toboolean(L, narg);
}
template<> float Eluna::CHECKVAL<float>(lua_State* L, int narg)
{
    return luaL_checknumber(L, narg);
}
template<> float Eluna::CHECKVAL<float>(lua_State* L, int narg, float def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optnumber(L, narg, def);
}
template<> double Eluna::CHECKVAL<double>(lua_State* L, int narg)
{
    return luaL_checknumber(L, narg);
}
template<> double Eluna::CHECKVAL<double>(lua_State* L, int narg, double def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optnumber(L, narg, def);
}
template<> int8 Eluna::CHECKVAL<int8>(lua_State* L, int narg)
{
    return luaL_checkint(L, narg);
}
template<> int8 Eluna::CHECKVAL<int8>(lua_State* L, int narg, int8 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optint(L, narg, def);
}
template<> uint8 Eluna::CHECKVAL<uint8>(lua_State* L, int narg)
{
    return luaL_checkunsigned(L, narg);
}
template<> uint8 Eluna::CHECKVAL<uint8>(lua_State* L, int narg, uint8 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optunsigned(L, narg, def);
}
template<> int16 Eluna::CHECKVAL<int16>(lua_State* L, int narg)
{
    return luaL_checkint(L, narg);
}
template<> int16 Eluna::CHECKVAL<int16>(lua_State* L, int narg, int16 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optint(L, narg, def);
}
template<> uint16 Eluna::CHECKVAL<uint16>(lua_State* L, int narg)
{
    return luaL_checkunsigned(L, narg);
}
template<> uint16 Eluna::CHECKVAL<uint16>(lua_State* L, int narg, uint16 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optunsigned(L, narg, def);
}
template<> uint32 Eluna::CHECKVAL<uint32>(lua_State* L, int narg)
{
    return luaL_checkunsigned(L, narg);
}
template<> uint32 Eluna::CHECKVAL<uint32>(lua_State* L, int narg, uint32 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optunsigned(L, narg, def);
}
template<> int32 Eluna::CHECKVAL<int32>(lua_State* L, int narg)
{
    return luaL_checklong(L, narg);
}
template<> int32 Eluna::CHECKVAL<int32>(lua_State* L, int narg, int32 def)
{
    if (lua_isnoneornil(L, narg) || !lua_isnumber(L, narg))
        return def;
    return luaL_optlong(L, narg, def);
}
template<> const char* Eluna::CHECKVAL<const char*>(lua_State* L, int narg)
{
    return luaL_checkstring(L, narg);
}
template<> const char* Eluna::CHECKVAL<const char*>(lua_State* L, int narg, const char* def)
{
    if (lua_isnoneornil(L, narg) || !lua_isstring(L, narg))
        return def;
    return luaL_optstring(L, narg, def);
}
template<> std::string Eluna::CHECKVAL<std::string>(lua_State* L, int narg)
{
    return luaL_checkstring(L, narg);
}
template<> std::string Eluna::CHECKVAL<std::string>(lua_State* L, int narg, std::string def)
{
    if (lua_isnoneornil(L, narg) || !lua_isstring(L, narg))
        return def;
    return luaL_optstring(L, narg, def.c_str());
}
template<> uint64 Eluna::CHECKVAL<uint64>(lua_State* L, int narg)
{
    const char* c_str = CHECKVAL<const char*>(L, narg, NULL);
    if (!c_str)
        return luaL_argerror(L, narg, "uint64 (as string) expected");
    uint64 l = 0;
    sscanf(c_str, UI64FMTD, &l);
    return l;
}
template<> uint64 Eluna::CHECKVAL<uint64>(lua_State* L, int narg, uint64 def)
{
    const char* c_str = CHECKVAL<const char*>(L, narg, NULL);
    if (!c_str)
        return def;
    uint64 l = 0;
    sscanf(c_str, UI64FMTD, &l);
    return l;
}
template<> int64 Eluna::CHECKVAL<int64>(lua_State* L, int narg)
{
    const char* c_str = CHECKVAL<const char*>(L, narg, NULL);
    if (!c_str)
        return luaL_argerror(L, narg, "int64 (as string) expected");
    int64 l = 0;
    sscanf(c_str, SI64FMTD, &l);
    return l;
}
template<> int64 Eluna::CHECKVAL<int64>(lua_State* L, int narg, int64 def)
{
    const char* c_str = CHECKVAL<const char*>(L, narg, NULL);
    if (!c_str)
        return def;
    int64 l = 0;
    sscanf(c_str, SI64FMTD, &l);
    return l;
}
#define TEST_OBJ(T, O, E, F)\
{\
    if (!O || !O->F())\
{\
    if (E)\
{\
    std::string errmsg(ElunaTemplate<T>::tname);\
    errmsg += " expected";\
    luaL_argerror(L, narg, errmsg.c_str());\
}\
    return NULL;\
}\
    return O->F();\
}
template<> Unit* Eluna::CHECKOBJ<Unit>(lua_State* L, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<WorldObject>(L, narg, false);
    TEST_OBJ(Unit, obj, error, ToUnit);
}
template<> Player* Eluna::CHECKOBJ<Player>(lua_State* L, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<WorldObject>(L, narg, false);
    TEST_OBJ(Player, obj, error, ToPlayer);
}
template<> Creature* Eluna::CHECKOBJ<Creature>(lua_State* L, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<WorldObject>(L, narg, false);
    TEST_OBJ(Creature, obj, error, ToCreature);
}
template<> GameObject* Eluna::CHECKOBJ<GameObject>(lua_State* L, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<WorldObject>(L, narg, false);
    TEST_OBJ(GameObject, obj, error, ToGameObject);
}
template<> Corpse* Eluna::CHECKOBJ<Corpse>(lua_State* L, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<WorldObject>(L, narg, false);
    TEST_OBJ(Corpse, obj, error, ToCorpse);
}
#undef TEST_OBJ

// Saves the function reference ID given to the register type's store for given entry under the given event
void Eluna::Register(uint8 regtype, uint32 id, uint32 evt, int functionRef)
{
    switch (regtype)
    {
    case REGTYPE_PACKET:
        if (evt < NUM_MSG_TYPES)
        {
            PacketEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_SERVER:
        if (evt < SERVER_EVENT_COUNT)
        {
            ServerEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_PLAYER:
        if (evt < PLAYER_EVENT_COUNT)
        {
            PlayerEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_GUILD:
        if (evt < GUILD_EVENT_COUNT)
        {
            GuildEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_GROUP:
        if (evt < GROUP_EVENT_COUNT)
        {
            GroupEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_VEHICLE:
        if (evt < VEHICLE_EVENT_COUNT)
        {
            VehicleEventBindings.Insert(evt, functionRef);
            return;
        }
        break;

    case REGTYPE_CREATURE:
        if (evt < CREATURE_EVENT_COUNT)
        {
            if (!sObjectMgr->GetCreatureTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a creature with (ID: %d)!", id);
                return;
            }

            Eluna::CreatureEventBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_CREATURE_GOSSIP:
        if (evt < GOSSIP_EVENT_COUNT)
        {
            if (!sObjectMgr->GetCreatureTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a creature with (ID: %d)!", id);
                return;
            }

            Eluna::CreatureGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_GAMEOBJECT:
        if (evt < GAMEOBJECT_EVENT_COUNT)
        {
            if (!sObjectMgr->GetGameObjectTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a gameobject with (ID: %d)!", id);
                return;
            }

            Eluna::GameObjectEventBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_GAMEOBJECT_GOSSIP:
        if (evt < GOSSIP_EVENT_COUNT)
        {
            if (!sObjectMgr->GetGameObjectTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a gameobject with (ID: %d)!", id);
                return;
            }

            Eluna::GameObjectGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_ITEM:
        if (evt < ITEM_EVENT_COUNT)
        {
            if (!sObjectMgr->GetItemTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a item with (ID: %d)!", id);
                return;
            }

            Eluna::ItemEventBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_ITEM_GOSSIP:
        if (evt < GOSSIP_EVENT_COUNT)
        {
            if (!sObjectMgr->GetItemTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a item with (ID: %d)!", id);
                return;
            }

            Eluna::ItemGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_PLAYER_GOSSIP:
        if (evt < GOSSIP_EVENT_COUNT)
        {
            Eluna::playerGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;
    }
    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
    luaL_error(L, "Unknown event type (regtype %d, id %d, event %d)", regtype, id, evt);
}

void Eluna::EventBind::Clear()
{
    for (ElunaEntryMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
    {
        for (ElunaBindingMap::iterator it = itr->second.begin(); it != itr->second.end(); ++it)
            luaL_unref(eluna.L, LUA_REGISTRYINDEX, (*it));
        itr->second.clear();
    }
    Bindings.clear();
}

void Eluna::EventBind::Insert(int eventId, int funcRef)
{
    Bindings[eventId].push_back(funcRef);
}

Eluna::EventBind::ElunaBindingMap* Eluna::EventBind::GetBindMap(int eventId)
{
    if (Bindings.empty())
        return NULL;
    ElunaEntryMap::iterator itr = Bindings.find(eventId);
    if (itr == Bindings.end())
        return NULL;

    return &itr->second;
}

bool Eluna::EventBind::HasEvents(int eventId) const
{
    if (Bindings.empty())
        return false;
    if (Bindings.find(eventId) == Bindings.end())
        return false;
    return true;
}

void Eluna::EventBind::BeginCall(int eventId) const
{
    lua_settop(eluna.L, 0); // stack should be empty
    eluna.Push(eluna.L, eventId);
}

void Eluna::EventBind::ExecuteCall()
{
    int eventId = eluna.CHECKVAL<int>(eluna.L, 1);
    int params = lua_gettop(eluna.L);
    for (ElunaBindingMap::const_iterator it = Bindings[eventId].begin(); it != Bindings[eventId].end(); ++it)
    {
        lua_rawgeti(eluna.L, LUA_REGISTRYINDEX, (*it)); // Fetch function
        for (int i = 1; i <= params; ++i) // Copy original pushed params
            lua_pushvalue(eluna.L, i);
        eluna.ExecuteCall(params, LUA_MULTRET); // Do call and leave results to stack
    }
    for (int i = params; i > 0; --i) // Remove original pushed params
        if (!lua_isnone(eluna.L, i))
            lua_remove(eluna.L, i);
    // Results in stack, otherwise stack clean
}

void Eluna::EventBind::EndCall() const
{
    lua_settop(eluna.L, 0); // stack should be empty
};

void Eluna::EntryBind::Clear()
{
    for (ElunaEntryMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
    {
        for (ElunaBindingMap::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it)
            luaL_unref(eluna.L, LUA_REGISTRYINDEX, it->second);
        itr->second.clear();
    }
    Bindings.clear();
}

void Eluna::EntryBind::Insert(uint32 entryId, int eventId, int funcRef)
{
    if (Bindings[entryId][eventId])
    {
        luaL_unref(eluna.L, LUA_REGISTRYINDEX, funcRef); // free the unused ref
        luaL_error(eluna.L, "A function is already registered for entry (%d) event (%d)", entryId, eventId);
    }
    else
        Bindings[entryId][eventId] = funcRef;
}

int Eluna::EntryBind::GetBind(uint32 entryId, int eventId) const
{
    if (Bindings.empty())
        return 0;
    ElunaEntryMap::const_iterator itr = Bindings.find(entryId);
    if (itr == Bindings.end() || itr->second.empty())
        return 0;
    ElunaBindingMap::const_iterator itr2 = itr->second.find(eventId);
    if (itr2 == itr->second.end())
        return 0;
    return itr2->second;
}

// Gets the binding std::map containing all registered events with the function refs for the entry
const Eluna::EntryBind::ElunaBindingMap* Eluna::EntryBind::GetBindMap(uint32 entryId) const
{
    if (Bindings.empty())
        return NULL;
    ElunaEntryMap::const_iterator itr = Bindings.find(entryId);
    if (itr == Bindings.end())
        return NULL;

    return &itr->second;
}

// Returns true if the entry has registered binds
bool Eluna::EntryBind::HasBinds(uint32 entryId) const
{
    if (Bindings.empty())
        return false;
    return Bindings.find(entryId) != Bindings.end();
}

EventMgr::LuaEvent::LuaEvent(EventProcessor* _events, int _funcRef, uint32 _delay, uint32 _calls, Object* _obj):
events(_events), funcRef(_funcRef), delay(_delay), calls(_calls), obj(_obj)
{
    if (_events)
        Eluna::m_EventMgr.LuaEvents[_events].insert(this); // Able to access the event if we have the processor
}

EventMgr::LuaEvent::~LuaEvent()
{
    if (events)
    {
        // Attempt to remove the pointer from LuaEvents
        EventMgr::EventMap::const_iterator it = Eluna::m_EventMgr.LuaEvents.find(events); // Get event set
        if (it != Eluna::m_EventMgr.LuaEvents.end())
            Eluna::m_EventMgr.LuaEvents[events].erase(this);// Remove pointer
    }
    luaL_unref(eluna.L, LUA_REGISTRYINDEX, funcRef); // Free lua function ref
}

bool EventMgr::LuaEvent::Execute(uint64 time, uint32 diff)
{
    ELUNA_GUARD();
    bool remove = (calls == 1);
    if (!remove)
        events->AddEvent(this, events->CalculateTime(delay)); // Reschedule before calling incase RemoveEvents used
    Eluna::BeginCall(funcRef);
    Eluna::Push(L, funcRef);
    Eluna::Push(L, delay);
    Eluna::Push(L, calls);
    if (!remove && calls)
        --calls;
    Eluna::Push(L, obj);
    Eluna::ExecuteCall(4, 0);
    return remove; // Destory (true) event if not run
}

// Lua taxi helper functions
uint32 LuaTaxiMgr::nodeId = 500;
void LuaTaxiMgr::StartTaxi(Player* player, uint32 pathid)
{
    if (pathid >= sTaxiPathNodesByPath.size())
        return;

    TaxiPathNodeList const& path = sTaxiPathNodesByPath[pathid];
    if (path.size() < 2)
        return;

    std::vector<uint32> nodes;
    nodes.resize(2);
    nodes[0] = path[0].index;
    nodes[1] = path[path.size() - 1].index;

    player->ActivateTaxiPathTo(nodes);
}

uint32 LuaTaxiMgr::AddPath(std::list<TaxiPathNodeEntry> nodes, uint32 mountA, uint32 mountH, uint32 price, uint32 pathId)
{
    if (nodes.size() < 2)
        return 0;
    if (!pathId)
        pathId = sTaxiPathNodesByPath.size();
    if (sTaxiPathNodesByPath.size() <= pathId)
        sTaxiPathNodesByPath.resize(pathId + 1);
    sTaxiPathNodesByPath[pathId].clear();
    sTaxiPathNodesByPath[pathId].resize(nodes.size());
    uint32 startNode = nodeId;
    uint32 index = 0;
    for (std::list<TaxiPathNodeEntry>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        TaxiPathNodeEntry entry = *it;
        entry.path = pathId;
        TaxiNodesEntry* nodeEntry = new TaxiNodesEntry();
        nodeEntry->ID = index;
        nodeEntry->map_id = entry.mapid;
        nodeEntry->MountCreatureID[0] = mountH;
        nodeEntry->MountCreatureID[1] = mountA;
        nodeEntry->x = entry.x;
        nodeEntry->y = entry.y;
        nodeEntry->z = entry.z;
        sTaxiNodesStore.SetEntry(nodeId, nodeEntry);
        entry.index = nodeId++;
        sTaxiPathNodesByPath[pathId].set(index++, TaxiPathNodePtr(new TaxiPathNodeEntry(entry)));
    }
    if (startNode >= nodeId)
        return 0;
    sTaxiPathSetBySource[startNode][nodeId - 1] = TaxiPathBySourceAndDestination(pathId, price);
    return pathId;
}

bool Eluna::WorldObjectInRangeCheck::operator()(WorldObject* u)
{
    if (i_typeMask && !u->isType(TypeMask(i_typeMask)))
        return false;
    if (i_entry && u->GetEntry() != i_entry)
        return false;
    if (i_obj->GET_GUID() == u->GET_GUID())
        return false;
    if (!i_obj->IsWithinDistInMap(u, i_range))
        return false;
    if (Unit* unit = u->ToUnit())
    {
#ifdef MANGOS
        if (!unit->isAlive())
            return false;
#else
        if (!unit->IsAlive())
            return false;
#endif
        if (i_hostile)
        {
            if (const Unit* obj = i_obj->ToUnit())
            {
                if ((i_hostile == 1) != obj->IsHostileTo(unit))
                    return false;
            }
        }
    }
    if (i_nearest)
        i_range = i_obj->GetDistance(u);
    return true;
}