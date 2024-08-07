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
	{ 0x5ba09203, "input_free_device" },
	{ 0x3da85aca, "input_unregister_device" },
	{ 0xe7043306, "iounmap" },
	{ 0xfe990052, "gpio_free" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x5642ffc5, "input_register_device" },
	{ 0x5998624f, "input_set_capability" },
	{ 0x503bc302, "input_allocate_device" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x674d3baf, "gpiod_to_irq" },
	{ 0xbcb0fcc2, "gpiod_direction_input" },
	{ 0x1fcb5e67, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0xc5850110, "printk" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xebc8a5d7, "input_event" },
	{ 0xfb578fc5, "memset" },
	{ 0xb43f9365, "ktime_get" },
};

MODULE_INFO(depends, "");

