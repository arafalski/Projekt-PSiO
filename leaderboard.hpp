#ifndef PROJEKT_LEADERBOARD_HPP
#define PROJEKT_LEADERBOARD_HPP

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

class Leaderboard {
public:
    explicit Leaderboard(const std::string &strPlayingTime);

    std::vector<float> getBestTimes() const;

private:
    std::vector<float> m_bestTimes;
};


#endif //PROJEKT_LEADERBOARD_HPP
