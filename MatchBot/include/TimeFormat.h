#pragma once

class CTimeFormat
{
public:
	static const size_t TIMEUNIT_SECONDS = 0;
	static const size_t TIMEUNIT_MINUTES = 1;
	static const size_t TIMEUNIT_HOURS = 2;
	static const size_t TIMEUNIT_DAYS = 3;
	static const size_t TIMEUNIT_WEEKS = 4;
	static const size_t TIMEUNIT_MONTHS = 5;
	static const size_t TIMEUNIT_YEARS = 6;

	static std::string GetTimeLength(long unitCnt, const size_t TimeUnit)
	{
		if (unitCnt > 0)
		{
			const std::vector<long> Seconds_In_Time = { 1, 60, 3600, 86400, 604800, 2592000, 31536000 };

			const std::array<std::string, TIMEUNIT_YEARS + 1> TimeUnitText = { "Second", "Minute", "Hour", "Day", "Week", "Month", "Year" };

			std::array<time_t, TIMEUNIT_YEARS + 1> UnitCount;

			UnitCount[TIMEUNIT_SECONDS] = (TimeUnit == TIMEUNIT_SECONDS) ? unitCnt : (unitCnt * Seconds_In_Time[TimeUnit]);

			std::array<std::string, TIMEUNIT_YEARS + 1> timeElement;

			auto maxElementIdx = -1;

			for (auto i = TIMEUNIT_YEARS + 1; i-- > TIMEUNIT_SECONDS; )
			{
				UnitCount[i] = UnitCount[TIMEUNIT_SECONDS] / Seconds_In_Time[i];
				UnitCount[TIMEUNIT_SECONDS] -= (UnitCount[i] * Seconds_In_Time[i]);

				if (UnitCount[i] > 0)
				{
					++maxElementIdx;

					timeElement[maxElementIdx] = std::to_string(UnitCount[i]);
					timeElement[maxElementIdx] += '\40';
					timeElement[maxElementIdx] += TimeUnitText[i];

					if (UnitCount[i] > 1)
					{
						timeElement[maxElementIdx] += "s";
					}
				}
			}

			switch (maxElementIdx)
			{
				case 0: return timeElement[0];
				case 1: return timeElement[0] + " " + "and" + " " + timeElement[1];
				case 2: return timeElement[0] + ", " + timeElement[1] + " " + "and" + " " + timeElement[2];
				case 3: return timeElement[0] + ", " + timeElement[1] + ", " + timeElement[2] + " " + "and" + " " + timeElement[3];
				case 4: return timeElement[0] + ", " + timeElement[1] + ", " + timeElement[2] + ", " + timeElement[3] + " " + "and" + " " + timeElement[4];
				case 5: return timeElement[0] + ", " + timeElement[1] + ", " + timeElement[2] + ", " + timeElement[3] + ", " + timeElement[4] + " " + "and" + " " + timeElement[5];
				case 6: return timeElement[0] + ", " + timeElement[1] + ", " + timeElement[2] + ", " + timeElement[3] + ", " + timeElement[4] + ", " + timeElement[5] + " " + "and" + " " + timeElement[6];
			}
		}

		return "Permanently";
	}
};
