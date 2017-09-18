

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <linux/can.h>
#include <stdlib.h>

int soc;

int open_port(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(soc < 0)
    {
        return (-1);
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);

    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
    {

        return (-1);
    }

    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {

        return (-1);
    }

    return 0;
}


int close_port()
{
    close(soc);
    return 0;
}

int main(void)
{
     open_port("vcan0");
//        struct can_frame *can;
//    can=malloc(sizeof(struct can_frame));
//    can->can_id=1280;
//    can->can_dlc=1;
//    can->data[0]=12;
//    char buf[256] = {};
//    char buf_to_send[256] = {};
//    int nfds, sd, r;
//    fd_set  wr;
//    int connect_init = 1;


    struct can_vesc {
        canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
        __u8    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
        __u8    __pad;   /* padding */
        __u8    __res0;  /* reserved / padding */
        __u8    __res1;  /* reserved / padding */
        __u8    data[CAN_MAX_DLEN] __attribute__((aligned(8)));// CANMAX=8  address aligned on multiple of 8
    };

    struct can_frame frame_wr;
    frame_wr.can_id=1280;
    frame_wr.can_dlc=1;
    frame_wr.__pad=0;
    frame_wr.__res0=0;
    frame_wr.__res1=0;
    frame_wr.data[0]=12;
    int writebyt = 0;
        struct timeval timeout = {1, 0};
        fd_set writeSet;
        FD_ZERO(&writeSet);
        FD_SET(soc, &writeSet);

        if (select((soc + 1), NULL, &writeSet, NULL, &timeout) >= 0) //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
        {
            if (FD_ISSET(soc, &writeSet))
            {
                writebyt = write(soc, &frame_wr, sizeof(struct can_frame));
                if(writebyt)
                {
                    printf("spedito");


                    }

                }
            }
        }



     // for loops ends
//    struct can_frame *can;
//    can=malloc(sizeof(struct can_frame));
//    can->can_id=1280;
//    can->can_dlc=1;
//    can->data[0]=12;
      // open_port("vcan0");
   // send_port(can);
//    open_port("vcan0");
//    read_port();
   // return 0;
//}