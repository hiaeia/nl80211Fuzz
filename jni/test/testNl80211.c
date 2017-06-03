#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include "../lnetlink.h"

int main(int argc, char** argv){
    int fd, pid, ret;
    unsigned char buf[8192];
    struct message msg;
    struct sockaddr_nl nl_addr, nl_keeper;
    struct nlattr *nla;
    unsigned int tlen;

    pid = getpid();
    memset(&nl_addr, 0, sizeof(nl_addr));
    memset(&nl_keeper, 0, sizeof(nl_keeper));
    //nl_addr = {AF_NETLINK, pid, 0};
    //nl_keeper = {AF_NETLINK, 0, 0};
    nl_addr.nl_family = AF_NETLINK;
    nl_addr.nl_pid = pid;
    nl_addr.nl_groups = 0;
    nl_keeper.nl_family = AF_NETLINK;
    nl_keeper.nl_pid = 0;
    nl_keeper.nl_groups = 0;

    fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (fd < 0){
        fprintf(stderr, "open socket failed\n");
        return -1;
    }
    printf("socket create successfull\n");

    if (bind(fd, (struct sockaddr_nl *)&nl_addr, sizeof(nl_addr)) < 0){
        fprintf(stderr, "bind sockaddr failed\n");
        return -1;
    }
    printf("socket bind successfull\n");

    memset(&msg, 0, sizeof(msg));
    tlen = NL_HDRLEN;
    //协议类型
    msg.hdr.nlmsg_type = 0x10;              //0x10以下都是netlink通信控制报文,
    msg.hdr.nlmsg_flags = NLM_F_REQUEST;//内核只处理NLM_F_REQUEST请求报文
    msg.hdr.nlmsg_seq = 0;              //每次发送+1
    msg.hdr.nlmsg_pid = pid;

    //通用类的命令字
    msg.gen_hdr.cmd = NL80211_CMD_GET_WIPHY;
    msg.gen_hdr.version = 0;

    //flag
    nla_put((struct nlattr *)(msg.data), NL80211_ATTR_SPLIT_WIPHY_DUMP, 0, NULL);
    tlen += NLA_HDRLEN;    

    msg.hdr.nlmsg_len = tlen;
    printf("msg fill successfull\n");
    ret = sendto(fd, &msg, msg.hdr.nlmsg_len, 0, (struct sockaddr_nl *)(&nl_keeper), sizeof(nl_keeper));
    if (ret <= 0){
        perror("send msg failed");
        return -1;
    }
    printf("send successfull\n");

    memset(buf, 0, 8192);
    ret = recvfrom(fd, buf, 8192, 0, NULL, 0);
    if (ret <= 0){
        perror("recv msg failed");
        return -1;
    }
    printf("recv successfull\n");
    m_printmem(buf, ret);

    close(fd);
    return 0;
}
