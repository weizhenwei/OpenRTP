/*
 ============================================================================
 * Name        : rtppacket.cpp
 * Author      : weizhenwei, <weizhenwei1988@gmail.com>
 * Date        : 2016.10.03
 *
 * Copyright   :
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
 * Description : implementation file of rtppacket.cpp
 ============================================================================
 */

#include "rtppacket.h"
#include <string.h>

#include <list>

namespace openrtp {

RtpPacket::RtpPacket() {
    memset(&m_rtphdr, 0, sizeof(m_rtphdr));
    memset(&m_rtphdr_ext, 0, sizeof(m_rtphdr_ext));
    m_uPayloadLength = 0;
    m_pPayload = nullptr;
}

RtpPacket::RtpPacket(bool has_padding, bool has_extension,
        uint8_t csrc_count, bool has_marker, uint8_t payload_type,
        uint16_t sequence_number, uint32_t timestamp, uint32_t ssrc,
        const std::list<uint32_t>& csrc) {
    m_rtphdr.version = 2;
    m_rtphdr.padding = has_padding;
    m_rtphdr.extension = has_extension;
    m_rtphdr.csrc_count = csrc_count;
    m_rtphdr.marker = has_marker;
    m_rtphdr.payload_type = payload_type;
    m_rtphdr.sequence_number = sequence_number;
    m_rtphdr.timestamp = timestamp;
    m_rtphdr.ssrc = ssrc;

    if (m_rtphdr.csrc_count > 0) {
        m_rtphdr.csrc = csrc;
    }

    memset(&m_rtphdr_ext, 0, sizeof(m_rtphdr_ext));

    m_uPayloadLength = 0;
    m_pPayload = nullptr;
}

RtpPacket::~RtpPacket() {
    if (m_rtphdr.extension && m_rtphdr_ext.extensions != nullptr) {
        delete [] m_rtphdr_ext.extensions;
        m_rtphdr_ext.extensions = nullptr;
    }

    if (m_pPayload != nullptr) {
        delete [] m_pPayload;
        m_pPayload = nullptr;
    }
}

void RtpPacket::SetHeaderExtension(rtpheader_extension ext) {
    m_rtphdr_ext.profile_specific = ext.profile_specific;
    m_rtphdr_ext.length = ext.length;
    if (ext.length > 0 && ext.extensions != nullptr) {
        m_rtphdr_ext.extensions = new uint32_t[ext.length];
        memcpy(m_rtphdr_ext.extensions, ext.extensions,
                sizeof(uint32_t) * ext.length);
    }
}

void RtpPacket::SetPayload(uint32_t payload_length, uint8_t *payload) {
    m_uPayloadLength = payload_length;
    if (payload_length > 0 && payload != nullptr) {
        m_pPayload = new uint8_t[m_uPayloadLength];
        memcpy(m_pPayload, payload, payload_length);
    }
}

}  // namespace openrtp

