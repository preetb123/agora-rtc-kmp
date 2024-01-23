/*************************************************************
 * Author:	Lionfore Hao (haolianfu@agora.io)
 * Date	 :	Aug 11th, 2021
 * Module:	AHPL objc relative declarations
 *
 *
 * This is a part of the Advanced High Performance Library.
 * Copyright (C) 2018 ~ 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#ifndef __AHPL_OBJC_H__
#define __AHPL_OBJC_H__


#ifdef __cplusplus
extern "C" {
#endif


extern void *objc_autoreleasePoolPush (void);
extern void objc_autoreleasePoolPop (void *pool);


#ifdef __cplusplus
}
#endif



#endif /* __AHPL_OBJC_H__ */
