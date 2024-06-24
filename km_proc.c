#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include "km_proc.h"

extern int km_debug_enable;

struct proc_dir_entry *km_proc_dir = NULL;         /* km模块文件夹，用于存放模块的所有proc文件 */
struct proc_dir_entry *km_debug_proc_file = NULL;  /* km_debug对应的proc文件 */

static int km_debug_proc_show(struct seq_file* file, void* v)
{
    seq_printf(file, "km_debug_enable:%d\n", km_debug_enable);
    return 0;
}

static int km_debug_proc_open(struct inode* inode, struct file* file)
{
    return single_open(file, km_debug_proc_show, NULL);
}

static ssize_t km_debug_proc_write(struct file* file, const char __user *buffer, size_t count, loff_t *pos)
{
    char buf[2] = {0};

    if (count > 2)
    {
        printk("error:please input 0 or 1");
        return -ENOSPC;
    }

    if (copy_from_user(buf, buffer, count))
    {
        return -EFAULT;
    }

    sscanf(buf, "%d", &km_debug_enable);
    printk("km_debug_enable:%d\n", km_debug_enable);

    return count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops km_debug_proc_fops = {
    .proc_open = km_debug_proc_open,
    .proc_read = seq_read,
    .proc_release = single_release,
    .proc_write = km_debug_proc_write,
};
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
static const struct file_operations km_debug_proc_fops = {
    .owner = THIS_MODULE,
    .open = km_debug_proc_open,
    .read = seq_read,
    .release = single_release,
    .write = km_debug_proc_write,
};
#else
#error "Please make sure your kernel vision" /* proc文件相关结构体和接口与内核版本有关，需要单独适配 */
#endif

int km_init_proc(void)
{
    int ret = 0;

    /* 创建/proc/km文件夹 */
    km_proc_dir = proc_mkdir(KM_PROC_DIR, NULL);
    if (NULL == km_proc_dir) /* 创建失败，退出 */
    {
        ret = -1;
        printk("proc_mkdir fail!\n");
        goto err;
    }

    /* 创建/proc/km/km_debug文件 */
    km_debug_proc_file = proc_create(KM_DEBUG_FILE, 0644, km_proc_dir, &km_debug_proc_fops);
    if (NULL == km_debug_proc_file)
    {
        ret = -1;
        printk("create /proc/%s/%s fail!\n", KM_PROC_DIR, KM_DEBUG_FILE);
        goto err1;
    }

    printk("proc init success!\n");
    return ret;

err1:
    /* 失败，销毁已经创建的文件 */
    remove_proc_entry(KM_PROC_DIR, NULL);
err:
    return ret;
}

void km_exit_proc(void)
{
    /* 按反向顺序一个个销毁，即先销毁文件、再销毁文件夹 */
    if (km_debug_proc_file)
    {
        remove_proc_entry(KM_DEBUG_FILE, km_proc_dir);
        km_debug_proc_file = NULL;
    }

    if (km_proc_dir)
    {
        remove_proc_entry(KM_PROC_DIR, NULL);
        km_proc_dir = NULL;
    }
    printk("proc exit complete!\n");
}