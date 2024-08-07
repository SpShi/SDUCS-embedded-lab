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
	{ 0x47229b5c, "gpio_request" },
	{ 0x1fcb5e67, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0xbcb0fcc2, "gpiod_direction_input" },
	{ 0xbe3505cd, "gpiod_direction_output_raw" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0xfe990052, "gpio_free" },
	{ 0x55cd5c50, "gpiod_set_raw_value" },
	{ 0xe7043306, "iounmap" },
	{ 0xf2da0e4b, "gpiod_get_raw_value" },
	{ 0x9e7d6bd0, "__udelay" },
};

MODULE_INFO(depends, "");

