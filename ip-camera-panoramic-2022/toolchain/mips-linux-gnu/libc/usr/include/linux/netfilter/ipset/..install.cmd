cmd_/home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset/.install := perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/linux/netfilter/ipset /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset mips ip_set.h ip_set_bitmap.h ip_set_hash.h ip_set_list.h; perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/linux/netfilter/ipset /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset mips ; perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/generated/linux/netfilter/ipset /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset mips ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset/$$F; done; touch /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/linux/netfilter/ipset/.install
