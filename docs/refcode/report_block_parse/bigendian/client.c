
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define SERVER_PORT 8000

#include "reportblock.h"

struct report_block buildReportBlock() {
  struct report_block rb;
  rb.ssrc = 0x12345678;
  rb.fractionlost = 0x12;
  rb.totallost = 0x345678;
  rb.seqnumcycles = 0x1234;
  rb.highestseqnum = 0x5678;

  rb.jitter = 0x44444444;
  rb.lastsr = 0x55555555;
  rb.delaysincelastsr = 0x12345678;

  printf("before send: ssrc:0x%x, fractionlost:0x%x, totallost:0x%x, "
      "seqnumcycles:0x%x, highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
      "delaysincelastsr:0x%x\n", rb.ssrc, rb.fractionlost, rb.totallost,
      rb.seqnumcycles, rb.highestseqnum, rb.jitter, rb.lastsr,
      rb.delaysincelastsr);
  return rb;
}

void write_four_bytes_be(uint32_t data, uint8_t *buf) {
  // write 0x12345678 as bigendian;
  uint8_t *p = buf;
  *p = (uint8_t)((data & 0xFF000000) >> 24);
  p++;
  *p = (uint8_t)((data & 0x00FF0000) >> 16);
  p++;
  *p = (uint8_t)((data & 0x0000FF00) >> 8);
  p++;
  *p = (uint8_t)(data & 0x000000FF);
}

void write_three_bytes_be(uint32_t data, uint8_t *buf) {
  // write 0x12345678 as bigendian;
  uint8_t *p = buf;
  *p = (uint8_t)((data & 0x00FF0000) >> 16);
  p++;
  *p = (uint8_t)((data & 0x0000FF00) >> 8);
  p++;
  *p = (uint8_t)(data & 0x000000FF);
}

void serializedReportBlock(const struct report_block rb,
    uint8_t *buffer, int length) {
  printf("serializedReportBlock(), length = %d\n", length);

  // write ssrc;
  uint8_t *p = buffer;
  write_four_bytes_be(rb.ssrc, p);
  p +=4;

  // write fraction_lost;
  *p = (uint8_t)rb.fractionlost;
  printf("ssrc:0x%x\n", *p);
  p++;

  // write total_lost;
  write_three_bytes_be(rb.totallost, p);
  p += 3;

  // write extend_highest_num;
  uint32_t extend_highest_num = (rb.seqnumcycles << 16) | (rb.highestseqnum);
  printf("extend_highest_num:0x%x\n", extend_highest_num);
  write_four_bytes_be(extend_highest_num, p);
  p +=4;


  // write jitter;
  printf("jitter:0x%x\n", rb.jitter);
  write_four_bytes_be(rb.jitter, p);
  p +=4;

  // write lsr;
  printf("lsr:0x%x\n", rb.lastsr);
  write_four_bytes_be(rb.lastsr, p);
  p +=4;

  // write delay_since_lsr;
  printf("delaysincelastlsr:0x%x\n", rb.delaysincelastsr);
  write_four_bytes_be(rb.delaysincelastsr, p);
  p +=4;
}

void serializedReportBlock2(const struct report_block rb,
    uint8_t *buffer, int length) {
  printf("serializedReportBlock(), length = %d\n", length);

  struct report_block *rrbb = (struct report_block*)buffer;
  rrbb->ssrc = htonl(rb.ssrc);
  rrbb->fractionlost = (uint8_t)rb.fractionlost;
  rrbb->totallost = htonl(rb.totallost) >> 8;
  rrbb->seqnumcycles = htons((uint16_t)rb.seqnumcycles);
  rrbb->highestseqnum = htons((uint16_t)rb.highestseqnum);
  rrbb->jitter = htonl(rb.jitter);
  rrbb->lastsr = htonl(rb.lastsr);
  rrbb->delaysincelastsr = htonl(rb.delaysincelastsr);
}

int main(int argc, char *argv[]) {
    int sock_fd = 0;
    char recvbuf[RECV_BUF_SIZE] = {0};
    char sendbuf[SEND_BUF_SIZE] = {0};
    int recvlen = 0;
    int retlen = 0;
    int sendlen = 0;
    int leftlen = 0;
    char *ptr = NULL;
    struct sockaddr_in ser_addr;
	
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", (struct in_addr *)&ser_addr.sin_addr);
    ser_addr.sin_port = htons(SERVER_PORT);
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        printf("%s:%d, create socket failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr))
        < 0) {
        printf("%s:%d, connect socket failed\n", __FILE__, __LINE__);
        exit(1);
    }

     //receive data
     recvlen = 0;
     retlen = 0;
     ptr = recvbuf;
     leftlen = RECV_BUF_SIZE -1;

     // do {
     {
         retlen = recv(sock_fd, ptr, leftlen, 0) ;
         if (retlen < 0) {
             if(errno == EINTR)
               retlen = 0;
             else
               exit(1);
         }
         recvlen += retlen;
         leftlen -= retlen;
         ptr += retlen;
     }
     // } while (recvlen && leftlen);

     printf("receive data is : %s\n", recvbuf);

     // sprintf(sendbuf, "hello server\n");
     struct report_block rb = buildReportBlock();
     // serializedReportBlock(rb, sendbuf, sizeof(rb));
     serializedReportBlock2(rb, sendbuf, sizeof(rb));

     uint32_t *p = (uint32_t*) sendbuf;
     printf("before send in bigendian: "
         "ssrc:0x%x, fractionlost and totallost:0x%x, "
        "seqnumcycles and highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
        "delaysincelastsr:0x%x\n", (uint32_t) *p, (uint32_t) *(p+1),
        (uint32_t) *(p+2), (uint32_t) *(p+3),
        (uint32_t) *(p+4), (uint32_t) *(p+5));

     //send data
     sendlen = strlen(sendbuf) + 1;
     sendlen = sizeof(rb) + 1;
     printf("client sending:%s with length %d\n", sendbuf, sendlen);
     retlen = 0;
     leftlen = sendlen;
     ptr = sendbuf;

     while (leftlen) {
         retlen = send(sock_fd, ptr, sendlen, 0);
         if (retlen < 0) {
             if (errno == EINTR)
               retlen = 0;
             else
               exit(1);
         }
         leftlen -= retlen;
         ptr += retlen;
     }
     close(sock_fd);

     return 0;
}

