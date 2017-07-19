#ifndef _TYPES_H_
#define _TYPES_H_

/* Signed integer definitions (32bit, 16bit, 8bit) follow... */
typedef int			S32;
typedef short		S16;
typedef char		S8;

/* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
typedef unsigned int	UNS32;
typedef unsigned short	UNS16;
typedef unsigned char	UNS8;

typedef volatile unsigned int 	VUNS32;
typedef volatile unsigned short VUNS16;
typedef volatile unsigned char 	VUNS8;

#define BIT(B) 		(1<<B)
#define CL_BIT(B)	(0<<B)

#endif
