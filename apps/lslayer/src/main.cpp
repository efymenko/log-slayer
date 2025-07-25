#include "Context.hpp"
#include "EventHandler.hpp"
#include "Recorder.hpp"
#include "StatisticsPrinter.hpp"
#include "common/CommunicationTable.hpp"
#include "common/SharedMemory.hpp"
#include "common/SharedMemoryGuard.hpp"

#include <cctype>
#include <csignal>
#include <termios.h>
#include <unistd.h>

std::atomic<bool> _isInterrupt = false;

void sigint_handler(int) {
    _isInterrupt = true;
}

int main(int argc, char** argv) {
    using namespace log_slayer;

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    ::sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    ::sigaction(SIGINT, &sa, NULL);


    ::termios oldt, newt;
    ::tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    ::tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cout << "Press (E - exit), (R - record), (S - save) and Ctrl+C to quit..." << std::endl;

    auto sharedMemoryName = log_slayer::options::SHARED_MEMORY_NAME;
    SharedMemoryGuard sharedMemoryGuard(sharedMemoryName);
    CommunicationTable& communicationTable = *log_slayer::SharedMemory::create(sharedMemoryName);
    Context context{ communicationTable };

    Recorder recorder;
    EventHandler eventHandler(context, recorder);
    StatitsticsPrinter statisticsPrinter(context);

    while (!_isInterrupt) {
        char ch;
        errno = 0;
        std::string message;
        if (::read(STDIN_FILENO, &ch, 1) > 0) {
            switch (std::tolower(ch)) {
                case 'r': 
                    recorder.startRecord(message)
                        ? std::cout << "\033[32m[✓] " << message << "\033[0m" << std::endl
                        : std::cout << "\033[33m[!] " << message << "\033[0m" << std::endl;
                    break;
                case 's':
                    recorder.saveRecord(message)
                        ? std::cout << "\033[32m[✓] " << message << "\033[0m" << std::endl
                        : std::cout << "\033[33m[!] " << message << "\033[0m" << std::endl;
                    break;
                case 'e':
                    _isInterrupt = true;
                    break;
            }
        } else {
            if (errno == EINTR) {
                continue;
            } else {
                perror("read");
            }
        }
    }

    std::cout << "Bye bye..." << std::endl;
    ::tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

