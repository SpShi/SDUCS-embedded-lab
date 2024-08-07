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
	{ 0xfe990052, "gpio_free" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xf9e5c454, "misc_deregister" },
	{ 0x2adc06e6, "misc_register" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x674d3baf, "gpiod_to_irq" },
	{ 0x1fcb5e67, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xe7043306, "iounmap" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0xc5850110, "printk" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0xd9caf6e1, "__copy_user" },
	{ 0x15de15c4, "__ua_limit" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x8570efca, "free_i2c_lock" },
	{ 0xfbcba51d, "I2c_RdByte" },
	{ 0x53202c25, "I2c_Stop" },
	{ 0xf9fb2f43, "I2c_WrByte" },
	{ 0x95407933, "I2c_Start" },
	{ 0x6f6b10df, "get_i2c_lock" },
};

MODULE_INFO(depends, "softi2c");

