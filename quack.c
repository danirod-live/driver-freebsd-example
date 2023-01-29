#include <sys/types.h>
#include <sys/systm.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/conf.h>
#include <sys/libkern.h>
#include <sys/uio.h>

static void print(char *msg) {
	log(LOG_DEBUG, "%s\n", msg);
	uprintf("%s\n", msg);
}

static int angry = 0;
static int count = 0;
static int answered = 0;

static int quack_read(
		struct cdev *cdev, struct uio *uio, int ioflag
		) {
	char buffer[64];
	int error = 0;

	if (!answered) {
		if (angry == 0) {
			sprintf(buffer, "*honk* %d\n", count);
		} else {
			sprintf(buffer, "👀🔪 estoy enfadado contigo\n");
		}
		error = uiomove(&buffer, strnlen(buffer, 63), uio);
		if (error != 0) {
			print("yo que se");
		}
		answered = 1;
	}
	return error;
}

static int
quack_write(
		struct cdev *dev, struct uio *uio, int ioflag	
	   )
{
	char buffer[64];
	int amount, error = 0;

	if (!answered) {
		amount = MIN(64, uio->uio_resid);
		error = uiomove(&buffer, amount, uio);
		count++;
		if (error) {
			print("se cagó");
		}
		if (strncmp((const char *) &buffer, "quack", 5) == 0) {
			print("uuh, la liaste");
			angry = 1;
		}
	}
	answered = 1;
	return error;
}

static int
quack_open(struct cdev *dev, int oflags, int devtype, struct thread *t)
{
	// print("abierto");
	answered = 0;
	return 0;
}

static int
quack_close(struct cdev *dev, int fflag, int devtype, struct thread *t)
{
	// print("cerrado");
	return 0;
}

static struct cdevsw quack_sw = {
	.d_version = D_VERSION,
	.d_open = quack_open,
	.d_close = quack_close,
	.d_read = quack_read,
	.d_write = quack_write,
	.d_name = "goose",
};

static struct cdev *quack_instance; 

static int
quack_handler(struct module *m, int what, void *arg) {
	int ecode;
	switch (what) {
		case MOD_LOAD:
			ecode = make_dev_p(
				MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
				&quack_instance,
				&quack_sw,
				0,
				UID_ROOT,
				GID_WHEEL,
				0600,
				"goose"
			);
			if (ecode != 0) {
				print("AYAYAYAYAYAY");
				return ecode;
			}
			print("buenos dias, *quack*\n");
			break;
		case MOD_UNLOAD:
			destroy_dev(quack_instance);
			print("hora de dormir, *quack*\n");
			break;
		default:
			return EOPNOTSUPP;
	}
	return 0;
}

DEV_MODULE(quack, quack_handler, NULL);
