#include "ZSConf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ZSCONF_STATUS moveComment(char *line)
{
    char *p = line;
    while(*p != 0)
    {
        if(*p == ZSCONF_COMMENT_CHART)
        {
            *p = 0;
            break;
        }
        p++;
    }
    return ZSCONF_SUCCESS;
}

static ZSCONF_STATUS moveBlankChar(char *line)
{
    if(NULL == line || line[0] == 0)
        return ZSCONF_FAIL;
    char newLine[ZSCONF_LINE_SIZE];
    memset(newLine, 0, sizeof(newLine));
    char *head = line;
    char *tail = line + strlen(line) - 1;
    char hflag = 0, tflag = 0;
    while(head != tail)
    {
        //33~126
        if(*head >= 33 && *head <= 126) hflag = 1;
        if(*tail >= 33 && *tail <= 126) tflag = 1;
        if(hflag && tflag) break;
        if(!hflag) head++;
        if(!tflag) tail--;
    }
    //printf("head = \'%c\', tail = \'%c\'\n", *head, *tail);
    if(tail - head > ZSCONF_LINE_SIZE || tail - head < 0)
        return ZSCONF_FAIL;

    if((head == tail) && (*head < 33 || *head > 126))
    {
        return ZSCONF_FAIL;
    }
    else
    {
        memcpy(newLine, head, tail - head + 1);
        memcpy(line, newLine, strlen(newLine) + 1);
        //printf("After move blank char:[%s]\n", line);
    }

    return ZSCONF_SUCCESS;
}


static ZSCONF_STATUS checkGroupLine(char *line, char *groupName)
{
    int len = strlen(line);
    if(line[0] == '[' && line[len - 1] == ']')
    {
        memcpy(groupName, line + 1, len - 2);
        return ZSCONF_SUCCESS;
    }
    return ZSCONF_FAIL;
}

static ZSCONF_STATUS getGroupItem(char *line, ZSConf_Item *item)
{
    char *p;
    p = line;
    while(*p != 0 && *p != '=') p++;
    if(*p != '=')
        return ZSCONF_FAIL;
    memset(item->key, 0, ZSCONF_KEY_SIZE);
    memcpy(item->key, line, p - line);
    if(ZSCONF_FAIL == moveBlankChar(item->key))
    {
        printf("Invalid key.\n");
        return ZSCONF_FAIL;
    }
    //printf("Get key = [%s], ", item->key);

    memset(item->valueString, 0, ZSCONF_LINE_SIZE);
    memcpy(item->valueString, p + 1, strlen(p));
    if(ZSCONF_FAIL == moveBlankChar(item->valueString))
    {
        printf("Invalid valueString.\n");
        return ZSCONF_FAIL;
    }
    //printf("Get valueString = [%s]\n", item->valueString);
    return ZSCONF_SUCCESS;
}

/*
static ZSConf_Item* findItem(ZSConf_t cfg, const char *groupName, const char *key)
{
    ZSConf_Group *group = NULL;
    ZSConf_Item *item = NULL;
    for(int i = 0; i < cfg.itemNumber; i++)
    {
        group = &(cfg.group[i]);
        if(0 == strcmp(groupName, group->groupName))
        {

            for(int j = 0; j < group->itemNumber; j++)
            {
                item = &(group->item[j]);
                printf("\tkey = %s, value = %s\n", item->key, item->valueString);
                if(0 == strcmp(key, item->key))
                {
                    return item;
                }
            }
        }

    }
    return NULL;
}
*/
ZSCONF_STATUS ZSConf_Parse(ZSConf_t *cfg, const char *confFile)
{
    char line[ZSCONF_LINE_SIZE];
    FILE *file = NULL;
    if(NULL == (file = fopen(confFile, "r+")))
    {
        return ZSCONF_FAIL;
    }

    cfg->itemNumber = 0;
    char groupName[ZSCONF_KEY_SIZE];
    ZSConf_Group *group;
    ZSConf_Item *item;
    while(0 == feof(file))
    {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), file);
        moveComment(line);
        if(ZSCONF_FAIL == moveBlankChar(line))
            continue;
        //printf("\nValid Line: [%s]\n", line);

        memset(groupName, 0, sizeof(groupName));
        if(ZSCONF_SUCCESS == checkGroupLine(line, groupName))
        {
            group = &(cfg->group[cfg->itemNumber]);
            group->itemNumber = 0;
            memcpy(group->groupName, groupName, strlen(groupName) + 1);
            //printf("Group name: %s\n", group->groupName);
            cfg->itemNumber++;
            if(cfg->itemNumber >= cfg->setGroupSize)
            {
                fclose(file);
                return ZSCONF_SUCCESS;
            }
        }
        else
        {
            item = &(group->item[group->itemNumber]);
            if(ZSCONF_SUCCESS == getGroupItem(line, item))
                group->itemNumber++;
            if(group->itemNumber >= cfg->setItemSize)
            {
                fclose(file);
                return ZSCONF_SUCCESS;
            }
        }
    }

    fclose(file);
    return ZSCONF_SUCCESS;
}

ZSCONF_STATUS ZSConf_Init(ZSConf_t *cfg, int groupSize, int itemSize)
{
    ZSConf_Group *group;
    if(groupSize > ZSCONF_GROUP_SIZE) 
        groupSize = ZSCONF_GROUP_SIZE;
    if(itemSize > ZSCONF_ITEM_SIZE)  
        itemSize = ZSCONF_ITEM_SIZE;

    cfg->setGroupSize = groupSize;
    cfg->setItemSize = itemSize;

    group = (ZSConf_Group*)malloc(sizeof(ZSConf_Group) * cfg->setGroupSize);
    if(NULL == group)
    {
        return ZSCONF_FAIL;
    }
    cfg->group = group;
    ZSConf_Group *ptrGroup = cfg->group;
    for(int i = 0; i < cfg->setGroupSize; i++)
    {
        ZSConf_Item *item = (ZSConf_Item*)malloc(sizeof(ZSConf_Item) * cfg->setItemSize);
        if(NULL == item)
        {
            //TODO:free
            return ZSCONF_FAIL;
        }
        ptrGroup->item = item;
        ptrGroup++;
    }
    return ZSCONF_SUCCESS;
}

ZSCONF_STATUS ZSConf_Destroy(ZSConf_t *cfg)
{
    ZSConf_Group *ptrGroup = cfg->group;
    for(int i = 0; i < cfg->setGroupSize; i++)
    {
        if(ptrGroup && ptrGroup->item)
            free(ptrGroup->item);
        ptrGroup++;
    } 

    if(cfg->group)
        free(cfg->group);
    return ZSCONF_SUCCESS;   
}

ZSCONF_STATUS ZSConf_GetItemInt(ZSConf_t *cfg, const char *groupName, const char *key, int *value)
{
    ZSConf_Group *group;
    for(int i = 0; i < cfg->itemNumber; i++)
    {
        group = &(cfg->group[i]);
        if(0 != strcmp(groupName, group->groupName))
            continue;
        ZSConf_Item *item;
        for(int j = 0; j < group->itemNumber; j++)
        {
            item = &(group->item[j]);
            if(0 != strcmp(key, item->key))
                continue;
            //printf("\tkey = %s, value = %s\n", item->key, item->valueString);
            char *p = item->valueString;
            if(*p == '-')
            {
                *value = atoi(p + 1);
                *value = 0 - *value;
            }
            else
                *value = atoi(item->valueString);
            return ZSCONF_SUCCESS;
        }

    }
    return ZSCONF_FAIL;
}

ZSCONF_STATUS ZSConf_GetItemFloat(ZSConf_t *cfg, const char *groupName, const char *key, float *value)
{
    ZSConf_Group *group;
    for(int i = 0; i < cfg->itemNumber; i++)
    {
        group = &(cfg->group[i]);
        if(0 != strcmp(groupName, group->groupName))
            continue;
        ZSConf_Item *item;
        for(int j = 0; j < group->itemNumber; j++)
        {
            item = &(group->item[j]);
            if(0 != strcmp(key, item->key))
                continue;
            //printf("\tkey = %s, value = %s\n", item->key, item->valueString);
            char *p = item->valueString;
            if(*p == '-')
            {
                *value = atof(p + 1);
                *value = 0 - *value;
            }
            else
                *value = atof(p);
            return ZSCONF_SUCCESS;
        }

    }
    return ZSCONF_FAIL;
}

ZSCONF_STATUS ZSConf_GetItemString(ZSConf_t *cfg, const char *groupName, const char *key, char *value)
{
    ZSConf_Group *group;
    for(int i = 0; i < cfg->itemNumber; i++)
    {
        group = &(cfg->group[i]);
        if(0 != strcmp(groupName, group->groupName))
            continue;
        ZSConf_Item *item;
        for(int j = 0; j < group->itemNumber; j++)
        {
            item = &(group->item[j]);
            if(0 != strcmp(key, item->key))
                continue;
            //printf("\tkey = %s, value = %s\n", item->key, item->valueString);
            memcpy(value, item->valueString, strlen(item->valueString) + 1);
            return ZSCONF_SUCCESS;
        }
    }
    return ZSCONF_FAIL;
}





#if 0
ZSCONF_STATUS ZSConf_test()
{
    ZSConf_t cfg;
    ZSConf_Parse(&cfg, "test2.conf");
    printf("\n");
    printf("ZSConf_Parse done:\n");
    for(int i = 0; i < cfg.itemNumber; i++)
    {
        ZSConf_Group *group = &(cfg.group[i]);
        printf("Group: %s\n", group->groupName);

        for(int j = 0; j < group->itemNumber; j++)
        {
            ZSConf_Item *item = &(group->item[j]);
            printf("\tkey = %s, value = %s\n", item->key, item->valueString);
        }
    }
//RF_PORT
    int iValue;
    ZSConf_GetItemInt(cfg, "VSG", "RF_PORT", &iValue);
    printf("==RF_PORT:%d\n", iValue);
    char sValue[20];
    ZSConf_GetItemString(cfg, "VSG", "Mode", sValue);
    printf("==Mode:%s\n", sValue);
    float fValue;
    ZSConf_GetItemFloat(cfg, "VSG", "TX_POWER_ATT", &fValue);
    printf("==TX_POWER_ATT:%f\n", fValue);
//ZSConf_GetItemFloat
    return ZSCONF_SUCCESS;
}
#endif

