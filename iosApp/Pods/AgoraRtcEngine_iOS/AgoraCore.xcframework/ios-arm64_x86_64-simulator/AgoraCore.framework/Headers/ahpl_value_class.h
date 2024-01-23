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

#ifndef __AHPL_VALUE_OBJ_CPP_H__
#define __AHPL_VALUE_OBJ_CPP_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_value.h>
#include <api/cpp/ahpl_kobj_class.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif


class ahpl_value_base: public ahpl_kobj_class {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_base () = delete;
	ahpl_value_base (const ahpl_value_base &) = delete;
	ahpl_value_base (ahpl_value_base &&) = delete;
	ahpl_value_base &operator = (const ahpl_value_base &) = delete;
	ahpl_value_base &operator = (ahpl_value_base &&) = delete;
	~ahpl_value_base () = delete;
#else
	ahpl_value_base ();
	ahpl_value_base (const ahpl_value_base &);
	ahpl_value_base &operator = (const ahpl_value_base &);
	~ahpl_value_base ();
#endif

public:
	static ssize_t get (const char *key, ahpl_value_t &val, ahpl_kfd_t dfd = AHPL_KFD_INVALID)
	{
		ahpl_value_init (&val);
		return ahpl_value_get (dfd, key, &val);
	}

	static ssize_t set (const char *key, const ahpl_value_t &val, ahpl_kfd_t dfd = AHPL_KFD_INVALID)
	{
		return ahpl_value_set (dfd, key, &val);
	}

public:
	static void *verify_register (ahpl_kfd_t dfd, const char *name, ahpl_value_verify_t f, ahpl_value_verify_dtor_t dtor, uintptr_t argc, ...)
	{
		va_list args;
		void *err;

		va_start (args, argc);
		err = ahpl_value_verify_register_args (dfd, name, f, dtor, argc, args);
		va_end (args);
		return err;
	}

	static void *verify_register_args (ahpl_kfd_t dfd, const char *name, ahpl_value_verify_t f, ahpl_value_verify_dtor_t dtor, uintptr_t argc, va_list args)
	{
		return ahpl_value_verify_register_args (dfd, name, f, dtor, argc, args);
	}

	static void *verify_register_argv (ahpl_kfd_t dfd, const char *name, ahpl_value_verify_t f, ahpl_value_verify_dtor_t dtor, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_value_verify_register_argv (dfd, name, f, dtor, argc, argv);
	}

	static int verify_unregister (ahpl_kfd_t dfd, const char *name, void *handle)
	{
		return ahpl_value_verify_unregister (dfd, name, handle);
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <int (const ahpl_value_t *val)> ahpl_value_verify_lambda_f;
	static void *verify_register (ahpl_kfd_t dfd, const char *name, ahpl_value_verify_lambda_f&& task)
	{
		ahpl_value_verify_lambda_f *task_obj = new ahpl_value_verify_lambda_f (std::move (task));
		void *err = verify_register (dfd, name, ____verify_f, ____verify_dtor, 1, task_obj);
		if (err == NULL)
			delete task_obj;

		return err;
	}

private:
	static int ____verify_f (const ahpl_value_t *val, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_value_verify_lambda_f *task_obj = reinterpret_cast<ahpl_value_verify_lambda_f *>(argv [0]);
		return (*task_obj) (val);
	}

	static void ____verify_dtor (uintptr_t argc, uintptr_t argv [])
	{
		ahpl_value_verify_lambda_f *task_obj = reinterpret_cast<ahpl_value_verify_lambda_f *>(argv [0]);
		delete task_obj;
	}
#endif
};

class ahpl_value_bool: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_bool () = delete;
	ahpl_value_bool (const ahpl_value_bool &) = delete;
	ahpl_value_bool (ahpl_value_bool &&) = delete;
	ahpl_value_bool &operator = (const ahpl_value_bool &) = delete;
	ahpl_value_bool &operator = (ahpl_value_bool &&) = delete;
	~ahpl_value_bool () = delete;
#else
	ahpl_value_bool ();
	ahpl_value_bool (const ahpl_value_bool &);
	ahpl_value_bool &operator = (const ahpl_value_bool &);
	~ahpl_value_bool ();
#endif

public:
	static bool get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, bool error_val = false)
	{
		int8_t v;
		if (ahpl_value_get_int8 (dfd, key, &v) != sizeof v)
			return error_val;

		return (bool)(v != 0);
	}

	static ssize_t set (const char *key, bool v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_int8 (dfd, key, weight, (int8_t)v);
	}
};

class ahpl_value_int8: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_int8 () = delete;
	ahpl_value_int8 (const ahpl_value_int8 &) = delete;
	ahpl_value_int8 (ahpl_value_int8 &&) = delete;
	ahpl_value_int8 &operator = (const ahpl_value_int8 &) = delete;
	ahpl_value_int8 &operator = (ahpl_value_int8 &&) = delete;
	~ahpl_value_int8 () = delete;
#else
	ahpl_value_int8 ();
	ahpl_value_int8 (const ahpl_value_int8 &);
	ahpl_value_int8 &operator = (const ahpl_value_int8 &);
	~ahpl_value_int8 ();
#endif

public:
	static int8_t get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, int8_t error_val = 0)
	{
		int8_t v;
		if (ahpl_value_get_int8 (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, int8_t v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_int8 (dfd, key, weight, v);
	}
};

class ahpl_value_int16: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_int16 () = delete;
	ahpl_value_int16 (const ahpl_value_int16 &) = delete;
	ahpl_value_int16 (ahpl_value_int16 &&) = delete;
	ahpl_value_int16 &operator = (const ahpl_value_int16 &) = delete;
	ahpl_value_int16 &operator = (ahpl_value_int16 &&) = delete;
	~ahpl_value_int16 () = delete;
#else
	ahpl_value_int16 ();
	ahpl_value_int16 (const ahpl_value_int16 &);
	ahpl_value_int16 &operator = (const ahpl_value_int16 &);
	~ahpl_value_int16 ();
#endif

public:
	static int16_t get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, int16_t error_val = 0)
	{
		int16_t v;
		if (ahpl_value_get_int16 (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, int16_t v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_int16 (dfd, key, weight, v);
	}
};

class ahpl_value_int32: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_int32 () = delete;
	ahpl_value_int32 (const ahpl_value_int32 &) = delete;
	ahpl_value_int32 (ahpl_value_int32 &&) = delete;
	ahpl_value_int32 &operator = (const ahpl_value_int32 &) = delete;
	ahpl_value_int32 &operator = (ahpl_value_int32 &&) = delete;
	~ahpl_value_int32 () = delete;
#else
	ahpl_value_int32 ();
	ahpl_value_int32 (const ahpl_value_int32 &);
	ahpl_value_int32 &operator = (const ahpl_value_int32 &);
	~ahpl_value_int32 ();
#endif

public:
	static int32_t get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, int32_t error_val = 0)
	{
		int32_t v;
		if (ahpl_value_get_int32 (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, int32_t v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_int32 (dfd, key, weight, v);
	}
};

class ahpl_value_int64: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_int64 () = delete;
	ahpl_value_int64 (const ahpl_value_int64 &) = delete;
	ahpl_value_int64 (ahpl_value_int64 &&) = delete;
	ahpl_value_int64 &operator = (const ahpl_value_int64 &) = delete;
	ahpl_value_int64 &operator = (ahpl_value_int64 &&) = delete;
	~ahpl_value_int64 () = delete;
#else
	ahpl_value_int64 ();
	ahpl_value_int64 (const ahpl_value_int64 &);
	ahpl_value_int64 &operator = (const ahpl_value_int64 &);
	~ahpl_value_int64 ();
#endif

public:
	static int64_t get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, int64_t error_val = 0)
	{
		int64_t v;
		if (ahpl_value_get_int64 (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, int64_t v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_int64 (dfd, key, weight, v);
	}
};

class ahpl_value_intptr: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_intptr () = delete;
	ahpl_value_intptr (const ahpl_value_intptr &) = delete;
	ahpl_value_intptr (ahpl_value_intptr &&) = delete;
	ahpl_value_intptr &operator = (const ahpl_value_intptr &) = delete;
	ahpl_value_intptr &operator = (ahpl_value_intptr &&) = delete;
	~ahpl_value_intptr () = delete;
#else
	ahpl_value_intptr ();
	ahpl_value_intptr (const ahpl_value_intptr &);
	ahpl_value_intptr &operator = (const ahpl_value_intptr &);
	~ahpl_value_intptr ();
#endif

public:
	static intptr_t get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, intptr_t error_val = 0)
	{
		intptr_t v;
		if (ahpl_value_get_intptr (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, intptr_t v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_intptr (dfd, key, weight, v);
	}
};

class ahpl_value_pointer: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_pointer () = delete;
	ahpl_value_pointer (const ahpl_value_pointer &) = delete;
	ahpl_value_pointer (ahpl_value_pointer &&) = delete;
	ahpl_value_pointer &operator = (const ahpl_value_pointer &) = delete;
	ahpl_value_pointer &operator = (ahpl_value_pointer &&) = delete;
	~ahpl_value_pointer () = delete;
#else
	ahpl_value_pointer ();
	ahpl_value_pointer (const ahpl_value_pointer &);
	ahpl_value_pointer &operator = (const ahpl_value_pointer &);
	~ahpl_value_pointer ();
#endif

public:
	static void *get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, void *error_val = NULL)
	{
		void *v;
		if (ahpl_value_get_pointer (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, void *v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_pointer (dfd, key, weight, v);
	}
};

class ahpl_value_float: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_float () = delete;
	ahpl_value_float (const ahpl_value_float &) = delete;
	ahpl_value_float (ahpl_value_float &&) = delete;
	ahpl_value_float &operator = (const ahpl_value_float &) = delete;
	ahpl_value_float &operator = (ahpl_value_float &&) = delete;
	~ahpl_value_float () = delete;
#else
	ahpl_value_float ();
	ahpl_value_float (const ahpl_value_float &);
	ahpl_value_float &operator = (const ahpl_value_float &);
	~ahpl_value_float ();
#endif

public:
	static float get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, float error_val = 1.0)
	{
		float v;
		if (ahpl_value_get_float (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, float v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_float (dfd, key, weight, v);
	}
};

class ahpl_value_double: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_double () = delete;
	ahpl_value_double (const ahpl_value_double &) = delete;
	ahpl_value_double (ahpl_value_double &&) = delete;
	ahpl_value_double &operator = (const ahpl_value_double &) = delete;
	ahpl_value_double &operator = (ahpl_value_double &&) = delete;
	~ahpl_value_double () = delete;
#else
	ahpl_value_double ();
	ahpl_value_double (const ahpl_value_double &);
	ahpl_value_double &operator = (const ahpl_value_double &);
	~ahpl_value_double ();
#endif

public:
	static double get (const char *key, ahpl_kfd_t dfd = AHPL_KFD_INVALID, double error_val = 1.0)
	{
		double v;
		if (ahpl_value_get_double (dfd, key, &v) != sizeof v)
			return error_val;

		return v;
	}

	static ssize_t set (const char *key, double v, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_double (dfd, key, weight, v);
	}
};

class ahpl_value_string: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_string () = delete;
	ahpl_value_string (const ahpl_value_string &) = delete;
	ahpl_value_string (ahpl_value_string &&) = delete;
	ahpl_value_string &operator = (const ahpl_value_string &) = delete;
	ahpl_value_string &operator = (ahpl_value_string &&) = delete;
	~ahpl_value_string () = delete;
#else
	ahpl_value_string ();
	ahpl_value_string (const ahpl_value_string &);
	ahpl_value_string &operator = (const ahpl_value_string &);
	~ahpl_value_string ();
#endif

public:
	static ssize_t get (const char *key, char *str, size_t len, ahpl_kfd_t dfd = AHPL_KFD_INVALID)
	{
		return ahpl_value_get_string (dfd, key, str, len);
	}

	static ssize_t set (const char *key, const char *str, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_string (dfd, key, weight, str);
	}
};

class ahpl_value_data: public ahpl_value_base {
private:
#if (__cplusplus >= 201103) || defined (_MSC_VER)
	ahpl_value_data () = delete;
	ahpl_value_data (const ahpl_value_data &) = delete;
	ahpl_value_data (ahpl_value_data &&) = delete;
	ahpl_value_data &operator = (const ahpl_value_data &) = delete;
	ahpl_value_data &operator = (ahpl_value_data &&) = delete;
	~ahpl_value_data () = delete;
#else
	ahpl_value_data ();
	ahpl_value_data (const ahpl_value_data &);
	ahpl_value_data &operator = (const ahpl_value_data &);
	~ahpl_value_data ();
#endif

public:
	static ssize_t get (const char *key, void *buf, size_t len, ahpl_kfd_t dfd = AHPL_KFD_INVALID)
	{
		return ahpl_value_get_data (dfd, key, buf, len);
	}

	static ssize_t set (const char *key, const void *buf, size_t len, ahpl_kfd_t dfd = AHPL_KFD_INVALID, uint32_t weight = 0)
	{
		return ahpl_value_set_data (dfd, key, weight, buf, len);
	}
};

class ahpl_value_class: public ahpl_value_t {
public:
	ahpl_value_class ()
	{
		ahpl_value_init (this);
	}

	~ahpl_value_class ()
	{
		ahpl_value_free (this);
	}
};


#endif /* __AHPL_VALUE_OBJ_CPP_H__ */