/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Oct 27th, 2019
 * Module:	AHPL queue object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 ~ 2019 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_QUEUE_CLASS_H__
#define __AHPL_QUEUE_CLASS_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_defs.h>
#include <api/ahpl_mpq.h>
#include <api/ahpl_ref.h>
#include <api/ahpl_queue.h>
#include <api/cpp/ahpl_ref_class.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif


class ahpl_queue_class: public ahpl_ref_class {
public:
	ahpl_queue_class (ahpl_mpq_t queue_q = ahpl_mpq_main ()): ahpl_ref_class (ahpl_queue_create (this, __dtor, queue_q))
	{
		if (ahpl_ref_invalid (ref ()))
			abort ();
	}

	int queue (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_queue_add_args (ref (), f_name, f, argc, args);
		va_end (args);

		return err;
	}

	int queue_args (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, va_list args)
	{
		return ahpl_queue_add_args (ref (), f_name, f, argc, args);
	}

	int queue_argv (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_queue_add_argv (ref (), f_name, f, argc, argv);
	}

	int call (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_queue_call_args (ref (), f_name, f, argc, args);
		va_end (args);

		return err;
	}

	int call_args (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, va_list args)
	{
		return ahpl_queue_call_args (ref (), f_name, f, argc, args);
	}

	int call_argv (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_queue_call_argv (ref (), f_name, f, argc, argv);
	}

	int run (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_queue_run_args (ref (), f_name, f, argc, args);
		va_end (args);

		return err;
	}

	int run_args (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, va_list args)
	{
		return ahpl_queue_run_args (ref (), f_name, f, argc, args);
	}

	int run_argv (const char *f_name, ahpl_queue_func_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_queue_run_argv (ref (), f_name, f, argc, argv);
	}

	int set_max (uint32_t max)
	{
		return ahpl_queue_set_max (ref (), max);
	}

	int count () const
	{
		return ahpl_queue_count (ref ());
	}

	int clear ()
	{
		return ahpl_queue_clear (ref ());
	}

	int run_func_arg (uintptr_t n, uintptr_t *arg_p) const
	{
		return ahpl_queue_run_func_arg (ref (), n, arg_p);
	}

protected:
	/* We do not allow delete this object directly. */
	virtual ~ahpl_queue_class ()
	{
	}

private:
	static void __dtor (void *arg)
	{
		ahpl_queue_class *__this = (ahpl_queue_class *)arg;
		delete __this;
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <void (ahpl_refobj_t qobj, bool free_only, const ahpl_ts_t &queued_ts)> ahpl_queue_lambda_f;

	int queue (const char *f_name, ahpl_queue_lambda_f&& queue)
	{
		ahpl_queue_lambda_f *queue_obj = new ahpl_queue_lambda_f (std::move (queue));
		int err = ahpl_queue_class::queue (f_name, ____queue_f, 1, queue_obj);
		if (err < 0)
			delete queue_obj;

		return err;
	}

	int call (const char *f_name, ahpl_queue_lambda_f&& queue, void *result_var_p = NULL)
	{
		ahpl_queue_lambda_f *queue_obj = new ahpl_queue_lambda_f (std::move (queue));
		int err = ahpl_queue_class::call (f_name, ____queue_f, 2, queue_obj, result_var_p);
		if (err < 0)
			delete queue_obj;

		return err;
	}

	int run (const char *f_name, ahpl_queue_lambda_f&& queue)
	{
		ahpl_queue_lambda_f *queue_obj = new ahpl_queue_lambda_f (std::move (queue));
		int err = ahpl_queue_class::run (f_name, ____queue_f, 1, queue_obj);
		if (err < 0)
			delete queue_obj;

		return err;
	}

	void *call_result_var_addr (void)
	{
		void *var_addr;

		if (run_func_arg (1, (uintptr_t *)&var_addr) < 0)
			return NULL;

		return var_addr;
	}

private:
	static void ____queue_f (ahpl_refobj_t qobj, int free_only, const ahpl_ts_t *queued_ts_p, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_queue_lambda_f *queue_obj = reinterpret_cast<ahpl_queue_lambda_f *>(argv [0]);
		(*queue_obj) (qobj, (bool)(free_only != 0), *queued_ts_p);
		delete queue_obj;
	}
#endif
};


#if (__cplusplus >= 201103) || defined (_MSC_VER)
typedef std::unique_ptr<ahpl_queue_class, ahpl_ref_class::deleter> ahpl_queue_class_unique_ptr;
#endif


#endif /* __AHPL_QUEUE_CLASS_H__ */