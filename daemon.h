#ifndef DAEMON_H
#define DAEMON_H

#include <string>

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

using namespace std;


struct sighandler{
    void (*sigterm)(int);
    void (*sigint)(int);
    void (*sighup)(int);
};

class daemond
{
    public:
        daemond();
        daemond(string cmd, string pidfile, sighandler sigh);
        virtual ~daemond();
        void startDaemon(string cmd, string pidfile, sighandler sigh);
    protected:
    private:
        int lockfile(int fd);
        void daemonize(string cmd);
        int running(string pidfile);
};


template <class T>
class iProc {
    public:
        sighandler sigh;
        iProc(){
            iProcMemFn s_term = &T::sigterm;
            iProcMemFn s_int = &T::sigint;
            iProcMemFn s_hup = &T::sighup;
            sigh.sigterm = (iProcFn)s_term;
            sigh.sigint = (iProcFn)s_int;
            sigh.sighup = (iProcFn)s_hup;
        }
        void start(string cmd, string pidfile){
            daemond *dd;
            dd = new daemond(cmd,pidfile,sigh);
        }
        typedef void (T::*iProcMemFn)(int);
        typedef void (*iProcFn)(int);
        virtual void sigterm(){};
        virtual void sigint(){};
        virtual void sighup(){};
};

#endif // DAEMON_H
