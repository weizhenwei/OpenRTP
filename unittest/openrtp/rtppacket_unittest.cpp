/*
 ============================================================================
 * Name        : rtppacket_unittest.cpp
 * Author      : weizhenwei, <weizhenwei1988@gmail.com>
 * Date        : 2016.10.05
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
 * Description : test class of CDmdVideoCaptureEngine interface.
 ============================================================================
 */

#include <string.h>

#include <list>
#include <map>
#include <utility>  // for std::pair;

#include "gtest/gtest.h"

#include "rtppacket.h"

namespace openrtp_unittest {

using openrtp::RtpPacket;
using std::map;
using std::pair;

class RtpPacketTest : public testing::Test {
public:
    RtpPacketTest() : rtppacket(false, false, 0, false,
                1, 23456, 2343453212, 232, std::list<uint32_t>()) {
    }

    virtual ~RtpPacketTest() {}

    virtual void SetUp() {}
    virtual void TearDown() {}

public:
    RtpPacket rtppacket;
};

TEST_F(RtpPacketTest, GetFunctions) {
    EXPECT_FALSE(rtppacket.Has_padding());
    EXPECT_FALSE(rtppacket.Has_extension());
    EXPECT_FALSE(rtppacket.Has_marker());

    EXPECT_EQ(0, rtppacket.CSRC_count());
    EXPECT_EQ(1, rtppacket.Payload_type());

    EXPECT_EQ(23456, rtppacket.Sequence_number());
    EXPECT_EQ(2343453212, rtppacket.Timestamp());
    EXPECT_EQ(232, rtppacket.SSRC());
}

}  // namespace openrtp_unittest

