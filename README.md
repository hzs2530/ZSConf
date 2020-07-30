# ZSConf
用于读取类似conf/ini格式配置文件的Ｃ语言接口

#Example
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ZSConf.h"

int main(int argc, char *argv[])
{
    ZSConf_t cfg;
    if(ZSCONF_FAIL == ZSConf_Init(&cfg, 10, 100))
    {
        printf("Call ZSConf_Init failed.\n");
        ZSConf_Destroy(&cfg);
        exit(-1);
    }
    ZSConf_Parse(&cfg, "test.conf");

    int conf_int;
    float conf_f;
    char conf_s[255];

    if(ZSCONF_SUCCESS == ZSConf_GetItemInt(&cfg, "GROUP1", "conf_int", &conf_int))
    {
        printf("Read GROUP1, conf_int = %d.\n", conf_int);
    }
    if(ZSCONF_SUCCESS == ZSConf_GetItemFloat(&cfg, "GROUP1", "conf_float", &conf_f))
    {
        printf("Read GROUP1, conf_float = %f.\n", conf_f);
    }
    if(ZSCONF_SUCCESS == ZSConf_GetItemString(&cfg, "GROUP1", "conf_string", &conf_s))
    {
        printf("Read GROUP1, conf_string = %s.\n", conf_s);
    }

    if(ZSCONF_SUCCESS == ZSConf_GetItemInt(&cfg, "GROUP2", "conf_int", &conf_int))
    {
        printf("Read GROUP2, conf_int = %d.\n", conf_int);
    }
    if(ZSCONF_SUCCESS == ZSConf_GetItemFloat(&cfg, "GROUP2", "conf_float", &conf_f))
    {
        printf("Read GROUP2, conf_float = %f.\n", conf_f);
    }
    if(ZSCONF_SUCCESS == ZSConf_GetItemString(&cfg, "GROUP2", "conf_string", &conf_s))
    {
        printf("Read GROUP2, conf_string = %s.\n", conf_s);
    }
    ZSConf_Destroy(&cfg);
    return 0;
}
```

#test.conf
```
[GROUP1]
conf_int = 125 #int
conf_string = hello#string
conf_float = 1.235#float

[GROUP2]
conf_int = 666
conf_string = hello world
conf_float = 5.123
```
