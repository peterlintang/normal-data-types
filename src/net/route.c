
/*
 *
 */
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <net/if.h>
//#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/route.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "route.h"
#include "str.h"

#define T_L ListD_T
#define T_LN ListDNode_T

#define T RouteEntry_T

#define FALSE	0
#define TRUE	1


int MODULE_FUN_NAME(Route, addRoute)(T entry)
{
    static int skfd = -1;
    struct rtentry rt;

    if (entry) 
	{
        memset((char *) &rt, 0, sizeof (rt));

        rt.rt_flags |= RTF_UP;

        if (entry->mask == 0xffffffff) 
		{
            rt.rt_flags |= RTF_HOST;
        }

        rt.rt_metric = 0;

        ((struct sockaddr_in *) &rt.rt_genmask)->sin_addr.s_addr = entry->mask;
        ((struct sockaddr_in *) &rt.rt_genmask)->sin_family = AF_INET;

        if (entry->iface[0] != '\0') 
		{
            rt.rt_dev = entry->iface;
        }

        ((struct sockaddr_in *) &rt.rt_dst)->sin_addr.s_addr = entry->dst;
        ((struct sockaddr_in *) &rt.rt_dst)->sin_family = AF_INET;

        ((struct sockaddr_in *) &rt.rt_gateway)->sin_addr.s_addr = entry->gateway;
        ((struct sockaddr_in *) &rt.rt_gateway)->sin_family = AF_INET;

        rt.rt_flags |= RTF_GATEWAY;

        if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
		{
            fprintf(stderr, "addRoute socket: %s", strerror(errno));
            return FALSE;
        }

        if (ioctl(skfd, SIOCADDRT, &rt) < 0) 
		{
            fprintf(stderr, "addRoute ioctl: %s", strerror(errno));
            close(skfd);
            return FALSE;
        }

        close(skfd);

        return TRUE;
    }
    return FALSE;

}

int MODULE_FUN_NAME(Route, delRoute)(T entry)
{
    static int skfd = -1;
    struct rtentry rt;

    if (entry) 
	{
        memset((char *) &rt, 0, sizeof (rt));

        rt.rt_flags |= RTF_UP;

        if (entry->mask == 0xffffffff) 
		{
            rt.rt_flags |= RTF_HOST;
        }

        rt.rt_metric = 0;

        ((struct sockaddr_in *) &rt.rt_genmask)->sin_addr.s_addr = entry->mask;
        ((struct sockaddr_in *) &rt.rt_genmask)->sin_family = AF_INET;

        if (entry->iface[0] != '\0') 
		{
            rt.rt_dev = entry->iface;
        }

        ((struct sockaddr_in *) &rt.rt_dst)->sin_addr.s_addr = entry->dst;
        ((struct sockaddr_in *) &rt.rt_dst)->sin_family = AF_INET;

        ((struct sockaddr_in *) &rt.rt_gateway)->sin_addr.s_addr = entry->gateway;
        ((struct sockaddr_in *) &rt.rt_gateway)->sin_family = AF_INET;

        rt.rt_flags |= RTF_GATEWAY;

        if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
		{
            fprintf(stderr, "delRoute socket: %s", strerror(errno));
            return FALSE;
        }

        if (ioctl(skfd, SIOCDELRT, &rt) < 0) 
		{
            fprintf(stderr, "delRoute ioctl: %s", strerror(errno));
            close(skfd);
            return FALSE;
        }

        close(skfd);

        return TRUE;
    }
    return FALSE;
}

static T_L routeLookup(T key) {

    T_L result = NULL;
    int skfd = -1;

    if (key) {

        struct {
            struct nlmsghdr n;
            struct rtmsg r;
            char buf[1024];
        } req;

        struct nhop {
            in_addr_t gw;
            char dev[IFNAMSIZ];
        };

        T_L nhops = NULL;

        struct nlmsghdr *h;
        struct rtmsg *rtmp;
        struct rtattr *rtatp;
        int rtattrlen;
        int rval = -1;
        char buf[4096];
        char dev[IFNAMSIZ];
        in_addr_t src = 0;
        in_addr_t dst = 0;
        in_addr_t mask = 0;
        in_addr_t gw = 0;

        if ((skfd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) {

            fprintf(stderr, "routeLookup socket: %s", strerror(errno));
            return FALSE;

        }

        memset(&req, 0, sizeof (req));
        req.n.nlmsg_len = NLMSG_LENGTH(sizeof (struct rtmsg));
        req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
        req.n.nlmsg_type = RTM_GETROUTE;
        req.n.nlmsg_len = NLMSG_ALIGN(req.n.nlmsg_len);

        if (send(skfd, &req, req.n.nlmsg_len, 0) < 0) {

            fprintf(stderr, "routeLookup send: %s", strerror(errno));
            close(skfd);
            return result;

        }

        if ((rval = recv(skfd, buf, sizeof (buf), 0)) < 0) {

            fprintf(stderr, "routeLookup recv: %s", strerror(errno));
            close(skfd);
            return result;

        }

        for (h = (struct nlmsghdr *) buf; NLMSG_OK(h, rval); h = NLMSG_NEXT(h, rval)) {

            rtmp = (struct rtmsg *) NLMSG_DATA(h);
            rtatp = (struct rtattr *) RTM_RTA(rtmp);
            rtattrlen = RTM_PAYLOAD(h);

            src = 0;
            dst = 0;
            mask = 0;
            gw = 0;


            if (result == NULL) {

                result = MODULE_FUN_NAME(ListD, new)();

            }

            for (; RTA_OK(rtatp, rtattrlen); rtatp = RTA_NEXT(rtatp, rtattrlen)) {

                switch (rtatp->rta_type) {

                    case RTA_OIF:
                    {
                        int oif_index = *(int *) RTA_DATA(rtatp);

                        if_indextoname(oif_index, dev);
                        break;
                    }

                    case RTA_PREFSRC: src = *((in_addr_t *) RTA_DATA(rtatp));
                        break;
                    case RTA_DST: dst = *((in_addr_t *) RTA_DATA(rtatp));
                        mask = rtmp->rtm_dst_len != 0 ? htonl(~0 << (32 - rtmp->rtm_dst_len)) : 0;
                        break;
                    case RTA_GATEWAY: gw = *((in_addr_t *) RTA_DATA(rtatp));
                        break;

                    case RTA_MULTIPATH:
                    {

                        nhops = MODULE_FUN_NAME(ListD, new)();

                        struct rtnexthop *nh = NULL;
                        
                        struct rtattr *nhrtattr = NULL;
                        int nh_len = RTA_PAYLOAD(rtatp);

                        for (nh = RTA_DATA(rtatp); RTNH_OK(nh, nh_len); nh = RTNH_NEXT(nh)) {

                            struct nhop *hop = calloc(1, sizeof (struct nhop));

                            int attr_len = nh->rtnh_len - sizeof (*nh);

                            if (nh_len < sizeof (*nh))
                                break;
                            if (nh->rtnh_len > nh_len)
                                break;

                            if (nh->rtnh_len > sizeof (*nh)) {

                                if_indextoname(nh->rtnh_ifindex, hop->dev);

                                for (nhrtattr = RTNH_DATA(nh); RTA_OK(nhrtattr, attr_len); nhrtattr = RTA_NEXT(nhrtattr, attr_len)) {

                                    switch (nhrtattr->rta_type) {

                                        case RTA_GATEWAY:
                                            hop->gw = *((in_addr_t *) RTA_DATA(nhrtattr));
                                            break;


                                    }

                                }

                                MODULE_FUN_NAME(ListD, insert)(nhops, MODULE_FUN_NAME(ListDNode, new)(hop));

                            }

                            nh_len -= NLMSG_ALIGN(nh->rtnh_len);
                        }

                        break;
                    }

                }

            }

            if (nhops == NULL) {

                if (key && ((key->dst != dst) || (key->iface[0] != '\0' && strcmp(key->iface, dev) != 0))) {

                    continue;

                }

                T r = calloc(1, sizeof(*r));

                r->gateway = gw;
                r->mask = mask;
                r->dst = dst;
                r->src = src;
                strcpy(r->iface, dev);

                MODULE_FUN_NAME(ListD, insert)(result, MODULE_FUN_NAME(ListDNode, new)(r));

            } else {

                struct nhop *h = NULL;
                ListDNode_T item = NULL;

                while (item = MODULE_FUN_NAME(ListD, head)(nhops)) {

					MODULE_FUN_NAME(ListD, remove)(nhops, item);

                    h = item->priv;

                    if (key && ((key->dst != dst) || (key->iface[0] != '\0' && strcmp(key->iface, h->dev) != 0))) {

                        MODULE_FUN_NAME(ListDNode, free)(&item);
                        free(h);
                        continue;

                    }

                    T r = calloc(1, sizeof(*r));

                    r->gateway = h->gw;
                    r->mask = mask;
                    r->dst = dst;
                    r->src = src;
                    strcpy(r->iface, h->dev);

                    MODULE_FUN_NAME(ListD, insert)(result, MODULE_FUN_NAME(ListDNode, new)(r));
                    MODULE_FUN_NAME(ListDNode, free)(&item);
                    free(h);

                }

                MODULE_FUN_NAME(ListD, free)(&nhops);

            }

        }

        close(skfd);

        return result;

    }

    return result;

}

T_L MODULE_FUN_NAME(Route, cacheLookup)(T dest)
{
    T_L result = NULL;

    FILE *f = fopen("/proc/net/rt_cache", "r");

    if (f) 
	{
        char buf[512];

        result = MODULE_FUN_NAME(ListD, new)();

        fgets(buf, sizeof (buf), f); // skip header

        while (!feof(f)) 
		{
            if (fgets(buf, sizeof (buf), f)) 
			{
                T_L fields = MODULE_FUN_NAME(Str, tokenize)(buf, "\t");

                int i = 0;
                ListDNode_T item;

                T entry = calloc(1, sizeof(*entry));

                while ((item = MODULE_FUN_NAME(ListD, head)(fields))) 
				{
					MODULE_FUN_NAME(ListD, remove)(fields, item);
                    switch (i) 
					{
                        case 0: strcpy(entry->iface, (char*) item->priv);
                            break;
                        case 1: sscanf((char*) item->priv, "%X", &entry->dst);
                            break;
                        case 2: sscanf((char*) item->priv, "%X", &entry->gateway);
                            break;
                        case 7: sscanf((char*) item->priv, "%X", &entry->src);
                            break;
                    }

                    free(item->priv);
                    MODULE_FUN_NAME(ListDNode, free)(&item);
                    i++;
                }

                MODULE_FUN_NAME(ListD, free)(&fields);

                if (dest) 
				{
                    if (dest->dst && dest->dst != entry->dst) 
					{
                        free(entry);
                        continue;
                    }
                }

                MODULE_FUN_NAME(ListD, insert)(result, MODULE_FUN_NAME(ListDNode, new)(entry));
            }
        }
        fclose(f);
    }

    return result;
}

int MODULE_FUN_NAME(Route, addHostRoute)(in_addr_t dst, in_addr_t gw, char *iface)
{
    struct T route = {.dst = dst, .src = 0, .gateway = gw, .mask = 0xffffffff};
    int result = FALSE;

    if (iface)
        strcpy(route.iface, iface);
    else
        route.iface[0] = '\0';

    result = MODULE_FUN_NAME(Route, addRoute)(&route);

    return result;
}

int MODULE_FUN_NAME(Route, delHostRoute)(in_addr_t dst, in_addr_t gw, char *iface)
{
    struct T route = {.dst = dst, .src = 0, .gateway = gw, .mask = 0xffffffff};
    int result = FALSE;

    if (iface)
        strcpy(route.iface, iface);
    else
        route.iface[0] = '\0';

    result = MODULE_FUN_NAME(Route, delRoute)(&route);

    return result;
}

in_addr_t MODULE_FUN_NAME(Route, getIfIP)(char *iface)
{
    in_addr_t result = 0;

    if (iface) 
	{
        struct ifreq req;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        memset(&req, 0, sizeof (struct ifreq));
        strcpy(req.ifr_name, iface);

        if (ioctl(sock, SIOCGIFADDR, &req) >= 0) 
		{
            result = ((struct sockaddr_in *) &req.ifr_addr)->sin_addr.s_addr;
        } else 
		{
            fprintf(stderr, "getIfIP ioctl: %s", strerror(errno));
        }

        close(sock);
    }

    return result;
}

in_addr_t MODULE_FUN_NAME(Route, getIfGW)(char *iface)
{
    struct T route = {.dst = 0, .src = 0, .gateway = 0, .mask = 0};
    T_L routes = NULL;
    in_addr_t result = 0;

    if (iface)
        strcpy(route.iface, iface);
    else
        route.iface[0] = '\0';

    routes = routeLookup(&route);

    if (routes) 
	{
        if (MODULE_FUN_NAME(ListD, count)(routes) > 0) 
		{
            ListDNode_T e = MODULE_FUN_NAME(ListD, head)(routes);
			MODULE_FUN_NAME(ListD, remove)(routes, e);

            T entry = (T)e->priv;
            result = entry->gateway;
            free(entry);

            MODULE_FUN_NAME(ListDNode, free)(&e);
        }

//        MODULE_FUN_NAME(ListD, clear)(routes, TRUE);
        MODULE_FUN_NAME(ListD, free)(&routes);
    }

    return result;
}


