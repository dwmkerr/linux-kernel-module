#include <linux/module.h>
#include <linux/init.h>

//  Define the module metadata.
MODULE_AUTHOR("Dave Kerr");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple kernel module to greet a user");
MODULE_VERSION("0.1");

//  Define the name parameter.
static char *name = "Bilbo";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init greeter_init(void)
{
    pr_info("hello: module loaded at 0x%p\n", greeter_init);
    pr_info("hello: greetings %s\n", name);
    return 0;
}

static void __exit greeter_exit(void)
{
    pr_info("hello: goodbye %s\n", name);
    pr_info("hello: module unloaded from 0x%p\n", greeter_exit);
}

module_init(greeter_init);
module_exit(greeter_exit);
