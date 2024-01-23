/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Aug 19th, 2020
 * Module:	AHPL file operations for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_FILE_CPP_H__
#define __AHPL_FILE_CPP_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_defs.h>
#include <api/ahpl_file.h>
#include <api/ahpl_errno.h>

#if (__cplusplus >= 201103) || defined (_MSC_VER)
#include <functional>
#include <memory>
#endif

class ahpl_file_class {
private:
	ahpl_fd_t fd;

public:
	ahpl_file_class ()
	{
		fd = AHPL_INVALID_FD;
	}

	int open (const char *file, int oflag, ...)
	{
		int mode = 0;

		if (!ahpl_fd_invalid (fd)) {
			ahpl_file_close (fd);
			fd = AHPL_INVALID_FD;
		}

		if ((oflag & O_CREAT) != 0) {
			va_list arg;
			va_start (arg, oflag);
			mode = va_arg (arg, int);
			va_end (arg);
		}

		fd = ahpl_file_open (file, oflag, mode);
		if (ahpl_fd_invalid (fd))
			return -1;

		return 0;
	}

	ssize_t size ()
	{
		return ahpl_file_size (fd);
	}

	ssize_t read (void *buf, size_t count)
	{
		return ahpl_file_read (fd, buf, count);
	}

	ssize_t write (const void *buf, size_t count)
	{
		return ahpl_file_write (fd, buf, count);
	}

	off_t lseek (off_t offset, int whence)
	{
		return ahpl_file_lseek (fd, offset, whence);
	}

	int aread (size_t count, ahpl_file_aread_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_file_aread_args (fd, count, f, argc, args);
		va_end (args);

		return err;
	}

	int aread_args (size_t count, ahpl_file_aread_t f, uintptr_t argc, va_list args)
	{
		return ahpl_file_aread_args (fd, count, f, argc, args);
	}

	int aread_argv (size_t count, ahpl_file_aread_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_file_aread_argv (fd, count, f, argc, argv);
	}

	int awrite (const void *buf, size_t count, ahpl_file_awrite_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_file_awrite_args (fd, buf, count, f, argc, args);
		va_end (args);

		return err;
	}

	int awrite_args (const void *buf, size_t count, ahpl_file_awrite_t f, uintptr_t argc, va_list args)
	{
		return ahpl_file_awrite_args (fd, buf, count, f, argc, args);
	}

	int awrite_argv (const void *buf, size_t count, ahpl_file_awrite_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_file_awrite_argv (fd, buf, count, f, argc, argv);
	}

	int alseek (off_t offset, int whence, ahpl_file_alseek_t f, uintptr_t argc, ...)
	{
		va_list args;
		int err;

		va_start (args, argc);
		err = ahpl_file_alseek_args (fd, offset, whence, f, argc, args);
		va_end (args);

		return err;
	}

	int alseek_args (off_t offset, int whence, ahpl_file_alseek_t f, uintptr_t argc, va_list args)
	{
		return ahpl_file_alseek_args (fd, offset, whence, f, argc, args);
	}

	int alseek_argv (off_t offset, int whence, ahpl_file_alseek_t f, uintptr_t argc, uintptr_t argv [])
	{
		return ahpl_file_alseek_argv (fd, offset, whence, f, argc, argv);
	}

	/* C++11 lambda encapsulations */
#if (__cplusplus >= 201103) || defined (_MSC_VER)
public:
	typedef std::function <void (bool free_only, ahpl_fd_t fd, ssize_t ret, void *buf)> ahpl_file_aread_lambda_f;
	typedef std::function <void (bool free_only, ahpl_fd_t fd, ssize_t ret)> ahpl_file_awrite_lambda_f;
	typedef std::function <void (bool free_only, ahpl_fd_t fd, off_t ret)> ahpl_file_alseek_lambda_f;

	int aread (size_t count, ahpl_file_aread_lambda_f &&lambda)
	{
		if (lambda != nullptr) {
			ahpl_file_aread_lambda_f *lambda_p = new ahpl_file_aread_lambda_f (std::move (lambda));
			int err = aread (count, ____aread_f, 1, lambda_p);
			if (err < 0)
				delete lambda_p;

			return err;
		}

		ahpl_errno = EINVAL;
		return -1;
	}

	int awrite (const void *buf, size_t count, ahpl_file_awrite_lambda_f &&lambda = nullptr)
	{
		int err;

		if (lambda == nullptr) {
			err = awrite (buf, count, NULL, 0);
		} else {
			ahpl_file_awrite_lambda_f *lambda_p = new ahpl_file_awrite_lambda_f (std::move (lambda));
			err = awrite (buf, count, ____awrite_f, 1, lambda_p);
			if (err < 0)
				delete lambda_p;
		}

		return err;
	}

	int alseek (off_t offset, int whence, ahpl_file_alseek_lambda_f &&lambda = nullptr)
	{
		int err;

		if (lambda == nullptr) {
			err = alseek (offset, whence, NULL, 0);
		} else {
			ahpl_file_alseek_lambda_f *lambda_p = new ahpl_file_alseek_lambda_f (std::move (lambda));
			err = alseek (offset, whence, ____alseek_f, 1, lambda_p);
			if (err < 0)
				delete lambda_p;
		}

		return err;
	}

private:
	static void ____aread_f (int free_only, ahpl_fd_t fd, ssize_t ret, void *buf, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_file_aread_lambda_f *lambda_f = (ahpl_file_aread_lambda_f *)argv [0];
		(*lambda_f) ((bool)(free_only != 0), fd, ret, buf);
		delete lambda_f;
	}

	static void ____awrite_f (int free_only, ahpl_fd_t fd, ssize_t ret, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_file_awrite_lambda_f *lambda_f = (ahpl_file_awrite_lambda_f *)argv [0];
		(*lambda_f) ((bool)(free_only != 0), fd, ret);
		delete lambda_f;
	}

	static void ____alseek_f (int free_only, ahpl_fd_t fd, off_t ret, uintptr_t argc, uintptr_t argv [])
	{
		ahpl_file_alseek_lambda_f *lambda_f = (ahpl_file_alseek_lambda_f *)argv [0];
		(*lambda_f) ((bool)(free_only != 0), fd, ret);
		delete lambda_f;
	}
#endif

public:
	int close ()
	{
		if (!ahpl_fd_invalid (fd)) {
			int err;
			err = ahpl_file_close (fd);
			fd = AHPL_INVALID_FD;
			return err;
		}

		return -1;
	}

	virtual ~ahpl_file_class ()
	{
		if (!ahpl_fd_invalid (fd))
			ahpl_file_close (fd);
	}
};


#endif /* __AHPL_FILE_CPP_H__ */