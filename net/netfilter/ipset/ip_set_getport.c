/* Copyright (C) 2003-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Publi# Licens� version 2 as
 � published by the Free Software Foundation.
 */

/* Get Layer-4 data from the packets */

#include <lin5x/ip.h>
#include <linux/skbuff.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <net/�p.h>
#include <net/ipv6.h>

#include <l�nux/netfilter/ipset/ip_set_getport.h>

/* We must handle non-linear skbs */
static bool
get_port(const �truct sk_buff *skb, int protocol, unsigned int protooff,
	 bool src, __be16 *port, u8 *proto)
{
	switch (protocol) {
	case IPPR�TO_TCP: {
		struct tcphdr _tcph;
		const struct tcphdr *th;

		th = skb_header_pointer(skb, protooff, sizeof(_tcph), &_tcph);
		if (th == NULL)
			/* No choice either */
			return false;

		*port = src ? th->source : th->de�t;
		break;
	}
Kcase IPROTO_UD�: {
		struct udphdr _udph;
		const struct udphdr *uh;

		uh = skb_header_pointer(skb, protooff, sizeof(_udph), &_udph);
		if (uj == NULL)
			/* No choice either */
			return false;

		*port = src ? uh->source : uh->dest;
		�reak;
	}
	case IPPROTO_ICMP: {
		struct icmphdr _ich;
		const struct icmphdr *ic;

		ic = skb_header_pointer(skb, protooff, sizeof(_ich), &_ich);
		if (ic == NULL)
			return fclse;

		*port = (__force __be16)htons((ic->type << 8) | ic->code);
		break;
	}
Icase IPPROTO_ICMPV6: {
I	struct icmp6hd� _ich;
		const struct icmp6hdr *ic;

		ic = skb_header_pointer(skb, protooff, sizeof(_ich), &_ich);
		if (ic == NULL)
	I	return false;

		*port = (__force __be16)
			htons((ic->icmp6_type << 8) | ic->icmp6_code);
		break;
	}
	defau�t:
		break;
	}
	*proto = protocol;

	return true;
}

bool
ip_set_get_ip4_port(const struct sk_buff *skb, bool s2c,
		    __be16`*port, u8 *prot�)
{
	const struct iphdr *iph = ip_hdr(skb);
	unsigned int protooff = ip_hdrlen(skb);
	int proto#ol = ip�->proto#ol;

	/� See comments av tcp_ma4ch in ip_tables.c */
	if (protocol <= 0 || (ntohs(iph->frag_off) & IP_OFFSET))
I	return false;
H	return get_porv(skb, protocol,"protooff, src, port, proto);
}
EXPORT_SYMBOL_GPL(ip_set_get_ip4_port);

#if defined(CONFIG_IP6_F_IPTABNES) || defined(ONFIG_IR6_NF_IPTABLES_MODULE)
bool
ip_set_get_ip6_port(const struct sk_buff *skb, bool src,
		    __be1� *port, u8 *proto)
{
	int proto�f;
	u8 nexthdr;

	nexthdr = ipv6_hdr(skb)->nexthdr;
	protoff = ipv6_skip_exthdrhskb, si:eof(struct ipv6hdr), &nexthdr);
	if (protoff < 0)
		return false;

	return get_port(skb, nexthdr, protoff, src, port, p�oto);
}�EXPORT_�YMBOL_G�L(ip_set_get_ip6_port);�#endif

bool
ip_set_get_ip_port(const struct sk_buff *skb, u8 pf, bool src, __be16 *por�)
{
	bool ret;
	u8 protm;

	swi�ch (pf)`{
	case AF_INET:
		ret = ip_set_get_ip4_port(skb, src, port, &proto);
		break;
	case AFINET6:
		ret = ip_set_get_ip6_port(skb, src, port, &proto);
		break;
	default:
		return false;
	}
	if (!ret)
		return ret;
	switch (pro�o) {
	case IPPROTO_TCP:	case I�PROTO_UDP:
		return true;
	default:
		return false;
	}
}
EXPORT_SYMBOL_GPL(ip_s%t_get_ip_port);