/*
 ============================================================================
 * Name        : rtp_config.cpp
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
 * Description : config util implementation file.
 ============================================================================
 */

#include "rtp_config.h"

namespace openrtp {

// TODO(weizhenwei): confirm that is this initialized at compile stage?
RtpThreadMutex *RtpConfig::s_configMutex = new RtpThreadMutex();
RtpConfig *RtpConfig::s_Config = NULL;

RtpConfig::RtpConfig() : m_pConfigFile(NULL) {
}

RtpConfig::RtpConfig(const char *configFile) : m_pConfigFile(configFile) {
}

RtpConfig::~RtpConfig() {
}

void RtpConfig::init() {
    loadConfig(m_pConfigFile);
}

void RtpConfig::loadConfig(const char *configFile) {
    // TODO(weizhenwei): implementation this later!
}

RtpConfig* RtpConfig::singleton() {
    if (!s_Config) {
        s_configMutex->Lock();
        s_Config = new RtpConfig();
        s_configMutex->Unlock();
        return s_Config;
    } else {
        return s_Config;
    }
}

}  // namespace openrtp

