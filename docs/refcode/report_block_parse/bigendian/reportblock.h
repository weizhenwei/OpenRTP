#ifndef REPORT_BLOCK_H
#define REPORT_BLOCK_H

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>

struct report_block {
  uint32_t ssrc;
  uint32_t fractionlost:8;
  uint32_t totallost:24;
  uint32_t seqnumcycles:16;
  uint32_t highestseqnum:16;
  uint32_t jitter;
  uint32_t lastsr;
  uint32_t delaysincelastsr;
};

#endif

