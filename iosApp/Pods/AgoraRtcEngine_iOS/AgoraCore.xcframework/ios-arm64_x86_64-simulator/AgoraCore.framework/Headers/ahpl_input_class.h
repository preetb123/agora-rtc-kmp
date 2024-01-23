/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	May 21st, 2019
 * Module:	AHPL input object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 ~ 2019 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_INPUT_CLASS_H__
#define __AHPL_INPUT_CLASS_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_defs.h>
#include <api/ahpl_mpq.h>
#include <api/ahpl_ref.h>
#include <api/ahpl_input.h>
#include <api/cpp/ahpl_ref_class.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif


class ahpl_input_class: public ahpl_ref_class {
public:
	ahpl_input_class (uintptr_t waitings_max, ahpl_mpq_t input_q = ahpl_mpq_main ())
				: ahpl_ref_class (ahpl_input_create (this, __dtor, waitings_max, input_q))
	{
		if (ahpl_ref_invalid (ref ()))
			abort ();
	}

	int exec (const char *f_name, ahpl_input_func_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_input_exec_args (ref (), f_name, f, argc, args);
		va_end (args);

		return err;
	}

	int exec_args (const char *f_name, ahpl_input_func_t f, uintptr_t argc, va_list args)
	{
		return ahpl_input_exec_args (ref (), f_name, f, argc, args);
	}

	int exec_argv (const char *f_name, ahpl_input_func_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_input_exec_argv (ref (), f_name, f, argc, argv);
	}

	int waitings_count ()
	{
		return ahpl_input_waitings_count (ref ());
	}

	int remove_waitings_head ()
	{
		return ahpl_input_remove_waitings_head (ref ());
	}

protected:
	/* We do not allow delete this object directly. */
	virtual ~ahpl_input_class ()
	{
	}

private:
	static void __dtor (void *arg)
	{
		ahpl_input_class *__this = (ahpl_input_class *)arg;
		delete __this;
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <void (ahpl_refobj_t iobj, bool free_only)> ahpl_input_lambda_f;

	int exec (const char *f_name, ahpl_input_lambda_f&& input)
	{
		ahpl_input_lambda_f *input_obj = new ahpl_input_lambda_f (std::move (input));
		int err = ahpl_input_class::exec (f_name, ____input_f, 1, input_obj);
		if (err < 0)
			delete input_obj;

		return err;
	}

private:
	static void ____input_f (ahpl_refobj_t iobj, int free_only, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_input_lambda_f *input_obj = reinterpret_cast<ahpl_input_lambda_f *>(argv [0]);
		(*input_obj) (iobj, (bool)(free_only != 0));
		delete input_obj;
	}
#endif
};


#if (__cplusplus >= 201103) || defined (_MSC_VER)
typedef std::unique_ptr<ahpl_input_class, ahpl_ref_class::deleter> ahpl_input_class_unique_ptr;
#endif


#endif /* __AHPL_INPUT_CLASS_H__ */