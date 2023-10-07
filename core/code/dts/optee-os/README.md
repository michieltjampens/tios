# Content
## stm32mp151-tios-mx.dts
The device tree generated with CubeMX and all the custom changes applied

## conf.mk
To compile optee using the ST fork, this file needs to replace the one in the repo.  
The reason for this is that conf.mk uses the filename to determine if it's 512MB or 1024MB but defaults to 1024MB.  
So this version replaces this part of the file so that 512MB is used for the tios device tree.
