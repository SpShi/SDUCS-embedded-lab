#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x777b0eb4, "module_layout" },
	{ 0x72fcbdfe, "class_destroy" },
	{ 0x10614389, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1c7f2d11, "cdev_del" },
	{ 0xfe990052, "gpio_free" },
	{ 0xe7043306, "iounmap" },
	{ 0x71ee7e5, "device_create" },
	{ 0x5f936026, "__class_create" },
	{ 0x54470aee, "cdev_add" },
	{ 0x7a8e73cc, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0xf2da0e4b, "gpiod_get_raw_value" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xfb578fc5, "memset" },
	{ 0xc5850110, "printk" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x55cd5c50, "gpiod_set_raw_value" },
	{ 0xd9caf6e1, "__copy_user" },
	{ 0x15de15c4, "__ua_limit" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xbe3505cd, "gpiod_direction_output_raw" },
	{ 0xbcb0fcc2, "gpiod_direction_input" },
	{ 0x1fcb5e67, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xcdbf1cc, "ioremap" },
};

MODULE_INFO(depends, "");

