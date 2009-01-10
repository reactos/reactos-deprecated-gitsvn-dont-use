/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <luserint.h>
/*#include <precomp.h>*/

extern int __mb_cur_max;
int __vfprintf(FILE *buf, const char*, va_list);

static int putc(int ch, FILE *f)
{
    **f = ch;
    (*f)++;
    return 1;
}

/*
 * @implemented
 */
int _vfprintf(FILE *buf, const char* fmt, va_list ap)
{
    return __vfprintf(buf, fmt, ap);
}


/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

/*
 * Appropiated for the reactos kernel, March 1998 -- David Welch
 */

#include <stdarg.h>

#include <ctype.h>
#include <string.h>
#include <math.h>
/*#include <internal/ieee.h>*/


#define ZEROPAD		1	/* pad with zero */
#define SIGN		2	/* unsigned/signed long */
#define PLUS		4	/* show plus */
#define SPACE		8	/* space if plus */
#define LEFT		16	/* left justified */
#define SPECIAL		32	/* 0x */
#define LARGE		64	/* use 'ABCDEF' instead of 'abcdef' */
#define ZEROTRUNC	128	/* truncate zero 's */


static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}


static int do_div(LONGLONG *n,int base)
{
    int nr = (int)*n, __res = nr % (unsigned) base;
    *n = nr / (unsigned) base;
    return __res;
}


static int number(FILE * f, LONGLONG num, int base, int size, int precision ,int type)
{
	char c,sign,tmp[66];
	const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
	int i, done = 0;

	if (type & LARGE)
		digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return done;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(&num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
		{
			if (putc(' ',f) == EOF)
				return -1;
			done++;
		}
	if (sign)
	{
		if (putc(sign,f) == EOF)
			return -1;
		done++;
	}
	if (type & SPECIAL) {
		if (base==8) {
			if (putc('0',f) == EOF)
				return -1;
			done++;
		}
		else if (base==16) {
			if (putc('0', f) == EOF)
				return -1;
			done++;
			if (putc(digits[33],f) == EOF)
				return -1;
			done++;
		}
	}
	if (!(type & LEFT))
		while (size-- > 0)
		{
			if (putc(c,f) == EOF)
				return -1;
			done++;
		}
	while (i < precision--)
	{
		if (putc('0', f) == EOF)
			return -1;
		done++;
	}
	while (i-- > 0)
	{
		if (putc(tmp[i],f) == EOF)
			return -1;
		done++;
	}
	while (size-- > 0)
	{
		if (putc(' ', f) == EOF)
			return -1;
		done++;
	}
	return done;
}

static int string(FILE *f, const char* s, int len, int field_width, int precision, int flags)
{
	int i, done = 0;
	if (s == NULL)
	{
		s = "<NULL>";
		len = 6;
	}
	else
	{
		if (len == -1)
		{
			len = 0;
			while ((unsigned int)len < (unsigned int)precision && s[len])
				len++;
		}
		else
		{
			if ((unsigned int)len > (unsigned int)precision)
				len = precision;
		}
	}
	if (!(flags & LEFT))
		while (len < field_width--)
		{
			if (putc(' ', f) == EOF)
				return -1;
			done++;
		}
	for (i = 0; i < len; ++i)
	{
		if (putc(*s++, f) == EOF)
			return -1;
		done++;
	}
	while (len < field_width--)
	{
		if (putc(' ', f) == EOF)
			return -1;
		done++;
	}
	return done;
}

static int stringw(FILE *f, const wchar_t* sw, int len, int field_width, int precision, int flags)
{
	int i, done = 0;
	char mb[5];
	if (sw == NULL)
	{
		sw = L"<NULL>";
		len = 6;
	}
	else
	{
		if (len == -1)
		{
			len = 0;
			while ((unsigned int)len < (unsigned int)precision && sw[len])
				len++;
		}
		else
		{
			if ((unsigned int)len > (unsigned int)precision)
				len = precision;
		}
	}
	if (!(flags & LEFT))
		while (len < field_width--)
		{
			if (putc(' ', f) == EOF)
				return -1;
			done++;
		}
	if(!mb)
		return -1;
	for (i = 0; i < len; ++i)
	{
		int mbcount, j;
		mbcount = 1;
                mb[0] = *sw++;
		if (mbcount <= 0)
		{
			break;
		}
		for (j = 0; j < mbcount; j++)
		{
			if (putc(mb[j], f) == EOF)
			{
				return -1;
			}
			done++;
		}
	}
	while (len < field_width--)
	{
		if (putc(' ', f) == EOF)
		{
			return -1;
		}
		done++;
	}
	return done;
}

int _isinf(double d) { return 0; }
int _isnan(double d) { return 0; }

int __vfprintf(FILE *f, const char *fmt, va_list args)
{
	int len;
	ULONGLONG num;
	int base;
	double _double;
	const char *s;
	const wchar_t *sw;
	int result, done = 0;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier = 0;	/* 'h', 'l', 'L' or 'I64' for integer fields */

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			if (putc(*fmt,f) == EOF)
				return -1;
			done++;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = 0;
		// %Z can be just stand alone or as size_t qualifier
		if ( *fmt == 'Z' ) {
			qualifier = *fmt;
			switch ( *(fmt+1)) {
				case 'o':
				case 'b':
				case 'X':
				case 'x':
				case 'd':
				case 'i':
				case 'u':
					++fmt;
					break;
				default:
					break;
			}
		} else if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'w') {
			qualifier = *fmt;
			++fmt;
		} else if (*fmt == 'I' && *(fmt+1) == '6' && *(fmt+2) == '4') {
			qualifier = *fmt;
			fmt += 3;
		}

		// go fine with ll instead of L
		if ( *fmt == 'l' ) {
			++fmt;
			qualifier = 'L';
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
				{
					if (putc(' ', f) == EOF)
						return -1;
					done++;
				}
			if (qualifier == 'l' || qualifier == 'w')
			{
				if (putc((unsigned char)(wchar_t) va_arg(args, int), f) == EOF)
					return -1;
				done++;
			}
			else
			{
				if (putc((unsigned char) va_arg(args, int), f) == EOF)
					return -1;
				done++;
			}
			while (--field_width > 0)
			{
				if (putc(' ', f) == EOF)
					return -1;
				done++;
			}
			continue;

		case 'C':
			if (!(flags & LEFT))
				while (--field_width > 0)
				{
					if (putc(' ', f) == EOF)
						return -1;
					done++;
				}
			if (qualifier == 'h')
			{
				if (putc((unsigned char) va_arg(args, int), f) == EOF)
					return -1;
				done++;
			}
			else
			{
				if (putc((unsigned char)(wchar_t) va_arg(args, int), f) == EOF)
					return -1;
				done++;
			}
			while (--field_width > 0)
			{
				if (putc(' ', f) == EOF)
					return -1;
				done++;
			}
			continue;

		case 's':
			if (qualifier == 'l' || qualifier == 'w') {
				/* print unicode string */
				sw = va_arg(args, wchar_t *);
				result = stringw(f, sw, -1, field_width, precision, flags);
			} else {
				/* print ascii string */
				s = va_arg(args, char *);
				result = string(f, s, -1, field_width, precision, flags);
			}
			if (result < 0)
				return -1;
			done += result;
			continue;

		case 'S':
			if (qualifier == 'h') {
				/* print ascii string */
				s = va_arg(args, char *);
				result = string(f, s, -1, field_width, precision, flags);
			} else {
				/* print unicode string */
				sw = va_arg(args, wchar_t *);
				result = stringw(f, sw, -1, field_width, precision, flags);
			}
			if (result < 0)
				return -1;
			done += result;
			continue;

		case 'Z':
			if (qualifier == 'w') {
				/* print counted unicode string */
				PUNICODE_STRING pus = va_arg(args, PUNICODE_STRING);
				if ((pus == NULL) || (pus->Buffer == NULL)) {
					sw = NULL;
					len = -1;
				} else {
					sw = pus->Buffer;
					len = pus->Length / sizeof(WCHAR);
				}
				result = stringw(f, sw, len, field_width, precision, flags);
			} else {
				/* print counted ascii string */
				PANSI_STRING pas = va_arg(args, PANSI_STRING);
				if ((pas == NULL) || (pas->Buffer == NULL)) {
					s = NULL;
					len = -1;
				} else {
					s = pas->Buffer;
					len = pas->Length;
				}
				result = string(f, s, -1, field_width, precision, flags);
			}
			if (result < 0)
				return -1;
			done += result;
			continue;

		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
         _double = (double)va_arg(args, double);

         if ( _isnan(_double) ) {
            s = "Nan";
            len = 3;
            while ( len > 0 ) {
               if (putc(*s++,f) == EOF)
                  return -1;
               done++;
               len --;
            }
         } else if ( _isinf(_double) < 0 ) {
            s = "-Inf";
            len = 4;
            while ( len > 0 ) {
               if (putc(*s++,f) == EOF)
                  return -1;
               done++;
               len --;
            }
         } else if ( _isinf(_double) > 0 ) {
            s = "+Inf";
            len = 4;
            while ( len > 0 ) {
               if (putc(*s++,f) == EOF)
                  return -1;
               done++;
               len --;
            }
         } else {
            if ( precision == -1 )
               precision = 6;
            result = 0; /*numberf(f,_double,*fmt,field_width,precision,flags);*/
            if (result < 0)
               return -1;
            done += result;
         }
			continue;

		case 'p':
			if (field_width == -1) {
				field_width = 2*sizeof(void *);
				flags |= ZEROPAD;
			}
			result = number(f,
				        (unsigned long) va_arg(args, void *), 16,
					field_width, precision, flags);
			if (result < 0)
				return -1;
			done += result;
			continue;

		case 'n':
			if (qualifier == 'l') {
				long * ip = va_arg(args, long *);
				*ip = 0;
			} else {
				int * ip = va_arg(args, int *);
				*ip = 0;
			}
			continue;

		/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'b':
			base = 2;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			if (*fmt != '%')
			{
				if (putc('%', f) == EOF)
					return -1;
				done++;
			}
			if (*fmt)
			{
				if (putc(*fmt, f) == EOF)
					return -1;
				done++;
			}
			else
				--fmt;
			continue;
		}

		if (qualifier == 'I')
			num = va_arg(args, ULONGLONG);
		else if (qualifier == 'l') {
			if (flags & SIGN)
				num = va_arg(args, long);
			else
				num = va_arg(args, unsigned long);
		}
		else if (qualifier == 'h') {
			if (flags & SIGN)
				num = va_arg(args, int);
			else
				num = va_arg(args, unsigned int);
		}
		else if (flags & SIGN)
			num = va_arg(args, int);
		else
			num = va_arg(args, unsigned int);
		result = number(f, num, base, field_width, precision, flags);
		if (result < 0)
			return -1;
		done += result;
	}
	//putc('\0',f);
	return done;
}

/* EOF */
