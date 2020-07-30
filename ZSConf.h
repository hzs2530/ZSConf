#ifndef ZSCONF_H
#define ZSCONF_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ZSCONF_SUCCESS            (1)
#define ZSCONF_FAIL               (0)
#define ZSCONF_COMMENT_CHART      '#'
#define ZSCONF_KEY_SIZE           (255)
#define ZSCONF_LINE_SIZE          (1024)
#define ZSCONF_GROUP_SIZE         (50)
#define ZSCONF_ITEM_SIZE          (500)


typedef char ZSCONF_STATUS;


typedef struct
{
    char key[ZSCONF_KEY_SIZE];
    char valueString[ZSCONF_LINE_SIZE];
}ZSConf_Item;

typedef struct
{
    char groupName[ZSCONF_KEY_SIZE];
    ZSConf_Item *item;
    int  itemNumber;
}ZSConf_Group;

typedef struct
{
    ZSConf_Group *group;
    int setGroupSize;
    int setItemSize;
    int  itemNumber;
}ZSConf_t;

ZSCONF_STATUS ZSConf_Init(ZSConf_t *cfg, int groupSize, int itemSize);
ZSCONF_STATUS ZSConf_Parse(ZSConf_t *cfg, const char *confFile);
ZSCONF_STATUS ZSConf_Destroy(ZSConf_t *cfg);
ZSCONF_STATUS ZSConf_GetItemInt(ZSConf_t *cfg, const char *groupName, const char *key, int *value);
ZSCONF_STATUS ZSConf_GetItemFloat(ZSConf_t *cfg, const char *groupName, const char *key, float *value);
ZSCONF_STATUS ZSConf_GetItemString(ZSConf_t *cfg, const char *groupName, const char *key, char *value);


ZSCONF_STATUS ZSConf_SetItemInt(ZSConf_t *cfg, const char *groupName, const char *key, int value);
ZSCONF_STATUS ZSConf_SetItemDouble(ZSConf_t *cfg, const char *groupName, const char *key, double value);
ZSCONF_STATUS ZSConf_SetItemString(ZSConf_t *cfg, const char *groupName, const char *key, char *value);

ZSCONF_STATUS ZSConf_test();

#ifdef __cplusplus
}
#endif
#if 0
#include <iostream>
ZSCONF_STATUS ZSConf_GetItem(ZSConf_t cfg, std::string groupName, std::string key, int &value)
{
    int iValue;
    ZSCONF_STATUS status = ZSConf_GetItemInt(cfg, groupName.c_str(), key.c_str(), &iValue);
    value = iValue;
    return status;
}
ZSCONF_STATUS ZSConf_GetItem(ZSConf_t cfg, std::string groupName, std::string key, float &value)
{
    float fValue;
    ZSCONF_STATUS status = ZSConf_GetItemFloat(cfg, groupName.c_str(), key.c_str(), &fValue);
    value = fValue;
    return status;
}
ZSCONF_STATUS ZSConf_GetItem(ZSConf_t cfg, std::string groupName, std::string key, std::string &value)
{
    char sValue[ZSCONF_LINE_SIZE];
    ZSCONF_STATUS status = ZSConf_GetItemString(cfg, groupName.c_str(), key.c_str(), sValue);
    value = sValue;
    return status;
}
#endif


#endif
