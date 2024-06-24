#ifndef __KM_PROC_H__
#define __KM_PROC_H__

#define KM_PROC_DIR "km"           /* km模块文件夹，用于存放模块的所有proc文件 */
#define KM_DEBUG_FILE "km_debug"   /* km_debug对应的proc文件 */

int km_init_proc(void);
void km_exit_proc(void);

#endif