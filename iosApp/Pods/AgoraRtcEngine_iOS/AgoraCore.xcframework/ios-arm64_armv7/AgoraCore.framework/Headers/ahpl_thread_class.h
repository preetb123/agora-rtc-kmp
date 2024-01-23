/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Jul 9th, 2019
 * Module:	AHPL thread object for C++ definition file
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 ~ 2019 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_THREAD_OBJ_CPP_H__
#define __AHPL_THREAD_OBJ_CPP_H__


#include <stdlib.h>

#include <api/ahpl_types.h>
#include <api/ahpl_thread.h>


class ahpl_lock_class {
private:
	ahpl_lock_t lk;

public:
	ahpl_lock_class (bool recursive = false)
	{
		lk = ahpl_lock_create ((int)recursive);
		if (lk == NULL)
			abort ();
	}

	void lock (void)
	{
		ahpl_lock_lock (lk);
	}

	bool try_lock (void)
	{
		return (bool)(ahpl_lock_trylock (lk) != 0);
	}

	void unlock (void)
	{
		ahpl_lock_unlock (lk);
	}

	~ahpl_lock_class ()
	{
		ahpl_lock_destroy (lk);
	}

public:
	class scoped {
	private:
		ahpl_lock_class &the_lock;
		bool locked;

	private:
		/* No copy constructor allowed */
		scoped (const scoped &);
		/* No copy operator= allowed */
		scoped &operator = (const scoped &);

	public:
		scoped (ahpl_lock_class &lk): the_lock (lk)
		{
			the_lock.lock ();
			locked = true;
		}

		void release (void)
		{
			if (!locked)
				abort ();

			locked = false;
			the_lock.unlock ();
		}

		void reacquire (void)
		{
			if (locked)
				abort ();

			the_lock.lock ();
			locked = true;
		}

		~scoped ()
		{
			if (locked)
				the_lock.unlock ();
		}
	};
};

class ahpl_rwlock_class {
private:
	ahpl_rwlock_t rw;

public:
	ahpl_rwlock_class ()
	{
		rw = ahpl_rwlock_create ();
		if (rw == NULL)
			abort ();
	}

	void rdlock (void)
	{
		ahpl_rwlock_rdlock (rw);
	}

	bool try_rdlock (void)
	{
		return (bool)(ahpl_rwlock_tryrdlock (rw) != 0);
	}

	void wrlock (void)
	{
		ahpl_rwlock_wrlock (rw);
	}

	bool try_wrlock (void)
	{
		return (bool)(ahpl_rwlock_trywrlock (rw) != 0);
	}

	void rdunlock (void)
	{
		ahpl_rwlock_rdunlock (rw);
	}

	void wrunlock (void)
	{
		ahpl_rwlock_wrunlock (rw);
	}

	~ahpl_rwlock_class ()
	{
		ahpl_rwlock_destroy (rw);
	}

public:
	class scoped_rdlock {
	private:
		ahpl_rwlock_class &the_rwlock;
		bool locked;

	private:
		/* No copy constructor allowed */
		scoped_rdlock (const scoped_rdlock &);
		/* No copy operator= allowed */
		scoped_rdlock &operator = (const scoped_rdlock &);

	public:
		scoped_rdlock (ahpl_rwlock_class &rwlock): the_rwlock (rwlock)
		{
			the_rwlock.rdlock ();
			locked = true;
		}

		void release (void)
		{
			if (!locked)
				abort ();

			locked = false;
			the_rwlock.rdunlock ();
		}

		void reacquire (void)
		{
			if (locked)
				abort ();

			the_rwlock.rdlock ();
			locked = true;
		}

		~scoped_rdlock ()
		{
			if (locked)
				the_rwlock.rdunlock ();
		}
	};

	class scoped_wrlock {
	private:
		ahpl_rwlock_class &the_rwlock;
		bool locked;

	private:
		/* No copy constructor allowed */
		scoped_wrlock (const scoped_wrlock &);
		/* No copy operator= allowed */
		scoped_wrlock &operator = (const scoped_wrlock &);

	public:
		scoped_wrlock (ahpl_rwlock_class &rwlock): the_rwlock (rwlock)
		{
			the_rwlock.wrlock ();
			locked = true;
		}

		void release (void)
		{
			if (!locked)
				abort ();

			locked = false;
			the_rwlock.wrunlock ();
		}

		void reacquire (void)
		{
			if (locked)
				abort ();

			the_rwlock.wrlock ();
			locked = true;
		}

		~scoped_wrlock ()
		{
			if (locked)
				the_rwlock.wrunlock ();
		}
	};
};



#endif /* __AHPL_THREAD_OBJ_CPP_H__ */