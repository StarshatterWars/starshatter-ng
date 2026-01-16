/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#ifndef NetServerConfig_h
#define NetServerConfig_h

#include "Types.h"
#include "Game.h"
#include "Text.h"
#include "List.h"

// +--------------------------------------------------------------------+

class NetAddr;
class NetUser;

// +--------------------------------------------------------------------+

class NetServerConfig
{
public:
    static const char* TYPENAME() { return "NetServerConfig"; }

    NetServerConfig();
    ~NetServerConfig();

    enum GAME_TYPE {
        NET_GAME_LAN,
        NET_GAME_PRIVATE,
        NET_GAME_PUBLIC
    };

    const Text& Hostname()           const { return hostname;         }
    const Text& Name()               const { return name;             }
    const Text& GetAdminName()       const { return admin_name;       }
    const Text& GetAdminPass()       const { return admin_pass;       }
    const Text& GetGamePass()        const { return game_pass;        }
    const Text& GetMission()         const { return mission;          }
    WORD        GetAdminPort()       const { return admin_port;       }
    WORD        GetLobbyPort()       const { return lobby_port;       }
    WORD        GetGamePort()        const { return game_port;        }
    int         GetPoolsize()        const { return poolsize;         }
    int         GetSessionTimeout()  const { return session_timeout;  }
    int         GetGameType()        const { return game_type;        }
    int         GetAuthLevel()       const { return auth_level;       }

    void        SetHostname(const char* s) { hostname = Clean(s);     }
    void        SetName(const char* s)     { name = Clean(s);         }
    void        SetAdminName(const char* s){ admin_name = Clean(s);   }
    void        SetAdminPass(const char* s){ admin_pass = Clean(s);   }
    void        SetGamePass(const char* s) { game_pass = Clean(s);    }
    void        SetMission(const char* s)  { mission = Clean(s);      }
    void        SetGameType(int t)         { game_type = t;           }
    void        SetAdminPort(WORD p)       { admin_port = p;          }
    void        SetLobbyPort(WORD p)       { lobby_port = p;          }
    void        SetGamePort(WORD p)        { game_port  = p;          }
    void        SetPoolsize(int s)         { poolsize = s;            }
    void        SetSessionTimeout(int t)   { session_timeout = t;     }
    void        SetAuthLevel(int n)        { auth_level = n;          }

    void        Load();
    void        Save();

    bool        IsUserBanned(NetUser* user);
    void        BanUser(NetUser* user);

    static void             Initialize();
    static void             Close();
    static NetServerConfig* GetInstance()  { return instance;         }

private:
    void        LoadBanList();
    Text        Clean(const char* s);

    Text        hostname;
    Text        name;
    Text        admin_name;
    Text        admin_pass;
    Text        game_pass;
    Text        mission;

    WORD        admin_port;
    WORD        lobby_port;
    WORD        game_port;
    int         poolsize;
    int         session_timeout;
    int         game_type;
    int         auth_level;

    List<NetAddr>  banned_addrs;
    List<Text>     banned_names;

    static NetServerConfig* instance;
};

#endif  // NetServerConfig_h
