KERN_DIR:= /work/system/linux-2.6.22.6
PWD:= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	make -C $(KERN_DIR) M=$(PWD) modules clean
	rm -rf modules.order

obj-m	+= base_text.o