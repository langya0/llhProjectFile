#pragma once
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>

#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512
//管理配置
typedef struct cJSON_Hooks {
    typedef void *(*MALLOC_FN)(size_t sz);
    typedef void(*FREE_FN)(void*);

    void *(*malloc_fn)(size_t sz);
    void(*free_fn)(void *ptr);
public:
    cJSON_Hooks(MALLOC_FN ma = NULL, FREE_FN fr = NULL)
        :malloc_fn(ma)
        , free_fn(fr)
    {}

    struct cJSON_Hooks* operator()()
    {
        if (!free_fn)
        { /* Reset hooks */
            malloc_fn = malloc;
            free_fn = free;
        }
        return this;
    }
} cJSON_Hooks;

static void *(*cJSON_malloc)(size_t sz) = malloc;
static void(*cJSON_free)(void *ptr) = free;

/* The cJSON structure: */
typedef struct cJSON {
protected:
    struct cJSON *next, *prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

    int type;					/* The type of the item, as above. */

    char *valuestring;			/* The item's string, if type==cJSON_String */
    int valueint;				/* The item's number, if type==cJSON_Number */
    double valuedouble;			/* The item's number, if type==cJSON_Number */

    char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */

protected:
    cJSON *create_reference(cJSON *item)
    { 
        cJSON *ref = cJSON_New_Item(); 
        if (!ref)
            return 0;
        memcpy(ref, item, sizeof(cJSON));
        ref->string = 0; 
        ref->type |= cJSON_IsReference;
        ref->next = ref->prev = 0; 
        return ref;
    }

    cJSON *cJSON_New_Item(void)
    {
        cJSON* node = (cJSON*)cJSON_Hooks()()->malloc_fn(sizeof(cJSON));

        if (node) 
            memset(node, 0, sizeof(cJSON));
        return node;
    }
    char* cJSON_strdup(const char* str)
    {
        size_t len;
        char* copy;

        len = strlen(str) + 1;
        if (!(copy = (char*)cJSON_Hooks()()->malloc_fn(len)))
            return 0;
        memcpy(copy, str, len);
        return copy;
    }
    void suffix_object(cJSON *prev, cJSON *item) 
    { 
        prev->next = item;
        item->prev = prev; 
    }

    int cJSON_strcasecmp(const char *s1, const char *s2)
    {
        if (!s1)
            return (s1 == s2) ? 0 : 1;
        if (!s2) return 1;
        for (; tolower(*s1) == tolower(*s2); ++s1, ++s2)	
            if (*s1 == 0)	
                return 0;
        return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
    }
public:

    /* These calls create a cJSON item of the appropriate type. */
    cJSON *cJSON_CreateNull(void)
    {
        cJSON *item = cJSON_New_Item();
        if (item)
            item->type = cJSON_NULL;
        return item;
    }

    cJSON *cJSON_CreateTrue(void)
    {
        cJSON *item = cJSON_New_Item();
        if (item)
            item->type = cJSON_True; 
        return item;
    }
    cJSON *cJSON_CreateFalse(void)
    {
        cJSON *item = cJSON_New_Item();
        if (item)
            item->type = cJSON_False;
        return item;
    }
    cJSON *cJSON_CreateBool(int b)
    {
        cJSON *item = cJSON_New_Item();
        if (item)
            item->type = b ? cJSON_True : cJSON_False; 
        return item;
    }
    cJSON *cJSON_CreateNumber(double num)
    {
        cJSON *item = cJSON_New_Item();
        if (item)
        {
            item->type = cJSON_Number;
            item->valuedouble = num; 
            item->valueint = (int)num;
        }
        return item; 
    }
    cJSON *cJSON_CreateString(const char *string)
    {
        cJSON *item = cJSON_New_Item();
        if (item) 
        { 
            item->type = cJSON_String;
            item->valuestring = cJSON_strdup(string); 
        }
        return item; 
    }
    cJSON *cJSON_CreateArray(void)
    {
        cJSON *item = cJSON_New_Item(); 
        if (item)
            item->type = cJSON_Array;
        return item;
    }
    cJSON *cJSON_CreateObject(void)
    {
        cJSON *item = cJSON_New_Item(); 
        if (item)item->type = cJSON_Object; 
        return item; 
    }

    /* These utilities create an Array of count items. */
    /* Create Arrays: */
    cJSON *cJSON_CreateIntArray(const int *numbers, int count)
    {
        int i;
        cJSON *n = 0,
            *p = 0,
            *a = cJSON_CreateArray();
        for (i = 0; a && i<count; i++) 
        {
            n = cJSON_CreateNumber(numbers[i]);     //获取元素
            if (!i)
                a->child = n; 
            else 
                suffix_object(p, n);
            p = n;
        }
        return a;
    }
    cJSON *cJSON_CreateFloatArray(const float *numbers, int count) { int i; cJSON *n = 0, *p = 0, *a = cJSON_CreateArray(); for (i = 0; a && i<count; i++) { n = cJSON_CreateNumber(numbers[i]); if (!i)a->child = n; else suffix_object(p, n); p = n; }return a; }
    cJSON *cJSON_CreateDoubleArray(const double *numbers, int count) { int i; cJSON *n = 0, *p = 0, *a = cJSON_CreateArray(); for (i = 0; a && i<count; i++) { n = cJSON_CreateNumber(numbers[i]); if (!i)a->child = n; else suffix_object(p, n); p = n; }return a; }
    cJSON *cJSON_CreateStringArray(const char **strings, int count) { int i; cJSON *n = 0, *p = 0, *a = cJSON_CreateArray(); for (i = 0; a && i<count; i++) { n = cJSON_CreateString(strings[i]); if (!i)a->child = n; else suffix_object(p, n); p = n; }return a; }

    /* Add item to array/object. */
    void cJSON_AddItemToArray(cJSON *array, cJSON *item)
    { 
        cJSON *c = array->child;
        if (!item)
            return;
        if (!c) 
            array->child = item; 
        else 
        {
            while (c && c->next)
                c = c->next; 
            suffix_object(c, item); 
        }
    }

    //push
    void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
    { 
        if (!item)          //所增加的结点为空
            return;
        if (item->string)       //string
            cJSON_free(item->string);
        item->string = cJSON_strdup(string);
        cJSON_AddItemToArray(object, item);
    }
    void cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item) { if (!item) return; if (!(item->type&cJSON_StringIsConst) && item->string) cJSON_free(item->string); item->string = (char*)string; item->type |= cJSON_StringIsConst; cJSON_AddItemToArray(object, item); }
    void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item) { cJSON_AddItemToArray(array, create_reference(item)); }
    void cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item) { cJSON_AddItemToObject(object, string, create_reference(item)); }

    void cJSON_Delete(cJSON *c)
    {
        cJSON *next;
        while (c)
        {
            next = c->next;
            if (!(c->type&cJSON_IsReference) && c->child)
                cJSON_Delete(c->child);
            if (!(c->type&cJSON_IsReference) && c->valuestring)
                cJSON_free(c->valuestring);
            if (!(c->type&cJSON_StringIsConst) && c->string)
                cJSON_free(c->string);
            cJSON_free(c);
            c = next;
        }
    }

    //类似pop函数
    cJSON *cJSON_DetachItemFromArray(cJSON *array, int which)
    {
        cJSON *c = array->child;
        while (c && which>0)
            c = c->next, which--;
        if (!c) 
            return 0;
        if (c->prev) 
            c->prev->next = c->next;
        if (c->next)
            c->next->prev = c->prev;
        if (c == array->child) 
            array->child = c->next;
        c->prev = c->next = 0;
        return c;
    }
    void   cJSON_DeleteItemFromArray(cJSON *array, int which) { cJSON_Delete(cJSON_DetachItemFromArray(array, which)); }
    cJSON *cJSON_DetachItemFromObject(cJSON *object, const char *string)
    { 
        int i = 0; 
        cJSON *c = object->child; 
        while (c && cJSON_strcasecmp(c->string, string))
            i++, c = c->next;
        if (c)
            return cJSON_DetachItemFromArray(object, i);
        return 0; 
    }
    void   cJSON_DeleteItemFromObject(cJSON *object, const char *string) { cJSON_Delete(cJSON_DetachItemFromObject(object, string)); }


}cJSON;

