/*
 ============================================================================
 * Name        : rtp_log.h
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
 * Description : log util header file.
 ============================================================================
 */

#ifndef SRC_UTIL_RTP_LOG_H
#define SRC_UTIL_RTP_LOG_H

#include <string>
#include <sstream>

#include "glog/logging.h"

#include "thread/rtp_thread_mutex.h"

using std::string;
using std::stringstream;


namespace openrtp {
typedef enum {
    RTP_LOG_LEVEL_INFO = 0,
    RTP_LOG_LEVEL_WARNING = 1,
    RTP_LOG_LEVEL_ERROR = 2,
    RTP_LOG_LEVEL_FATAL = 3,
} RTP_LOG_LEVEL_T;

class RtpLog {
public:
    void Log(RTP_LOG_LEVEL_T level, const char *file, int line,
            const string &msg);

    static RtpLog* singleton();

private:
    // constructor and destructor as private to make RtpLog Singleton.
    RtpLog();
    explicit RtpLog(RTP_LOG_LEVEL_T logLevel);
    virtual ~RtpLog();

    RTP_LOG_LEVEL_T m_uLevel;
    void initGLog();

    static RtpThreadMutex s_Mutex;
    static RtpLog* s_Log;
};

#define RTP_LOG(level, msg) \
    do { \
        stringstream strstream; \
        strstream << msg; \
        RtpLog::singleton()->Log(level, __FILE__, __LINE__, strstream.str()); \
    } while (0)

#define RTP_LOG_INFO(msg) RTP_LOG(RTP_LOG_LEVEL_INFO, msg)
#define RTP_LOG_WARNING(msg) RTP_LOG(RTP_LOG_LEVEL_WARNING, msg)
#define RTP_LOG_ERROR(msg) RTP_LOG(RTP_LOG_LEVEL_ERROR, msg)
#define RTP_LOG_FATAL(msg) RTP_LOG(RTP_LOG_LEVEL_FATAL, msg)

#define RTP_CHECK_NOTNULL(ptr) CHECK_NOTNULL(ptr)
}  // namespace openrtp

#endif  // SRC_UTIL_RTP_LOG_H

