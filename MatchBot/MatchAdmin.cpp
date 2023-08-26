#include "precompiled.h"

CMatchAdmin gMatchAdmin;

// On Server activate
void CMatchAdmin::ServerActivate()
{
    // Clear Admin Data
    this->m_Data.clear();

    // Clear Flags Data
    this->m_Flag.clear();

    // Memory Script instance
    CMemScript* lpMemScript = new CMemScript;

    // If is not null
    if (lpMemScript)
    {
        // Try to load file
        if(lpMemScript->SetBuffer(MB_ADMIN_LIST_FILE))
        {
            try
            {
                // Loop lines
                while (true)
                {
                    // If file content ended
                    if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
                    {
                        // Break loop
                        break;
                    }

                    // Admin Info struct
                    P_ADMIN_INFO Info = { };

                    // Read Name as string
                    Info.Name = lpMemScript->GetString();

                    // Read SteamID as string
                    Info.Auth = lpMemScript->GetAsString();

                    // Read Flags as string
                    Info.Flag = lpMemScript->GetAsString();

                    // Insert on container
                    this->m_Data.insert(std::make_pair(Info.Auth, Info));
                }
            }
            catch (...)
            {
                // Catch for erros
                LOG_CONSOLE(PLID, "[%s] %s", __func__, lpMemScript->GetError().c_str());
            }
        }

        // Delete Memory Script instance
        delete lpMemScript;
    }
}

// Read flags from character array
int CMatchAdmin::ReadFlags(const char* Flags)
{
    // Zero
    int Result = ADMIN_ALL;

    // If is not null
    if (Flags)
    {
        // If is not empty
        if (Flags[0u] != '\0')
        {
            // While has charcaters to read
            while (*Flags)
            {
                // Add a Flag
                Result |= BIT(*Flags++ - 'a');
            }
        }
    }

    // Return flags
    return Result;
}

// Set Flags on Entity Index on connect
bool CMatchAdmin::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
    // If entity is not null
    if (!FNullEnt(pEntity))
    {
        // Entity Index
        auto EntityIndex = ENTINDEX(pEntity);

        // Set empty Flags to user
        this->m_Flag[EntityIndex] = ADMIN_ALL;

        // If admin information is not found, set default user Z Flag
        this->m_Flag[EntityIndex] |= this->ReadFlags("z");

        // Get Player Auth Index
        auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEntity);

        // If is not empty
        if (Auth)
        {
            // If is not empty
            if (Auth[0] != '\0')
            {
                // If found
                if (this->m_Data.find(Auth) != this->m_Data.end())
                {
                    this->m_Flag[EntityIndex] |= this->ReadFlags(this->m_Data[Auth].Flag.c_str());
                }
            }
        }
    }

    // Return true to allow player connection
    return true;
}

// Return admin flags
int CMatchAdmin::GetFlags(int EntityIndex)
{
    return this->m_Flag[EntityIndex];
}

// Check if user has access level
int CMatchAdmin::Access(int EntityIndex, int Level)
{
    // If level is ADMIN_ADMIN
    if (Level == ADMIN_ADMIN)
    {
        // Check if has any admin flag (ADMIN_ADMIN)
        return (this->m_Flag[EntityIndex] > ADMIN_ALL && !(this->m_Flag[EntityIndex] & ADMIN_USER));
    }
    else if (Level == ADMIN_ALL) // If is zero (ADMIN_ALL)
    {
        return 1;
    }

    // Return level check
    return (this->m_Flag[EntityIndex] & Level);
}

// Check access by auth index
int CMatchAdmin::Access(std::string Auth, int Level)
{
    auto Admin = this->m_Data.find(Auth);

    // If found
    if (Admin != this->m_Data.end())
    {
        // Set Flags to this entity
        auto Flags = this->ReadFlags(Admin->second.Flag.c_str());

        // If level is ADMIN_ADMIN
        if (Level == ADMIN_ADMIN)
        {
            // Check if has any admin flag (ADMIN_ADMIN)
            return (Flags > ADMIN_ALL && !(Flags & ADMIN_USER));
        }
        else if (Level == ADMIN_ALL) // If is zero (ADMIN_ALL)
        {
            return 1;
        }

        // Return level check
        return (Flags & Level);
    }

    // Player do not have an access from that level
    return 0;
}

// Get Flags
std::string CMatchAdmin::GetFlags(edict_t* pEdict)
{
    // Get player auth index
    auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

    // If is not empty
    if (Auth)
    {
        // If is not at end
        if (this->m_Data.find(Auth) != this->m_Data.end())
        {
            // Return flags
            return this->m_Data[Auth].Flag;
        }
    }

    // Empty
    return "z";
}
