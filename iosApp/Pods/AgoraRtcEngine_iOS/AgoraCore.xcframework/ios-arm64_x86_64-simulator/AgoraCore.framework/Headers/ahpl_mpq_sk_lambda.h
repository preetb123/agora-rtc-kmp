/*************************************************************
 * Author:	Lionfore Hao
 * Date	 :	Nov 6th, 2018
 * Module:	AHPL mpq socket C++ 11 lambda implementation
 *
 *
 * This is a part of the Agora SDK.
 * Copyright (C) 2018 Agora IO. All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_MPQ_SK_LAMBDA_H__
#define __AHPL_MPQ_SK_LAMBDA_H__

#include <stdlib.h>

#include <functional>
#include <api/ahpl_types.h>
#include <api/ahpl_mpq_fd.h>
#include <api/ahpl_mpq_net.h>
#include <api/cpp/ahpl_mpq_fd_class.h>

typedef std::function <ssize_t (const void *data, size_t len)> ahpl_fd_chk_pkt_lambda_t;
typedef std::function <void (void *data, size_t len)> ahpl_fd_data_lambda_t;
typedef std::function <void (void *data, size_t len, const ahpl_sk_addr_t *addr)> ahpl_dgram_sk_data_lambda_t;
typedef std::function <void (ahpl_fd_t sk, int event)> ahpl_fd_event_lambda_t;
typedef std::function <void (const ahpl_accept_data_t *accept_data)> ahpl_listen_sk_accept_lambda_t;

class ahpl_mpq_dgram_sk: public ahpl_mpq_fd_class {
public:
	ahpl_mpq_dgram_sk (ahpl_fd_t sk, size_t max_pkt_size,
						ahpl_dgram_sk_data_lambda_t&& data_cb,
							ahpl_fd_event_lambda_t&& event_cb,
							ahpl_mpq_t qid = ahpl_mpq_this ())
						: ahpl_mpq_fd_class (sk),
						  on_data (std::move (data_cb)), 
						  on_event (std::move (event_cb))
	{
		if (ahpl_mpq_add_dgram_socket (qid, sk, max_pkt_size, &__on_data, &__on_event, 1, this) < 0)
			abort ();
	}

private:
	static void __on_data (void *data, size_t len, uintptr_t argc, uintptr_t argv [], const ahpl_sk_addr_t *addr)
	{
		ahpl_mpq_dgram_sk *__this = (ahpl_mpq_dgram_sk *)argv [0];
		__this->on_data (data, len, addr);
	}

	static void __on_event (ahpl_fd_t sk, int event, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_dgram_sk *__this = (ahpl_mpq_dgram_sk *)argv [0];
		__this->on_event (sk, event);
	}

	const ahpl_dgram_sk_data_lambda_t on_data;
	const ahpl_fd_event_lambda_t on_event;

private:
	ahpl_mpq_dgram_sk () = delete;
	ahpl_mpq_dgram_sk (const ahpl_mpq_dgram_sk &) = delete;
	ahpl_mpq_dgram_sk (ahpl_mpq_dgram_sk &&) = delete;
	ahpl_mpq_dgram_sk &operator = (const ahpl_mpq_dgram_sk &) = delete;
	ahpl_mpq_dgram_sk &operator = (ahpl_mpq_dgram_sk &&) = delete;
};

class ahpl_mpq_stream_sk: public ahpl_mpq_fd_class {
public:
	ahpl_mpq_stream_sk (ahpl_fd_t sk, int enable, size_t max_pkt_size,
								ahpl_fd_chk_pkt_lambda_t&& chk_pkt_cb,
									ahpl_fd_data_lambda_t&& data_cb,
									ahpl_fd_event_lambda_t&& event_cb,
									ahpl_mpq_t qid = ahpl_mpq_this ())
					: ahpl_mpq_fd_class (sk),
					  on_chk_pkt (std::move (chk_pkt_cb)),
					  on_data (std::move (data_cb)), 
					  on_event (std::move (event_cb))
	{
		if (ahpl_mpq_add_stream_socket (qid, sk, enable, max_pkt_size, &__on_chk_pkt, &__on_data, &__on_event, 1, this) < 0)
			abort ();
	}

private:
	static ssize_t __on_chk_pkt (const void *data, size_t len, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_stream_sk *__this = (ahpl_mpq_stream_sk *)argv [0];
		return __this->on_chk_pkt (data, len);
	}

	static void __on_data (void *data, size_t len, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_stream_sk *__this = (ahpl_mpq_stream_sk *)argv [0];
		__this->on_data (data, len);
	}

	static void __on_event (ahpl_fd_t sk, int event, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_stream_sk *__this = (ahpl_mpq_stream_sk *)argv [0];
		__this->on_event (sk, event);
	}

	const ahpl_fd_chk_pkt_lambda_t on_chk_pkt;
	const ahpl_fd_data_lambda_t on_data;
	const ahpl_fd_event_lambda_t on_event;

private:
	ahpl_mpq_stream_sk () = delete;
	ahpl_mpq_stream_sk (const ahpl_mpq_stream_sk &) = delete;
	ahpl_mpq_stream_sk (ahpl_mpq_stream_sk &&) = delete;
	ahpl_mpq_stream_sk &operator = (const ahpl_mpq_stream_sk &) = delete;
	ahpl_mpq_stream_sk &operator = (ahpl_mpq_stream_sk &&) = delete;
};

class ahpl_mpq_connect_stream_sk: public ahpl_mpq_stream_sk {
public:
	ahpl_mpq_connect_stream_sk (ahpl_fd_t sk, size_t max_pkt_size,
							ahpl_fd_chk_pkt_lambda_t&& chk_pkt_cb,
								ahpl_fd_data_lambda_t&& data_cb,
								ahpl_fd_event_lambda_t&& event_cb,
								ahpl_mpq_t qid = ahpl_mpq_this ())
								: ahpl_mpq_stream_sk (sk, 0/* !enabled */,
								  max_pkt_size,
								  std::move (chk_pkt_cb),
								  std::move (data_cb),
								  std::move (event_cb), qid) {}

	int connect (const struct sockaddr *dest_addr, socklen_t addrlen, int timeo)
	{
		return ahpl_mpq_connect (get_fd (), dest_addr, addrlen, timeo);
	}

private:
	ahpl_mpq_connect_stream_sk () = delete;
	ahpl_mpq_connect_stream_sk (const ahpl_mpq_connect_stream_sk &) = delete;
	ahpl_mpq_connect_stream_sk (ahpl_mpq_connect_stream_sk &&) = delete;
	ahpl_mpq_connect_stream_sk &operator = (const ahpl_mpq_connect_stream_sk &) = delete;
	ahpl_mpq_connect_stream_sk &operator = (ahpl_mpq_connect_stream_sk &&) = delete;
};

class ahpl_mpq_accepted_stream_sk: public ahpl_mpq_stream_sk {
public:
	ahpl_mpq_accepted_stream_sk (ahpl_fd_t sk, size_t max_pkt_size,
							ahpl_fd_chk_pkt_lambda_t&& chk_pkt_cb,
								ahpl_fd_data_lambda_t&& data_cb,
								ahpl_fd_event_lambda_t&& event_cb,
								ahpl_mpq_t qid = ahpl_mpq_this ())
								: ahpl_mpq_stream_sk (sk, 1/* enabled */,
								  max_pkt_size,
								  std::move (chk_pkt_cb),
								  std::move (data_cb),
								  std::move (event_cb), qid) {}

private:
	ahpl_mpq_accepted_stream_sk () = delete;
	ahpl_mpq_accepted_stream_sk (const ahpl_mpq_accepted_stream_sk &) = delete;
	ahpl_mpq_accepted_stream_sk (ahpl_mpq_accepted_stream_sk &&) = delete;
	ahpl_mpq_accepted_stream_sk &operator = (const ahpl_mpq_accepted_stream_sk &) = delete;
	ahpl_mpq_accepted_stream_sk &operator = (ahpl_mpq_accepted_stream_sk &&) = delete;
};

class ahpl_mpq_listen_sk: public ahpl_mpq_fd_class {
public:
	ahpl_mpq_listen_sk (ahpl_fd_t sk, int backlog,
		ahpl_listen_sk_accept_lambda_t&& accept_cb,
				ahpl_fd_event_lambda_t&& event_cb,
				ahpl_mpq_t qid = ahpl_mpq_this ())
					: ahpl_mpq_fd_class (sk),
					  on_accept (std::move (accept_cb)),
					  on_event (std::move (event_cb))
	{
		if (ahpl_mpq_add_listen_socket (qid, sk, backlog, __on_accept, __on_event, 1, this) < 0)
			abort ();
	}

private:
	static void __on_accept (ahpl_accept_data_t *accept_data, size_t len, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_listen_sk *__this = (ahpl_mpq_listen_sk *)argv [0];
		__this->on_accept (accept_data);
	}

	static void __on_event (ahpl_fd_t sk, int event, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_listen_sk *__this = (ahpl_mpq_listen_sk *)argv [0];
		__this->on_event (sk, event);
	}

	const ahpl_listen_sk_accept_lambda_t on_accept;
	const ahpl_fd_event_lambda_t on_event;

private:
	ahpl_mpq_listen_sk () = delete;
	ahpl_mpq_listen_sk (const ahpl_mpq_listen_sk &) = delete;
	ahpl_mpq_listen_sk (ahpl_mpq_listen_sk &&) = delete;
	ahpl_mpq_listen_sk &operator = (const ahpl_mpq_listen_sk &) = delete;
	ahpl_mpq_listen_sk &operator = (ahpl_mpq_listen_sk &&) = delete;
};


#endif /* __AHPL_MPQ_SK_LAMBDA_H__ */