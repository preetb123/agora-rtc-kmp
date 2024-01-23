/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Nov 19th, 2018
 * Module:	AHPL task object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_TASK_CLASS_H__
#define __AHPL_TASK_CLASS_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_defs.h>
#include <api/ahpl_mpq.h>
#include <api/ahpl_ref.h>
#include <api/ahpl_task.h>
#include <api/cpp/ahpl_ref_class.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif


class ahpl_task_class: public ahpl_ref_class {
public:
	ahpl_task_class (ahpl_task_type_t type, bool must_serial = false, ahpl_mpq_t st_op_q = ahpl_mpq_main (), bool resume = true, ahpl_task_res_wait_t wait_f = NULL)
				: ahpl_ref_class (ahpl_task_create (this, __dtor, type, (int)must_serial, st_op_q, (int)(resume == true), wait_f))
	{
		if (ahpl_ref_invalid (ref ()))
			abort ();
	}

	ahpl_task_class (): ahpl_ref_class (AHPL_REF_INVALID)
	{
	}

	ahpl_task_type_t get_type ()
	{
		return ahpl_task_get_type (ref ());
	}

	int exec (const char *f_name, ahpl_task_func_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_task_exec_args (ref (), f_name, f, argc, args);
		va_end (args);

		return err;
	}

	int exec_args (const char *f_name, ahpl_task_func_t f, uintptr_t argc, va_list args)
	{
		return ahpl_task_exec_args (ref (), f_name, f, argc, args);
	}

	int exec_argv (const char *f_name, ahpl_task_func_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_task_exec_argv (ref (), f_name, f, argc, argv);
	}

	int exclusive_exec (ahpl_task_func_t exclusive_f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_task_exclusive_exec_args (ref (), exclusive_f, argc, args);
		va_end (args);

		return err;
	}

	int exclusive_exec_args (ahpl_task_func_t exclusive_f, uintptr_t argc, va_list args)
	{
		return ahpl_task_exclusive_exec_args (ref (), exclusive_f, argc, args);
	}

	int exclusive_exec_argv (ahpl_task_func_t exclusive_f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_task_exclusive_exec_argv (ref (), exclusive_f, argc, argv);
	}

	int waiting_ops_count ()
	{
		return ahpl_task_waiting_ops_count (ref ());
	}

	int remove_waiting_ops_head ()
	{
		return ahpl_task_remove_waiting_ops_head (ref ());
	}

	int clear ()
	{
		return ahpl_task_clear (ref ());
	}

	int async_done ()
	{
		return ahpl_task_async_done (ref ());
	}

	int async_done (uintptr_t opaque)
	{
		return ahpl_task_async_done_opaque (ref (), opaque);
	}

protected:
	/* We do not allow delete this object directly. */
	virtual ~ahpl_task_class ()
	{
	}

private:
	static void __dtor (void *arg)
	{
		ahpl_task_class *__this = (ahpl_task_class *)arg;
		delete __this;
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <void (ahpl_refobj_t tobj, ahpl_task_act_t action, uintptr_t opaque, uintptr_t &res)> ahpl_task_lambda_f;
	typedef std::function <void (ahpl_task_act_t action)> ahpl_task_lambda_exclusive_f;

	int exec (const char *f_name, ahpl_task_lambda_f&& task)
	{
		ahpl_task_lambda_f *task_obj = new ahpl_task_lambda_f (std::move (task));
		int err = ahpl_task_class::exec (f_name, ____task_f, 2, 0, task_obj);
		if (err < 0)
			delete task_obj;

		return err;
	}

	int exclusive_exec (ahpl_task_lambda_exclusive_f&& task)
	{
		ahpl_task_lambda_exclusive_f *task_obj = new ahpl_task_lambda_exclusive_f (std::move (task));
		int err = ahpl_task_class::exclusive_exec (____exclusive_exec_f, 1, task_obj);
		if (err < 0)
			delete task_obj;

		return err;
	}

private:
	static void ____task_f (ahpl_refobj_t tobj, ahpl_task_act_t action, uintptr_t opaque, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_task_lambda_f *task_obj = reinterpret_cast<ahpl_task_lambda_f *>(argv [1]);
		(*task_obj) (tobj, action, opaque, argv [0]);
		if (action != ahpl_task_act_exec) {
			/**
			 * Do not free the task object for the task object 'run' case,
			 * because the task object is still used later.
			 * -- Lionfore Hao Nov 19th, 2018
			 **/
			delete task_obj;
		}
	}

	static void ____exclusive_exec_f (ahpl_refobj_t tobj, ahpl_task_act_t action, uintptr_t opaque, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_task_lambda_exclusive_f *task_obj = reinterpret_cast<ahpl_task_lambda_exclusive_f *>(argv [0]);
		(*task_obj) (action);
		delete task_obj;
	}

	/**
	 * The stackless coroutine like implementation in AHPL. We could not
	 * support the real stackless coroutine except in the language level,
	 * so we just provide similar equivalent functionals here.
	 **/
public:
	typedef std::function <int (int free_only)> ahpl_task_prepare_lambda_f;

	static int prepare (ahpl_stack_id_t stack_id, const char *f_name, ahpl_task_prepare_lambda_f&& task)
	{
		ahpl_task_prepare_lambda_f *prepare_f = new ahpl_task_prepare_lambda_f (std::move (task));
		int err = ahpl_task_prepare (stack_id, f_name, ____async_prepare_f, 1, prepare_f);
		if (err < 0)
			delete prepare_f;

		return err;
	}

private:
	static int ____async_prepare_f (int free_only, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_task_prepare_lambda_f *prepare_f = reinterpret_cast<ahpl_task_prepare_lambda_f *>(argv [0]);
		int err;
		err = (*prepare_f) (free_only);
		delete prepare_f;
		return err;
	}

public:
	typedef std::function <void (int free_only)> ahpl_task_resume_lambda_f;

	static int resume (ahpl_stack_id_t stack_id, const char *f_name, ahpl_task_resume_lambda_f&& task)
	{
		ahpl_task_resume_lambda_f *resume_f = new ahpl_task_resume_lambda_f (std::move (task));
		int err = ahpl_task_resume (stack_id, f_name, ____async_resume_f, 1, resume_f);
		if (err < 0)
			delete resume_f;

		return err;
	}

private:
	static void ____async_resume_f (int free_only, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_task_resume_lambda_f *resume_f = reinterpret_cast<ahpl_task_resume_lambda_f *>(argv [0]);
		(*resume_f) (free_only);
		delete resume_f;
	}
#endif
};


#if (__cplusplus >= 201103) || defined (_MSC_VER)
typedef std::unique_ptr<ahpl_task_class, ahpl_ref_class::deleter> ahpl_task_class_unique_ptr;
#endif



#endif /* __AHPL_TASK_CLASS_H__ */