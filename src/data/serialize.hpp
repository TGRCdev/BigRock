#ifndef BIGROCK_SERIALIZE_H
#define BIGROCK_SERIALIZE_H

#include <limits>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <list>
#include <stddef.h>
#include <string.h> // memcpy

namespace bigrock {
namespace data {

/// Thanks to David Cournapeau on Stack Overflow
/// https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program/1001373#1001373
inline bool is_big_endian(void)
{
    union {
        unsigned int i;
        char c[4];
    } num = {0x01020304};

    return num.c[0] == 1; 
}

enum SerialType
{
    SERIAL_TYPE_INT,
    SERIAL_TYPE_UINT,
    SERIAL_TYPE_LONG,
    SERIAL_TYPE_ULONG,
    SERIAL_TYPE_BOOL,
    SERIAL_TYPE_CHAR,
    SERIAL_TYPE_UCHAR,
    SERIAL_TYPE_FLOAT
};

template<class T>
struct Member
{
    Member(SerialType type, size_t offset)
    {
        this->type = type;
        this->offset = offset;
    }

    size_t offset;
    SerialType type;
};

// Overload this to use serialization
template<class T>
std::list<Member<T> > get_member_list();

template<typename T>
int serialize_primitive(T val, char *buffer)
{
    if(!buffer)
        return 0;
    
    if(is_big_endian())
        memcpy(buffer, &val, sizeof(T));
    else
    {
        union {
            T val;
            char bytes[sizeof(T)];
        } data;
        data.val = val;
        for(int i = 0; i < sizeof(T); i++)
            buffer[i] = data.bytes[sizeof(T) - (1 + i)];
    }
    return sizeof(T);
}

template<class T>
int serialize(const T& object, char *buffer, bool include_prefix = false)
{
    if(!buffer)
        return 0;
    
    unsigned int pos = 0;
    if(include_prefix)
    {
        buffer[0] = 'B';
        buffer[1] = 'R';
        pos = 2;
    }

    std::list<Member<T> > members = get_member_list<T>();
    for(typename std::list<Member<T> >::iterator it = members.begin(); it != members.end(); it++)
    {
        #define SERIALIZE(type) \
            pos += serialize_primitive<type>(*reinterpret_cast<const type*>(reinterpret_cast<const char*>(&object) + it->offset), buffer + pos);\
            break

        switch(it->type)
        {
            case SERIAL_TYPE_INT:
            SERIALIZE(int);
            case SERIAL_TYPE_UINT:
            SERIALIZE(unsigned int);
            case SERIAL_TYPE_LONG:
            SERIALIZE(long);
            case SERIAL_TYPE_ULONG:
            SERIALIZE(unsigned long);
            case SERIAL_TYPE_BOOL:
            SERIALIZE(bool);
            case SERIAL_TYPE_CHAR:
            SERIALIZE(char);
            case SERIAL_TYPE_UCHAR:
            SERIALIZE(unsigned char);
            case SERIAL_TYPE_FLOAT:
            SERIALIZE(float);
        }
    }

    return pos;
}

template<typename T>
int deserialize_primitive(T& member, const char *buffer)
{
    if(!buffer)
        return 0;
    
    if(is_big_endian())
        memcpy(&member, buffer, sizeof(T));
    else
    {
        union {
            T val;
            char bytes[sizeof(T)];
        } data;
        for(int i = 0; i < sizeof(T); i++)
            data.bytes[i] = buffer[sizeof(T) - (1 + i)];
        member = data.val;
    }
    return sizeof(T);
}

template<class T>
int deserialize(T& object, const char *buffer)
{
    if(!buffer)
        return 0;
    
    unsigned int pos = 0;
    if(buffer[0] == 'B' && buffer[1] == 'R')
        pos = 2;
    
    std::list<Member<T> > member_list = get_member_list<T>();

    for(typename std::list<Member<T> >::iterator it = member_list.begin(); it != member_list.end(); it++)
    {
        #define DESERIALIZE(type)\
        pos += deserialize_primitive<type>(*reinterpret_cast<type*>(reinterpret_cast<char*>(&object) + it->offset), buffer + pos);\
        break

        switch(it->type)
        {
            case SERIAL_TYPE_INT:
            DESERIALIZE(int);
            case SERIAL_TYPE_UINT:
            DESERIALIZE(unsigned int);
            case SERIAL_TYPE_LONG:
            DESERIALIZE(long);
            case SERIAL_TYPE_ULONG:
            DESERIALIZE(unsigned long);
            case SERIAL_TYPE_BOOL:
            DESERIALIZE(bool);
            case SERIAL_TYPE_CHAR:
            DESERIALIZE(char);
            case SERIAL_TYPE_UCHAR:
            DESERIALIZE(unsigned char);
            case SERIAL_TYPE_FLOAT:
            DESERIALIZE(float);
        }
    }

    return pos;
}

}}

#endif