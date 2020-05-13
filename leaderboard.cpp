#include "leaderboard.hpp"

Leaderboard::Leaderboard(const std::string &strPlayingTime) {
    std::fstream leaderboardInputFile("leaderboard.txt", std::ios::in);

    if (leaderboardInputFile.is_open()) {
        while (!leaderboardInputFile.eof()) {
            std::string time;
            std::getline(leaderboardInputFile, time);
            this->m_bestTimes.emplace_back(std::stof(time));
        }
    }
    leaderboardInputFile.close();

    this->m_bestTimes.emplace_back(std::stof(strPlayingTime.substr(0, strPlayingTime.find('.') + 3)));

    std::sort(this->m_bestTimes.begin(), this->m_bestTimes.end());

    while (this->m_bestTimes.size() > 5) {
        this->m_bestTimes.pop_back();
    }

    std::fstream leaderboardOutputFile("leaderboard.txt", std::ios::out);
    if (leaderboardOutputFile.is_open()) {
        for (unsigned int i = 0; i < this->m_bestTimes.size(); i++) {
            leaderboardOutputFile << this->m_bestTimes[i];
            if (i < this->m_bestTimes.size() - 1 && i < 4) {
                leaderboardOutputFile << '\n';
            }
        }
    }
    leaderboardOutputFile.close();
}

std::vector<float> Leaderboard::getBestTimes() const {
    return this->m_bestTimes;
}
