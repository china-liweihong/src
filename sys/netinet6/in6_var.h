/*	$OpenBSD: in6_var.h,v 1.55 2015/09/09 15:51:40 mpi Exp $	*/
/*	$KAME: in6_var.h,v 1.55 2001/02/16 12:49:45 itojun Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 1985, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)in_var.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET6_IN6_VAR_H_
#define _NETINET6_IN6_VAR_H_

/*
 * Interface address, Internet version.  One of these structures
 * is allocated for each interface with an Internet address.
 * The ifaddr structure contains the protocol-independent part
 * of the structure and is assumed to be first.
 */

/*
 * pltime/vltime are just for future reference (required to implements 2
 * hour rule for hosts).  they should never be modified by nd6_timeout or
 * anywhere else.
 *	userland -> kernel: accept pltime/vltime
 *	kernel -> userland: throw up everything
 *	in kernel: modify preferred/expire only
 */
struct in6_addrlifetime {
	time_t ia6t_expire;	/* valid lifetime expiration time */
	time_t ia6t_preferred;	/* preferred lifetime expiration time */
	u_int32_t ia6t_vltime;	/* valid lifetime */
	u_int32_t ia6t_pltime;	/* prefix lifetime */
};

#ifdef _KERNEL
struct nd_ifinfo;
struct in6_ifextra {
	struct in6_ifstat *in6_ifstat;
	struct nd_ifinfo *nd_ifinfo;
	void *rs_lhcookie;
	int nprefixes;
	int ndefrouters;
};

struct	in6_ifaddr {
	struct	ifaddr ia_ifa;		/* protocol-independent info */
#define	ia_ifp		ia_ifa.ifa_ifp
#define	ia_flags	ia_ifa.ifa_flags

	struct	sockaddr_in6 ia_addr;	/* interface address */
	struct	sockaddr_in6 ia_dstaddr; /* space for destination addr */
	struct	sockaddr_in6 ia_prefixmask; /* prefix mask */
	TAILQ_ENTRY(in6_ifaddr) ia_list;	/* list of IP6 addresses */
	int	ia6_flags;

	struct in6_addrlifetime ia6_lifetime;
	time_t	ia6_createtime; /* the creation time of this address, which is
				 * currently used for temporary addresses only.
				 */
	time_t	ia6_updatetime;

	/* back pointer to the prefix (for autoconf) */
	struct nd_prefix *ia6_ndpr;

	/* multicast addresses joined from the kernel */
	LIST_HEAD(, in6_multi_mship) ia6_memberships;
};
#endif /* _KERNEL */

/*
 * IPv6 interface statistics, as defined in RFC2465 Ipv6IfStatsEntry (p12).
 */
struct in6_ifstat {
	u_int64_t ifs6_in_receive;	/* # of total input datagram */
	u_int64_t ifs6_in_hdrerr;	/* # of datagrams with invalid hdr */
	u_int64_t ifs6_in_toobig;	/* # of datagrams exceeded MTU */
	u_int64_t ifs6_in_noroute;	/* # of datagrams with no route */
	u_int64_t ifs6_in_addrerr;	/* # of datagrams with invalid dst */
	u_int64_t ifs6_in_protounknown;	/* # of datagrams with unknown proto */
					/* NOTE: increment on final dst if */
	u_int64_t ifs6_in_truncated;	/* # of truncated datagrams */
	u_int64_t ifs6_in_discard;	/* # of discarded datagrams */
					/* NOTE: fragment timeout is not here */
	u_int64_t ifs6_in_deliver;	/* # of datagrams delivered to ULP */
					/* NOTE: increment on final dst if */
	u_int64_t ifs6_out_forward;	/* # of datagrams forwarded */
					/* NOTE: increment on outgoing if */
	u_int64_t ifs6_out_request;	/* # of outgoing datagrams from ULP */
					/* NOTE: does not include forwrads */
	u_int64_t ifs6_out_discard;	/* # of discarded datagrams */
	u_int64_t ifs6_out_fragok;	/* # of datagrams fragmented */
	u_int64_t ifs6_out_fragfail;	/* # of datagrams failed on fragment */
	u_int64_t ifs6_out_fragcreat;	/* # of fragment datagrams */
					/* NOTE: this is # after fragment */
	u_int64_t ifs6_reass_reqd;	/* # of incoming fragmented packets */
					/* NOTE: increment on final dst if */
	u_int64_t ifs6_reass_ok;	/* # of reassembled packets */
					/* NOTE: this is # after reass */
					/* NOTE: increment on final dst if */
	u_int64_t ifs6_reass_fail;	/* # of reass failures */
					/* NOTE: may not be packet count */
					/* NOTE: increment on final dst if */
	u_int64_t ifs6_in_mcast;	/* # of inbound multicast datagrams */
	u_int64_t ifs6_out_mcast;	/* # of outbound multicast datagrams */
};

/*
 * ICMPv6 interface statistics, as defined in RFC2466 Ipv6IfIcmpEntry.
 * XXX: I'm not sure if this file is the right place for this structure...
 */
struct icmp6_ifstat {
	/*
	 * Input statistics
	 */
	/* ipv6IfIcmpInMsgs, total # of input messages */
	u_int64_t ifs6_in_msg;
	/* ipv6IfIcmpInErrors, # of input error messages */
	u_int64_t ifs6_in_error;
	/* ipv6IfIcmpInDestUnreachs, # of input dest unreach errors */
	u_int64_t ifs6_in_dstunreach;
	/* ipv6IfIcmpInAdminProhibs, # of input administratively prohibited errs */
	u_int64_t ifs6_in_adminprohib;
	/* ipv6IfIcmpInTimeExcds, # of input time exceeded errors */
	u_int64_t ifs6_in_timeexceed;
	/* ipv6IfIcmpInParmProblems, # of input parameter problem errors */
	u_int64_t ifs6_in_paramprob;
	/* ipv6IfIcmpInPktTooBigs, # of input packet too big errors */
	u_int64_t ifs6_in_pkttoobig;
	/* ipv6IfIcmpInEchos, # of input echo requests */
	u_int64_t ifs6_in_echo;
	/* ipv6IfIcmpInEchoReplies, # of input echo replies */
	u_int64_t ifs6_in_echoreply;
	/* ipv6IfIcmpInRouterSolicits, # of input router solicitations */
	u_int64_t ifs6_in_routersolicit;
	/* ipv6IfIcmpInRouterAdvertisements, # of input router advertisements */
	u_int64_t ifs6_in_routeradvert;
	/* ipv6IfIcmpInNeighborSolicits, # of input neighbor solicitations */
	u_int64_t ifs6_in_neighborsolicit;
	/* ipv6IfIcmpInNeighborAdvertisements, # of input neighbor advertisements */
	u_int64_t ifs6_in_neighboradvert;
	/* ipv6IfIcmpInRedirects, # of input redirects */
	u_int64_t ifs6_in_redirect;
	/* ipv6IfIcmpInGroupMembQueries, # of input MLD queries */
	u_int64_t ifs6_in_mldquery;
	/* ipv6IfIcmpInGroupMembResponses, # of input MLD reports */
	u_int64_t ifs6_in_mldreport;
	/* ipv6IfIcmpInGroupMembReductions, # of input MLD done */
	u_int64_t ifs6_in_mlddone;

	/*
	 * Output statistics. We should solve unresolved routing problem...
	 */
	/* ipv6IfIcmpOutMsgs, total # of output messages */
	u_int64_t ifs6_out_msg;
	/* ipv6IfIcmpOutErrors, # of output error messages */
	u_int64_t ifs6_out_error;
	/* ipv6IfIcmpOutDestUnreachs, # of output dest unreach errors */
	u_int64_t ifs6_out_dstunreach;
	/* ipv6IfIcmpOutAdminProhibs, # of output administratively prohibited errs */
	u_int64_t ifs6_out_adminprohib;
	/* ipv6IfIcmpOutTimeExcds, # of output time exceeded errors */
	u_int64_t ifs6_out_timeexceed;
	/* ipv6IfIcmpOutParmProblems, # of output parameter problem errors */
	u_int64_t ifs6_out_paramprob;
	/* ipv6IfIcmpOutPktTooBigs, # of output packet too big errors */
	u_int64_t ifs6_out_pkttoobig;
	/* ipv6IfIcmpOutEchos, # of output echo requests */
	u_int64_t ifs6_out_echo;
	/* ipv6IfIcmpOutEchoReplies, # of output echo replies */
	u_int64_t ifs6_out_echoreply;
	/* ipv6IfIcmpOutRouterSolicits, # of output router solicitations */
	u_int64_t ifs6_out_routersolicit;
	/* ipv6IfIcmpOutRouterAdvertisements, # of output router advertisements */
	u_int64_t ifs6_out_routeradvert;
	/* ipv6IfIcmpOutNeighborSolicits, # of output neighbor solicitations */
	u_int64_t ifs6_out_neighborsolicit;
	/* ipv6IfIcmpOutNeighborAdvertisements, # of output neighbor advertisements */
	u_int64_t ifs6_out_neighboradvert;
	/* ipv6IfIcmpOutRedirects, # of output redirects */
	u_int64_t ifs6_out_redirect;
	/* ipv6IfIcmpOutGroupMembQueries, # of output MLD queries */
	u_int64_t ifs6_out_mldquery;
	/* ipv6IfIcmpOutGroupMembResponses, # of output MLD reports */
	u_int64_t ifs6_out_mldreport;
	/* ipv6IfIcmpOutGroupMembReductions, # of output MLD done */
	u_int64_t ifs6_out_mlddone;
};

struct	in6_ifreq {
	char	ifr_name[IFNAMSIZ];
	union {
		struct	sockaddr_in6 ifru_addr;
		struct	sockaddr_in6 ifru_dstaddr;
		short	ifru_flags;
		int	ifru_flags6;
		int	ifru_metric;
		caddr_t	ifru_data;
		struct in6_addrlifetime ifru_lifetime;
		struct in6_ifstat ifru_stat;
		struct icmp6_ifstat ifru_icmp6stat;
	} ifr_ifru;
};

struct	in6_aliasreq {
	char	ifra_name[IFNAMSIZ];
	union {
		struct	sockaddr_in6 ifrau_addr;
		int	ifrau_align;
	 } ifra_ifrau;
#ifndef ifra_addr
#define ifra_addr	ifra_ifrau.ifrau_addr
#endif
	struct	sockaddr_in6 ifra_dstaddr;
	struct	sockaddr_in6 ifra_prefixmask;
	int	ifra_flags;
	struct in6_addrlifetime ifra_lifetime;
};

/* prefix type macro */
#define IN6_PREFIX_ND	1
#define IN6_PREFIX_RR	2

/*
 * prefix related flags passed between kernel(NDP related part) and
 * user land command(ifconfig) and daemon(rtadvd).
 */
struct prf_ra {
	u_int onlink : 1;
	u_int autonomous : 1;
	u_int router : 1;
	u_int reserved : 5;
};

struct in6_prflags {
	struct prf_ra prf_ra;
	u_char prf_reserved1;
	u_short prf_reserved2;
	/* want to put this on 4byte offset */
	struct prf_rr {
		u_int decrvalid : 1;
		u_int decrprefd : 1;
		u_int reserved : 6;
	} prf_rr;
	u_char prf_reserved3;
	u_short prf_reserved4;
};

struct  in6_prefixreq {
	char	ipr_name[IFNAMSIZ];
	u_char	ipr_origin;
	u_char	ipr_plen;
	u_int32_t ipr_vltime;
	u_int32_t ipr_pltime;
	struct in6_prflags ipr_flags;
	struct	sockaddr_in6 ipr_prefix;
};

#define PR_ORIG_RA	0
#define PR_ORIG_RR	1
#define PR_ORIG_STATIC	2
#define PR_ORIG_KERNEL	3

#define ipr_raf_onlink		ipr_flags.prf_ra.onlink
#define ipr_raf_auto		ipr_flags.prf_ra.autonomous

#define ipr_statef_onlink	ipr_flags.prf_state.onlink

#define ipr_rrf_decrvalid	ipr_flags.prf_rr.decrvalid
#define ipr_rrf_decrprefd	ipr_flags.prf_rr.decrprefd

struct	in6_rrenumreq {
	char	irr_name[IFNAMSIZ];
	u_char	irr_origin;
	u_char	irr_m_len;	/* match len for matchprefix */
	u_char	irr_m_minlen;	/* minlen for matching prefix */
	u_char	irr_m_maxlen;	/* maxlen for matching prefix */
	u_char	irr_u_uselen;	/* uselen for adding prefix */
	u_char	irr_u_keeplen;	/* keeplen from matching prefix */
	struct irr_raflagmask {
		u_int onlink : 1;
		u_int autonomous : 1;
		u_int reserved : 6;
	} irr_raflagmask;
	u_int32_t irr_vltime;
	u_int32_t irr_pltime;
	struct in6_prflags irr_flags;
	struct	sockaddr_in6 irr_matchprefix;
	struct	sockaddr_in6 irr_useprefix;
};

#define irr_raf_mask_onlink	irr_raflagmask.onlink
#define irr_raf_mask_auto	irr_raflagmask.autonomous
#define irr_raf_mask_reserved	irr_raflagmask.reserved

#define irr_raf_onlink		irr_flags.prf_ra.onlink
#define irr_raf_auto		irr_flags.prf_ra.autonomous

#define irr_statef_onlink	irr_flags.prf_state.onlink

#define irr_rrf			irr_flags.prf_rr
#define irr_rrf_decrvalid	irr_flags.prf_rr.decrvalid
#define irr_rrf_decrprefd	irr_flags.prf_rr.decrprefd

/*
 * Given a pointer to an in6_ifaddr (ifaddr),
 * return a pointer to the addr as a sockaddr_in6
 */
#define IA6_IN6(ia)	(&((ia)->ia_addr.sin6_addr))
#define IA6_DSTIN6(ia)	(&((ia)->ia_dstaddr.sin6_addr))
#define IA6_MASKIN6(ia)	(&((ia)->ia_prefixmask.sin6_addr))
#define IA6_SIN6(ia)	(&((ia)->ia_addr))
#define IA6_DSTSIN6(ia)	(&((ia)->ia_dstaddr))
#define IFA_IN6(x)	(&((struct sockaddr_in6 *)((x)->ifa_addr))->sin6_addr)
#define IFA_DSTIN6(x)	(&((struct sockaddr_in6 *)((x)->ifa_dstaddr))->sin6_addr)

#ifdef _KERNEL
#define IN6_ARE_MASKED_ADDR_EQUAL(d, a, m)	(	\
	(((d)->s6_addr32[0] ^ (a)->s6_addr32[0]) & (m)->s6_addr32[0]) == 0 && \
	(((d)->s6_addr32[1] ^ (a)->s6_addr32[1]) & (m)->s6_addr32[1]) == 0 && \
	(((d)->s6_addr32[2] ^ (a)->s6_addr32[2]) & (m)->s6_addr32[2]) == 0 && \
	(((d)->s6_addr32[3] ^ (a)->s6_addr32[3]) & (m)->s6_addr32[3]) == 0 )
#endif /* _KERNEL */

#define SIOCSIFADDR_IN6		 _IOW('i', 12, struct in6_ifreq)
#define SIOCGIFADDR_IN6		_IOWR('i', 33, struct in6_ifreq)

#ifdef _KERNEL
/*
 * SIOCSxxx ioctls should be unused (see comments in in6.c), but
 * we do not shift numbers for binary compatibility.
 */
#define SIOCSIFDSTADDR_IN6	 _IOW('i', 14, struct in6_ifreq)
#define SIOCSIFNETMASK_IN6	 _IOW('i', 22, struct in6_ifreq)
#endif /* _KERNEL */

#define SIOCGIFDSTADDR_IN6	_IOWR('i', 34, struct in6_ifreq)
#define SIOCGIFNETMASK_IN6	_IOWR('i', 37, struct in6_ifreq)

#define SIOCDIFADDR_IN6		 _IOW('i', 25, struct in6_ifreq)
#define SIOCAIFADDR_IN6		 _IOW('i', 26, struct in6_aliasreq)

#define SIOCSIFPHYADDR_IN6       _IOW('i', 70, struct in6_aliasreq)
#define	SIOCGIFPSRCADDR_IN6	_IOWR('i', 71, struct in6_ifreq)
#define	SIOCGIFPDSTADDR_IN6	_IOWR('i', 72, struct in6_ifreq)

#define SIOCGIFAFLAG_IN6	_IOWR('i', 73, struct in6_ifreq)

#define SIOCGIFINFO_IN6		_IOWR('i', 108, struct in6_ndireq)
#define SIOCSNDFLUSH_IN6	_IOWR('i', 77, struct in6_ifreq)
#define SIOCGNBRINFO_IN6	_IOWR('i', 78, struct in6_nbrinfo)
#define SIOCSPFXFLUSH_IN6	_IOWR('i', 79, struct in6_ifreq)
#define SIOCSRTRFLUSH_IN6	_IOWR('i', 80, struct in6_ifreq)

#define SIOCGIFALIFETIME_IN6	_IOWR('i', 81, struct in6_ifreq)
#define SIOCSIFALIFETIME_IN6	_IOWR('i', 82, struct in6_ifreq)
#define SIOCGIFSTAT_IN6		_IOWR('i', 83, struct in6_ifreq)
#define SIOCGIFSTAT_ICMP6	_IOWR('i', 84, struct in6_ifreq)

#define SIOCSIFINFO_FLAGS	_IOWR('i', 87, struct in6_ndireq) /* XXX */

#define SIOCSIFPREFIX_IN6	_IOW('i', 100, struct in6_prefixreq) /* set */
#define SIOCGIFPREFIX_IN6	_IOWR('i', 101, struct in6_prefixreq) /* get */
#define SIOCDIFPREFIX_IN6	_IOW('i', 102, struct in6_prefixreq) /* del */
#define SIOCAIFPREFIX_IN6	_IOW('i', 103, struct in6_rrenumreq) /* add */
#define SIOCCIFPREFIX_IN6	_IOW('i', 104, \
				     struct in6_rrenumreq) /* change */
#define SIOCSGIFPREFIX_IN6	_IOW('i', 105, \
				     struct in6_rrenumreq) /* set global */

#define SIOCGETSGCNT_IN6	_IOWR('u', 106, \
				      struct sioc_sg_req6) /* get s,g pkt cnt */
#define SIOCGETMIFCNT_IN6	_IOWR('u', 107, \
				      struct sioc_mif_req6) /* get pkt cnt per if */

#define IN6_IFF_ANYCAST		0x01	/* anycast address */
#define IN6_IFF_TENTATIVE	0x02	/* tentative address */
#define IN6_IFF_DUPLICATED	0x04	/* DAD detected duplicate */
#define IN6_IFF_DETACHED	0x08	/* may be detached from the link */
#define IN6_IFF_DEPRECATED	0x10	/* deprecated address */
#define IN6_IFF_AUTOCONF	0x40	/* autoconfigurable address. */
#define IN6_IFF_PRIVACY		0x80	/* RFC 4941 temporary address */

/* do not input/output */
#define IN6_IFF_NOTREADY (IN6_IFF_TENTATIVE|IN6_IFF_DUPLICATED)

#ifdef _KERNEL
#define IN6_ARE_SCOPE_CMP(a,b) ((a)-(b))
#define IN6_ARE_SCOPE_EQUAL(a,b) ((a)==(b))

TAILQ_HEAD(in6_ifaddrhead, in6_ifaddr);
extern struct in6_ifaddrhead in6_ifaddr;

extern struct icmp6stat icmp6stat;
#define in6_ifstat_inc(ifp, tag) \
do {								\
	if (ifp)						\
		((struct in6_ifextra *)((ifp)->if_afdata[AF_INET6]))->in6_ifstat->tag++; \
} while (0)

/*
 * Multi-cast membership entry.  One for each group/ifp that a PCB
 * belongs to.
 */
struct in6_multi_mship {
	struct	in6_multi *i6mm_maddr;	/* Multicast address pointer */
	LIST_ENTRY(in6_multi_mship) i6mm_chain;  /* multicast options chain */
};

struct in6_multi {
	struct ifmaddr		in6m_ifma;   /* Protocol-independent info */
#define in6m_refcnt		in6m_ifma.ifma_refcnt
#define in6m_ifidx		in6m_ifma.ifma_ifidx

	struct sockaddr_in6	in6m_sin;   /* IPv6 multicast address */
#define in6m_addr		in6m_sin.sin6_addr

	u_int			in6m_state; /* state of membership */
	u_int			in6m_timer; /* MLD6 membership report timer */
};

static __inline struct in6_multi *
ifmatoin6m(struct ifmaddr *ifma)
{
       return ((struct in6_multi *)(ifma));
}

/*
 * Macros for looking up the in6_multi record for a given IP6 multicast
 * address on a given interface. If no matching record is found, "in6m"
 * returns NULL.
 */
#define IN6_LOOKUP_MULTI(addr, ifp, in6m)				\
	/* struct in6_addr addr; */					\
	/* struct ifnet *ifp; */					\
	/* struct in6_multi *in6m; */					\
do {									\
	struct ifmaddr *ifma;						\
									\
	(in6m) = NULL;							\
	TAILQ_FOREACH(ifma, &(ifp)->if_maddrlist, ifma_list)		\
		if (ifma->ifma_addr->sa_family == AF_INET6 &&		\
		    IN6_ARE_ADDR_EQUAL(&ifmatoin6m(ifma)->in6m_addr,	\
				       &(addr))) {			\
			(in6m) = ifmatoin6m(ifma);			\
			break;						\
		}							\
} while (/* CONSTCOND */ 0)

struct	in6_multi *in6_addmulti(struct in6_addr *, struct ifnet *, int *);
void	in6_delmulti(struct in6_multi *);
struct in6_multi_mship *in6_joingroup(struct ifnet *, struct in6_addr *, int *);
int	in6_leavegroup(struct in6_multi_mship *);
int	in6_control(struct socket *, u_long, caddr_t, struct ifnet *);
int	in6_update_ifa(struct ifnet *, struct in6_aliasreq *,
	struct in6_ifaddr *);
void	in6_purgeaddr(struct ifaddr *);
int	in6if_do_dad(struct ifnet *);
void	*in6_domifattach(struct ifnet *);
void	in6_domifdetach(struct ifnet *, void *);
struct in6_ifaddr *in6ifa_ifpforlinklocal(struct ifnet *, int);
struct in6_ifaddr *in6ifa_ifpwithaddr(struct ifnet *, struct in6_addr *);
int	in6_ifpprefix(const struct ifnet *, const struct in6_addr *);
int	in6_addr2scopeid(unsigned int, struct in6_addr *);
int	in6_matchlen(struct in6_addr *, struct in6_addr *);
int	in6_are_prefix_equal(struct in6_addr *, struct in6_addr *, int);
void	in6_prefixlen2mask(struct in6_addr *, int);
void	in6_purgeprefix(struct ifnet *);
#endif /* _KERNEL */

#endif /* _NETINET6_IN6_VAR_H_ */
