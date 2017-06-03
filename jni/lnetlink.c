#include "lnetlink.h"
#include "general/generalLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>

void *nla_data(const struct nlattr *nla)
{
    return (char *) nla + NLA_HDRLEN;
}

int nla_len(const struct nlattr *nla)
{
    return nla->nla_len - NLA_HDRLEN;
}

int nla_type(const struct nlattr *nla)
{
    return nla->nla_type & NLA_TYPE_MASK;
}

int nla_attr_size(int payload)
{
    return NLA_HDRLEN + payload;
}

int nla_total_size(int payload)
{
    return NLA_ALIGN(nla_attr_size(payload));
}

int nla_padlen(int payload)
{
    return nla_total_size(payload) - nla_attr_size(payload);
}

struct nlattr * nla_put(struct nlattr  *nla, int attrtype, int datalen, void *data)
{
    int len = datalen + NLA_HDRLEN;
    nla->nla_type = attrtype;
    nla->nla_len  = len;   

    //glog(D, "nla %p\n", (void *)nla + datalen - 1);
    //glog(D, "data %p\n", data + datalen - 1);

    if (datalen > 0) {
        memcpy(nla_data(nla), data, datalen);
    }
    //m_printmem(nla, NLA_ALIGN(len));
    return (struct nlattr *)((void *)nla + NLA_ALIGN(len));
}

int get_famliy_id(char *name){
    int fd, ret, tlen, pid;
    char buf[4096];
    struct message msg, *ans;
    struct sockaddr_nl local, keeper;
    struct nlattr *nla;

    pid = getpid();
    memset(&local, 0, sizeof(local));
    memset(&keeper, 0, sizeof(keeper));
    local.nl_family = AF_NETLINK;
    local.nl_pid = pid;
    local.nl_groups = 0;
    keeper.nl_family = AF_NETLINK;
    keeper.nl_pid = 0;
    keeper.nl_groups = 0;

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0){
        glog(E, "open socket failed while getting famliy id\n");
        return -1;
    }
    
    
    if (bind(fd, (struct sockaddr_nl *)&local, sizeof(local)) < 0){
        glog(E, "bind socket failed while getting famliy id\n");
        goto out2;
    }

    memset(&msg, 0, sizeof(msg));
    tlen = NL_HDRLEN;
    msg.hdr.nlmsg_type = GENL_ID_CTRL;              //0x10以下都是netlink通信控制报文,
    msg.hdr.nlmsg_flags = NLM_F_REQUEST;//内核只处理NLM_F_REQUEST请求报文
    msg.hdr.nlmsg_seq = 0;              //每次发送+1
    msg.hdr.nlmsg_pid = pid;
    msg.gen_hdr.cmd = CTRL_CMD_GETFAMILY;
    msg.gen_hdr.version = 0x1;

    nla_put((struct nlattr *)(msg.data), CTRL_ATTR_FAMILY_NAME, strlen(name) + 1, name);
    tlen += (NLA_HDRLEN + NLA_ALIGN((strlen(name) + 1)));

    msg.hdr.nlmsg_len = tlen;
    //m_printmem(msg, tlen);
    ret = sendto(fd, &msg, msg.hdr.nlmsg_len, 0, (struct sockaddr_nl *)(&keeper), sizeof(keeper));
    if (ret <= 0){
        perror("send msg failed while getting famliy id\n");
        goto out2;
    }

    memset(buf, 0, sizeof(buf));
    ret = recvfrom(fd, buf, sizeof(buf), 0, NULL, 0);
    if (ret <= 0){
        perror("recv msg failed");
        goto out2;
    }
    ans = (struct message *)buf;
    if (ans->hdr.nlmsg_type == NLMSG_ERROR){
        glog(E, "recv error message\n");
    } else {
        //m_printmem(ans, 8);
        nla = (struct nlattr *)GENLMSG_DATA(ans);
        nla = (struct nlattr *)((void *)nla + NLA_ALIGN(nla->nla_len));
        if(nla->nla_type == CTRL_ATTR_FAMILY_ID){
            //m_printmem(NLA_DATA(nla), 8);
            close(fd);
            return  *((unsigned short *)(NLA_DATA(nla)));
        }
    }
out:
    m_printmem(buf, ret);
out2:
    close(fd);
    return -1;
}

static inline int nla_ok(const struct nlattr *nla, int remaining)
{
    //m_printmem(nla, nla->nla_len);
    //printf("rem:%d\n", remaining);
    //printf("nl_len:%d\n", nla->nla_len);
    return remaining >= (int) sizeof(*nla) &&
           nla->nla_len >= sizeof(*nla) &&
           nla->nla_len <= remaining;
}

static inline struct nlattr *nla_next(const struct nlattr *nla, int *remaining)
{
    int totlen = NLA_ALIGN(nla->nla_len);

    *remaining -= totlen;
    return (struct nlattr *) ((char *) nla + totlen);
}

#define nla_for_each_attr(pos, head, len, rem) \
    for (pos = head, rem = len; \
         nla_ok(pos, rem); \
         pos = nla_next(pos, &(rem)))

void m_printnla(struct nlattr *head, int len){
    struct nlattr *nla;
    int rem;

    nla_for_each_attr(nla, head, len, rem){
        printf("\ntype:%d\n", nla->nla_type);
        m_printmem(nla, nla->nla_len);
    }
}
