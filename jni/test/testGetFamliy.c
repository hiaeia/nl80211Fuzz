#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include "../lnetlink.h"

int main(int argc, char** argv){
    int id = get_famliy_id("nl80211");
    printf("id:%d\n", id);
    return 0;
}
