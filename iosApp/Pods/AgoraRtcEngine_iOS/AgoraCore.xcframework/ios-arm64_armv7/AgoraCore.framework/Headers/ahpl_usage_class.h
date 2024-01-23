/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Jan 6th, 2019
 * Module:	AHPL simple usage object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2019 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_USAGE_OBJ_CPP_H__
#define __AHPL_USAGE_OBJ_CPP_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_atomic.h>


class ahpl_usage_class {
private:
	bool atomic;
	int usage;

protected:
	/* We do not allow delete this object directly. */
	virtual ~ahpl_usage_class () {}

public:
	/**
	 * The parameter 'atomic_op' indicates whether the operations
	 * on the usage should be atomic operations, true for atomic,
	 * and false for non-atomic. The default value is false.
	 **/
	ahpl_usage_class (bool atomic_op = false)
	{
		atomic = atomic_op;

		if (atomic_op) {
			ahpl_atomic_set ((ahpl_atomic_t *)&usage, 1);
		} else {
			usage = 1;
		}
	}

	void inc_ref ()
	{
		if (atomic) {
			ahpl_atomic_inc ((ahpl_atomic_t *)&usage);
		} else {
			usage++;
		}
	}

	/**
	 * Return values:
	 *       true: the usage count reached zero, and deleted 'this' object already;
	 *      false: the usage count still greater than zero, 'this' object is still alive;
	 **/
	bool dec_ref ()
	{
		bool b_val;

		if (atomic) {
			b_val = (bool)!!ahpl_atomic_dec_and_test ((ahpl_atomic_t *)&usage);
		} else {
			usage--;
			b_val = (bool)(usage == 0);
		}

		if (b_val) {
			delete this;
			return true;
		}

		return false;
	}
};


#endif /* __AHPL_USAGE_OBJ_CPP_H__ */