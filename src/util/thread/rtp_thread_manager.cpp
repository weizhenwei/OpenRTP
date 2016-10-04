/*
 ============================================================================
 * Name        : rtp_thread_manager.cpp
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
 * Description : thread manager implementation file.
 ============================================================================
 */

#include <signal.h>
#include <pthread.h>

#include "rtp_log.h"
#include "rtp_thread_utils.h"
#include "rtp_datatype.h"

#include "rtp_thread_manager.h"

namespace openrtp {

RtpThreadManager *g_ThreadManager = NULL;
RtpThreadManager *RtpThreadManager::s_ThreadManager = NULL;

RtpThreadManager::RtpThreadManager() {
}

RtpThreadManager::~RtpThreadManager() {
    if (0 != m_listThreadList.size()) {
        m_listThreadList.clear();
    }
}

RTP_RESULT RtpThreadManager::addThread(RtpThreadType eType,
        RtpThreadRoutine pRoutine) {
    RTP_RESULT ret = RTP_S_OK;

    RtpThread *pThread = getThread(eType);
    if (NULL != pThread) {
        RTP_LOG_ERROR("RtpThreadManager::addThread(), "
                << "thread with type " << rtpThreadType[eType]
                << " already added to thread manager");
        ret = RTP_S_FAIL;
        return ret;
    }

    pThread = new RtpThread(eType, pRoutine);

    m_mtxThreadManagerMutex.Lock();
    m_listThreadList.push_back(pThread);
    m_mtxThreadManagerMutex.Unlock();

    return ret;
}

RtpThread *RtpThreadManager::getThread(RtpThreadType eType) {
    RtpThreadListIterator iter;
    for (iter = m_listThreadList.begin(); iter != m_listThreadList.end();
            iter++) {
        if (eType == (*iter)->getThreadType()) {
            return *iter;
        }
    }

    return NULL;
}

RTP_RESULT RtpThreadManager::spawnThread(RtpThreadType eType) {
    RTP_RESULT ret = RTP_S_OK;
    RtpThread *pThread = getThread(eType);
    if (NULL == pThread) {
        RTP_LOG_ERROR("RtpThreadManager::spawnThread(), "
                << "thread with type " << rtpThreadType[eType]
                << " is not added to thread manager yet");
        ret = RTP_S_FAIL;
        return ret;
    }

    if (pThread->isThreadSpawned()) {
        RTP_LOG_ERROR("RtpThreadManager::spawnThread(), "
                << "thread with type " << rtpThreadType[eType]
                << " is already spawned");
        ret = RTP_S_FAIL;
        return ret;
    }

    ret = pThread->spawnThread();
    return ret;
}

RTP_RESULT RtpThreadManager::spawnAllThreads() {
    RTP_RESULT ret = RTP_S_OK;
    RtpThread *pThread = NULL;
    RtpThreadListIterator iter;
    for (iter = m_listThreadList.begin(); iter != m_listThreadList.end();
            iter++) {
        pThread = *iter;

        RtpThreadType eType = pThread->getThreadType();
        if (pThread->isThreadSpawned()) {
            RTP_LOG_WARNING("RtpThreadManager::spawnAllThreads(), "
                    << "thread with type " << rtpThreadType[eType]
                    << " is already spawned");
            continue;
        }

        if (RTP_S_OK != (ret = pThread->spawnThread())) {
            return ret;
        }
    }

    return ret;
}

RTP_RESULT RtpThreadManager::killThread(RtpThreadType eType) {
    RTP_RESULT ret = RTP_S_OK;
    RtpThread *pThread = getThread(eType);
    if (NULL == pThread) {
        RTP_LOG_ERROR("RtpThreadManager::killThread(), "
                << "thread with type " << rtpThreadType[eType]
                << " is not added to thread manager yet");
        ret = RTP_S_FAIL;
        return ret;
    }

    if (!pThread->isThreadSpawned()) {
        RTP_LOG_ERROR("RtpThreadManager::killThread(), "
                << "thread with type " << rtpThreadType[eType]
                << " is not spawned yet");
        ret = RTP_S_FAIL;
        return ret;
    }

    int val = -1;
    RtpThreadHandler handler = pThread->getThreadHandler();
    if (0 != (val = pthread_kill(handler, SIGUSR1))) {
        RTP_LOG_ERROR("RtpThreadManager::killThread(), "
                << "could not to send SIGUSR1 signal to "
                << "thread with type " << rtpThreadType[eType]
                << ", error number:" << val);
        ret = RTP_S_FAIL;
        return ret;
    }

    if (0 != (val = pthread_join(handler, NULL))) {
        RTP_LOG_ERROR("RtpThreadManager::killThread(), "
                << "could not call pthread_join, error number:" << val);
        ret = RTP_S_FAIL;
        return ret;
    }

    return ret;
}

RTP_RESULT RtpThreadManager::killAllThreads() {
    RTP_RESULT ret = RTP_S_OK;
    RtpThread *pThread = NULL;
    RtpThreadListIterator iter;
    for (iter = m_listThreadList.begin(); iter != m_listThreadList.end();
            iter++) {
        pThread = *iter;

        RtpThreadType eType = pThread->getThreadType();
        if (!pThread->isThreadSpawned()) {
            RTP_LOG_ERROR("RtpThreadManager::killAllThreads(), "
                    << "thread with type " << rtpThreadType[eType]
                    << " is not spawned yet");
            ret = RTP_S_FAIL;
            return ret;
        }
#if 0
        int val = -1;
        RtpThreadHandler handler = pThread->getThreadHandler();
        if (0 != (val = pthread_kill(handler, SIGUSR1))) {
            RTP_LOG_ERROR("RtpThreadManager::killAllThreads(), "
                    << "could not to send SIGINT signal to "
                    << "thread with type " << rtpThreadType[eType]
                    << ", error number:" << val);
            ret = RTP_S_FAIL;
            return ret;
        }

        if (0 != (val = pthread_join(handler, NULL))) {
            RTP_LOG_ERROR("RtpThreadManager::killAllThreads(), "
                    << "could not call pthread_join, error number:" << val);
            ret = RTP_S_FAIL;
            return ret;
        }
#endif
    }

    return ret;
}

void RtpThreadManager::cleanThread(RtpThreadType eType) {
    RtpThreadListIterator iter;
    for (iter = m_listThreadList.begin(); iter != m_listThreadList.end();
            iter++) {
        if (eType == (*iter)->getThreadType()) {
            m_listThreadList.erase(iter);
            break;
        }
    }  // for
}

void RtpThreadManager::cleanAllThreads() {
    if (0 != m_listThreadList.size()) {
        m_listThreadList.clear();
    }
}

RtpThreadManager *RtpThreadManager::singleton() {
    if (NULL == s_ThreadManager) {
        s_ThreadManager = new RtpThreadManager();
    }

    return s_ThreadManager;
}

}  // namespace openrtp

