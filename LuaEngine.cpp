/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "LuaEngine.h"
#include "Includes.h"
#include <ace/Dirent.h>
#include <ace/OS_NS_fcntl.h>
#include <ace/OS_NS_sys_stat.h>

extern void RegisterFunctions(lua_State* L);
extern void AddElunaScripts();

template <typename K, typename V> UNORDERED_MAP< K, V > RWHashMap<K, V>::hashmap;
template <typename K, typename V> typename RWHashMap< K, V >::LockType RWHashMap<K, V>::lock;
template class RWHashMap<lua_State*, Eluna*>;
ACE_Based::LockedQueue<StateMsg*, ACE_Thread_Mutex> Eluna::StateMsgQue;
Eluna* Eluna::GEluna = NULL;

LoadedScripts Eluna::script_extensions;
LoadedScripts Eluna::script_global;
LoadedScripts Eluna::script_world;
LoadedScripts Eluna::script_map;

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

void StateMsg::Data::Push(lua_State* L) const
{
    switch (_type)
    {
    default:
    case TYPE_NIL:
        Eluna::Push(L);
        break;
    case TYPE_BOOL:
        Eluna::Push(L, _bool);
        break;
    case TYPE_STRING:
        Eluna::Push(L, _str);
        break;
    case TYPE_NUMBER:
        Eluna::Push(L, _num);
        break;
    }
};

Eluna* StateMsg::GetTarget() const
{
    if (t_mapid == MAPID_INVALID)
        return Eluna::GEluna;
    if (Map* map = esMapMgr->FindMap(t_mapid, t_instanceid))
        return map->GetEluna();
    return NULL;
}

void StateMsg::Push(lua_State* L) const
{
    for (MsgData::const_iterator it = msgs.begin(); it != msgs.end(); ++it)
        it->Push(L);
}

Eluna* Eluna::GetEluna(lua_State* L)
{
    return ElunaMap.Find(L);
}

// Loads the scripts from given path and pushes them to scripts
static void LoadLuaScripts(std::string path, std::string folder, LoadedScripts& scripts)
{
    ELUNA_LOG_DEBUG("[Eluna]: LoadLuaScripts `%s`, `%s`", path.c_str(), folder.c_str());
    scripts.clear();
    Eluna::CreateDir(path, folder);
    Eluna::GetScripts(path, folder, scripts);
}

void Eluna::Initialize()
{
    ELUNA_LOG_INFO("[Eluna]: Initialize...");
    static const std::string path = "lua_scripts"; // for config setting in the future
    uint32 oldMSTime = getMSTime();
    CreateDir("", path);
    LoadLuaScripts(path, "extensions", script_extensions);
    LoadLuaScripts(path, "global", script_global);
    LoadLuaScripts(path, "world", script_world);
    LoadLuaScripts(path, "map", script_map);
    uint32 size = script_extensions.size() + script_global.size() + script_world.size() + script_map.size();
    ELUNA_LOG_INFO("[Eluna]: Loaded %u Lua scripts in %u ms", size, GetMSTimeDiffToNow(oldMSTime));

    GEluna = new Eluna(NULL);
}

void Eluna::Uninitialize()
{
    delete GEluna;
}

Eluna::Eluna(Map* _map):
GMap(_map),
L(luaL_newstate()),
GlobalEventBindings(*this),
ServerEventBindings(*this),
PacketEventBindings(*this),
GuildEventBindings(*this),
GroupEventBindings(*this),
MapEventBindings(*this),
PlayerEventBindings(*this),

CreatureEventBindings(*this),
GameObjectEventBindings(*this),
ItemEventBindings(*this),
CreatureGossipBindings(*this),
GameObjectGossipBindings(*this),
ItemGossipBindings(*this),
playerGossipBindings(*this)
{
    ELUNA_LOG_DEBUG("[Eluna]: Creating new lua state");
    uint32 oldMSTime = getMSTime();
    ElunaMap.Insert(L, this);
    luaL_openlibs(L);
    RegisterFunctions();

    RunScripts(script_extensions);
    RunScripts(script_global);
    if(GMap)
        RunScripts(script_map);
    else
        RunScripts(script_world);
    ELUNA_LOG_DEBUG("[Eluna]: Loaded lua scripts in %u ms", GetMSTimeDiffToNow(oldMSTime));

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
    OnCloseLua();

    ElunaMap.Remove(L);

    // Unregisters and stops all timed events
    m_EventMgr.RemoveEvents();

    // Remove bindings
    GlobalEventBindings.Clear();
    ServerEventBindings.Clear();
    PacketEventBindings.Clear();
    GuildEventBindings.Clear();
    GroupEventBindings.Clear();
    MapEventBindings.Clear();
    PlayerEventBindings.Clear();

    CreatureEventBindings.Clear();
    GameObjectEventBindings.Clear();
    ItemEventBindings.Clear();
    CreatureGossipBindings.Clear();
    GameObjectGossipBindings.Clear();
    ItemGossipBindings.Clear();
    playerGossipBindings.Clear();

    lua_close(L);
}

// Start or restart eluna. Returns true if started
//bool StartEluna_OBSOLETE()
//{
//#ifndef ELUNA
//#ifndef MANGOS
//    {
//        ELUNA_LOG_ERROR("[Eluna]: LuaEngine is Disabled. (If you want to use it please enable in cmake)");
//        return false;
//    }
//#endif
//#endif
//
//    //ELUNA_GUARD();
//    //bool restart = false;
//    //if (eluna.L)
//    //{
//    //    restart = true;
//    //    Eluna::OnEngineRestart();
//
//    //}
//    //else
//    //    AddElunaScripts();
//
//#ifdef MANGOS
//    // Check config file for eluna is enabled or disabled
//    if (!esWorld->getConfig(CONFIG_BOOL_ELUNA_ENABLED))
//    {
//        ELUNA_LOG_ERROR("[Eluna]: LuaEngine is Disabled. (If you want to use it please set config in 'mangosd.conf')");
//        return false;
//    }
//#endif
//
//    return true;
//}

void Eluna::ReloadLuaStates()
{
    ELUNA_LOG_INFO("Reloading Lua states...");
    uint32 oldMSTime = getMSTime();
    // Delete global state
    Uninitialize();
    std::vector<Map*> reloaded_maps;
    Eluna::ElunaMapData::MapType& states = ElunaMap.GetContainer();
    Eluna::ElunaMapData::MapType::const_iterator it;
    // Delete old states and store maps that had states
    while ((it = states.begin()) != states.end())
    {
        if (it->second->GMap) // skip global
            reloaded_maps.push_back(it->second->GMap);
        delete it->second;
    }
    // reload scripts and create global state
    Initialize();
    // create new map states
    for (std::vector<Map*>::const_iterator it = reloaded_maps.begin(); it != reloaded_maps.end(); ++it)
        (*it)->luadata = new Eluna(*it);
    ELUNA_LOG_INFO("Reloaded %u lua states in %u ms", (uint32)states.size(), GetMSTimeDiffToNow(oldMSTime));
}

void Eluna::CreateDir(std::string path, std::string name)
{
    if (!path.empty())
        path += "/";
    path += name;
    ELUNA_LOG_DEBUG("[Eluna]: CreateDir `%s`", path.c_str());
    ACE_Dirent dir;
    if (dir.open(path.c_str()) == -1)
    {
        ELUNA_LOG_ERROR("[Eluna]: Error No `%s` directory found, creating it", path.c_str());
        ACE_OS::mkdir(path.c_str());
        return;
    }
}

// Finds lua script files from given path (including subdirectories) and pushes them to scripts
void Eluna::GetScripts(std::string path, std::string folder, LoadedScripts& scripts)
{
    if (!path.empty())
        path += "/";
    path += folder;
    ELUNA_LOG_DEBUG("[Eluna]: GetScripts from path `%s`", path.c_str());

    ACE_Dirent dir;
    if (dir.open(path.c_str()) == -1)
    {
        ELUNA_LOG_ERROR("[Eluna]: Error No `%s` directory found", path.c_str());
        return;
    }

    ACE_DIRENT *directory = 0;
    while ((directory = dir.read()))
    {
        // Skip the ".." and "." files.
        if (ACE::isdotdir(directory->d_name))
            continue;

        std::string fullpath = path + "/" + directory->d_name;

        ACE_stat stat_buf;
        if (ACE_OS::lstat(fullpath.c_str(), &stat_buf) == -1)
            continue;

        // load subfolder
        if ((stat_buf.st_mode & S_IFMT) == (S_IFDIR))
        {
            GetScripts(path, directory->d_name, scripts);
            continue;
        }

        // was file, check extension
        ELUNA_LOG_DEBUG("[Eluna]: GetScripts Checking file `%s`", fullpath.c_str());
        std::string ext = fullpath.substr(fullpath.length() - 4, 4);
        if (ext != ".lua" && ext != ".dll")
            continue;

        // was correct, load script to memory
        ELUNA_LOG_DEBUG("[Eluna]: GetScripts read file `%s`", fullpath.c_str());
        ACE_HANDLE handle = ACE_OS::open(fullpath.c_str(), O_RDWR);
        if (!handle)
        {
            ELUNA_LOG_ERROR("[Eluna]: GetScripts couldnt read file `%s`", fullpath.c_str());
            continue;
        }

        // load chuncks of code to buffer and append it to scripts for the filepath
        char buf[1024];
        ssize_t length = 0;
        std::string luaname = "@" + fullpath;
        while ((length = ACE_OS::read(handle, buf, sizeof(buf)-1)))
        {
            buf[length] = 0;
            scripts[luaname] += buf;
        }
        ACE_OS::close(handle);
    }
}

void Eluna::RunScripts(LoadedScripts& scripts)
{
    ELUNA_LOG_DEBUG("[Eluna]: RunScripts on map %u", GMap ? GMap->GetId() : MAPID_INVALID);
    // load last first to load extensions first
    for (LoadedScripts::const_iterator it = scripts.begin(); it != scripts.end(); ++it)
    {
        if (!luaL_loadbuffer(L, it->second.c_str(), it->second.length(), it->first.c_str()) && !lua_pcall(L, 0, 0, 0))
            continue;
        ELUNA_LOG_ERROR("[Eluna]: Error loading file `%s` on map %u", it->first.substr(1, std::string::npos).c_str(), GMap ? GMap->GetId() : MAPID_INVALID);
        report(L);
    }
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

// Saves the function reference ID given to the register type's store for given entry under the given event
void Eluna::Register(uint8 regtype, uint32 id, uint32 evt, int functionRef)
{
    switch (regtype)
    {
    case REGTYPE_GLOBAL:
        if (evt < GLOBAL_EVENT_COUNT)
        {
            GlobalEventBindings.Insert(evt, functionRef);
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

    case REGTYPE_PACKET:
        if (evt < NUM_MSG_TYPES)
        {
            PacketEventBindings.Insert(evt, functionRef);
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

    case REGTYPE_MAP:
        if (evt < MAP_EVENT_COUNT)
        {
            MapEventBindings.Insert(evt, functionRef);
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

    case REGTYPE_CREATURE:
        if (evt < CREATURE_EVENT_COUNT)
        {
            if (!sObjectMgr->GetCreatureTemplate(id))
            {
                luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                luaL_error(L, "Couldn't find a creature with (ID: %d)!", id);
                return;
            }

            CreatureEventBindings.Insert(id, evt, functionRef);
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

            GameObjectEventBindings.Insert(id, evt, functionRef);
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

            ItemEventBindings.Insert(id, evt, functionRef);
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

            CreatureGossipBindings.Insert(id, evt, functionRef);
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

            GameObjectGossipBindings.Insert(id, evt, functionRef);
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

            ItemGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;

    case REGTYPE_PLAYER_GOSSIP:
        if (evt < GOSSIP_EVENT_COUNT)
        {
            playerGossipBindings.Insert(id, evt, functionRef);
            return;
        }
        break;
    }
    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
    luaL_error(L, "Unknown event type (regtype %d, id %d, event %d)", regtype, id, evt);
}

void EventBind::Clear()
{
    for (ElunaEntryMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
    {
        for (ElunaBindingMap::iterator it = itr->second.begin(); it != itr->second.end(); ++it)
            luaL_unref(eluna.L, LUA_REGISTRYINDEX, (*it));
        itr->second.clear();
    }
    Bindings.clear();
}

void EventBind::Insert(int eventId, int funcRef)
{
    Bindings[eventId].push_back(funcRef);
}

EventBind::ElunaBindingMap* EventBind::GetBindMap(int eventId)
{
    if (Bindings.empty())
        return NULL;
    ElunaEntryMap::iterator itr = Bindings.find(eventId);
    if (itr == Bindings.end())
        return NULL;

    return &itr->second;
}

bool EventBind::HasEvents(int eventId) const
{
    if (Bindings.empty())
        return false;
    if (Bindings.find(eventId) == Bindings.end())
        return false;
    return true;
}

void EventBind::BeginCall(int eventId) const
{
    lua_settop(eluna.L, 0); // stack should be empty
    eluna.Push(eluna.L, eventId);
}

void EventBind::ExecuteCall()
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

void EventBind::EndCall() const
{
    lua_settop(eluna.L, 0); // stack should be empty
};

void EntryBind::Clear()
{
    for (ElunaEntryMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
    {
        for (ElunaBindingMap::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it)
            luaL_unref(eluna.L, LUA_REGISTRYINDEX, it->second);
        itr->second.clear();
    }
    Bindings.clear();
}

void EntryBind::Insert(uint32 entryId, int eventId, int funcRef)
{
    if (Bindings[entryId][eventId])
    {
        luaL_unref(eluna.L, LUA_REGISTRYINDEX, funcRef); // free the unused ref
        luaL_error(eluna.L, "A function is already registered for entry (%d) event (%d)", entryId, eventId);
    }
    else
        Bindings[entryId][eventId] = funcRef;
}

int EntryBind::GetBind(uint32 entryId, int eventId) const
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
const EntryBind::ElunaBindingMap* EntryBind::GetBindMap(uint32 entryId) const
{
    if (Bindings.empty())
        return NULL;
    ElunaEntryMap::const_iterator itr = Bindings.find(entryId);
    if (itr == Bindings.end())
        return NULL;

    return &itr->second;
}

// Returns true if the entry has registered binds
bool EntryBind::HasBinds(uint32 entryId) const
{
    if (Bindings.empty())
        return false;
    return Bindings.find(entryId) != Bindings.end();
}

LuaEvent::LuaEvent(Eluna* _E, int _funcRef, uint32 _delay, uint32 _calls, EventProcessor* _events, WorldObject* _obj):
E(_E), funcRef(_funcRef), delay(_delay), calls(_calls), events(_events), obj(_obj)
{
    if (events)
    {
        E->m_EventMgr.LuaEvents[events].insert(this); // Able to access the event if we have the processor
    }
}

LuaEvent::~LuaEvent()
{
    if (events)
    {
        // Attempt to remove the pointer from LuaEvents
        EventMgr::EventMap::const_iterator it = E->m_EventMgr.LuaEvents.find(events); // Get event set
        if (it != E->m_EventMgr.LuaEvents.end())
            E->m_EventMgr.LuaEvents[events].erase(this);// Remove pointer
    }
    luaL_unref(E->L, LUA_REGISTRYINDEX, funcRef); // Free lua function ref
}

bool LuaEvent::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    bool remove = (calls == 1);
    if (!remove)
        events->AddEvent(this, events->CalculateTime(delay)); // Reschedule before calling incase RemoveEvents used
    E->BeginCall(funcRef);
    E->Push(E->L, funcRef);
    E->Push(E->L, delay);
    E->Push(E->L, calls);
    if (!remove && calls)
        --calls;
    E->Push(E->L, obj);
    E->ExecuteCall(4, 0);
    return remove; // Destory (true) event if not run
}

EventMgr::~EventMgr()
{
    RemoveEvents();
}

void EventMgr::Update(uint32 diff)
{
    GlobalEvents.Update(diff);
}

void EventMgr::KillAllEvents(EventProcessor* events)
{
    if (!events)
        return;
    if (LuaEvents.empty())
        return;
    EventMap::const_iterator it = LuaEvents.find(events); // Get event set
    if (it == LuaEvents.end())
        return;
    if (it->second.empty())
        return;
    for (EventSet::const_iterator itr = it->second.begin(); itr != it->second.end();) // Loop events
        (*(itr++))->to_Abort = true; // Abort event
}

void EventMgr::RemoveEvents()
{
    if (!LuaEvents.empty())
        for (EventMap::const_iterator it = LuaEvents.begin(); it != LuaEvents.end();) // loop processors
            KillAllEvents((it++)->first);
    LuaEvents.clear(); // remove pointers
    // This is handled automatically on delete
    // for (ProcessorMap::iterator it = Processors.begin(); it != Processors.end();)
    //    (it++)->second.KillAllEvents(true);
    // Processors.clear(); // remove guid saved processors
    GlobalEvents.KillAllEvents(true);
}

void EventMgr::RemoveEvents(EventProcessor* events)
{
    if (!events)
        return;
    KillAllEvents(events);
    LuaEvents.erase(events); // remove pointer set
}

int EventMgr::AddEvent(Eluna* E, int funcRef, uint32 delay, uint32 calls, EventProcessor* events, WorldObject* obj)
{
    if (!events || funcRef <= 0) // If funcRef <= 0, function reference failed
        return 0; // on fail always return 0. funcRef can be negative.
    events->AddEvent(new LuaEvent(E, funcRef, delay, calls, events, obj), events->CalculateTime(delay));
    return funcRef; // return the event ID
}

LuaEvent* EventMgr::GetEvent(EventProcessor* events, int eventId)
{
    if (!events || !eventId)
        return NULL;
    if (LuaEvents.empty())
        return NULL;
    EventMap::const_iterator it = LuaEvents.find(events); // Get event set
    if (it == LuaEvents.end())
        return NULL;
    if (it->second.empty())
        return NULL;
    for (EventSet::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr) // Loop events
        if ((*itr) && (*itr)->funcRef == eventId) // Check if the event has our ID
            return *itr; // Return the event if found
    return NULL;
}

bool EventMgr::RemoveEvent(EventProcessor* events, int eventId)
{
    if (!events || !eventId)
        return false;
    LuaEvent* luaEvent = GetEvent(events, eventId);
    if (!luaEvent)
        return false;
    luaEvent->to_Abort = true; // Set to remove on next call
    LuaEvents[events].erase(luaEvent); // Remove pointer
    return true;
}

void EventMgr::RemoveEvent(int eventId)
{
    if (!eventId)
        return;
    if (LuaEvents.empty())
        return;
    for (EventMap::const_iterator it = LuaEvents.begin(); it != LuaEvents.end();) // loop processors
        if (RemoveEvent((it++)->first, eventId))
            break; // succesfully remove the event, stop loop.
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
