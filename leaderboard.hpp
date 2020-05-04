#ifndef PROJEKT_LEADERBOARD_HPP
#define PROJEKT_LEADERBOARD_HPP

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

class Leaderboard {
    static std::vector<float> bestTimes;
public:
    static void update(const std::string &strPlayingTime);
    static std::vector<float> getBestTimes();
};


#endif //PROJEKT_LEADERBOARD_HPP
