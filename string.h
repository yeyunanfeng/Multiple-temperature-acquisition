/*--------------------------------------------------------------------------
STRING.H

String functions.
Copyright (c) 1988-2014 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef __STRING_H__
#define __STRING_H__

#ifndef _SIZE_T
 #define _SIZE_T
 typedef unsigned int size_t;
#endif

#ifndef NULL
 #define NULL ((void *) 0L)
#endif

#pragma SAVE
#pragma REGPARMS
extern char  *strcat  (char *s1, const char *s2);
extern char  *strncat (char *s1, const char *s2, size_t n);

extern char   strcmp  (const char *s1, const char *s2);
extern char   strncmp (const char *s1, const char *s2, size_t n);

extern char  *strcpy  (char *s1, const char *s2);
extern char  *strncpy (char *s1, const char *s2, size_t n);

extern size_t strlen  (const char *);

extern char  *strchr  (const char *s, char c);
extern int    strpos  (const char *s, char c);
extern char  *strrchr (const char *s, char c);
extern int    strrpos (const char *s, char c);

extern size_t strspn  (const char *s, const char *set);
extern size_t strcspn (const char *s, const char *set);
extern char  *strpbrk (const char *s, const char *set);
extern char  *strrpbrk(const char *s, const char *set);
extern char  *strstr  (const char *s, const char *sub);
extern char  *strtok  (char *str, const char *set);

extern char   memcmp  (const void *s1, const void *s2, size_t n);
extern void  *memcpy  (void *s1, const void *s2, size_t n);
extern void  *memchr  (const void *s, char val, size_t n);
extern void  *memccpy (void *s1, const void *s2, char val, size_t n);
extern void  *memmove (void *s1, const void *s2, size_t n);
extern void  *memset  (void *s, char val, size_t n);
#pragma RESTORE

#endif
