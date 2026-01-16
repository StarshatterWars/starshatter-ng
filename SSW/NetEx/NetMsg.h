/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    User level network message
*/

#ifndef NetMsg_h
#define NetMsg_h

#include <cstdint>


class NetMsg
{
public:
    static const char* TYPENAME() { return "NetMsg"; }

    enum FLAGS { RELIABLE = 0x01, PRIORITY = 0x02, SCATTER = 0x04 };
    enum TYPES { INVALID    = 0,
                 RESERVED   = 0xF0,
                 MULTIPART  = 0xF1
    };
    enum       { MAX_SIZE   = 250 };

    NetMsg(std::uint32_t nid, void* d, int l, std::uint8_t f=0);
    NetMsg(std::uint32_t nid, std::uint8_t type, const char* text, int len, std::uint8_t f=0);
    ~NetMsg();

    int operator == (const NetMsg& m) const { return msgid == m.msgid &&
                                                     netid == m.netid;   }
    int operator <  (const NetMsg& m) const;

    std::uint32_t       Sequence()        const { return msgid;            }
    std::uint32_t       NetID()           const { return netid;            }
    std::uint8_t*       Data()                  { return data;             }
    std::uint8_t        Type()            const { return data ? *data : 0; }
    int                 Length()          const { return len;              }
    std::uint8_t        Flags()           const { return flags;            }

    bool        IsReliable()      const { return flags & RELIABLE ? true : false; }
    bool        IsPriority()      const { return flags & PRIORITY ? true : false; }
    bool        IsScatter()       const { return flags & SCATTER  ? true : false; }

    void        SetSequence(std::uint32_t s)    { msgid = s;               }

private:
    std::uint32_t msgid;
    std::uint32_t netid;
    std::uint8_t* data;
    int   len;
    std::uint8_t  flags;
};


struct NetMsgMultipart {
    std::uint8_t  type;
    std::uint8_t  len;
    std::uint32_t msgid;
    std::uint32_t partno;
    std::uint32_t nparts;
    std::uint8_t  payload[256];
};


#endif  // NetMsg_h
