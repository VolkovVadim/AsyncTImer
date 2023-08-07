#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>
#include <algorithm>  // find_if
#include <cctype>     // is_digit

using namespace std;

void log(const string& message) {
    auto timestamp = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream sstream;
    sstream << put_time(localtime(&timestamp), "%d-%m-%Y %H:%M:%S") << " : ";
    sstream << "Thread ID : " << this_thread::get_id() << " : ";
    sstream << message;
    cout << sstream.str() << endl;
}

void help() {
    string header = "Control (type command and press enter):\n" \
        "\tt, toggle - pause or resume the timer\n" \
        "\tq, quit   - close application\n";

    cout << header << endl;
}

class AsyncTimer {
public:
    ~AsyncTimer() {
        is_started = false;
        cv_timer_state_chaged.notify_all();
        if(work_thread.joinable())
            work_thread.join();
    }

    void setInterval(int milliseconds) {
        if(interval != milliseconds) {
            lock_guard<mutex> lock(mtx_state);
            interval = milliseconds;
            cv_timer_state_chaged.notify_all();
        }
        string message = "Timer interval chaged to " + to_string(milliseconds) + " ms";
        log(message);
    }

    bool toggle() {
        lock_guard<mutex> lock(mtx_state);
        is_paused = !is_paused;
        cv_timer_state_chaged.notify_all();
        is_paused ? log("Timer paused") : log("Timer resumed");
        return is_paused;
    }

private:
    void generate_tick() {
        log("Timer started");
        unique_lock<mutex> lock(mtx_state);
        int64_t curr_interval = interval;

        while(is_started) {
            cv_timer_state_chaged.wait(lock, [this](){ return !is_paused || !is_started; });
            while(!is_paused) {
                bool state_changed = cv_timer_state_chaged.wait_for(lock, chrono::milliseconds(curr_interval), [&]{
                    return !is_started || is_paused || curr_interval != interval;
                });

                if(!is_started || is_paused)
                    break;

                if(state_changed) {
                    curr_interval = interval;
                } else {
                    log("Timer tick");
                }
            }
        }

        log("Timer stopped");
    }

    thread work_thread{ [this](){ generate_tick(); }};

    int64_t            interval                = 1000;  // 1 second
    atomic_bool        is_started              = true;
    atomic_bool        is_paused               = false;
    mutex              mtx_state;
    condition_variable cv_timer_state_chaged;
};

int main()
{
    log("Application started");
    help();

    AsyncTimer timer;

    enum command {
        QUIT = 0,
        TOGGLE
    };

    unordered_map<string, command> commands;
    commands["q"]       = QUIT;
    commands["quit"]    = QUIT;
    commands["t"]       = TOGGLE;
    commands["toggle"]  = TOGGLE;

    bool is_application_started = true;
    while(is_application_started) {
        string input;
        getline(cin, input);
        if(input.size() == 0)
            continue;

        bool is_number = find_if(input.begin(), input.end(), [&](auto letter) {
            return !isdigit(letter);
        }) == input.end();

        if(is_number) {
            timer.setInterval(stoll(input));
        } else {
            if(commands.count(input) == 0) {
                string error_message = "Error : invalid command <" + input + ">";
                log(error_message);
                continue;
            }

            switch(commands[input]) {
                case QUIT:
                    is_application_started = false;
                    break;
                case TOGGLE:
                    timer.toggle();
                    break;
            }
        }
    }

    log("Application closed");
    return 0;
}
