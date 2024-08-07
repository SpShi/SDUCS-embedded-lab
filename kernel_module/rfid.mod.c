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
	{ 0x15de15c4, "__ua_limit" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x1fcb5e67, "gpio_to_desc" },
	{ 0xdf8c695a, "__ndelay" },
	{ 0xd9caf6e1, "__copy_user" },
	{ 0x2adc06e6, "misc_register" },
	{ 0xfb578fc5, "memset" },
	{ 0xc5850110, "printk" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0xbcb0fcc2, "gpiod_direction_input" },
	{ 0xbe3505cd, "gpiod_direction_output_raw" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xfe990052, "gpio_free" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x55cd5c50, "gpiod_set_raw_value" },
	{ 0xe7043306, "iounmap" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xf2da0e4b, "gpiod_get_raw_value" },
	{ 0xf9e5c454, "misc_deregister" },
	{ 0x9e7d6bd0, "__udelay" },
};

MODULE_INFO(depends, "");

