#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "playerstream.h"

#include <string>
#include <vector>

namespace Engine {

class PlayerData {
   public:
    PlayerData(filedesc_t read_fd,
               filedesc_t write_fd,
               filedesc_t err_fd,
               std::string program_name,
               int player_id);

    const std::string& getProgramName() const { return program_name; }
    int getPlayerId() const { return player_id; }

    playerstream& playerStream() { return *player_stream; }
    std::ostream& errorStream() { return *error_stream; }

    PlayerData& operator=(const PlayerData&) = delete;
    PlayerData& operator=(PlayerData&&) = default;
    PlayerData(const PlayerData&) = delete;
    PlayerData(PlayerData&&) = default;

   private:
    std::string program_name;
    int player_id;
    std::unique_ptr<playerstream> player_stream;
    std::unique_ptr<std::ostream> error_stream;
};

struct GameResult {
    enum ResultType { Win, Draw, EngineError };

    ResultType type;
    std::vector<double> player_scores;

    std::string pretty_result;

    static GameResult createWin(const std::vector<PlayerData>& players,
                                const PlayerData& winner,
                                std::string result_details = "");

    static GameResult createDraw(const std::vector<PlayerData>& players,
                                 std::string result_details = "");

    static GameResult createError(const std::vector<PlayerData>& players,
                                  std::string error_details);

   private:
    GameResult();
};

GameResult play_game(std::vector<PlayerData>& players) noexcept;

}  // namespace Engine

#endif  // !ENGINE_H
