/*
 * Assumptions:
 * 1. The judge is not resistant to engine bugs/hack attempts.
 *    It is not safe to run the judge against 3rd-party engines.
 *
 */

#include "common.h"
#include "engine.h"
#include "err.h"

#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

using Engine::GameResult;
using Engine::play_game;
using Engine::PlayerData;
using std::cerr;
using std::cout;
using std::endl;
using std::ostringstream;
using std::string;
using std::vector;

constexpr int NUM_PROGRAMS = 2;

const char* LOG_FOLDER = "logs/";

static void command(const char* cmd) {
    int retcode = system(cmd);
    if (retcode != 0) {
        fprintf(stderr, "command '%s' failed", cmd);
        exit(retcode);
    }
}

static void remove_folder(const char* path) {
    ostringstream cmd;
    cmd << "rm -r " << path;
    command(cmd.str().c_str());
}

static void make_folder(const char* path) {
    ostringstream cmd;
    cmd << "mkdir -p " << path;
    command(cmd.str().c_str());
}

static string get_battle_folder_path(int battle_id) {
    ostringstream folder;
    folder << LOG_FOLDER << battle_id << '/';
    return folder.str();
}

static string get_filename(const string& path) {
    size_t startPos = path.find_last_of('/');
    if (startPos == string::npos)
        startPos = 0;
    else
        startPos++;
    return path.substr(startPos);
}

static string get_battle_stderr_path(int battle_id,
                                     int program_id,
                                     const string& process_name) {
    ostringstream path;
    path << get_battle_folder_path(battle_id) << program_id << "."
         << get_filename(process_name) << ".err";
    return path.str();
}

static void make_battle_folder(int battle_id) {
    make_folder(get_battle_folder_path(battle_id).c_str());
}

static vector<double> play_match(vector<string> programs, int battle_id) {
    assert(programs.size() == NUM_PROGRAMS);
    make_battle_folder(battle_id);
    int read_pipes[NUM_PROGRAMS][2];
    int write_pipes[NUM_PROGRAMS][2];
    int err_files[NUM_PROGRAMS];
    vector<pid_t> children_pids;

    for (int i = 0; i < NUM_PROGRAMS; i++) {
        SYSCALL_WITH_CHECK(pipe(read_pipes[i]));
        SYSCALL_WITH_CHECK(pipe(write_pipes[i]));
        string err_file_path =
            get_battle_stderr_path(battle_id, i, programs[i]);
        cerr << "Creating error file " << err_file_path << endl;
        SYSCALL_WITH_CHECK(err_files[i] =
                               open(err_file_path.c_str(),
                                    O_WRONLY | O_CREAT | O_APPEND, 0640));

        pid_t child_pid;
        unsigned seed = rand();
        switch ((child_pid = fork())) {
            case -1:
                syserr("Error in fork\n");
                exit(1);
            case 0:
                for (int j = 0; j <= i; j++) {
                    SYSCALL_WITH_CHECK(close(read_pipes[j][PIPE_READ_END]));
                    SYSCALL_WITH_CHECK(close(write_pipes[j][PIPE_WRITE_END]));
                }
                for (int j = 0; j <= i - 1; j++) {
                    SYSCALL_WITH_CHECK(close(err_files[j]));
                }

                SYSCALL_WITH_CHECK(close(fileno(stdin)));
                SYSCALL_WITH_CHECK(dup(write_pipes[i][PIPE_READ_END]));

                SYSCALL_WITH_CHECK(close(fileno(stdout)));
                SYSCALL_WITH_CHECK(dup(read_pipes[i][PIPE_WRITE_END]));

                SYSCALL_WITH_CHECK(close(fileno(stderr)));
                SYSCALL_WITH_CHECK(dup(err_files[i]));

                execlp(programs[i].c_str(), programs[i].c_str(),
                       std::to_string(seed).c_str(), nullptr);

                syserr("Cannot use/find the program binary on $PATH: %s\n",
                       programs[i].c_str());

                exit(1);

            default:
                children_pids.push_back(child_pid);
                SYSCALL_WITH_CHECK(close(write_pipes[i][PIPE_READ_END]));
                SYSCALL_WITH_CHECK(close(read_pipes[i][PIPE_WRITE_END]));
        }
    }

    vector<Engine::PlayerData> players;
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        players.emplace_back(read_pipes[i][PIPE_READ_END],
                             write_pipes[i][PIPE_WRITE_END], err_files[i],
                             programs[i], i);
    }

    GameResult result = play_game(players);
    cout << result.pretty_result << endl;

    for (pid_t child_pid : children_pids)
        SYSCALL_WITH_CHECK(kill(child_pid, SIGKILL));

    for (size_t i = 0; i < children_pids.size(); i++)
        wait(nullptr);

    return result.player_scores;
}

template <class T>
vector<T>& operator+=(vector<T>& v1, const vector<T>& v2) {
    for (size_t i = 0; i < std::min(v1.size(), v2.size()); i++) {
        v1[i] += v2[i];
    }
    return v1;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc != NUM_PROGRAMS + 1) {
        fprintf(stderr, "USAGE: %s <program1> <program2>\n", argv[0]);
        return 1;
    }
    playerstream_base::ignore_sigpipe();
    remove_folder(LOG_FOLDER);
    vector<double> match_scores(NUM_PROGRAMS);
    const int reps = 10;
    for (int i = 0; i < reps; i++)
        match_scores += play_match({argv[1], argv[2]}, i);
    cout << "Final scores:" << endl;
    for (int i = 0; i < NUM_PROGRAMS; i++)
        cout << "Bot #" << i << "(" << argv[i + 1] << ") has total score "
             << match_scores[i] << endl;
    return 0;
}
