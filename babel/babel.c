//  Note:
//  This code is based on Derek Molloy's excellent tutorial on creating Linux
//  Kernel Modules:
//    http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

//  Define the module metadata.
#define MODULE_NAME "babel"
#define  DEVICE_NAME "babel"
#define  CLASS_NAME  "babel"
MODULE_AUTHOR("Dave Kerr");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A module which provides a device which talks gibberish");
MODULE_VERSION("0.1");

//  Define the name parameter.
static char *name = "Bilbo";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

//  The device number, automatically set. The message buffer and current message
//  size. The number of device opens and the device class struct pointers.
static int    majorNumber;
static char   message[256] = {0};
static short  messageSize;
static int    numberOpens = 0;
static struct class*  babelClass  = NULL;
static struct device* babelDevice = NULL;
static DEFINE_MUTEX(ioMutex);

//  Prototypes for our device functions.
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

//   Our main 'babel' function...
static char    babel(char input);

//  Create the file operations instance for our driver.
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init mod_init(void)
{
    pr_info("%s: module loaded at 0x%p\n", MODULE_NAME, mod_init);

    //  Create a mutex to guard io operations.
    mutex_init(&ioMutex);

    //  Register the device, allocating a major number.
    majorNumber = register_chrdev(0 /* i.e. allocate a major number for me */, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        pr_alert("%s: failed to register a major number\n", MODULE_NAME);
        return majorNumber;
    }
    pr_info("%s: registered correctly with major number %d\n", MODULE_NAME, majorNumber);

    //  Create the device class.
    babelClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(babelClass)) {
        //  Cleanup resources and fail.
        unregister_chrdev(majorNumber, DEVICE_NAME);
        pr_alert("%s: failed to register device class\n", MODULE_NAME);

        //  Get the error code from the pointer.
        return PTR_ERR(babelClass);
    }
    pr_info("%s: device class registered correctly\n", MODULE_NAME);

    //  Create the device.
    babelDevice = device_create(babelClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(babelDevice)) {
        class_destroy(babelClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        pr_alert("%s: failed to create the device\n", DEVICE_NAME);
        return PTR_ERR(babelDevice);
    }
    pr_info("%s: device class created correctly\n", DEVICE_NAME);

    //  Success!
    return 0;
}

static void __exit mod_exit(void)
{
    pr_info("%s: unloading...\n", MODULE_NAME);
    device_destroy(babelClass, MKDEV(majorNumber, 0));
    class_unregister(babelClass);
    class_destroy(babelClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    mutex_destroy(&ioMutex);
    pr_info("%s: device unregistered\n", MODULE_NAME);
}

/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
    //  Try and lock the mutex.
    if(!mutex_trylock(&ioMutex)) {
        pr_alert("%s: device in use by another process", MODULE_NAME);
        return -EBUSY;
    }

    numberOpens++;
    pr_info("%s: device has been opened %d time(s)\n", MODULE_NAME, numberOpens);
    return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   int i = 0;
   char babelMessage[256] = { 0 };

   //   Create the babel content.
   for (; i<messageSize; i++) {
       babelMessage[i] = babel(message[i]);
   }

   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, babelMessage, messageSize);

   if (error_count==0) {
      pr_info("%s: sent %d characters to the user\n", MODULE_NAME, messageSize);
      //    Clear the position, return 0.
      messageSize = 0;
      return 0;
   }
   else {
      pr_err("%s: failed to send %d characters to the user\n", MODULE_NAME, error_count);
      //    Failed -- return a bad address message (i.e. -14)
      return -EFAULT;              
   }
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    //  Write the string into our message memory. Record the length.
    copy_from_user(message, buffer, len);
    messageSize = len;
    pr_info("%s: received %zu characters from the user\n", MODULE_NAME, len);
    return len;
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
     mutex_unlock(&ioMutex);
     pr_info("%s: device successfully closed\n", MODULE_NAME);
     return 0;
}

static char babel(char input) {
    if ((input >= 'a' && input <= 'm') || (input >= 'A' && input <= 'M')) {
       return input + 13;
    }
    if ((input >= 'n' && input <= 'z') || (input >= 'N' && input <= 'Z')) {
        return input - 13;
    }
    return input;
}
    

module_init(mod_init);
module_exit(mod_exit);

