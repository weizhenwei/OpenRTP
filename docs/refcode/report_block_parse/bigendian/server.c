#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>

#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 8000

#include "reportblock.h"

struct report_block buildReportBlock() {
  struct report_block rb;
  rb.ssrc = 0x11111111;
  rb.fractionlost = 0x12;
  rb.totallost = 0x345678;
  rb.seqnumcycles = 0x2222;
  rb.highestseqnum = 0x3333;

  rb.jitter = 0x44444444;
  rb.lastsr = 0x55555555;
  rb.delaysincelastsr = 0x12345678;

  printf("before send: ssrc:0x%x, fractionlost:0x%x, totallost:0x%x, "
      "seqnumcycles:0x%x, highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
      "delaysincelastsr:0x%x\n", rb.ssrc, rb.fractionlost, rb.totallost,
      rb.seqnumcycles, rb.highestseqnum, rb.jitter, rb.lastsr,
      rb.delaysincelastsr);

}

uint32_t read_bigendian_three_bytes(uint32_t data) {
  uint32_t ret = 0;
  printf("in read_bigendian_three_bytes, "
      "data is 0x%x\n", data);
  // 0x00785634;
  ret = ((data & 0x0000FF) << 16)
    | (((data & 0x00FF00) >> 8) << 8)
    | ((data & 0xFF0000) >> 16);

  printf("in read_bigendian_three_bytes, "
      "after read, ret is 0x%x\n", ret);

  uint32_t ret2 = ntohl(data) >> 8;
  printf("in read_bigendian_three_bytes, "
      "after read, ret2 is 0x%x\n", ret2);

  return ret;
}

uint32_t read_bigendian_two_bytes(uint32_t data) {
  uint32_t ret = 0;
  printf("in read_bigendian_two_bytes, "
      "data is 0x%x\n", data);
  // 0x007856;
  ret = ((data & 0x00FF) << 8)
    | ((data & 0xFF00) >> 8);

  printf("in read_bigendian_two_bytes, "
      "after read, ret is 0x%x\n", ret);

  uint16_t ret2 = ntohs(data);
  printf("in read_bigendian_two_bytes, "
      "after read, ret2 is 0x%x\n", ret2);

  return ret;
}

struct report_block parseReportBlock(uint8_t *buffer, int length) {
  struct report_block *rrbb = (struct report_block*)buffer;
  printf("server received report block: "
      "ssrc:0x%x, fractionlost:0x%x, totallost:0x%x, "
      "seqnumcycles:0x%x, highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
      "delaysincelastsr:0x%x\n", rrbb->ssrc, rrbb->fractionlost,
      rrbb->totallost, rrbb->seqnumcycles, rrbb->highestseqnum,
      rrbb->jitter, rrbb->lastsr, rrbb->delaysincelastsr);

  struct report_block rb;
  rb.ssrc = ntohl(rrbb->ssrc);
  rb.fractionlost = rrbb->fractionlost;
  rb.totallost = read_bigendian_three_bytes(rrbb->totallost);
  rb.seqnumcycles = read_bigendian_two_bytes(rrbb->seqnumcycles);
  rb.highestseqnum = read_bigendian_two_bytes(rrbb->highestseqnum);
  rb.jitter = ntohl(rrbb->jitter);
  rb.lastsr = ntohl(rrbb->lastsr);
  rb.delaysincelastsr = ntohl(rrbb->delaysincelastsr);

   printf("after parse, received report block: "
    "ssrc:0x%x, fractionlost:0x%x, totallost:0x%x, "
    "seqnumcycles:0x%x, highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
    "delaysincelastsr:0x%x\n", rb.ssrc, rb.fractionlost, rb.totallost,
    rb.seqnumcycles, rb.highestseqnum, rb.jitter, rb.lastsr,
    rb.delaysincelastsr);

   return rb;
}

int main(int argc, char *argv[]) {
  int listen_sock = 0;
  int app_sock = 0;
  struct sockaddr_in hostaddr;
  struct sockaddr_in clientaddr;
  int socklen = sizeof(clientaddr);
  char sendbuf[SEND_BUF_SIZE] = {0};
  char recvbuf[RECV_BUF_SIZE] = {0};
  int sendlen = 0;
  int recvlen = 0;
  int retlen = 0;
  int leftlen = 0;
  char *ptr = NULL;
  memset((void *)&hostaddr, 0, sizeof(hostaddr));
  memset((void *)&clientaddr, 0, sizeof(clientaddr));
  hostaddr.sin_family = AF_INET;
  hostaddr.sin_port = htons(LISTEN_PORT);
  hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  listen_sock = socket(AF_INET, SOCK_STREAM, 0);

  if (listen_sock < 0) {
      printf("%s:%d, create socket failed\n", __FILE__, __LINE__);
      exit(1);
  }

  if (bind(listen_sock, (struct sockaddr *)&hostaddr, sizeof(hostaddr))
      < 0) {
      printf("%s:%d, bind socket failed\n", __FILE__, __LINE__);
      exit(1);
  }
  if (listen(listen_sock, MAX_LISTEN_NUM) < 0) {
      printf("%s:%d, listen failed\n", __FILE__, __LINE__);
      exit(1);
  }

  while(1) {
      printf("before accepting...\n");
      app_sock = accept(listen_sock, (struct sockaddr *)&clientaddr,
          &socklen);
      if (app_sock < 0) {
        printf("%s:%d, accept failed\n", __FILE__, __LINE__);
        exit(1);
     }
     // sprintf(sendbuf, "welcome %s:%d here!\n",
     //     clientaddr.sin_addr.s_addr, clientaddr.sin_port);
     sprintf(sendbuf, "welcome %d here!\n", clientaddr.sin_port);
     printf("server sending buffer:%s\n", sendbuf);

     //send data
     sendlen = strlen(sendbuf) + 1;
     retlen = 0;
     leftlen = sendlen;
     ptr = sendbuf;
     while (leftlen) {
         retlen = send(app_sock, ptr, sendlen, 0);
         if (retlen < 0) {
            if(errno == EINTR)
              retlen = 0;
            else
              exit(1);
         }

        leftlen -= retlen;
        ptr += retlen;
     }

     printf("buffer sended\n");
     //receive data
     recvlen = 0;
     retlen = 0;
     ptr = recvbuf;
     leftlen = RECV_BUF_SIZE - 1;
     // do {
     {
         retlen = recv(app_sock, ptr, leftlen, 0) ;
         if(retlen < 0) {
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

     printf("receive data is : %s with length %d\n", recvbuf, recvlen);
     struct report_block rb = parseReportBlock(recvbuf, recvlen);
     printf("out of parse, received report block: "
      "ssrc:0x%x, fractionlost:0x%x, totallost:0x%x, "
      "seqnumcycles:0x%x, highestseqnum:0x%x, jitter:0x%x, lastsr:0x%x, "
      "delaysincelastsr:0x%x\n", rb.ssrc, rb.fractionlost, rb.totallost,
      rb.seqnumcycles, rb.highestseqnum, rb.jitter, rb.lastsr,
      rb.delaysincelastsr);

     close(app_sock);
  }

  close(listen_sock);
  
  return 0;
}

