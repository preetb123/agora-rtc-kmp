/*************************************************************
 * Author:	Lionfore Hao
 * Date	 :	Jun 30th, 2019
 * Module:	AHPL mpq fd C++ implementation
 *
 *
 * This is a part of the Agora SDK.
 * Copyright (C) 2019 Agora IO. All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_MPQ_FD_CLASS_H__
#define __AHPL_MPQ_FD_CLASS_H__

#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_mpq_fd.h>


class ahpl_mpq_fd_class {
public:
	ahpl_mpq_fd_class (ahpl_fd_t fd): attached_fd (fd)
	{
	}

	virtual ~ahpl_mpq_fd_class ()
	{
		if (!ahpl_fd_invalid (attached_fd))
			ahpl_close (attached_fd);
	}

	ahpl_fd_t get_fd ()
	{
		return attached_fd;
	}

private:
	const ahpl_fd_t attached_fd;

private:
	ahpl_mpq_fd_class () = delete;
	ahpl_mpq_fd_class (const ahpl_mpq_fd_class &) = delete;
	ahpl_mpq_fd_class (ahpl_mpq_fd_class &&) = delete;
	ahpl_mpq_fd_class &operator = (const ahpl_mpq_fd_class &) = delete;
	ahpl_mpq_fd_class &operator = (ahpl_mpq_fd_class &&) = delete;
};


#endif /* __AHPL_MPQ_FD_CLASS_H__ */