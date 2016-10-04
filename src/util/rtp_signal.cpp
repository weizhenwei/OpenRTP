/*
 ============================================================================
 * Name        : rtp_signal.cpp
 * Author      : weizhenwei, <weizhenwei1988@gmail.com>
 * Date        : 2016.10.04
 *
 * Copyright (c) 2016, weizhenwei
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the {organization} nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Description : signal processing implementation file.
 ============================================================================
 */

#include <assert.h>
#include <signal.h>

#include <string>

#include "rtp_log.h"

#include "rtp_signal.h"

namespace openrtp {

void RtpRegisterDefaultSignal() {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
}

void RtpRegisterSignalHandler(int sig, RtpSignalHandler pSigHandler) {
    signal(sig, pSigHandler);
}

/*
 *  No Name      Default Action    Description
 *  2  SIGINT    terminate process interrupt program
 *  3  SIGQUIT   create core image quit program
 *  4  SIGILL    create core image illegal instruction
 *  5  SIGTRAP   create core image trace trap
 *  6  SIGABRT   create core image abort program (formerly SIGIOT)
 *  7  SIGEMT    create core image emulate instruction executed
 *  8  SIGFPE    create core image floating-point exception
 *  9  SIGKILL   terminate process kill program
 *  10 SIGBUS    create core image bus error
 *  11 SIGSEGV   create core image segmentation violation
 *  12 SIGSYS    create core image non-existent system call invoked
 *  13 SIGPIPE   terminate process write on a pipe with no reader
 *  14 SIGALRM   terminate process real-time timer expired
 *  15 SIGTERM   terminate process software termination signal
 *  16 SIGURG    discard signal    urgent condition present on socket
 *  17 SIGSTOP   stop process      stop (cannot be caught or ignored)
 *  18 SIGTSTP   stop process      stop signal generated from keyboard
 *  19 SIGCONT   discard signal    continue after stop
 *  20 SIGCHLD   discard signal    child status has changed
 *  21 SIGTTIN   stop process      background read attempted from control terminal
 *  22 SIGTTOU   stop process      background write attempted to control terminal
 *  23 SIGIO     discard signal    I/O is possible on a descriptor (see fcntl(2))
 *  24 SIGXCPU   terminate process cpu time limit exceeded (see setrlimit(2))
 *  25 SIGXFSZ   terminate process file size limit exceeded (see setrlimit(2))
 *  26 SIGVTALRM terminate process virtual time alarm (see setitimer(2))
 *  27 SIGPROF   terminate process profiling timer alarm (see setitimer(2))
 *  28 SIGWINCH  discard signal    Window size change
 *  29 SIGINFO   discard signal    status request from keyboard
 *  30 SIGUSR1   terminate process User defined signal 1
 *  31 SIGUSR2   terminate process User defined signal 2
 */
string RtpSignalToString(int sig) {
    if (sig == SIGHUP) {
        return "SIGHUP";
    } else if (sig == SIGINT) {
        return "SIGINT";
    } else if (sig == SIGQUIT) {
        return "SIGQUIT";
    } else if (sig == SIGILL) {
        return "SIGILL";
    } else if (sig == SIGTRAP) {
        return "SIGTRAP";
    } else if (sig == SIGABRT) {
        return "SIGABRT";
    /*
    } else if (sig == SIGEMT) {  // didn't define on linux;
        return "SIGEMT";
    */
    } else if (sig == SIGFPE) {
        return "SIGFPE";
    } else if (sig == SIGKILL) {
        return "SIGKILL";
    } else if (sig == SIGBUS) {
        return "SIGBUS";
    } else if (sig == SIGSEGV) {
        return "SIGSEGV";
    } else if (sig == SIGSYS) {
        return "SIGSYS";
    } else if (sig == SIGPIPE) {
        return "SIGPIPE";
    } else if (sig == SIGALRM) {
        return "SIGALRM";
    } else if (sig == SIGTERM) {
        return "SIGTERM";
    } else if (sig == SIGURG) {
        return "SIGURG";
    } else if (sig == SIGSTOP) {
        return "SIGSTOP";
    } else if (sig == SIGTSTP) {
        return "SIGTSTP";
    } else if (sig == SIGCONT) {
        return "SIGCONT";
    } else if (sig == SIGCHLD) {
        return "SIGCHLD";
    } else if (sig == SIGTTIN) {
        return "SIGTTIN";
    } else if (sig == SIGTTOU) {
        return "SIGTTOU";
    } else if (sig == SIGIO) {
        return "SIGIO";
    } else if (sig == SIGXCPU) {
        return "SIGXCPU";
    } else if (sig == SIGXFSZ) {
        return "SIGXFSZ";
    } else if (sig == SIGVTALRM) {
        return "SIGVTALRM";
    } else if (sig == SIGPROF) {
        return "SIGPROF";
    } else if (sig == SIGWINCH) {
        return "SIGWINCH";
    /*
    } else if (sig == SIGINFO) {  // didn't define on linux;
        return "SIGINFO";
    */
    } else if (sig == SIGUSR1) {
        return "SIGUSR1";
    } else if (sig == SIGUSR2) {
        return "SIGUSR2";
    } else {
        return "Unknown signal";
    }
}

}  // namespace openrtp

