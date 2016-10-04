/*
 ============================================================================
 * Name        : rtp_thread.cpp
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
 * Description : thread implementation file.
 ============================================================================
 */

#include "rtp_log.h"

#include "rtp_thread.h"

namespace openrtp {

RtpThread::RtpThread() : m_eThreadType(RTP_THREAD_UNKNOWN),
    m_pThreadRoutine(NULL), m_ulThreadHandler(0), m_bThreadSpawned(false) {
}

RtpThread::RtpThread(RtpThreadType eType, RtpThreadRoutine pThreadRoutine)
    : m_eThreadType(eType), m_pThreadRoutine(pThreadRoutine),
    m_ulThreadHandler(0), m_bThreadSpawned(false) {
}

RtpThread::~RtpThread() {
    if (m_bThreadSpawned) {
        pthread_exit(NULL);
    }
    m_bThreadSpawned = false;
}

RTP_RESULT RtpThread::spawnThread() {
    RTP_RESULT ret = RTP_S_OK;
    if (m_bThreadSpawned) {
        RTP_LOG_ERROR("RtpThread::spawnThread(), "
                      << "thread with type " << rtpThreadType[m_eThreadType]
                      << " is already spawned");
        ret = RTP_S_FAIL;
        return ret;
    }

    // spawn thread as detached.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int val = pthread_create(&m_ulThreadHandler, &attr, m_pThreadRoutine, NULL);
    if (val != 0) {
        RTP_LOG_ERROR("RtpThread::spawnThread(), call pthread_create() failed, "
                      << "error number:" << val);
        pthread_attr_destroy(&attr);
        ret = RTP_S_FAIL;
        return ret;
    }
    pthread_attr_destroy(&attr);

    m_bThreadSpawned = true;
    RTP_LOG_INFO("RtpThread::spawnThread(), "
                 << "thread with type " << rtpThreadType[m_eThreadType]
                 << " spawned");
    return ret;
}

}  // namespace openrtp

