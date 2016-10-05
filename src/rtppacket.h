/*
 ============================================================================
 * Name        : rtppacket.h
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
 * Description : header file of rtppacket.h
 ============================================================================
 */

#ifndef SRC_RTPPACKET_H
#define SRC_RTPPACKET_H

#include <list>

#include "rtpheader.h"

namespace openrtp {

class RtpPacket {
public:
    RtpPacket();
    RtpPacket(bool has_padding, bool has_extension, uint8_t csrc_count,
            bool has_marker, uint8_t payload_type, uint16_t sequence_number,
            uint32_t timestamp, uint32_t ssrc, const std::list<uint32_t>& csrc);

    ~RtpPacket();

    // public interface;
    void SetHeaderExtension(rtpheader_extension ext);
    void SetPayload(uint32_t payload_length, uint8_t *payload);

    bool Has_padding() { return m_rtphdr.padding; }
    void Set_padding(bool padding) { m_rtphdr.padding = padding; }

    bool Has_extension() { return m_rtphdr.extension; }
    void Set_extension(bool extension) { m_rtphdr.extension = extension; }

    uint8_t CSRC_count() { return m_rtphdr.csrc_count; }
    void Set_CSRC_count(uint8_t csrc_count) {
        m_rtphdr.csrc_count = csrc_count;
    }

    std::list<uint32_t>& CSRC() { return m_rtphdr.csrc; }
    void Set_CSRC(const std::list<uint32_t>& csrc) {
        m_rtphdr.csrc = csrc;
    }

    bool Has_marker() { return m_rtphdr.marker; }
    void Set_marker(bool marker) { m_rtphdr.marker = marker; }

    uint8_t Payload_type() { return m_rtphdr.payload_type; }
    void Set_payload_type(uint8_t payload_type) {
        m_rtphdr.payload_type = payload_type;
    }

    uint16_t Sequence_number() { return m_rtphdr.sequence_number; }
    void Set_sequence_number(uint16_t sequence_number) {
        m_rtphdr.sequence_number = sequence_number;
    }

    uint32_t Timestamp() { return m_rtphdr.timestamp; }
    void Set_timestamp(uint32_t timestamp) {
        m_rtphdr.timestamp = timestamp;
    }

    uint32_t SSRC() { return m_rtphdr.ssrc; }
    void Set_ssrc(uint32_t ssrc) {
        m_rtphdr.ssrc = ssrc;
    }

private:
    rtpheader m_rtphdr;
    rtpheader_extension m_rtphdr_ext;
    uint32_t m_uPayloadLength;
    uint8_t *m_pPayload;
};

}  // namespace openrtp

#endif  // SRC_RTPPACKET_H

