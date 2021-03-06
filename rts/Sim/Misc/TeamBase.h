/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef TEAM_BASE_H
#define TEAM_BASE_H

#include <string>

#include "System/float3.h"
#include "System/UnorderedMap.hpp"
#include "System/creg/creg_cond.h"


class TeamBase
{
	CR_DECLARE(TeamBase)

public:
	typedef spring::unordered_map<std::string, std::string> customOpts;

	TeamBase();
	virtual ~TeamBase() {}

	void SetValue(const std::string& key, const std::string& value);
	const customOpts& GetAllValues() const { return customValues; }

	const std::string& GetSide() const { return side; }

	void SetStartPos(const float3& pos) { startPos = pos; }
	const float3& GetStartPos() const { return startPos; }

	bool HasValidStartPos() const {
		// if a player never chose (net-sent) a position
		if (startPos == ZeroVector)
			return false;
		// start-positions that are sent across the net
		// will always be clamped to a team's start-box
		// (and hence the map) when clients receive them
		// so this should be redundant
		if (!startPos.IsInBounds())
			return false;

		return true;
	}

	bool HasLeader() const { return (leader != -1); }
	void SetLeader(int leadPlayer) { leader = leadPlayer; }
	int GetLeader() const { return leader; }


	/**
	 * Sets the (dis-)advantage.
	 * The default is 0.0 -> no advantage, no disadvantage.
	 * Common values are [-1.0, 1.0].
	 * Valid values are [-1.0, FLOAT_MAX].
	 * Advantage is a meta value. It can be used
	 * to set multiple (dis-)advantage values simultaneously.
	 * As of now, the incomeMultiplier is the only means of giving an advantage.
	 * Possible extensions: buildTimeMultiplier, losMultiplier, ...
	 *
	 * Note: former handicap/bonus
	 * In lobbies, you will often be able to define this through
	 * a value called handicap or bonus in %.
	 *
	 * @see incomeMultiplier
	 */
	void SetAdvantage(float advantage);

	/// @see incomeMultiplier
	void SetIncomeMultiplier(float incomeMultiplier);
	/// @see incomeMultiplier
	float GetIncomeMultiplier() const { return incomeMultiplier; }

	void SetDefaultColor(int teamNum) {
		for (size_t colorIdx = 0; colorIdx < 3; ++colorIdx) {
			color[colorIdx] = teamDefaultColor[teamNum % NUM_DEFAULT_TEAM_COLORS][colorIdx];
		}

		color[3] = 255;
	}

public:
	/**
	 * Player ID of the player in charge of this team.
	 * The player either controls this team directly,
	 * or an AI running on his computer does so.
	 */
	int leader;
	/**
	 * The team-color in RGB, with values in [0, 255].
	 * The fourth channel (alpha) has to be 255, always.
	 */
	unsigned char color[4];
	unsigned char origColor[4];

	int teamStartNum;
	int teamAllyteam;

	static constexpr int NUM_DEFAULT_TEAM_COLORS = 10;
	static unsigned char teamDefaultColor[NUM_DEFAULT_TEAM_COLORS][4];

protected:
	/**
	 * All the teams resource income is multiplied by this factor.
	 * The default value is 1.0f, the valid range is [0.0, FLOAT_MAX].
	 *
	 * @see #SetAdvantage()
	 */
	float incomeMultiplier;

	/**
	 * Side/Factions name, eg. "ARM" or "CORE".
	 */
	std::string side;

	float3 startPos;

private:
	customOpts customValues;
};

#endif // TEAM_BASE_H
