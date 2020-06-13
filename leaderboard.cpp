#include "leaderboard.hpp"

#include <algorithm>
#include <fstream>

Leaderboard::Leaderboard(const std::string& strPlayingTime) {
    std::fstream leaderboardInputFile("leaderboard.txt", std::ios::in);

    if (leaderboardInputFile.is_open()) {
        while (!leaderboardInputFile.eof()) {
            std::string time;
            std::getline(leaderboardInputFile, time);
            m_bestTimes.emplace_back(std::stof(time));
        }
    }
    leaderboardInputFile.close();

    m_bestTimes.emplace_back(std::stof(strPlayingTime.substr(0, strPlayingTime.find('.') + 3)));

    std::sort(m_bestTimes.begin(), m_bestTimes.end());

    while (m_bestTimes.size() > 5) {
        m_bestTimes.pop_back();
    }

    std::fstream leaderboardOutputFile("leaderboard.txt", std::ios::out);
    if (leaderboardOutputFile.is_open()) {
        for (unsigned int i = 0; i < m_bestTimes.size(); i++) {
            leaderboardOutputFile << m_bestTimes[i];
            if (i < m_bestTimes.size() - 1 && i < 4) {
                leaderboardOutputFile << '\n';
            }
        }
    }
    leaderboardOutputFile.close();
}

std::vector<float> Leaderboard::getBestTimes() const {
    return m_bestTimes;
}
