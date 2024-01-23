#ifndef __AHPL_LOG_OBJC_H__
#define __AHPL_LOG_OBJC_H__

#import <Foundation/NSString.h>

#include <api/ahpl_types.h>
#include <api/ahpl_log.h>


#ifdef __cplusplus
extern "C" {
#endif



typedef void (*ahpl_objc_vlog_t) (int level, const NSString *format, va_list args);

extern void ahpl_set_vlog_func_objc (ahpl_objc_vlog_t vlog);



#ifdef __cplusplus
}
#endif



#endif /* __AHPL_LOG_OBJC_H__ */
