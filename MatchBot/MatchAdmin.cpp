#include "precompiled.h"

CMatchAdmin gMatchAdmin;

// On Server activate
void CMatchAdmin::ServerActivate()
{
    // Clear Admin Data
	this->m_Data.clear();

    // Clear Flags Data
    this->m_Flag.clear();

    // Ifstream
    std::ifstream fp;

    // Open admin list file
    fp.open(ADMIN_LIST_FILE, std::ifstream::in);

    // If file was opened
    if (fp.is_open())
    {
        // Admin info struct
        P_ADMIN_INFO Info;

        // While we can read file lines
        for (std::string Line; std::getline(fp, Line);)
        {
            // If line is not empty
            if (!Line.empty())
            {
                // If is not commented
                if (Line[0] != ';')
                {
                    // Get string stream
                    std::stringstream LinePointer(Line);

                    // While pointer can put values into struct
                    while (LinePointer >> Info.Auth >> Info.Name >> Info.Flag)
                    {
                        // Remove Auth quotes
                        Info.Auth.erase(std::remove(Info.Auth.begin(), Info.Auth.end(), '\"' ), Info.Auth.end());

                        // Remove Name quotes
                        Info.Auth.erase(std::remove(Info.Name.begin(), Info.Name.end(), '\"' ), Info.Name.end());

                        // Remove Flag quotes
                        Info.Flag.erase(std::remove(Info.Flag.begin(), Info.Flag.end(), '\"' ), Info.Flag.end());

                        // If SteamID, Name and Flags is not empty
                        if (!Info.Auth.empty() && !Info.Name.empty() && !Info.Flag.empty())
                        {
                            // Insert into admin Data
                            this->m_Data[Info.Auth] = Info;
                        }
                    }
                }
            }
        }

        // Close file at end
        fp.close();
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
    auto Auth = GETPLAYERAUTHID(pEntity);

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
                // And flags is not empty
                if (!Admin->second.Flag.empty())
                {
                    // Set Flags to this entity
                    this->m_Flag[ENTINDEX(pEntity)] |= this->ReadFlags(Admin->second.Flag.c_str());

                    // Return true to allow player connection
                    return true;
                }
            }
        }
    }

    // If admin information is not found, set default user Z Flag
    this->m_Flag[ENTINDEX(pEntity)] |= this->ReadFlags("z");

    // Return true to allow player connection
    return true;
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
