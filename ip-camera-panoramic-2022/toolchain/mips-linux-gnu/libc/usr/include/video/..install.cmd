cmd_/home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video/.install := perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/video /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video mips edid.h sisfb.h uvesafb.h; perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/video /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video mips ; perl scripts/headers_install.pl /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/include/generated/video /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video mips ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video/$$F; done; touch /home/toolchains/r2.3.3/obj/linux-2012.09-99-mips-linux-gnu-i686-pc-linux-gnu/tmp-install/include/video/.install
