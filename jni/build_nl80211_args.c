#include <stdio.h>
#include <stdlib.h>
#include "general/generalLib.h"
#include "lnetlink.h"
#include "nl80211.h"

struct nlattr * build_nla_arg(struct nla_policy *p, struct nlattr * nla, unsigned int type);

int build_nl80211_args(struct nlattr *nla){
    struct nla_policy *p;
    struct nlattr *next;
    int i;
    
    //glog(D, "enter build nl80211 args\n");    
    next = nla;
    for (i = 1; i < NL80211_ATTR_MAX; ++i){// 0 是未定义的参数，下面不会赋值
        next = build_nla_arg(&(nl80211_policy[i]), next, i);
        glog(D, "hased len:%d\n", ((void *)next - (void *)nla)); 
    }
    //glog(D, "outter build nl80211 args\n");
    return ((void *)next - (void *)nla);
}

struct nlattr * build_nla_arg(struct nla_policy *p, struct nlattr * nla, unsigned int type){
    struct nlattr *ret;
    unsigned short len, max, len1, len2;
    //内核中没有定义max最大值，意味着max为65535，很可能一个netlink报文无法完成，netlink协议会不会自动完成分片？
    max = 1024;
    ret = nla;
    if (!p) return -1;
    switch(p->type){
        case NLA_FLAG:
//            glog(D, "nla_flag\n");
            ret = nla_put(nla, type, 0, NULL);
            break;

       case NLA_NUL_STRING:// '\0' is in the end
            glog(D, "nla_nul_string\n");
            if (p->len){
                ret = nla_put(nla, type, p->len + 1, m_srand(p->len));
            } else{
                len = m_urand(max);
                ret = nla_put(nla, type, len + 1, m_srand(len));//可能存在 strlen(srand) < len, 即溢出情况
            }
            break;

        case NLA_STRING:
//            glog(D, "nla_string\n");
            if (p->len){
                len = p->len;
            } else {
                while((len=m_urand(max)) == 0);
            }
            ret = nla_put(nla, type, len, m_brand(len));
            break;

        case NLA_BINARY:
//            glog(D, "nla_binary\n");
            if (p->len){
                len = p->len;
            } else {
                len = m_urand(max);
            }
            ret = nla_put(nla, type, len, m_brand(len));
            break;


//        case NLA_NESTED_COMPAT://wifi模块没有用到,其他模块未知
//            glog(D, "nla_nested_compat\n");
//            if ((NLA_ALIGN(p->len) - p->len) > 0){
//                //(pt->len)    <=  attrlen  < NLA_ALIGN(pt->len)
//                if((NLA_ALIGN(p->len) - p->len) == 1)
//                    len = p->len;
//                if((NLA_ALIGN(p->len) - p->len) > 1)
//                    len = p->len + m_urand(NLA_ALIGN(p->len) - p->len - 1);
//                ret = nla_put(nla, type, len, m_brand(len));
//                break;
//            }
//            //attrlen >= NLA_ALIGN(pt->len) + NLA_HDRLEN + nla_len(nla)
//            len1 = m_urand(max);
//            len2 = m_urand(max);
//            ret = nla_put(nla, type, len1 + 2, m_brand(len1));
//            ret = nla_put(ret, type, len2, m_brand(len2));
//            break;
//
        case NLA_NESTED://长度为0时，不做校验
//            glog(D, "nla_nested\n");
            if (p->len){
                len = p->len;
            } else {
                len = m_urand(max);
            }
            next = nla_put(next, type, 0, NULL);
            j = m_urand(24);
            next = nla;
            for(;i < j;i++){
                len = m_urand(4);
                next = nla_put(next, i, len, m_brand(len));
            }
            if(m_urand(255) % 2){
                next = nla_put(next, i, 255, m_brand(255));
            }

            nla->
            ret = next;
            break;

        default:
//            glog(D, "nla_defualt\n");
            if(p->len)
                len = p->len;
            else if (p->type != NLA_UNSPEC){
                len = nla_attr_minlen[p->type];
            }
            ret = nla_put(nla, type, len, m_brand(len));
            break;
    }
    return ret;
}

#define nla_put_nest(nla, type, len, str) do\
{\
    ;\
}while(0);
