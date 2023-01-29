#include <sys/types.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/libkern.h>
#include <sys/syslog.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/uio.h>

/* Set this to 1 to make the goose angry (will not answer to read) */
static int angry = 0;

/* Number of times you have talked to the goose */
static int count = 0;

/* Remember it has been talked (otherwise it enters infinite loop) */
static int answered = 0;

static int goose_read(struct cdev *cdev, struct uio *uio, int ioflag) {
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
      uprintf("yo que se");
    }

    answered = 1;
  }
  return error;
}

static int goose_write(struct cdev *dev, struct uio *uio, int ioflag) {
  char buffer[64];
  int amount, error = 0;

  if (!answered) {
    amount = MIN(64, uio->uio_resid);
    error = uiomove(&buffer, amount, uio);
    if (error) {
      uprintf("lo rompí");
	  return error;
    }

    if (strncmp((const char *)&buffer, "quack", 5) == 0) {
      uprintf("cagaste");
      angry = 1;
    }
    count++;
  }
  answered = 1;
  return error;
}

static int goose_open(struct cdev *dev, int oflags, int devtype,
                      struct thread *t) {
  answered = 0;
  return 0;
}

static int goose_close(struct cdev *dev, int fflag, int devtype,
                       struct thread *t) {
  return 0;
}

static struct cdevsw goose_sw = {
    .d_version = D_VERSION,
    .d_open = goose_open,
    .d_close = goose_close,
    .d_read = goose_read,
    .d_write = goose_write,
    .d_name = "goose",
};

static struct cdev *goose_instance;

static int goose_handler(struct module *m, int what, void *arg) {
  int ecode;
  switch (what) {
  case MOD_LOAD:
    ecode = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK, &goose_instance,
                       &goose_sw, 0, UID_ROOT, GID_WHEEL, 0600, "goose");
    if (ecode != 0) {
      uprintf("AYAYAYAYAYAY");
      return ecode;
    }
    uprintf("the goose has been deployed\n");
    break;
  case MOD_UNLOAD:
    destroy_dev(goose_instance);
    uprintf("the goose is sleeping\n");
    break;
  default:
    return EOPNOTSUPP;
  }
  return 0;
}

DEV_MODULE(goose, goose_handler, NULL);
