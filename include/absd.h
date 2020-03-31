
/*	 _______________________________________________________________
 *	|								|
 *	|	absd.h		    (c) 2000-2018 Alexandre Botao	|
 *	|_______________________________________________________________|
 */

/*____________________________________________________________________________
*/

# if	defined (__OpenBSD__) || defined (OpenBSD) || defined (OPENBSD) 

#	ifndef _OBSD_H_

#		define _OBSD_H_

/*		size_t strlcpy(char *dst, const char *src, size_t dstsize);	*/
/*		size_t strlcat(char *dst, const char *src, size_t dstsize);	*/

#		define	_STR_CPY_(D,S)		strlcpy(D,S,sizeof(D))
#		define	_STR_CAT_(D,S)		strlcat(D,S,sizeof(D))

#	endif /* _OBSD_H_ */

# else	/* ! ( __OpenBSD__ || OpenBSD || OPENBSD ) */

#	define	_STR_CPY_(D,S)		strcpy(D,S)
#	define	_STR_CAT_(D,S)		strcat(D,S)

# endif /* defined (__OpenBSD__) || defined (OpenBSD) || defined (OPENBSD) */

/*____________________________________________________________________________
*/

/*
 * vi:nu tabstop=8
 */
