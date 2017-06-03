#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include "lnetlink.h"
#include "general/generalLib.h"

int main(int argc, char** argv){
    int fd, pid, ret, type;
#ifdef DEBUG
    int flag;
    unsigned char buf[NL_MSG_MAX];
    struct message *ans;
#endif
    struct message msg;
    struct sockaddr_nl nl_addr, nl_keeper;
    struct nlattr *nla;
    unsigned int tlen;

    pid = getpid();
    type = get_famliy_id("nl80211");
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

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0){
        glog(E, "open socket failed\n");
        return -1;
    }

    if (bind(fd, (struct sockaddr_nl *)&nl_addr, sizeof(nl_addr)) < 0){
        glog(E, "bind sockaddr failed\n");
        goto out2;
    }

    memset(&msg, 0, sizeof(msg));
    //协议类型
    msg.hdr.nlmsg_type = type;              //0x10以下都是netlink通信控制报文,
    msg.hdr.nlmsg_seq = 0;              //每次发送+1
    msg.hdr.nlmsg_pid = pid;

    //通用类的命令字
    msg.gen_hdr.version = 0x1;
#define FUZZ_LOOP
    flag = 0;

#ifdef FUZZ_LOOP
    for (;;){
        msg.hdr.nlmsg_seq++;              //每次发送+1
#endif
        tlen = NL_HDRLEN;
        msg.hdr.nlmsg_flags = NLM_F_REQUEST;//内核只处理NLM_F_REQUEST请求报文
        //printf("seq:%d\n", msg.hdr.nlmsg_seq);
#ifdef FUZZ_LOOP
        while((msg.gen_hdr.cmd = m_urand(NL80211_CMD_MAX)) == 0);
        
#else
        msg.gen_hdr.cmd = 0x1;
#endif
        switch(msg.gen_hdr.cmd){
        case NL80211_CMD_GET_WIPHY:
        case NL80211_CMD_GET_INTERFACE:
        case NL80211_CMD_GET_STATION:
        case NL80211_CMD_GET_MPATH:
        case NL80211_CMD_GET_SCAN:
        case NL80211_CMD_TESTMODE:
        case NL80211_CMD_GET_SURVEY:
            msg.hdr.nlmsg_flags |= NLM_F_DUMP;
            break;
        default:
            ;
        }
        //printf("cmd:%d\n", msg.gen_hdr.cmd);
        memset(&(msg.data), 0, sizeof(msg.data));
        if(flag == 0){
            tlen += build_nl80211_args((struct nlattr *)(msg.data));
            msg.hdr.nlmsg_len = tlen;
            //printf("len:%d\n", tlen); 
            //m_printnla(&(msg.data), tlen);
        }
        ret = sendto(fd, &msg, msg.hdr.nlmsg_len, 0, (struct sockaddr_nl *)(&nl_keeper), sizeof(nl_keeper));
        if (ret <= 0){
            perror("send msg failed");
        } else {
            glog(D, "send msg success.\n");
        }

#ifdef DEBUG
        memset(buf, 0, sizeof(buf));
        ret = recvfrom(fd, buf, sizeof(buf), 0, NULL, 0);
        if (ret <= 0){
            glog(E, "revc msg len is %d\n", ret);
            perror("recv msg failed");
        } else {
            //printf("ret:%d\n", ret);
            ans = (struct message *)buf;
            if (ans->hdr.nlmsg_type == NLMSG_ERROR){
                //glog(E, "cmd:%d\t\trecv error code:%d\n", msg.gen_hdr.cmd, *(int *)(NLMSG_DATA(ans)));
                //glog(E, "%d\t", *(int *)(NLMSG_DATA(ans)));
                //m_printmem(buf, ret);
            } else {
                glog(I, "cmd:%d\t\trecv msg success.\n", msg.gen_hdr.cmd);
                //m_printmem(buf, ret);
            }
        }
        flag++;
        flag %= NL80211_CMD_MAX;
#endif
#ifdef FUZZ_LOOP
    }
#endif
out2:
    close(fd);
    return 0;
}
