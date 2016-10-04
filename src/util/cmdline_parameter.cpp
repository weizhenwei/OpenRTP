/*
 ============================================================================
 * Name        : cmdline_parameter.cpp
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
 * Description : implementation of cmdline parameter operation.
 ============================================================================
 */

#include "cmdline_parameter.h"

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include <cstdio>
#include <cstdlib>
#include <string>

#include "glog/logging.h"

#include "rtp_log.h"


namespace openrtp {

CmdlineParameter *CmdlineParameter::s_pCmdlineParameter
    = new CmdlineParameter();

CmdlineParameter::CmdlineParameter() : m_bValidParameter(true),
    m_bShowHelp(false), m_bShowVersion(false), m_bDaemonize(false),
    m_sProgramName(new std::string("openRTP")), m_sPidFile(NULL),
    m_sCfgFile(NULL) {
}

CmdlineParameter::~CmdlineParameter() {
    if (m_sProgramName) {
        delete m_sProgramName;
        m_sProgramName = NULL;
    }
    if (m_sPidFile) {
        delete m_sPidFile;
        m_sPidFile = NULL;
    }
    if (m_sCfgFile) {
        delete m_sCfgFile;
        m_sCfgFile = NULL;
    }
}

CmdlineParameter *CmdlineParameter::singleton() {
    RTP_CHECK_NOTNULL(s_pCmdlineParameter);
    return s_pCmdlineParameter;
}

void CmdlineParameter::showHelp() {
    fprintf(stdout, "Usage:%s [OPTION...]\n", m_sProgramName->c_str());
    fprintf(stdout, "  -p, --pid-file=FILE,    Use specified pid file\n");
    fprintf(stdout, "  -f, --cfg-file=FILE,    Use specified config file\n");
    fprintf(stdout, "  -d, --daemonize,        Run openrtp in daemon mode\n");
    fprintf(stdout, "  -v, --version,          Display the version number\n");
    fprintf(stdout, "  -h, --help,             Display this help message\n");
}

void CmdlineParameter::showVersion() {
    // TODO(weizhenwei): Add more copyright message here.
    fprintf(stdout, "OpenRTP 0.0.1\n");
}

void CmdlineParameter::daemonize() {
    pid_t pid;
    int fd0, fd1, fd2;
    struct rlimit r1;
    struct sigaction sa;
    // FILE *fp = NULL;

    // clear file creation mask.
    umask(0);

    // get maximum numbers of file descriptors.
    if (getrlimit(RLIMIT_NOFILE, &r1) == -1) {
        RTP_LOG_ERROR("getrlimit-file limit error: " \
                << strerror(errno) << "\n");
        exit(EXIT_FAILURE);
    }

    // fork and parent exits.
    if ((pid = fork()) == -1) {
        RTP_LOG_ERROR("fork error: " << strerror(errno) << "\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {  // parent
        exit(EXIT_SUCCESS);
    }

    // the folloing is runing by child.

    // become a session leader to lose controlling  TTY.
    if ((pid = setsid()) == -1) {
        RTP_LOG_ERROR("setsid error: " << strerror(errno) << "\n");
        exit(EXIT_FAILURE);
    }

    // set SIGHUP sighandler;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        RTP_LOG_ERROR("sigaction error.\n");
        exit(EXIT_FAILURE);
    }

    // fork again, the grandchild alive since then.
    if ((pid = fork()) == -1) {
        RTP_LOG_ERROR("fork 2 error: " << strerror(errno) << "\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {  // parent
        exit(EXIT_SUCCESS);
    }

    // change the current working directory to the root
    // so we won't prevent file systems from being unmounted.
    if (chdir("/") == -1) {
        RTP_LOG_ERROR("chdir to / error: " << strerror(errno) << "\n");
        exit(EXIT_FAILURE);
    }

    // close all open file descriptors.
    if (r1.rlim_max == RLIM_INFINITY) {
        r1.rlim_max = 1024;
    }
    for (unsigned int i = 0; i < r1.rlim_max; i++) {
        close(i);
    }

    // attach file descriptors 0, 1, and 2 to /dev/null.
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        RTP_LOG_ERROR("unexpected file descriptors: " \
                << fd0 << " " << fd1 << " " << fd2 << "\n");
        exit(EXIT_FAILURE);
    }

#if 0
    // write pid to file
    pid = getpid();
    global.pid = pid;  // refresh global's pid member;
    if ((fp = fopen(global.pid_file, "w")) == NULL) {
        rtp_log(LOG_ERR, "fopen pid file error:%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d", pid);
    fclose(fp);
#endif
}

void CmdlineParameter::parseCmdlineParameter(int argc, char *argv[]) {
    if (m_sProgramName) {
        delete m_sProgramName;
        m_sProgramName = NULL;
    }
    m_sProgramName = new std::string(argv[0]);

    struct option long_options[] = {
        {"pid-file",           required_argument,  0, 'p'},
        {"cfg-file",           required_argument,  0, 'f'},
        {"daemonize",          no_argument,        0, 'd'},
        {"version",            no_argument,        0, 'v'},
        {"help",               no_argument,        0, 'h'},
        {0, 0, 0, 0},
    };

    // man 3 getopt_long for more information about getopt_long;
    int c;
    while ((c = getopt_long(argc, argv, "p:f:dvh", long_options, NULL))
            != EOF) {
        switch (c) {
            case 'v':
                m_bShowVersion = true;
                break;
            case 'h':
                m_bShowHelp = true;
                break;
            case 'd':
                m_bDaemonize = true;
                break;
            case 'p':
                m_sPidFile = new std::string(optarg);
                break;
            case 'f':
                m_sCfgFile = new std::string(optarg);
                break;
            default:
                m_bValidParameter = false;
                break;
        }
    }  // while
}

}  // namespace openrtp

