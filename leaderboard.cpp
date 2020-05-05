#include "leaderboard.hpp"

Leaderboard::Leaderboard(const std::string &strPlayingTime) {
    std::fstream leaderboardInputFile("leaderboard.txt", std::ios::in);

    if (leaderboardInputFile.is_open()) {
        while (!leaderboardInputFile.eof()) {
            std::string time;
            std::getline(leaderboardInputFile, time);
            this->bestTimes.emplace_back(std::stof(time));
        }
    }
    leaderboardInputFile.close();

    this->bestTimes.emplace_back(std::stof(strPlayingTime.substr(0, strPlayingTime.find('.') + 3)));

    std::sort(this->bestTimes.begin(), this->bestTimes.end());

    while(this->bestTimes.size() > 5){
        this->bestTimes.pop_back();
    }

    std::fstream leaderboardOutputFile("leaderboard.txt", std::ios::out);
    if(leaderboardOutputFile.is_open()){
        for (unsigned int i = 0; i < this->bestTimes.size(); i++){
            leaderboardOutputFile << this->bestTimes[i];
            if(i < this->bestTimes.size() - 1 && i < 4){
                leaderboardOutputFile << '\n';
            }
        }
    }
    leaderboardOutputFile.close();
}

std::vector<float> Leaderboard::getBestTimes() const {
    return this->bestTimes;
}
