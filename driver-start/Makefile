obj-m := hello.o

all:
	make -C /usr/src/kernels/$(shell uname -r) M=$(shell pwd) modules

clean:
	make -C /usr/src/kernels/$(shell uname -r) M=$(shell pwd) clean
