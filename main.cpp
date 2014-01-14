#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <syslog.h>
#include <unistd.h>

#include "daemon.h"

using namespace std;

class proc_d : public iProc<proc_d>{
    public:
        proc_d():iProc<proc_d>(){}

        void sigterm(int signo){
            syslog(LOG_INFO, "got SIGTERM; exiting");
            exit(0);
        }

        void sigint(int signo){
            syslog(LOG_INFO, "got SIGINT; exiting");
            exit(0);
        }

        void sighup(int signo){
            syslog(LOG_INFO, "Re-reading configuration file");
        }
};

int main(int argc, char **argv){
    string cmd;
    string pidfile = "/var/run/daemon/daemon.pid";
    string args(argv[0]);
    size_t found = args.rfind('/');
    if ( found != string::npos ){
        cmd = args.substr(found+1);
    }else{
        cmd = string(argv[0]);
    }

    proc_d pd;
    pd.start(cmd,pidfile);
    while(1){
        sleep(30);
    }
    exit(0);
}
