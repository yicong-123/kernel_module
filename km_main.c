#include <linux/kernel.h>
#include <linux/module.h>                    /* 内核模块必需的头文件 */

MODULE_LICENSE("GPL");                       /* 声明这是GNU General Public License的任意版本 */
MODULE_AUTHOR("Myself");                     /* 模块开发者，一般写开发者邮箱 */
MODULE_DESCRIPTION("Linux kernel module");   /* 模块功能简单描述 */

static int __init km_init(void)              /* 模块初始化 */
{
    int ret = 0;

    printk("My kernel module init success!\n");

    return ret;
}

static void __exit km_exit(void)             /* 模块退出 */
{
    printk("My kernel module exit success!\n");
}

module_init(km_init); /* 模块初始化函数声明：装载模块时从此函数开始运行 */
module_exit(km_exit); /* 模块退出函数声明：模块卸载时调用此函数 */