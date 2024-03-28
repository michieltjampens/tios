# Content
## stm32mp151-tios-mx.dts
The device tree generated with CubeMX and all the custom changes applied

## conf.path
To compile optee using the ST fork, conf.mk needs to be altered.
The reason for this is that conf.mk uses the filename to determine if it's 512MB or 1024MB but defaults to 1024MB.  
So this patch adds the dts to the list of 512MB.
