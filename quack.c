#include <sys/types.h>
#include <sys/systm.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>

static void print(char *msg) {
	log(LOG_DEBUG, "%s\n", msg);
	uprintf("%s\n", msg);
}

static int
quack_handler(struct module *m, int what, void *arg) {
	switch (what) {
		case MOD_LOAD:
			print("buenos dias, *quack*\n");
			break;
		case MOD_UNLOAD:
			print("hora de dormir, *quack*\n");
			break;
		default:
			return EOPNOTSUPP;
	}
	return 0;
}

static moduledata_t quack_struct = {
	"quack",
	quack_handler,
	NULL,
};

DECLARE_MODULE(quack, quack_struct, SI_SUB_KLD, SI_ORDER_ANY);
