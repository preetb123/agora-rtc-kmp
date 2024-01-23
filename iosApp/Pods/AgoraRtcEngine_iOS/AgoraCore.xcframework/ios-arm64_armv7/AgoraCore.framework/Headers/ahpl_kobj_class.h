/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Dec 8th, 2022
 * Module:	AHPL value object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 ~ 2022 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_KOBJ_CPP_H__
#define __AHPL_KOBJ_CPP_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_kobj.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif


class ahpl_kobj_class {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_kobj_class () = delete;
	ahpl_kobj_class (const ahpl_kobj_class &) = delete;
	ahpl_kobj_class (ahpl_kobj_class &&) = delete;
	ahpl_kobj_class &operator = (const ahpl_kobj_class &) = delete;
	ahpl_kobj_class &operator = (ahpl_kobj_class &&) = delete;
	~ahpl_kobj_class () = delete;
#else
	ahpl_kobj_class ();
	ahpl_kobj_class (const ahpl_kobj_class &);
	ahpl_kobj_class &operator = (const ahpl_kobj_class &);
	~ahpl_kobj_class ();
#endif

public:
	static void *notify_register (ahpl_kfd_t dfd, const char *name, int touch, ahpl_mpq_t qid, ahpl_ref_t ref, const char *f_name, ahpl_kobj_notify_t f, ahpl_kobj_notify_dtor_t dtor, uintptr_t argc, ...)
	{
		va_list args;
		void *err;

		va_start (args, argc);
		err = ahpl_kobj_notify_register_args (dfd, name, touch, qid, ref, f_name, f, dtor, argc, args);
		va_end (args);
		return err;
	}

	static void *notify_register_args (ahpl_kfd_t dfd, const char *name, int touch, ahpl_mpq_t qid, ahpl_ref_t ref, const char *f_name, ahpl_kobj_notify_t f, ahpl_kobj_notify_dtor_t dtor, uintptr_t argc, va_list args)
	{
		return ahpl_kobj_notify_register_args (dfd, name, touch, qid, ref, f_name, f, dtor, argc, args);
	}

	static void *notify_register_argv (ahpl_kfd_t dfd, const char *name, int touch, ahpl_mpq_t qid, ahpl_ref_t ref, const char *f_name, ahpl_kobj_notify_t f, ahpl_kobj_notify_dtor_t dtor, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_kobj_notify_register_argv (dfd, name, touch, qid, ref, f_name, f, dtor, argc, argv);
	}

	static int notify_unregister (ahpl_kfd_t dfd, const char *name, ahpl_mpq_t qid, ahpl_ref_t ref, void *handle)
	{
		return ahpl_kobj_notify_unregister (dfd, name, handle);
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <void (ahpl_refobj_t robj, const char *dirpath, const char *name, void *notif_data)> ahpl_kobj_notify_lambda_f;
	static void *notify_register (ahpl_kfd_t dfd, const char *name, int touch, ahpl_mpq_t qid, ahpl_ref_t ref, const char *f_name, ahpl_kobj_notify_lambda_f&& task)
	{
		ahpl_kobj_notify_lambda_f *task_obj = new ahpl_kobj_notify_lambda_f (std::move (task));
		void *err = notify_register (dfd, name, touch, qid, ref, f_name, ____notify_f, ____notify_dtor, 1, task_obj);
		if (err == NULL)
			delete task_obj;

		return err;
	}

private:
	static void ____notify_f (ahpl_refobj_t robj, const char *dirpath, const char *name, void *notif_data, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_kobj_notify_lambda_f *task_obj = reinterpret_cast<ahpl_kobj_notify_lambda_f *>(argv [0]);
		(*task_obj) (robj, dirpath, name, notif_data);
	}

	static void ____notify_dtor (uintptr_t argc, uintptr_t argv [])
	{
		ahpl_kobj_notify_lambda_f *task_obj = reinterpret_cast<ahpl_kobj_notify_lambda_f *>(argv [0]);
		delete task_obj;
	}
#endif
};


#endif /* __AHPL_KOBJ_CPP_H__ */