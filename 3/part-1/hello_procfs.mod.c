#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x92997ed8, "_printk" },
	{ 0x1bd38186, "proc_create" },
	{ 0x37a0cba, "kfree" },
	{ 0x19c7fb41, "kmalloc_caches" },
	{ 0xcd523610, "kmalloc_trace" },
	{ 0xc620c109, "remove_proc_entry" },
	{ 0x98cf60b3, "strlen" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xb394217f, "module_layout" },
};

MODULE_INFO(depends, "");

