/*************************************************************
 * Author:	Lionfore Hao
 * Date	 :	Nov 6th, 2018
 * Module:	AHPL mpq timer C++ 11 lambda implementation
 *
 *
 * This is a part of the Agora SDK.
 * Copyright (C) 2018 Agora IO. All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_MPQ_TIMER_LAMBDA_H__
#define __AHPL_MPQ_TIMER_LAMBDA_H__

#include <stdlib.h>
#include <functional>
#include <api/ahpl_types.h>
#include <api/ahpl_time.h>
#include <api/ahpl_mpq_timer.h>

typedef std::function <void (const ahpl_ts_t &now)> ahpl_timer_lambda_func_t;

class ahpl_mpq_timer_base {
public:
	ahpl_mpq_timer_base (ahpl_timer_lambda_func_t&& timer_cb, uintptr_t interval, ahpl_ts_t expire_time,
					ahpl_mpq_t q = AHPL_MPQ_INVALID, bool start = true): on_timer (std::move (timer_cb))
	{
		if (ahpl_mpq_invalid (q)) {
			q = ahpl_mpq_this ();
			if (ahpl_mpq_invalid (q))
				abort ();
		}

		if (expire_time != 0) {
			timer = ahpl_mpq_set_oneshot_timer (q, expire_time, __on_timer, NULL, 1, this);
		} else {
			if (interval == AHPL_INVALID_TIMER_INTERVAL) {
				timer = ahpl_mpq_create_oneshot_timer (q, __on_timer, NULL, 1, this);
			} else {
				if (start) {
					timer = ahpl_mpq_set_timer (q, interval, __on_timer, NULL, 1, this);
				} else {
					timer = ahpl_mpq_create_timer (q, interval, __on_timer, NULL, 1, this);
				}
			}
		}

		if (ahpl_mpq_timer_invalid (timer))
			abort ();
	}

	~ahpl_mpq_timer_base ()
	{
		if (ahpl_mpq_kill_timer (timer) < 0)
			abort ();
	}

	ahpl_timer_t timer_id (void)
	{
		return timer;
	}

	int interval (uintptr_t *interval_p)
	{
		return ahpl_mpq_timer_interval (timer, interval_p);
	}

	int active (bool *active_p)
	{
		int active;
		int err = ahpl_mpq_timer_active (timer, &active);
		if (err == 0 && active_p != NULL)
			*active_p = (bool)(active != 0);

		return err;
	}

	void cancel ()
	{
		ahpl_mpq_cancel_timer (timer);
	}

private:
	static void __on_timer (ahpl_timer_t timer, const ahpl_ts_t *now_p, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_mpq_timer_base *__this = (ahpl_mpq_timer_base *)argv [0];
		__this->on_timer (*now_p);
	}

	ahpl_timer_t timer;
	const ahpl_timer_lambda_func_t on_timer;

private:
	ahpl_mpq_timer_base () = delete;
	ahpl_mpq_timer_base (const ahpl_mpq_timer_base &) = delete;
	ahpl_mpq_timer_base (ahpl_mpq_timer_base &&) = delete;
	ahpl_mpq_timer_base &operator = (const ahpl_mpq_timer_base &) = delete;
	ahpl_mpq_timer_base &operator = (ahpl_mpq_timer_base &&) = delete;
};


class ahpl_mpq_timer: public ahpl_mpq_timer_base {
public:
	ahpl_mpq_timer (ahpl_timer_lambda_func_t&& timer_cb, uintptr_t interval, ahpl_mpq_t q = AHPL_MPQ_INVALID, bool start = true)
				: ahpl_mpq_timer_base (std::move (timer_cb), interval, 0, q, start) {}

	int resched (uintptr_t interval = AHPL_INVALID_TIMER_INTERVAL)
	{
		return ahpl_mpq_resched_timer (timer_id (), interval);
	}

private:
	ahpl_mpq_timer () = delete;
	ahpl_mpq_timer (const ahpl_mpq_timer &) = delete;
	ahpl_mpq_timer (ahpl_mpq_timer &&) = delete;
	ahpl_mpq_timer &operator = (const ahpl_mpq_timer &) = delete;
	ahpl_mpq_timer &operator = (ahpl_mpq_timer &&) = delete;
};

class ahpl_mpq_oneshot_timer: public ahpl_mpq_timer_base {
public:
	ahpl_mpq_oneshot_timer (ahpl_timer_lambda_func_t&& timer_cb, ahpl_ts_t expire_time = 0, ahpl_mpq_t q = AHPL_MPQ_INVALID)
				: ahpl_mpq_timer_base (std::move (timer_cb), AHPL_INVALID_TIMER_INTERVAL, expire_time, q) {}

	int sched (ahpl_ts_t expire_time)
	{
		return ahpl_mpq_resched_oneshot_timer (timer_id (), expire_time);
	}

private:
	ahpl_mpq_oneshot_timer () = delete;
	ahpl_mpq_oneshot_timer (const ahpl_mpq_oneshot_timer &) = delete;
	ahpl_mpq_oneshot_timer (ahpl_mpq_oneshot_timer &&) = delete;
	ahpl_mpq_oneshot_timer &operator = (const ahpl_mpq_oneshot_timer &) = delete;
	ahpl_mpq_oneshot_timer &operator = (ahpl_mpq_oneshot_timer &&) = delete;
};


#endif /* __AHPL_MPQ_TIMER_LAMBDA_H__ */