/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef __ELUNA__H
#define __ELUNA__H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include <ace/Atomic_Op.h>
#include "HookMgr.h"

// Base
#include "Common.h"
#include "SharedDefines.h"

// classes
#include "EventProcessor.h"
#include "ThreatManager.h"

// enums
#include "Group.h"
#include "Item.h"
#ifdef MANGOS
#include "Player.h"
#endif
#include "Weather.h"
#include "World.h"

#ifdef MANGOS
typedef SpellEffectIndex SpellEffIndex;
typedef SpellEntry SpellInfo;
typedef ItemPrototype ItemTemplate;
#define GetTemplate             GetProto
#ifdef CLASSIC
typedef int Difficulty;
#endif
#endif

struct AreaTriggerEntry;
#ifdef MANGOS
class ReactorAI;
typedef ReactorAI ScriptedAI;
#else
struct ScriptedAI;
#endif
class AuctionHouseObject;
class Channel;
class Creature;
class CreatureAI;
class GameObject;
class Guild;
class Group;
class Item;
class Player;
class Quest;
class Spell;
class SpellCastTargets;
class Transport;
class Unit;
class Weather;
class WorldPacket;
#ifndef CLASSIC
#ifndef TBC
#ifdef TRINITY
class Vehicle;
#else
class VehicleInfo;
typedef VehicleInfo Vehicle;
#endif
#endif
#endif

// path, script
typedef std::map<std::string, std::string> LoadedScripts;

#ifdef MANGOS
#define esWorld                 (&MaNGOS::Singleton<World>::Instance())
#define esMapMgr                (&MapManager::Instance())
#define esGuildMgr              (&MaNGOS::Singleton<GuildMgr>::Instance())
#define esObjectMgr             (&MaNGOS::Singleton<ObjectMgr>::Instance())
#define esAccountMgr            (&MaNGOS::Singleton<AccountMgr>::Instance())
#define esObjectAccessor        (&ObjectAccessor::Instance())
#define MAKE_NEW_GUID(l, e, h)  ObjectGuid(h, e, l)
#define GET_GUID                GetObjectGuid
#define GetGameObjectTemplate   GetGameObjectInfo
#define GetItemTemplate         GetItemPrototype
#define ELUNA_LOG_INFO(...)     sLog.outString(__VA_ARGS__);
#define ELUNA_LOG_ERROR(...)    sLog.outErrorEluna(__VA_ARGS__);
#define ELUNA_LOG_DEBUG(...)    sLog.outDebug(__VA_ARGS__);
#define CORE_VERSION            REVISION_NR
#define CORE_NAME               "MaNGOS"
#define SERVER_MSG_STRING       SERVER_MSG_CUSTOM
#define MAX_LOCALES             MAX_LOCALE
#define OVERRIDE                override
#define DIALOG_STATUS_SCRIPTED_NO_STATUS    DIALOG_STATUS_UNDEFINED
#define TARGETICONCOUNT         TARGET_ICON_COUNT
typedef TemporarySummon TempSummon;
#ifndef CLASSIC
#define PLAYER_FIELD_LIFETIME_HONORABLE_KILLS   PLAYER_FIELD_LIFETIME_HONORBALE_KILLS
#endif
#define MAX_TALENT_SPECS        MAX_TALENT_SPEC_COUNT
#define GUID_ENPART(guid)       ObjectGuid(guid).GetEntry()
#define GUID_LOPART(guid)       ObjectGuid(guid).GetCounter()
#define GUID_HIPART(guid)       ObjectGuid(guid).GetHigh()
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,                               // Just selects a random target
    SELECT_TARGET_TOPAGGRO,                                 // Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,                              // Selects targets from bottom aggro to top
    SELECT_TARGET_NEAREST,
    SELECT_TARGET_FARTHEST
};
#ifdef TBC
#define SPELL_AURA_MOD_KILL_XP_PCT  SPELL_AURA_MOD_XP_PCT
#endif
#ifdef CLASSIC
#undef Opcodes
#define Opcodes                 OpcodesList
#endif
#else
#define esWorld                 sWorld
#define esMapMgr                sMapMgr
#define esGuildMgr              sGuildMgr
#define esObjectMgr             sObjectMgr
#define esAccountMgr            sAccountMgr
#define esObjectAccessor        sObjectAccessor
#ifndef CATA
typedef uint64 ObjectGuid;
#endif
#define GET_GUID                GetGUID
#define CORE_VERSION            _DATE
#define CORE_NAME               "TrinityCore"
#define REGEN_TIME_FULL
#define ELUNA_LOG_INFO(...)     TC_LOG_INFO("eluna", __VA_ARGS__);
#define ELUNA_LOG_ERROR(...)    TC_LOG_ERROR("eluna", __VA_ARGS__);
#define ELUNA_LOG_DEBUG(...)    TC_LOG_DEBUG("eluna", __VA_ARGS__);
typedef ThreatContainer::StorageType ThreatList;
#ifdef CATA
#define NUM_MSG_TYPES           NUM_OPCODE_HANDLERS
#endif
#endif

enum ElunaEnvironments
{
    ENV_NONE,
    ENV_MAP,    // For current map only
    ENV_WORLD,  // For world state only
    ENV_GLOBAL, // For world and map
    ENV_MAX
};

struct ElunaFunctions
{
    ElunaEnvironments env;
    const char* name;
    int(*mfunc)(lua_State*);
};

template<typename T>
struct ElunaMethods
{
    ElunaEnvironments env;
    const char* name;
    int(*mfunc)(lua_State*, T*);
};

template <typename K, typename V>
class RWHashMap
{
public:

    typedef UNORDERED_MAP<K, V> MapType;
    typedef ACE_RW_Thread_Mutex LockType;

    static void Insert(K key, V value)
    {
        ACE_Write_Guard< LockType > GUARD(lock);
        if (GUARD.locked() == 0) ASSERT(false);
        hashmap[key] = value;
    }

    static void Remove(K key)
    {
        ACE_Write_Guard< LockType > GUARD(lock);
        if (GUARD.locked() == 0) ASSERT(false);
        hashmap.erase(key);
    }

    static V Find(K key)
    {
        ACE_Read_Guard< LockType > GUARD(lock);
        if (GUARD.locked() == 0) ASSERT(false);
        typename MapType::iterator itr = hashmap.find(key);
        return (itr != hashmap.end()) ? itr->second : NULL;
    }

    static MapType& GetContainer() { return hashmap; }

    static LockType* GetLock() { return &lock; }

private:
    RWHashMap() {}

    static LockType lock;
    static MapType hashmap;
};

class Eluna;

template<typename T>
class ElunaTemplate
{
public:
    static const char* tname;
    static bool manageMemory;

    static int type(lua_State* L)
    {
        lua_pushstring(L, tname);
        return 1;
    }

    static int gcT(lua_State* L)
    {
        if (!manageMemory)
            return 0;
        T* obj = check(L, 1);
        delete obj; // Deleting NULL should be safe
        return 0;
    }

    // name will be used as type name
    // If gc is true, lua will handle the memory management for object pushed
    // gc should be used if pushing for example WorldPacket,
    // that will only be needed on lua side and will not be managed by TC/mangos/<core>
    static void Register(lua_State* L, const char* name, bool gc = false)
    {
        tname = name;
        manageMemory = gc;

        lua_settop(L, 0); // clean stack

        lua_newtable(L);
        int methods = lua_gettop(L);

        luaL_newmetatable(L, tname);
        int metatable = lua_gettop(L);

        // store method table in globals so that
        // scripts can add functions in Lua
        lua_pushvalue(L, methods);
        lua_setglobal(L, tname);

        // hide metatable
        lua_pushvalue(L, methods);
        lua_setfield(L, metatable, "__metatable");

        lua_pushvalue(L, methods);
        lua_setfield(L, metatable, "__index");

        lua_pushcfunction(L, tostringT);
        lua_setfield(L, metatable, "__tostring");

        lua_pushcfunction(L, gcT);
        lua_setfield(L, metatable, "__gc");

        lua_newtable(L);
        lua_setmetatable(L, methods);
    }

    template<typename C>
    static void SetMethods(Eluna* E, ElunaMethods<C>* methodTable);

    static int push(lua_State* L, T const* obj)
    {
        if (!obj)
        {
            lua_pushnil(L);
            return lua_gettop(L);
        }
        luaL_getmetatable(L, tname);
        if (lua_isnoneornil(L, -1))
            return luaL_error(L, "%s missing metatable", tname);
        T const** ptrHold = (T const**)lua_newuserdata(L, sizeof(T**));
        if (ptrHold)
        {
            *ptrHold = obj;
            lua_pushvalue(L, -2);
            lua_setmetatable(L, -2);
        }
        lua_replace(L, -2);
        return lua_gettop(L);
    }

    static T* check(lua_State* L, int narg, bool error = true)
    {
        T** ptrHold = static_cast<T**>(lua_touserdata(L, narg));
        if (!ptrHold)
        {
            if (error)
            {
                std::string errmsg(tname);
                errmsg += " expected";
                luaL_argerror(L, narg, errmsg.c_str());
            }
            return NULL;
        }
        return *ptrHold;
    }

    static int thunk(lua_State* L)
    {
        T* obj = check(L, 1); // get self
        ElunaMethods<T>* l = static_cast<ElunaMethods<T>*>(lua_touserdata(L, lua_upvalueindex(1)));
        if (!obj)
            return 0;
        int args = lua_gettop(L);
        int expected = l->mfunc(L, obj);
        args = lua_gettop(L) - args;
        if (args <= 0 || args > expected)
        {
            if (args < 0 || args > expected) // Assert instead?
            {
                ELUNA_LOG_ERROR("[Eluna]: %s returned unexpected amount of arguments %i out of %i. Report to devs", l->name, args, expected);
            }
            return expected;
        }
        for (; args < expected; ++args)
            lua_pushnil(L);
        return expected;
    }

    static int tostringT(lua_State* L)
    {
        char buff[32];
        T** ptrHold = (T**)lua_touserdata(L, 1);
        sprintf(buff, "%p", *ptrHold);
        lua_pushfstring(L, "%s (%s)", tname, buff);
        return 1;
    }
};

struct EventBind
{
    typedef std::vector<int> ElunaBindingMap;
    typedef UNORDERED_MAP<int, ElunaBindingMap> ElunaEntryMap;
    Eluna& eluna;

    EventBind(Eluna& _eluna): eluna(_eluna)
    {
    }
    ~EventBind() { Clear(); }

    void Clear(); // unregisters all registered functions and clears all registered events from the bind std::maps (reset)
    void Insert(int eventId, int funcRef); // Inserts a new registered event

    // Gets the binding std::map containing all registered events with the function refs for the entry
    ElunaBindingMap* GetBindMap(int eventId);

    // Checks if there are events for ID
    bool HasEvents(int eventId) const;
    // Cleans stack and pushes eventId
    void BeginCall(int eventId) const;
    // Loops through all registered events for the eventId at stack index 1
    // Copies the whole stack as arguments for the called function. Before Executing, push all params to stack!
    // Leaves return values from all functions in order to the stack.
    void ExecuteCall();
    void EndCall() const;

    ElunaEntryMap Bindings; // Binding store Bindings[eventId] = {funcRef};
};

struct EntryBind
{
    typedef std::map<int, int> ElunaBindingMap;
    typedef UNORDERED_MAP<uint32, ElunaBindingMap> ElunaEntryMap;
    Eluna& eluna;

    EntryBind(Eluna& _eluna): eluna(_eluna)
    {
    }
    ~EntryBind() { Clear(); }

    void Clear(); // unregisters all registered functions and clears all registered events from the bind std::maps (reset)
    void Insert(uint32 entryId, int eventId, int funcRef); // Inserts a new registered event

    // Gets the function ref of an entry for an event
    int GetBind(uint32 entryId, int eventId) const;

    // Gets the binding std::map containing all registered events with the function refs for the entry
    const ElunaBindingMap* GetBindMap(uint32 entryId) const;

    // Returns true if the entry has registered binds
    bool HasBinds(uint32 entryId) const;

    ElunaEntryMap Bindings; // Binding store Bindings[entryId][eventId] = funcRef;
};

struct LuaEvent : public BasicEvent
{
    LuaEvent(Eluna* _E, int _funcRef, uint32 _delay, uint32 _calls, EventProcessor* _events, WorldObject* _obj = NULL);

    ~LuaEvent();

    // Should never execute on dead events
    bool Execute(uint64 time, uint32 diff);

    Eluna* E;       // State containing lua function to run (using Eluna to avoid locking)
    int funcRef;    // Lua function reference ID, also used as event ID
    uint32 delay;   // Delay between event calls
    uint32 calls;   // Amount of calls to make, 0 for infinite
    EventProcessor* events; // Pointer to events (holds the timed event)
    WorldObject* obj;   // Object to push
};

struct EventMgr
{
    typedef std::set<LuaEvent*> EventSet;
    typedef std::map<EventProcessor*, EventSet> EventMap;

    EventMap LuaEvents; // LuaEvents[processor] = {LuaEvent, LuaEvent...}
    EventProcessor GlobalEvents;

    ~EventMgr();

    // Should be run on world tick
    void Update(uint32 diff);

    // Aborts all lua events
    void KillAllEvents(EventProcessor* events);

    // Remove all timed events
    void RemoveEvents();

    // Remove timed events from processor
    void RemoveEvents(EventProcessor* events);

    // Adds a new event to the processor and returns the eventID or 0 (Never negative)
    int AddEvent(Eluna* E, int funcRef, uint32 delay, uint32 calls, EventProcessor* events, WorldObject* obj = NULL);

    // Finds the event that has the ID from events
    LuaEvent* GetEvent(EventProcessor* events, int eventId);

    // Remove the event with the eventId from processor
    // Returns true if event is removed
    bool RemoveEvent(EventProcessor* events, int eventId); // eventId = funcRef

    // Removes the eventId from all events
    void RemoveEvent(int eventId);
};

class StateMsg
{
public:

    enum StateMsgType
    {
        TYPE_NIL,
        TYPE_BOOL,
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_MAX
    };

    class Data
    {
    public:
        Data(): _type(TYPE_NIL) {}
        Data(bool val): _type(TYPE_BOOL), _bool(val) {}
        Data(double val): _type(TYPE_NUMBER), _num(val) {}
        Data(std::string val): _type(TYPE_STRING), _str(val) {}

        StateMsgType GetType()
        {
            return _type;
        }

        void Push(lua_State* L) const;

    private:
        StateMsgType _type;
        bool _bool;
        double _num;
        std::string _str;
    };

    typedef std::vector<Data> MsgData;

    StateMsg(uint32 targetmapid, uint32 targetinstanceid):
        t_mapid(targetmapid), t_instanceid(targetinstanceid)
    {
    }
    void AddValue()
    {
        msgs.push_back(Data());
    }
    void AddValue(bool val)
    {
        msgs.push_back(Data(val));
    }
    void AddValue(double val)
    {
        msgs.push_back(Data(val));
    }
    void AddValue(std::string val)
    {
        msgs.push_back(Data(val));
    }
    // Thread unsafe
    Eluna* GetTarget() const;
    // pushes channel, value1, value2 ...
    void Push(lua_State* L) const;

private:
    StateMsg(): t_mapid(MAPID_INVALID), t_instanceid(0) {}

    uint32 t_mapid;         // target mapid (MAPID_INVALID for global)
    uint32 t_instanceid;    // target instanceid (0 for non instance)
    MsgData msgs;
};

class Eluna
{
    friend class Map;
    friend class World;
    friend class WorldRunnable;

private:
    Eluna(const Eluna&);
    Eluna& operator=(const Eluna&);

    typedef RWHashMap<lua_State*, Eluna*> ElunaMapData;
    static ElunaMapData ElunaMap;

    static ACE_Based::LockedQueue<StateMsg*, ACE_Thread_Mutex> StateMsgQue;

    // script stores
    static LoadedScripts script_extensions;
    static LoadedScripts script_global;
    static LoadedScripts script_world;
    static LoadedScripts script_map;

    // Used to start global lua state. MUST be used before ANY hook is triggered.
    static void Initialize();
    // Used to stop global lua state. Should be after all maps unload and after shutdown hook
    static void Uninitialize();
    Eluna(Map* _map);
    ~Eluna();

public:
    Map* GMap; // NULL for global state
    EventMgr m_EventMgr;
    lua_State* L; // Always valid

    static Eluna* GEluna; // NOTE! only use for threadunsafe hooks (world update)
    static Eluna* GetEluna(lua_State* L); // Using lock for thread safety, use seldom

    // Use templates for EventBind
    EventBind GlobalEventBindings;
    EventBind ServerEventBindings;
    EventBind PacketEventBindings;
    EventBind GuildEventBindings;
    EventBind GroupEventBindings;
    EventBind MapEventBindings;
    EventBind PlayerEventBindings;

    EntryBind CreatureEventBindings;
    EntryBind GameObjectEventBindings;
    EntryBind ItemEventBindings;
    EntryBind CreatureGossipBindings;
    EntryBind GameObjectGossipBindings;
    EntryBind ItemGossipBindings;
    EntryBind playerGossipBindings;

    // FOR TESTING ONLY! Closes and starts lua states for all maps and the global one.
    // Thread unsafe and only locks on delete!
    static void ReloadLuaStates();
    static void report(lua_State* L);
    void RegisterGlobals();
    void RegisterFunctions();
    static void CreateDir(std::string path, std::string name);
    static void GetScripts(std::string path, std::string folder, LoadedScripts& scripts);
    void RunScripts(LoadedScripts& scripts);
    void Register(uint8 reg, uint32 id, uint32 evt, int func);
    void BeginCall(int fReference);
    bool ExecuteCall(int params, int res);
    void EndCall(int res);

    static void AddStateMsg(StateMsg* msg)
    {
        StateMsgQue.add(msg);
    }

    // Static functions so can access with plain L
    // Pushes
    static void Push(lua_State* L); // nil
    static void Push(lua_State* L, const uint64);
    static void Push(lua_State* L, const int64);
    static void Push(lua_State* L, const uint32);
    static void Push(lua_State* L, const int32);
    static void Push(lua_State* L, const bool);
    static void Push(lua_State* L, const float);
    static void Push(lua_State* L, const double);
    static void Push(lua_State* L, const char*);
    static void Push(lua_State* L, const std::string);
    template<typename T> static void Push(lua_State* L, T const* ptr)
    {
        ElunaTemplate<T>::push(L, ptr);
    }
    static void Push(lua_State* L, Object const* obj);
    static void Push(lua_State* L, WorldObject const* obj);
    static void Push(lua_State* L, Unit const* unit);
    static void Push(lua_State* L, Pet const* pet);
    static void Push(lua_State* L, TempSummon const* summon);

    // Checks
    template<typename T> static T CHECKVAL(lua_State* L, int narg);
    template<typename T> static T CHECKVAL(lua_State* L, int narg, T def);
    template<typename T> static T* CHECKOBJ(lua_State* L, int narg, bool error = true)
    {
        return ElunaTemplate<T>::check(L, narg, error);
    }

    struct ObjectGUIDCheck
    {
        ObjectGUIDCheck(ObjectGuid guid): _guid(guid) {}
        bool operator()(WorldObject* object)
        {
            return object->GET_GUID() == _guid;
        }

        ObjectGuid _guid;
    };

    // Binary predicate to sort WorldObjects based on the distance to a reference WorldObject
    struct ObjectDistanceOrderPred
    {
        ObjectDistanceOrderPred(WorldObject const* pRefObj, bool ascending = true): m_refObj(pRefObj), m_ascending(ascending) {}
        bool operator()(WorldObject const* pLeft, WorldObject const* pRight) const
        {
            return m_ascending ? m_refObj->GetDistanceOrder(pLeft, pRight) : !m_refObj->GetDistanceOrder(pLeft, pRight);
        }

        WorldObject const* m_refObj;
        const bool m_ascending;
    };

    // Doesn't get self
    struct WorldObjectInRangeCheck
    {
        WorldObjectInRangeCheck(bool nearest, WorldObject const* obj, float range,
            uint16 typeMask = 0, uint32 entry = 0, uint32 hostile = 0):
            i_obj(obj), i_range(range), i_typeMask(typeMask),
            i_entry(entry), i_nearest(nearest), i_hostile(hostile)
        {
        }
        WorldObject const& GetFocusObject() const { return *i_obj; }
        bool operator()(WorldObject* u);
        WorldObject const* i_obj;
        float i_range;
        uint16 i_typeMask;
        uint32 i_entry;
        bool i_nearest;
        uint32 i_hostile;

        WorldObjectInRangeCheck(WorldObjectInRangeCheck const&);
    };

    /* Custom */
    void OnStateMessage(StateMsg* msg);
    bool OnCommand(Player* player, const char* text);
    void OnLootItem(Player* pPlayer, Item* pItem, uint32 count, uint64 guid);
    void OnLootMoney(Player* pPlayer, uint32 amount);
    void OnFirstLogin(Player* pPlayer);
    void OnEquip(Player* pPlayer, Item* pItem, uint8 bag, uint8 slot);
    void OnRepop(Player* pPlayer);
    void OnResurrect(Player* pPlayer);
    void OnQuestAbandon(Player* pPlayer, uint32 questId);
    InventoryResult OnCanUseItem(const Player* pPlayer, uint32 itemEntry);
    void OnCloseLua();
    bool OnAddonMessage(Player* sender, uint32 type, std::string& msg, Player* receiver, Guild* guild, Group* group, Channel* channel);

    /* Server */
    void OnOpenStateChange(bool open);
    void OnConfigLoad(bool reload);
    void OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask);
    void OnShutdownCancel();
    void OnWorldUpdate(uint32 diff);
    void OnStartup();
    void OnShutdown();

    /* Item */
    bool OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, Item* pTarget);
    bool OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest);
    bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);
    bool OnExpire(Player* pPlayer, ItemTemplate const* pProto);
    bool OnRemove(Player* pPlayer, Item* item);
    void HandleGossipSelectOption(Player* pPlayer, Item* item, uint32 sender, uint32 action, std::string code);

    /* Creature */
    bool OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, Creature* pTarget);
    bool OnGossipHello(Player* pPlayer, Creature* pCreature);
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action);
    bool OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, const char* code);
    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    bool OnQuestComplete(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    bool OnQuestReward(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    uint32 GetDialogStatus(Player* pPlayer, Creature* pCreature);
    void OnSummoned(Creature* creature, Unit* summoner);
    CreatureAI* GetAI(Creature* creature);
    // AI
    void UpdateAI(Creature* me, const uint32 diff);
    void EnterCombat(Creature* me, Unit* target);
    void DamageTaken(Creature* me, Unit* attacker, uint32& damage);
    void JustDied(Creature* me, Unit* killer);
    void KilledUnit(Creature* me, Unit* victim);
    void JustSummoned(Creature* me, Creature* summon);
    void SummonedCreatureDespawn(Creature* me, Creature* summon);
    void MovementInform(Creature* me, uint32 type, uint32 id);
    void AttackStart(Creature* me, Unit* target);
    void EnterEvadeMode(Creature* me);
    void AttackedBy(Creature* me, Unit* attacker);
    void JustRespawned(Creature* me);
    void JustReachedHome(Creature* me);
    void ReceiveEmote(Creature* me, Player* player, uint32 emoteId);
    void CorpseRemoved(Creature* me, uint32& respawnDelay);
    void MoveInLineOfSight(Creature* me, Unit* who);
    void On_Reset(Creature* me);
    void SpellHit(Creature* me, Unit* caster, SpellInfo const* spell);
    void SpellHitTarget(Creature* me, Unit* target, SpellInfo const* spell);
#ifndef MANGOS
    void IsSummonedBy(Creature* me, Unit* summoner);
    void SummonedCreatureDies(Creature* me, Creature* summon, Unit* killer);
    void OwnerAttackedBy(Creature* me, Unit* attacker);
    void OwnerAttacked(Creature* me, Unit* target);
#endif

    /* GameObject */
    bool OnDummyEffect(Unit* pCaster, uint32 spellId, SpellEffIndex effIndex, GameObject* pTarget);
    bool OnGossipHello(Player* pPlayer, GameObject* pGameObject);
    bool OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action);
    bool OnGossipSelectCode(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, const char* code);
    bool OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    bool OnQuestComplete(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    bool OnQuestReward(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    uint32 GetDialogStatus(Player* pPlayer, GameObject* pGameObject);
#ifndef CLASSIC
#ifndef TBC
    void OnDestroyed(GameObject* pGameObject, Player* pPlayer);
    void OnDamaged(GameObject* pGameObject, Player* pPlayer);
#endif
#endif
    void OnLootStateChanged(GameObject* pGameObject, uint32 state);
    void OnGameObjectStateChanged(GameObject* pGameObject, uint32 state);
    void UpdateAI(GameObject* pGameObject, uint32 diff);
    void OnSpawn(GameObject* gameobject);

    /* Packet */
    bool OnPacketSend(WorldSession* session, WorldPacket& packet);
    bool OnPacketReceive(WorldSession* session, WorldPacket& packet);

    /* Player */
    void OnPlayerEnterCombat(Player* pPlayer, Unit* pEnemy);
    void OnPlayerLeaveCombat(Player* pPlayer);
    void OnPVPKill(Player* pKiller, Player* pKilled);
    void OnCreatureKill(Player* pKiller, Creature* pKilled);
    void OnPlayerKilledByCreature(Creature* pKiller, Player* pKilled);
    void OnLevelChanged(Player* pPlayer, uint8 oldLevel);
    void OnFreeTalentPointsChanged(Player* pPlayer, uint32 newPoints);
    void OnTalentsReset(Player* pPlayer, bool noCost);
    void OnMoneyChanged(Player* pPlayer, int32& amount);
    void OnGiveXP(Player* pPlayer, uint32& amount, Unit* pVictim);
    void OnReputationChange(Player* pPlayer, uint32 factionID, int32& standing, bool incremental);
    void OnDuelRequest(Player* pTarget, Player* pChallenger);
    void OnDuelStart(Player* pStarter, Player* pChallenger);
    void OnDuelEnd(Player* pWinner, Player* pLoser, DuelCompleteType type);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Group* pGroup);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Guild* pGuild);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Channel* pChannel);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Player* pReceiver);
    void OnEmote(Player* pPlayer, uint32 emote);
    void OnTextEmote(Player* pPlayer, uint32 textEmote, uint32 emoteNum, uint64 guid);
    void OnSpellCast(Player* pPlayer, Spell* pSpell, bool skipCheck);
    void OnLogin(Player* pPlayer);
    void OnLogout(Player* pPlayer);
    void OnCreate(Player* pPlayer);
    void OnDelete(uint32 guid);
    void OnSave(Player* pPlayer);
    void OnBindToInstance(Player* pPlayer, Difficulty difficulty, uint32 mapid, bool permanent);
    void OnUpdateZone(Player* pPlayer, uint32 newZone, uint32 newArea);
    void HandleGossipSelectOption(Player* pPlayer, uint32 menuId, uint32 sender, uint32 action, std::string code);

#ifndef CLASSIC
#ifndef TBC
    /* Vehicle */
    void OnInstall(Vehicle* vehicle);
    void OnUninstall(Vehicle* vehicle);
    void OnInstallAccessory(Vehicle* vehicle, Creature* accessory);
    void OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 seatId);
    void OnRemovePassenger(Vehicle* vehicle, Unit* passenger);
#endif
#endif

    /* AreaTrigger */
    bool OnAreaTrigger(Player* pPlayer, AreaTriggerEntry const* pTrigger);

    /* Weather */
    void OnChange(Weather* weather, WeatherState state, float grade);

    /* Auction House */
    void OnAdd(AuctionHouseObject* auctionHouse);
    void OnRemove(AuctionHouseObject* auctionHouse);
    void OnSuccessful(AuctionHouseObject* auctionHouse);
    void OnExpire(AuctionHouseObject* auctionHouse);

    /* Guild */
    void OnAddMember(Guild* guild, Player* player, uint32 plRank);
    void OnRemoveMember(Guild* guild, Player* player, bool isDisbanding);
    void OnMOTDChanged(Guild* guild, const std::string& newMotd);
    void OnInfoChanged(Guild* guild, const std::string& newInfo);
    void OnCreate(Guild* guild, Player* leader, const std::string& name);
    void OnDisband(Guild* guild);
    void OnMemberWitdrawMoney(Guild* guild, Player* player, uint32& amount, bool isRepair);
    void OnMemberDepositMoney(Guild* guild, Player* player, uint32& amount);
    void OnItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId, bool isDestBank, uint8 destContainer, uint8 destSlotId);
    void OnEvent(Guild* guild, uint8 eventType, uint32 playerGuid1, uint32 playerGuid2, uint8 newRank);
    void OnBankEvent(Guild* guild, uint8 eventType, uint8 tabId, uint32 playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId);

    /* Group */
    void OnAddMember(Group* group, uint64 guid);
    void OnInviteMember(Group* group, uint64 guid);
    void OnRemoveMember(Group* group, uint64 guid, uint8 method);
    void OnChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid);
    void OnDisband(Group* group);
    void OnCreate(Group* group, uint64 leaderGuid, GroupType groupType);

    /* Map */
    void OnCreate();
    void OnDestroy();
    void OnPlayerEnter(Map* map, Player* player);
    void OnPlayerLeave(Map* map, Player* player);
    void OnUpdate(uint32 diff);
};
template<> Unit* Eluna::CHECKOBJ<Unit>(lua_State* L, int narg, bool error);
template<> Player* Eluna::CHECKOBJ<Player>(lua_State* L, int narg, bool error);
template<> Creature* Eluna::CHECKOBJ<Creature>(lua_State* L, int narg, bool error);
template<> GameObject* Eluna::CHECKOBJ<GameObject>(lua_State* L, int narg, bool error);
template<> Corpse* Eluna::CHECKOBJ<Corpse>(lua_State* L, int narg, bool error);
#endif
