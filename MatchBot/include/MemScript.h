#pragma once

enum class eTokenResult
{
	TOKEN_NUMBER = 0,
	TOKEN_STRING = 1,
	TOKEN_END = 2,
	TOKEN_ERROR = 3
};

class CMemScript
{
public:
	CMemScript()
	{
		this->m_Buff.clear();
		this->m_Path.clear();
		this->m_Code.clear();

		this->m_Count = 0;
		this->m_Timer = 0;

		this->m_Number = 0.0f;
		this->m_String.clear();

		this->SetError(0);
	};

	virtual ~CMemScript()
	{
		this->m_Buff.clear();
		this->m_Path.clear();
		this->m_Code.clear();

		this->m_Count = 0;
		this->m_Timer = 0;

		this->m_Number = 0.0f;
		this->m_String.clear();
	};

	bool SetBuffer(const std::string& Path)
	{
		this->m_Path = Path;

		std::ifstream File(Path, std::ios::binary);

		if (!File)
		{
			this->SetError(1);
			return false;
		}

		this->m_Buff.clear();

		File.seekg(0, std::ios::end);

		this->m_Buff.reserve(File.tellg());

		File.seekg(0, std::ios::beg);

		this->m_Buff.assign((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());

		if (this->m_Buff.empty())
		{
			this->SetError(2);
			return false;
		}

		this->m_Count = 0;

		this->m_Timer = std::time(NULL);

		return true;
	};

	std::string GetBuffer()
	{
		if (this->m_Buff.empty())
		{
			this->SetError(3);
		}

		return this->m_Buff;
	};

	char GetChar()
	{
		if (this->m_Count >= this->m_Buff.size())
		{
			return -1;
		}

		return this->m_Buff[this->m_Count++];
	}

	void UnGetChar(char ch)
	{
		if (this->m_Count == 0)
		{
			return;
		}

		this->m_Buff[--this->m_Count] = ch;
	}

	char CheckComment(char ch)
	{
		if (ch != '/' || (ch = this->GetChar()) != '/')
		{
			return ch;
		}

		while (true)
		{
			if ((ch = this->GetChar()) == -1)
			{
				return ch;
			}

			if (ch == '\n')
			{
				return ch;
			}
		}

		return ch;
	}

	eTokenResult GetToken()
	{
		if ((std::time(NULL) - this->m_Timer) > 0)
		{
			this->SetError(4);
			throw 1;
		}

		this->m_Number = 0.0f;

		this->m_String.clear();

		char ch;

		while (true)
		{
			if ((ch = this->GetChar()) == -1)
			{
				return eTokenResult::TOKEN_END;
			}

			if (std::isspace(ch) != 0)
			{
				continue;
			}

			if ((ch = this->CheckComment(ch)) == -1)
			{
				return eTokenResult::TOKEN_END;
			}
			else if (ch != '\n')
			{
				break;
			}
		}

		if (ch == '-' || ch == '.' || ch == '*' || std::isdigit(ch) != 0)
		{
			return this->GetTokenNumber(ch);
		}

		if (ch == '"')
		{
			return this->GetTokenString(ch);
		}

		return this->GetTokenCommon(ch);
	};

	eTokenResult GetTokenNumber(char ch)
	{
		this->UnGetChar(ch);

		while ((ch = this->GetChar()) != -1 && (ch == '-' || ch == '.' || ch == '*' || std::isdigit(ch) != 0))
		{
			this->m_String.push_back(ch);
		}

		if (this->m_String.compare("*") == 0)
		{
			this->m_Number = -1.0f;
		}
		else
		{
			this->m_Number = (float)std::atof(this->m_String.c_str());
		}

		return eTokenResult::TOKEN_NUMBER;
	};

	eTokenResult GetTokenString(char ch)
	{
		while ((ch = this->GetChar()) != -1 && ch != '"')
		{
			this->m_String.push_back(ch);
		}

		if (ch != '"')
		{
			this->UnGetChar(ch);
		}

		return eTokenResult::TOKEN_STRING;
	};

	eTokenResult GetTokenCommon(char ch)
	{
		if (std::isalpha(ch) == 0)
		{
			return eTokenResult::TOKEN_ERROR;
		}

		this->m_String.push_back(ch);

		while ((ch = this->GetChar()) != -1 && (ch == '.' || ch == '_' || std::isalnum(ch) != 0))
		{
			this->m_String.push_back(ch);
		}

		this->UnGetChar(ch);

		return eTokenResult::TOKEN_STRING;
	}

	int GetNumber()
	{
		return (int)this->m_Number;
	}

	int GetAsNumber()
	{
		this->GetToken();

		return (int)this->m_Number;
	}

	float GetFloatNumber()
	{
		return this->m_Number;
	}

	float GetAsFloatNumber()
	{
		this->GetToken();

		return this->m_Number;
	}

	std::string GetString()
	{
		return this->m_String;
	}

	std::string GetAsString()
	{
		this->GetToken();

		return this->m_String;
	}
	
	void SetError(int Error)
	{
		switch (Error)
		{
			case 0:
			{
				this->m_Code = "[" + this->m_Path + "] The file were not configured correctly.";
				break;
			}
			case 1:
			{
				this->m_Code = "[" + this->m_Path + "] Could not open file.";
				break;
			}
			case 2:
			{
				this->m_Code = "[" + this->m_Path + "] Could not alloc file buffer.";
				break;
			}
			case 3:
			{
				this->m_Code = "[" + this->m_Path + "] Could not get file buffer.";
				break;
			}
			case 4:
			{
				this->m_Code = "[" + this->m_Path + "] The file were not configured correctly.";
				break;
			}
		}
	};

	std::string GetError()
	{
		return this->m_Code;
	};

private:
	std::string m_Buff;
	std::string m_Path;
	std::string m_Code;
	size_t m_Count = 0;
	time_t m_Timer = 0;
	float m_Number = 0.0f;
	std::string m_String;
};