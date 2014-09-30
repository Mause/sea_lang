#ifndef OBJECT_H
#define OBJECT_H

#define SeaObjectHeader \
    void* object_type; \// pointers will always be unique, so use them as idents
    int ref_count;


typedef struct {
    void* object_type; // pointers will always be unique, so use them as idents
    int ref_count;
} SeaObject;


#endif
