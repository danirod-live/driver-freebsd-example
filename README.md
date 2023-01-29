Some tries at writing kernels for FreeBSD.

## Folders

* 01-quack: a simple module that does nothing but says quack when installed or when the system starts if you load it at boot. This is just reading the chapter 9.1 of the Architecture Handbook.

* 02-angry-goose: a more complex example, generates a /dev/goose file which honks at you when you read the file, you can greet the goose by writing to the file, and it will count the number of times greeted (see the number when reading /dev/goose). If you echo "quack" into /dev/goose, it will get angry at you and will not honk anymore.

## Disclaimer

I'm learning, don't try this at home, I'm not responsible if your computer poops