#pragma once

#include "math.hxx"
#include "util.hxx"

class Vector {
public:
	float x, y, z;
};

class CUserCmd
{
public:

	virtual ~CUserCmd() { };

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	Vector	viewangles;
    // Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	BYTE    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions

#ifndef CLIENT_DLL
	int		server_random_seed; // Only the server populates this seed
#endif

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;

	// Back channel to communicate IK state
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > entitygroundcontact;
#endif

#if defined ( DOTA_DLL )
	CUnitOrders dota_unitorders;
#endif

#if defined ( PORTAL2 )
	// Portal 2's grab code is on the client to support multiplayer
	short player_held_entity;
	// This one is temporary-- some server code needs to know if this trace 
	// went through a portal. This should go away when we move the grabcontrollers
	// down to the client as well.
	short held_entity_was_grabbed_through_portal;

	unsigned short command_acknowledgements_pending; //so we can properly sync portal teleportation angle changes. The server tells us the last command it acknowledged, now we also tell it how many acknowledgments we're waiting on (command_number - engine->GetLastAcknowledgedCommand())
	uint8 predictedPortalTeleportations; //should probably enumerate which transforms we went through if we want perfect accuracy
#endif // PORTAL2

	// TrackIR
	Vector headangles;
	Vector headoffset;
	// TrackIR

#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
	Vector crosshairtrace;		// world location directly beneath the player's crosshair
#endif

#ifdef INFESTED_DLL
	short crosshair_entity;			// index of the entity under the player's crosshair
	byte forced_action;
	short sync_kill_ent;
	Vector skill_dest;
	short skill_dest_ent;
#endif

#if defined( KEEP_COMMAND_REPREDICTION_COUNT )
	unsigned int debug_RepredictionCount;
#endif
};

class CInput
{
	// Interface
public:
	CInput(void);
	~CInput(void);

	virtual        void        Init_All(void);
	virtual        void        Shutdown_All(void);
	virtual        int            GetButtonBits(int);
};

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2
#define	IN_ATTACK3		(1 << 25)

class CBaseHandle
{
    friend class CBaseEntityList;

public:

    CBaseHandle();
    //CBaseHandle(INVALID_EHANDLE_tag) {};
    CBaseHandle(const CBaseHandle& other);
    explicit CBaseHandle(void* pHandleObj);
    CBaseHandle(int iEntry, int iSerialNumber);

    // NOTE: The following constructor is not type-safe, and can allow creating an
    //       arbitrary CBaseHandle that doesn't necessarily point to an actual object.
    //
    // It is your responsibility to ensure that the target of the handle actually points
    // to the object you think it does.  Generally, the argument to this function should
    // have been obtained from CBaseHandle::ToInt() on a valid handle.
    static CBaseHandle UnsafeFromIndex(int index);

    void Init(int iEntry, int iSerialNumber);
    void Term();

    // Even if this returns true, Get() still can return return a non-null value.
    // This just tells if the handle has been initted with any values.
    bool IsValid() const;

    int GetEntryIndex() const;
    int GetSerialNumber() const;

    int ToInt() const;
    bool operator !=(const CBaseHandle& other) const;
    bool operator ==(const CBaseHandle& other) const;
    bool operator ==(const void* pEnt) const;
    bool operator !=(const void* pEnt) const;
    bool operator <(const CBaseHandle& other) const;
    bool operator <(const void* pEnt) const;

    // Assign a value to the handle.
    const CBaseHandle& operator=(const void* pEntity);
    const CBaseHandle& Set(const void* pEntity);

    // Use this to dereference the handle.
    // Note: this is implemented in game code (ehandle.h)
    void* Get() const;


protected:
    // The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
    // The high NUM_SERIAL_NUM_BITS bits are the serial number.
    uint32_t m_Index;
};


class CEntity {
public:
    constexpr const Vector3<float> &GetAbsOrigin() noexcept {
        return util::callVfunc<const Vector3<float> &>(this, 10);
    }

    constexpr int32_t GetHealth() noexcept {
        return util::callVfunc<int32_t>(this, 122);
    }

    constexpr int32_t GetIndex() noexcept {
        return util::callVfunc<int32_t>(this + 0x8, 10);
    }
};

class IEntityList {
private:
    virtual void Unused_0() = 0;
    virtual void Unused_1() = 0;
    virtual void Unused_2() = 0;

public:
    virtual CEntity* GetClientEntity(int index) = 0;
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable = false);

    virtual CEntity* GetClientUnknownFromHandle(CBaseHandle hEnt);
    virtual CEntity* GetClientNetworkableFromHandle(CBaseHandle hEnt);
    virtual CEntity* GetClientEntityFromHandle(CBaseHandle hEnt);
};

class C_CSPlayer
{
public:
    static auto* GetLocalCSPlayer() noexcept
    {
        static auto fn = []()
        {
            return (C_CSPlayer*(__cdecl*)())((uintptr_t)GetModuleHandle("client.dll") + 0xA23E0);
        }();

        return fn();
    }

    [[nodiscard]] Vector3<float>& m_vecVelocity() const noexcept
    {
        return *(Vector3<float>*)((uintptr_t)this + 0xF4);
    }

    [[nodiscard]] Vector3<float>& m_vecAbsVelocity() const noexcept {
        return *(Vector3<float>*)((uintptr_t )this + 0x130);
    }

    [[nodiscard]] Vector3<float>& m_vecNetworkOrigin() const noexcept {
        return *(Vector3<float>*)((uintptr_t )this + 0x338);
    }

    [[nodiscard]] float& m_flSimulationTime() const noexcept {
        return *(float*)((uintptr_t )this + 0x64);
    }

    [[nodiscard]] float& m_flOldSimulationTime() const noexcept {
        return *(float*)((uintptr_t )this + 0x68);
    }

    [[nodiscard]] std::int32_t& m_fFlags() const noexcept
    {
        return *(std::int32_t*)((uintptr_t)this + 0x350);
    }

    [[nodiscard]] Vector3<float>& GetOldOrigin() const noexcept {
        return *(Vector3<float>*)((uintptr_t )this + 0x278);
    }
};

// helper class for user commands
class CBotCmd
{
public:
    CBotCmd()
    {
        Reset();
    }

    virtual ~CBotCmd() { };

    void Reset()
    {
        command_number = 0;
        tick_count = 0;
        viewangles = {};
        forwardmove = 0.0f;
        sidemove = 0.0f;
        upmove = 0.0f;
        buttons = 0;
        impulse = 0;
        weaponselect = 0;
        weaponsubtype = 0;
        random_seed = 0;
        mousedx = 0;
        mousedy = 0;

        hasbeenpredicted = false;
    }

    CBotCmd& operator =( const CBotCmd& src )
    {
        if ( this == &src )
            return *this;

        command_number		= src.command_number;
        tick_count			= src.tick_count;
        viewangles			= src.viewangles;
        forwardmove			= src.forwardmove;
        sidemove			= src.sidemove;
        upmove				= src.upmove;
        buttons				= src.buttons;
        impulse				= src.impulse;
        weaponselect		= src.weaponselect;
        weaponsubtype		= src.weaponsubtype;
        random_seed			= src.random_seed;
        mousedx				= src.mousedx;
        mousedy				= src.mousedy;
        hasbeenpredicted	= src.hasbeenpredicted;
        return *this;
    }

    // For matching server and client commands for debugging
    int		command_number;

    // the tick the client created this command
    int		tick_count;

    // Player instantaneous view angles.
    Vector3<float>	viewangles;
    // Intended velocities
    //	forward velocity.
    float	forwardmove;
    //  sideways velocity.
    float	sidemove;
    //  upward velocity.
    float	upmove;
    // Attack button states
    int		buttons;
    // Impulse command issued.
    std::uint8_t   impulse;
    // Current weapon id
    int		weaponselect;
    int		weaponsubtype;

    int		random_seed;	// For shared random functions

    short	mousedx;		// mouse accum in x from create move
    short	mousedy;		// mouse accum in y from create move

    // Client only, tracks whether we've predicted this command at least once
    bool	hasbeenpredicted;
};

class IPlayerInfo
        {
                public:
                // returns the players name (UTF-8 encoded)
                virtual const char *GetName() = 0;
                // returns the userid (slot number)
                virtual int		GetUserID() = 0;
                // returns the string of their network (i.e Steam) ID
                virtual const char *GetNetworkIDString() = 0;
                // returns the team the player is on
                virtual int GetTeamIndex() = 0;
                // changes the player to a new team (if the game dll logic allows it)
                virtual void ChangeTeam( int iTeamNum ) = 0;
                // returns the number of kills this player has (exact meaning is mod dependent)
                virtual int	GetFragCount() = 0;
                // returns the number of deaths this player has (exact meaning is mod dependent)
                virtual int	GetDeathCount() = 0;
                // returns if this player slot is actually valid
                virtual bool IsConnected() = 0;
                // returns the armor/health of the player (exact meaning is mod dependent)
                virtual int	GetArmorValue() = 0;

                // extensions added to V2

                // various player flags
                virtual bool IsHLTV() = 0;
                virtual bool IsPlayer() = 0;
                virtual bool IsFakeClient() = 0;
                virtual bool IsDead() = 0;
                virtual bool IsInAVehicle() = 0;
                virtual bool IsObserver() = 0;

                // player position and size
                virtual const Vector GetAbsOrigin() = 0;
                virtual const Vector3<float> GetAbsAngles() = 0;
                virtual const Vector GetPlayerMins() = 0;
                virtual const Vector GetPlayerMaxs() = 0;
                // the name of the weapon currently being carried
                virtual const char *GetWeaponName() = 0;
                // the name of the player model in use
                virtual const char *GetModelName() = 0;
                // current player health
                virtual const int GetHealth() = 0;
                // max health value
                virtual const int GetMaxHealth() = 0;
                // the last user input from this player
                virtual CBotCmd GetLastUserCommand() = 0;

                virtual bool IsReplay() = 0;
        };

class CGlobalVarsBase {
public:

    CGlobalVarsBase(bool bIsClient);

    // This can be used to filter debug output or to catch the client or server in the act.
    bool IsClient() const;

    // for encoding m_flSimulationTime, m_flAnimTime
    int GetNetworkBase(int nTick, int nEntity);

public:

    // Absolute time (per frame still - Use Plat_FloatTime() for a high precision real time
    //  perf clock, but not that it doesn't obey host_timescale/host_framerate)
    float realtime;
    // Absolute frame counter
    int framecount;
    // Non-paused frametime
    float absoluteframetime;

    // Current time
    //
    // On the client, this (along with tickcount) takes a different meaning based on what
    // piece of code you're in:
    //
    //   - While receiving network packets (like in PreDataUpdate/PostDataUpdate and proxies),
    //     this is set to the SERVER TICKCOUNT for that packet. There is no interval between
    //     the server ticks.
    //     [server_current_Tick * tick_interval]
    //
    //   - While rendering, this is the exact client clock
    //     [client_current_tick * tick_interval + interpolation_amount]
    //
    //   - During prediction, this is based on the client's current tick:
    //     [client_current_tick * tick_interval]
    float curtime;

    // Time spent on last server or client frame (has nothing to do with think intervals)
    float frametime;
    // current maxplayers setting
    int maxClients;

    // Simulation ticks
    int tickcount;

    // Simulation tick interval
    float interval_per_tick;

    // interpolation amount ( client-only ) based on fraction of next tick which has elapsed
    float interpolation_amount;
    int simTicksThisFrame;

    int network_protocol;

    // current saverestore data
    void *pSaveData;

private:
    // Set to true in client code.
    bool m_bClient;

    // 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
    int nTimestampNetworkingBase;
    // 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
    //  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
    int nTimestampRandomizeWindow;

};

class IPlayerInfoManager
        {
                public:
                virtual IPlayerInfo *GetPlayerInfo( void *pEdict ) = 0;
                virtual CGlobalVarsBase *GetGlobalVars() = 0;
        };
