/*
 * Copyright (C) 2021+ ChromieCraft <www.chromiecraft.com>
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "Common.h"


#define PVPSTATS_ANNOUNCER_CONF_ENABLED "PvPstatsAnnouncer.Enabled"
#define PVPSTATS_ANNOUNCER_CONF_LIMIT "PvPstatsAnnouncer.Limit"
#define PVPSTATS_ANNOUNCER_CONF_BRACKET_ID "PvPstatsAnnouncer.BracketId"
#define PVPSTATS_ANNOUNCER_CONF_REPEAT_MINUTES "PvPstatsAnnouncer.RepeatMinutes"
#define PVPSTATS_ANNOUNCER_CONF_START_TEXT "PvPstatsAnnouncer.StartText"
#define PVPSTATS_ANNOUNCER_CONF_END_TEXT "PvPstatsAnnouncer.EndText"

class PvPstatsAnnouncer : public WorldScript {
private:
    uint32 time = 0;
public:

    PvPstatsAnnouncer() : WorldScript("PvPstatsAnnouncer") { }

    void OnUpdate(uint32 diff)
    {
        if (!sConfigMgr->GetBoolDefault(PVPSTATS_ANNOUNCER_CONF_ENABLED, false))
        {
            return;
        }

        auto repeatTime = sConfigMgr->GetIntDefault(PVPSTATS_ANNOUNCER_CONF_REPEAT_MINUTES, 40) * MINUTE * IN_MILLISECONDS;
        this->time += diff;

        if (this->time > repeatTime)
        {
            auto limit = static_cast<uint8_t>(sConfigMgr->GetIntDefault(PVPSTATS_ANNOUNCER_CONF_LIMIT, 5));
            auto bracketId = sConfigMgr->GetIntDefault(PVPSTATS_ANNOUNCER_CONF_BRACKET_ID, 1);

            auto stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PVPSTATS_BRACKET_MONTH);
            stmt->setUInt8(0, bracketId);
            stmt->setUInt8(1, limit);

            auto result = CharacterDatabase.Query(stmt);

            if (result)
            {
                sLog->outString("%s", sConfigMgr->GetStringDefault(PVPSTATS_ANNOUNCER_CONF_START_TEXT, "").c_str());

                uint8 i = 1;
                do
                {
                    Field* fields = result->Fetch();
                    if (fields)
                    {
                        sLog->outString("%u. %s - %u", i, fields[2].GetCString(), fields[1].GetUInt32());
                        i++;
                    }
                } while (result->NextRow());

                sLog->outString("%s", sConfigMgr->GetStringDefault(PVPSTATS_ANNOUNCER_CONF_END_TEXT, "").c_str());
            }

            this->time = 0;
        }
    }
};

void AddPvPstatsAnnouncerScripts() {
    new PvPstatsAnnouncer();
}

