#include "precompiled.h"

CMatchAdmin gMatchAdmin;

// On Server activate
void CMatchAdmin::ServerActivate()
{
    // Clear Admin Data
	this->m_Data.clear();

    // Clear Flags Data
    this->m_Flag.clear();

    // File stream
    std::ifstream fp(MB_ADMIN_LIST_FILE);

    try
    {
        // Reset pointer
        fp.clear();

        // Go to begin of file
        fp.seekg(0, std::ios::beg);

        // Read data from json file
        auto json = nlohmann::json::parse(fp, nullptr, true, true);

        // Loop each item of array
        for (auto const& row : json.items())
        {
            // Get admin data as map string
            auto Admin = row.value().get<std::map<std::string, std::string>>();

            // If Auth and Flags fields is here
            if (!Admin["Auth"].empty() && !Admin["Flags"].empty())
            {
                // Insert info on admin data
                this->m_Data.insert(std::make_pair(Admin["Auth"], Admin["Flags"]));
            }
        }
    }
    catch (nlohmann::json::parse_error& e)
    {
        LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
    }
}

// Read flags from character array
int CMatchAdmin::ReadFlags(const char* c)
{
    // Zero
    int Flags = ADMIN_ALL;

    // While has charcaters to read
    while (*c)
    {
        // Add a Flag
        Flags |= BIT(*c++ - 'a');
    }

    // Return flags
    return Flags;
}

// Set Flags on Entity Index on connect
bool CMatchAdmin::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
    // Add empty Flags to user
    this->m_Flag[ENTINDEX(pEntity)] = ADMIN_ALL;

    // Get Player Auth Index
    auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEntity);

    // If is not empty
    if (Auth)
    {
        // If is not empty
        if (Auth[0] != '\0')
        {
            // Find admin Information
            auto Admin = this->m_Data.find(Auth);

            // If found
            if (Admin != this->m_Data.end())
            {
                // Set Flags to this entity
                this->m_Flag[ENTINDEX(pEntity)] |= this->ReadFlags(Admin->second.c_str());

                // Return true to allow player connection
                return true;
            }
        }
    }

    // If admin information is not found, set default user Z Flag
    this->m_Flag[ENTINDEX(pEntity)] |= this->ReadFlags("z");

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
