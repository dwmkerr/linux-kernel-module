#include <linux/module.h>
#include <linux/init.h>

//  Define the module metadata.
MODULE_AUTHOR("Dave Kerr");
MODULE_LICENSE("GPL v2");

//  Define the name parameter.
static char *name = "Bilbo";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init my_init(void)
{
    pr_info("goodbye: module loaded at 0x%p\n", my_init);
    pr_info("goodbye: name is %s\n", name);
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("goodbye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);
