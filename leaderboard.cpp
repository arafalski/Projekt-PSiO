#include "leaderboard.hpp"

void Leaderboard::update(const std::string &strPlayingTime) {
    std::fstream leaderboardInputFile("leaderboard.txt", std::ios::in);

    if (leaderboardInputFile.is_open()) {
        while (!leaderboardInputFile.eof()) {
            std::string time;
            std::getline(leaderboardInputFile, time);
            bestTimes.emplace_back(std::stof(time));
        }
    }
    leaderboardInputFile.close();

    bestTimes.emplace_back(std::stof(strPlayingTime.substr(0, strPlayingTime.find('.') + 3)));

    std::sort(bestTimes.begin(), bestTimes.end());

    std::fstream leaderboardOutputFile("leaderboard.txt", std::ios::out);
    if(leaderboardOutputFile.is_open()){
        for (unsigned int i = 0; i < bestTimes.size() && i < 5; i++){
            leaderboardOutputFile << bestTimes[i];
            if(i < bestTimes.size() - 1 && i < 4){
                leaderboardOutputFile << '\n';
            }
        }
    }
    leaderboardOutputFile.close();
}

std::vector<float> Leaderboard::getBestTimes() {
    return bestTimes;
}
