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
        // Line
        std::string Line = "";

        // Double quotes delimiter
        std::string delimiter = "\"";

        // Starting position
        size_t Position = 0;

        // Current token
        std::string Token = "";

        // Admin info
        std::vector<std::string> Info;

        // While we can read file lines
        while(std::getline(fp, Line))
        {
            // If line is not empty
            if (!Line.empty())
            {
                // If is not commented
                if (Line[0] != ';')
                {
                    // Find double quote delimiter
                    while ((Position = Line.find(delimiter)) != std::string::npos) 
                    {
                        // Get token inside quotes
                        Token = Line.substr(0, Position);

                        // If token is not empty
                        if (!Token.empty() && !std::all_of(Token.begin(), Token.end(), isspace))
                        {
                            // Insert at vector
                            Info.push_back(Token);
                        }

                        // Erease current token from line
                        Line.erase(0, Position + delimiter.length());
                    }

                    // If has SteamID, Name and Flags in vector
                    if (Info.size() >= 3)
                    {
                        // Insert info on admin data
                        this->m_Data.insert(std::make_pair(Info[0], Info));
                    }

                    // Clear current line info
                    Info.clear();
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
                this->m_Flag[ENTINDEX(pEntity)] |= this->ReadFlags(Admin->second[2].c_str());

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
