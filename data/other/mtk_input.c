Input子系统详解

一．Input子系统架构

Linux系统提供了input子系统，按键、触摸屏、键盘、鼠标等输入都可以利用input接口函数来实现设备驱动，下面是Input子系统架构：

Input子系统架构
二．Input系统的组成

输入子系统由驱动层（Drivers），输入子系统核心层（ Input Core ）和事件处理层（Event Handler）三部份组成。一个输入事件，如鼠标移动，键盘按键按下等都是通过 
Driver -> InputCore -> Eventhandler -> userspace 的顺序到达用户空间传给应用程序。下面介绍各部分的功能：

（1）驱动层功能：负责和底层的硬件设备打交道，将底层硬件设备对用户输入的响应转换为标准的输入事件以后再向上发送给输入子系统核心层（Input Core）。

（2）Input系统核心层：Input Core即Input Layer，由driver/input/input.c及相关头文件实现，它对下提供了设备驱动层的接口，对上提供了事件处理层（Event Handler）
的编程接口。

（3）事件处理层将硬件设备上报的事件分发到用户空间和内核。

三．Input设备驱动编写

在Linux内核中，input设备用input_dev结构体描述，使用input子系统实现输入设备驱动的时候，驱动的核心工作是向系统报告按键、触摸屏、键盘、鼠标等输入事件（event，
通过input_event结构体描述），不再需要关心文件操作接口，因为input子系统已经完成了文件操作接口。驱动报告的事件经过InputCore和 Eventhandler最终到达用户空间。
下面给出一个使用input子系统的例子，通过这个例子来解析input子系统的方方面面。

（1）键盘驱动

//这个按钮中断只是简单的将事件上报给input子系统
static void button_interrupt(int irq, void *dummy, struct pt_regs *fp)
{

    input_report_key(&button_dev, BTN_1, inb(BUTTON_PORT) & 1);
    input_sync(&button_dev);
}

//初始化，支持哪些时间，注册中断和中断处理函数
static int __init button_init(void)
{

    if (request_irq(BUTTON_IRQ, button_interrupt, 0, "button", NULL)) {
        printk(KERN_ERR "button.c: Can''t allocate irq %d\n", button_irq);
        return -EBUSY;
    }

    button_dev.evbit[0] = BIT(EV_KEY);

    button_dev.keybit[LONG(BTN_0)] = BIT(BTN_0);

    input_register_device(&button_dev);
}

 

static void __exit button_exit(void)
{
    input_unregister_device(&button_dev);

    free_irq(BUTTON_IRQ, button_interrupt);
}

module_init(button_init);
module_exit(button_exit);
   

    这是个最简单使用input子系统的例子，权且引出这input子系统，这个驱动中主要涉及input子系统的函数下面一一列出，后面会有详细的介绍：

1)set_bit(EV_KEY, button_dev.evbit);

set_bit(BTN_0, button_dev.keybit);

分别用来设置设备所产生的事件以及上报的按键值。
struct input_dev中有两个成员，一个是evbit.一个是keybit，分别用表示设备所支持的动作和按键类型。

2)input_register_device(&button_dev);

用来注册一个input device.

3) input_report_key()

用于给上层上报一个按键动作

4）input_sync()

用来告诉上层，本次的事件已经完成了.

5) input_unregister_device（）

用来注销一个input_dev设备

四．Input子系统探幽

（1）input设备注册分析

Input设备注册的接口为：input_register_device()。代码如下：

int input_register_device(struct input_dev *dev)
{
    static atomic_t input_no = ATOMIC_INIT(0);
    struct input_handler *handler;
    const char *path;
    int error;

    __set_bit(EV_SYN, dev->evbit);

    init_timer(&dev->timer);
    if (!dev->rep[REP_DELAY] && !dev->rep[REP_PERIOD]) {
    dev->timer.data = (long) dev;
    dev->timer.function = input_repeat_key;
    dev->rep[REP_DELAY] = 250;
    dev->rep[REP_PERIOD] = 33;
}

在 前面的分析中曾分析过。Input_device的evbit表示该设备所支持的事件。在这里将其EV_SYN置位，即所有设备都支持这个事件。如果 dev->rep[REP_DELAY]和
dev->rep[REP_PERIOD]没有设值，则将其赋默认值。这主要是处理重复按键的。

if (!dev->getkeycode)
    dev->getkeycode = input_default_getkeycode;

if (!dev->setkeycode)
    dev->setkeycode = input_default_setkeycode;

snprintf(dev->dev.bus_id, sizeof(dev->dev.bus_id),
"input%ld", (unsigned long) atomic_inc_return(&input_no) - 1);

error = device_add(&dev->dev);
if (error)
    return error;

path = kobject_get_path(&dev->dev.kobj, GFP_KERNEL);
printk(KERN_INFO "input: %s as %s\n",
dev->name ? dev->name : "Unspecified device", path ? path : "N/A");
kfree(path);

error = mutex_lock_interruptible(&input_mutex);
if (error) {
device_del(&dev->dev);
return error;
}

如 果input device没有定义getkeycode和setkeycode.则将其赋默认值。还记得在键盘驱动中的分析吗?这两个操作函数就可以用来取键的扫描码 和设置键的扫描码。
然后调用device_add()将input_dev中封装的device注册到sysfs。

    list_add_tail(&dev->node, &input_dev_list);

    list_for_each_entry(handler, &input_handler_list, node)
    input_attach_handler(dev, handler);

    input_wakeup_procfs_readers();

    mutex_unlock(&input_mutex);

    return 0;
}

这 里就是重点了，将input device 挂到input_dev_list链表上.然后，对每一个挂在input_handler_list的handler调用 input_attach_handler().
在这里的情况有好比设备模型中的device和driver的匹配。所有的input device都挂在input_dev_list链上。所有的handler都挂在input_handler_list上。

看一下这个匹配的详细过程。匹配是在input_attach_handler()中完成的。代码如下：

static int input_attach_handler(struct input_dev *dev, struct input_handler *handler)
{
    const struct input_device_id *id;
    int error;

    if (handler->blacklist && input_match_device(handler->blacklist, dev))
    return -ENODEV;

    id = input_match_device(handler->id_table, dev);
    if (!id)
    return -ENODEV;

    error = handler->connect(handler, dev, id);
    if (error && error != -ENODEV)
    printk(KERN_ERR
    "input: failed to attach handler %s to device %s, "
    "error: %d\n",

    handler->name, kobject_name(&dev->dev.kobj), error);

    return error;
}

如 果handle的blacklist被赋值。要先匹配blacklist中的数据跟dev->id的数据是否匹配。匹配成功过后再来匹配 handle->id和dev->id中的数据。
如果匹配成功，则调用handler->connect()。

来看一下具体的数据匹配过程，这是在input_match_device()中完成的。代码如下：

static const struct input_device_id *input_match_device(const struct input_device_id *id,
struct input_dev *dev)
{
    int i;

    for (; id->flags || id->driver_info; id++) {

    if (id->flags & INPUT_DEVICE_ID_MATCH_BUS)
    if (id->bustype != dev->id.bustype)
    continue;

    if (id->flags & INPUT_DEVICE_ID_MATCH_VENDOR)
    if (id->vendor != dev->id.vendor)
    continue;

    if (id->flags & INPUT_DEVICE_ID_MATCH_PRODUCT)
    if (id->product != dev->id.product)
    continue;

    if (id->flags & INPUT_DEVICE_ID_MATCH_VERSION)
    if (id->version != dev->id.version)
    continue;

    MATCH_BIT(evbit, EV_MAX);
    MATCH_BIT(,, KEY_MAX);
    MATCH_BIT(relbit, REL_MAX);
    MATCH_BIT(absbit, ABS_MAX);
    MATCH_BIT(mscbit, MSC_MAX);
    MATCH_BIT(ledbit, LED_MAX);
    MATCH_BIT(sndbit, SND_MAX);
    MATCH_BIT(ffbit, FF_MAX);
    MATCH_BIT(swbit, SW_MAX);

    return id;
    }

    return NULL;
}

MATCH_BIT宏的定义如下：

#define MATCH_BIT(bit, max)
for (i = 0; i < BITS_TO_LONGS(max); i++)
if ((id->bit[i] & dev->bit[i]) != id->bit[i])
break;
if (i != BITS_TO_LONGS(max))
continue;

由 此看到。在id->flags中定义了要匹配的项。定义INPUT_DEVICE_ID_MATCH_BUS。则是要比较input device和input handler的总线类型。
INPUT_DEVICE_ID_MATCH_VENDOR，INPUT_DEVICE_ID_MATCH_PRODUCT，INPUT_DEVICE_ID_MATCH_VERSION 分别要求设备厂商。设备号和设备版本。
如果id->flags定义的类型匹配成功。或者是id->flags没有定义，就会进入到 MATCH_BIT的匹配项了。

从MATCH_BIT宏的定义可以看出。只有当iput device和input handler的id成员在evbit, keybit,… swbit项相同才会匹配成功。而且匹配的顺序是从evbit,
 keybit到swbit.只要有一项不同，就会循环到id中的下一项进行比较。简而言之,注册input device的过程就是为input device设置默认值，并将其挂以input_dev_list.
 与挂载在input_handler_list中的handler相匹配。如果匹配成功，就会调用handler的connect函数.
（2）handler注册分析

Handler注册的接口如下所示:

int input_register_handler(struct input_handler *handler)
{
struct input_dev *dev;
int retval;

retval = mutex_lock_interruptible(&input_mutex);
if (retval)
return retval;

INIT_LIST_HEAD(&handler->h_list);

if (handler->fops != NULL) {
if (input_table[handler->minor >> 5]) {
retval = -EBUSY;
goto out;
}
input_table[handler->minor >> 5] = handler;
}

list_add_tail(&handler->node, &input_handler_list);

list_for_each_entry(dev, &input_dev_list, node)
input_attach_handler(dev, handler);

input_wakeup_procfs_readers();

out:
mutex_unlock(&input_mutex);
return retval;
}

handler->minor表示对应input设备节点的次设备号.以handler->minor右移五位做为索引值插入到input_table[ ]中..之后再来分析input_talbe[ ]的作用。

然后将handler挂到input_handler_list中.然后将其与挂在input_dev_list中的input device匹配.这个过程和input device的注册有相似的地方.都是注册到各自的链表，
然后与另外一条链表的对象相匹配.

 

（3）handle的注册

int input_register_handle(struct input_handle *handle)
{

struct input_handler *handler = handle->handler;
struct input_dev *dev = handle->dev;
int error;

/*
* We take dev->mutex here to prevent race with
* input_release_device().
*/
error = mutex_lock_interruptible(&dev->mutex);
if (error)
return error;
list_add_tail_rcu(&handle->d_node, &dev->h_list);
mutex_unlock(&dev->mutex);
synchronize_rcu();

list_add_tail(&handle->h_node, &handler->h_list);

if (handler->start)
handler->start(handle);

return 0;
}

在这个函数里所做的处理其实很简单.将handle挂到所对应input device的h_list链表上.还将handle挂到对应的handler的hlist链表上.如果handler定义了start函数,将调用之。

到这里,我们已经看到了input device, handler和handle是怎么关联起来的了。

（4）event事件的处理

我们在开篇的时候曾以linux kernel文档中自带的代码作分析.提出了几个事件上报的API.这些API其实都是input_event()的封装，代码如下:

void input_event(struct input_dev *dev,
unsigned int type, unsigned int code, int value)
{
    unsigned long flags;

    //判断设备是否支持这类事件
    if (is_event_supported(type, dev->evbit, EV_MAX)) {

    spin_lock_irqsave(&dev->event_lock, flags);
    //利用键盘输入来调整随机数产生器
    add_input_randomness(type, code, value);
    input_handle_event(dev, type, code, value);
    spin_unlock_irqrestore(&dev->event_lock, flags);
    }
}

先判断设备产生的这个事件是否合法.如果合法,流程转入到input_handle_event()中，代码如下:

static void input_handle_event(struct input_dev *dev,
unsigned int type, unsigned int code, int value)
{
int disposition = INPUT_IGNORE_EVENT;

switch (type) {

case EV_SYN:
switch (code) {
case SYN_CONFIG:
disposition = INPUT_PASS_TO_ALL;
break;

case SYN_REPORT:
if (!dev->sync) {
dev->sync = 1;
disposition = INPUT_PASS_TO_HANDLERS;
}
break;
}
break;

case EV_KEY:
//判断按键值是否被支持
if (is_event_supported(code, dev->keybit, KEY_MAX) &&
!!test_bit(code, dev->key) != value) {

if (value != 2) {
__change_bit(code, dev->key);
if (value)
input_start_autorepeat(dev, code);
}

disposition = INPUT_PASS_TO_HANDLERS;
}
break;

case EV_SW:
if (is_event_supported(code, dev->swbit, SW_MAX) &&
!!test_bit(code, dev->sw) != value) {

__change_bit(code, dev->sw);
disposition = INPUT_PASS_TO_HANDLERS;
}
break;

 

case EV_ABS:

if (is_event_supported(code, dev->absbit, ABS_MAX)) {

value = input_defuzz_abs_event(value,
dev->abs[code], dev->absfuzz[code]);

if (dev->abs[code] != value) {
dev->abs[code] = value;
disposition = INPUT_PASS_TO_HANDLERS;
}
}
break;

case EV_REL:
if (is_event_supported(code, dev->relbit, REL_MAX) && value)
disposition = INPUT_PASS_TO_HANDLERS;

break;

case EV_MSC:
if (is_event_supported(code, dev->mscbit, MSC_MAX))
disposition = INPUT_PASS_TO_ALL;

break;

case EV_LED:
if (is_event_supported(code, dev->ledbit, LED_MAX) &&
!!test_bit(code, dev->led) != value) {

__change_bit(code, dev->led);
disposition = INPUT_PASS_TO_ALL;
}
break;

case EV_SND:
if (is_event_supported(code, dev->sndbit, SND_MAX)) {

if (!!test_bit(code, dev->snd) != !!value)
__change_bit(code, dev->snd);
disposition = INPUT_PASS_TO_ALL;
}
break;

case EV_REP:
if (code <= REP_MAX && value >= 0 && dev->rep[code] != value) {
dev->rep[code] = value;
disposition = INPUT_PASS_TO_ALL;
}
break;

case EV_FF:
if (value >= 0)
disposition = INPUT_PASS_TO_ALL;
break;

case EV_PWR:
disposition = INPUT_PASS_TO_ALL;
break;
}

if (type != EV_SYN)
dev->sync = 0;

if ((disposition & INPUT_PASS_TO_DEVICE) && dev->event)
dev->event(dev, type, code, value);

if (disposition & INPUT_PASS_TO_HANDLERS)
input_pass_event (dev, type, code, value);
}

在 这里，我们忽略掉具体事件的处理.到最后,如果该事件需要input device来完成的,就会将disposition设置成INPUT_PASS_TO_DEVICE.如果需要handler来完成的,
就将 dispostion设为INPUT_PASS_TO_HANDLERS.如果需要两者都参与，将disposition设置为 INPUT_PASS_TO_ALL。

需要输入设备参与的,回调设备的event函数.如果需要handler参与的.调用input_pass_event().代码如下:

static void input_pass_event(struct input_dev *dev,
unsigned int type, unsigned int code, int value)
{
struct input_handle *handle;

rcu_read_lock();

handle = rcu_dereference(dev->grab);
if (handle)
handle->handler->event(handle, type, code, value);
else
list_for_each_entry_rcu(handle, &dev->h_list, d_node)
if (handle->open)
handle->handler->event(handle,type, code, value);
rcu_read_unlock();
}

如果input device被强制指定了handler,则调用该handler的event函数。结合handle注册的分析，我们知道会将handle挂到input device的h_list链表上。如 果没有为
input device强制指定handler.就会遍历input device->h_list上的handle成员.如果该handle被打开,则调用与输入设备对应的handler的event()函数.注意,
只有在handle被打开的情况下才会接收到事件。

另外,输入设备的handler强制设置一般是用带EVIOCGRAB标志的ioctl来完成的.如下是发图的方示总结evnet的处理过程。我们已经分析了input device,handler和handle
的注册过程以及事件的上报和处理.下面以evdev为实例做分析.来贯穿理解一下整个过程.
五．evdev概述

Evdev对应的设备节点一般位于/dev/input/event0 ~ /dev/input/event4.理论上可以对应32个设备节点.分别代表被handler匹配的32个input device。可以用
cat /dev/input/event0.然后移动鼠标或者键盘按键就会有数据输出(两者之间只能选一.因为一个设备文件只能关能一个输入设备).还可以往这个文件里写数据,使其产生特定的事件.
这个过程我们之后再详细分析。为了分析这一过程,必须从input子系统的初始化说起。

（1）       input子系统的初始化

Input子系统的初始化函数为input_init().代码如下：

static int __init input_init(void)
{
int err;

err = class_register(&input_class);
if (err) {
printk(KERN_ERR "input: unable to register input_dev class\n");
return err;
}

err = input_proc_init();
if (err)
goto fail1;

err = register_chrdev(INPUT_MAJOR, "input", &input_fops);
if (err) {
printk(KERN_ERR "input: unable to register char major %d", INPUT_MAJOR);
goto fail2;
}

return 0;

fail2: input_proc_exit();
fail1: class_unregister(&input_class);
return err;
}

在这个初始化函数里,先注册了一个名为”input”的类.所有input device都属于这个类.在sysfs中表现就是．所有input device所代表的目录都位于/dev/class/input下面，
然后调用input_proc_init()在/proc下面建立相关的交互文件，再后调用register_chrdev()注册了主设备号为INPUT_MAJOR(13).次设备号为0~255的字符设备.它的操作指针
为input_fops.

在这里,我们看到.所有主设备号13的字符设备的操作最终都会转入到input_fops中.在前面分析的/dev/input/event0~/dev/input/event4的主设备号为13.操作也不例外的落在
了input_fops中。Input_fops定义如下:

static const struct file_operations input_fops = {
.owner = THIS_MODULE,
.open = input_open_file,
};

打开文件所对应的操作函数为input_open_file.代码如下示:

static int input_open_file(struct inode *inode, struct file *file)
{
struct input_handler *handler = input_table[iminor(inode) >> 5];
const struct file_operations *old_fops, *new_fops = NULL;
int err;


if (!handler || !(new_fops = fops_get(handler->fops)))
return -ENODEV;

iminor(inode) 为打开文件所对应的次设备号。input_table是一个struct input_handler全局数组，在这里它先设备结点的次设备号右移5位做为索引值到input_table中取对应项。

从这里我们也可以看到.一 个handle代表1<<5个设备节点(因为在input_table中取值是以次备号右移5位为索引的.即低5位相同的次备号对应的是同一个索引)。在这里,终于看到
了input_talbe大显身手的地方了，input_talbe[ ]中取值和input_talbe[ ]的赋值,这两个过程是相对应的.在input_table中找到对应的handler之后,就会检验这个handle
是否存,是否带有fops文件操作集.如果没有.则返回一个设备不存在的错误.

if (!new_fops->open) {
fops_put(new_fops);
return -ENODEV;
}
old_fops = file->f_op;
file->f_op = new_fops;

err = new_fops->open(inode, file);

if (err) {
fops_put(file->f_op);
file->f_op = fops_get(old_fops);
}
fops_put(old_fops);
return err;
}

然后将handler中的fops替换掉当前的fops.如果新的fops中有open()函数,则调用它。

（2）evdev的初始化

Evdev的模块初始化函数为evdev_init().代码如下:

static int __init evdev_init(void)
{
return input_register_handler(&evdev_handler);
}

它调用了input_register_handler注册了一个handler，注意在这里evdev_handler中定义的minor为EVDEV_MINOR_BASE(64)。也就是说evdev_handler所表示的设备
文件范围为(13,64)到(13,64+32)。

从之前的分析我们知道.匹配成功的关键在于handler中的blacklist和id_talbe. Evdev_handler的id_table定义如下:

static const struct input_device_id evdev_ids[] = {
{ .driver_info = 1 },
{ },
};

它没有定义flags.也没有定义匹配属性值.这个handler是匹配所有input device的.从前面的分析我们知道.匹配成功之后会调用handler->connect函数。在Evdev_handler中,
该成员函数如下所示:

static int evdev_connect(struct input_handler *handler, struct input_dev *dev,
const struct input_device_id *id)
{
struct evdev *evdev;
int minor;
int error;

for (minor = 0; minor < EVDEV_MINORS; minor++)
if (!evdev_table[minor])
break;

if (minor == EVDEV_MINORS) {
printk(KERN_ERR "evdev: no more free evdev devices\n");
return -ENFILE;
}

EVDEV_MINORS定义为32.表示evdev_handler所表示的32个设备文件.evdev_talbe是一个struct evdev类型的数组.struct evdev是模块使用的封装结构.在接下来的
代码中我们可以看到这个结构的使用。这一段代码的在evdev_talbe找到为空的那一项.minor就是数组中第一项为空的序号。

evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);
if (!evdev)
return -ENOMEM;

INIT_LIST_HEAD(&evdev->client_list);
spin_lock_init(&evdev->client_lock);

mutex_init(&evdev->mutex);
init_waitqueue_head(&evdev->wait);

snprintf(evdev->name, sizeof(evdev->name), "event%d", minor);
evdev->exist = 1;
evdev->minor = minor;

evdev->handle.dev = input_get_device(dev);
evdev->handle.name = evdev->name;
evdev->handle.handler = handler;
evdev->handle.private = evdev;

接 下来,分配了一个evdev结构,并对这个结构进行初始化.在这里我们可以看到,这个结构封装了一个handle结构,这结构与我们之前所讨论的 handler是不相同的.注意有一个字母
的差别哦.我们可以把handle看成是handler和input device的信息集合体.在这个结构里集合了匹配成功的handler和input device。

strlcpy(evdev->dev.bus_id, evdev->name, sizeof(evdev->dev.bus_id));
evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);
evdev->dev.class = &input_class;
evdev->dev.parent = &dev->dev;
evdev->dev.release = evdev_free;
device_initialize(&evdev->dev);

在这段代码里主要完成evdev封装的device的初始化.注意在这里,使它所属的类指向input_class.这样在/sysfs中创建的设备目录就会在/sys/class/input/下面显示。

error = input_register_handle(&evdev->handle);
if (error)
goto err_free_evdev;
error = evdev_install_chrdev(evdev);
if (error)
goto err_unregister_handle;

error = device_add(&evdev->dev);
if (error)
goto err_cleanup_evdev;

return 0;

err_cleanup_evdev:
evdev_cleanup(evdev);
err_unregister_handle:
input_unregister_handle(&evdev->handle);
err_free_evdev:
put_device(&evdev->dev);
return error;
}

注册handle,如果是成功的,那么调用evdev_install_chrdev将evdev_table的minor项指向evdev. 然后将evdev->device注册到sysfs.如果失败,将进行相关的错误处理。
万事俱备了,但是要接收事件，还是要打开相应的handle，这个打开过程是在文件的open()中完成的。

（3）evdev设备结点的open()操作

我们知道.对主设备号为INPUT_MAJOR的设备节点进行操作,会将操作集转换成handler的操作集，在evdev中,这个操作集就是evdev_fops对应的open函数如下示:

static int evdev_open(struct inode *inode, struct file *file)
{
struct evdev *evdev;
struct evdev_client *client;
int i = iminor(inode) - EVDEV_MINOR_BASE;
int error;

if (i >= EVDEV_MINORS)
return -ENODEV;

error = mutex_lock_interruptible(&evdev_table_mutex);
if (error)
return error;
evdev = evdev_table[i];
if (evdev)
get_device(&evdev->dev);
mutex_unlock(&evdev_table_mutex);

if (!evdev)
return -ENODEV;

client = kzalloc(sizeof(struct evdev_client), GFP_KERNEL);
if (!client) {
error = -ENOMEM;
goto err_put_evdev;
}
spin_lock_init(&client->buffer_lock);
client->evdev = evdev;
evdev_attach_client(evdev, client);

error = evdev_open_device(evdev);
if (error)
goto err_free_client;

file->private_data = client;
return 0;

err_free_client:
evdev_detach_client(evdev, client);
kfree(client);
err_put_evdev:
put_device(&evdev->dev);
return error;
}

iminor(inode) - EVDEV_MINOR_BASE就得到了在evdev_table[ ]中的序号.然后将数组中对应的evdev取出.递增devdev中device的引用计数。

分配并初始化一个client.并将它和evdev关联起来: client->evdev指向它所表示的evdev. 将client挂到evdev->client_list上. 将client赋为file的私有区。
对应handle的打开是在此evdev_open_device()中完成的，代码如下:

static int evdev_open_device(struct evdev *evdev)
{
int retval;

retval = mutex_lock_interruptible(&evdev->mutex);
if (retval)
return retval;

if (!evdev->exist)
retval = -ENODEV;
else if (!evdev->open++) {
retval = input_open_device(&evdev->handle);
if (retval)
evdev->open--;
}

mutex_unlock(&evdev->mutex);
return retval;
}

如果evdev是第一次打开,就会调用input_open_device()打开evdev对应的handle，跟踪一下这个函数:

int input_open_device(struct input_handle *handle)
{
struct input_dev *dev = handle->dev;
int retval;

retval = mutex_lock_interruptible(&dev->mutex);
if (retval)
return retval;

if (dev->going_away) {
retval = -ENODEV;
goto out;
}

handle->open++;

if (!dev->users++ && dev->open)
retval = dev->open(dev);

if (retval) {
dev->users--;if (!--handle->open) {

synchronize_rcu();
}
}

out:
mutex_unlock(&dev->mutex);
return retval;
}

在这个函数中,我们看到递增handle的打开计数，如果是第一次打开.则调用input device的open()函数.

（4）evdev的事件处理

经过上面的分析.每当input device上报一个事件时,会将其交给和它匹配的handler的event函数处理.在evdev中.这个event函数对应的代码为:

static void evdev_event(struct input_handle *handle,
unsigned int type, unsigned int code, int value)
{
struct evdev *evdev = handle->private;
struct evdev_client *client;
struct input_event event;

do_gettimeofday(&event.time);
event.type = type;
event.code = code;
event.value = value;

rcu_read_lock();

client = rcu_dereference(evdev->grab);
if (client)
evdev_pass_event(client, &event);
else
list_for_each_entry_rcu(client, &evdev->client_list, node)
evdev_pass_event(client, &event);

rcu_read_unlock();

wake_up_interruptible(&evdev->wait);
}

首先构造一个struct input_event结构.并设备它的type.code,value为处理事件的相关属性.如果该设备被强制设置了handle.则调用如之对应的client。

我们在open的时候分析到.会初始化clinet并将其链入到evdev->client_list. 这样,就可以通过evdev->client_list找到这个client了。对于找到的第一个client都会调用
evdev_pass_event( )，代码如下:

static void evdev_pass_event(struct evdev_client *client,
struct input_event *event)
{

spin_lock(&client->buffer_lock);
client->buffer[client->head++] = *event;
client->head &= EVDEV_BUFFER_SIZE - 1;
spin_unlock(&client->buffer_lock);

kill_fasync(&client->fasync, SIGIO, POLL_IN);
}

这里的操作很简单.就是将event保存到client->buffer中.而client->head就是当前的数据位置.注意这里是一个环形缓存区.写数据是从client->head写.而读数据则是从
client->tail中读。

（5）设备节点的read处理

对于evdev设备节点的read操作都会由evdev_read()完成.它的代码如下:

static ssize_t evdev_read(struct file *file, char __user *buffer,
size_t count, loff_t *ppos)
{
struct evdev_client *client = file->private_data;
struct evdev *evdev = client->evdev;
struct input_event event;
int retval;

if (count < evdev_event_size())
return -EINVAL;

if (client->head == client->tail && evdev->exist &&
(file->f_flags & O_NONBLOCK))
return -EAGAIN;

retval = wait_event_interruptible(evdev->wait,
client->head != client->tail || !evdev->exist);
if (retval)
return retval;

if (!evdev->exist)
return -ENODEV;

while (retval + evdev_event_size() <= count &&
evdev_fetch_next_event(client, &event)) {

if (evdev_event_to_user(buffer + retval, &event))
return -EFAULT;

retval += evdev_event_size();
}

return retval;
}

首先,它判断缓存区大小是否足够.在读取数据的情况下,可能当前缓存区内没有数据可读.在这里先睡眠等待缓存区中有数据。如果在睡眠的时候，条件满足，是不会进行睡眠状态而直接返回的。
根据read()提够的缓存区大小.将client中的数据写入到用户空间的缓存区中。

（6）设备节点的写操作

同样.对设备节点的写操作是由evdev_write()完成的.代码如下:

static ssize_t evdev_write(struct file *file, const char __user *buffer,
size_t count, loff_t *ppos)
{
struct evdev_client *client = file->private_data;
struct evdev *evdev = client->evdev;
struct input_event event;
int retval;

retval = mutex_lock_interruptible(&evdev->mutex);
if (retval)
return retval;

if (!evdev->exist) {
retval = -ENODEV;
goto out;
}

while (retval < count) {

if (evdev_event_from_user(buffer + retval, &event)) {
retval = -EFAULT;
bsp; goto out;
}

input_inject_event(&evdev->handle,
event.type, event.code, event.value);
retval += evdev_event_size();
}

out:
mutex_unlock(&evdev->mutex);
return retval;
}

首先取得操作设备文件所对应的evdev，实际上,这里写入设备文件的是一个event结构的数组.我们在之前分析过,这个结构里包含了事件的type.code和event，将写入设备的event
数组取出，然后对每一项调用event_inject_event()。

这个函数的操作和input_event()差不多，就是将第一个参数handle转换为输入设备结构，然后这个设备再产生一个事件。代码如下:

void input_inject_event(struct input_handle *handle,
unsigned int type, unsigned int code, int value)
{
struct input_dev *dev = handle->dev;
struct input_handle *grab;
unsigned long flags;

if (is_event_supported(type, dev->evbit, EV_MAX)) {
spin_lock_irqsave(&dev->event_lock, flags);

rcu_read_lock();
grab = rcu_dereference(dev->grab);
if (!grab || grab == handle)
input_handle_event(dev, type, code, value);
rcu_read_unlock();

spin_unlock_irqrestore(&dev->event_lock, flags);
}
}

我们在这里也可以跟input_event()对比一下,这里设备可以产生任意事件,而不需要和设备所支持的事件类型相匹配，由此可见，对于写操作而言.就是让与设备文件相关的输入设备产生
一个特定的事件。

六.小结

在 这一节点,分析了整个input子系统的架构,各个环节的流程。最后还以evdev为例将各个流程贯穿在一起，以加深对input子系统的理解。

由此也可以看出：linux设备驱动采用了分层的模式，从最下层的设备模型到设备,驱动和总线，再到input子系统最后到input device。这样的分层结构使得最上层的驱动不必关心下
层是怎么实现的，而下层驱动又为多种型号同样功能的驱动提供了一个统一的接口。