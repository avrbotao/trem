
/*
 *          |          _                     _______________________
 *          |\       _/ \_                  |                       |
 *          | \_    /_    \_                |    Alexandre Botao    |
 *          \   \__/  \__   \               |     www.botao.org     |
 *           \_    \__/  \_  \              |    55-11-8244-UNIX    |
 *             \_   _/     \ |              |  alexandre@botao.org  |
 *               \_/        \|              |_______________________|
 *                           |
 */

/*                                       _______________________________
 *                                      |                               |
 *                                      |   trem           tree mimic   |
 *                                      |_______________________________|
 */

/*______________________________________________________________________
 |                                                                      |
 |  This file is part of the 'TREM' ("tree mimic") software            |
 |  as released by Alexandre Botao <botao.org> ;                        |
 |                                                                      |
 |  'TREM' is Free and Open Source software (FOSS); this means you can  |
 |  redistribute it and/or modify it under the terms of the GNU General |
 |  Public License as published by the Free Software Foundation, either |
 |  version 3 of the License, or (at your option) any later version.    |
 |                                                                      |
 |  'TREM' is distributed in the hope that it will be useful,           |
 |  but WITHOUT ANY WARRANTY; without even the implied warranty of      |
 |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                |
 |  See the GNU General Public License for more details.                |
 |                                                                      |
 |  You should have received a copy of the GNU General Public License	|
 |  along with 'TREM'.  If not, see <http://www.gnu.org/licenses/>, or  |
 |  write to the Free Software Foundation, Inc.,                        |
 |  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.            |
 |______________________________________________________________________|
 */

#ifndef _TREM_H_

#define _TREM_H_

/*________________________________________________________________________
*/

#include "config.h"

#include "version.h"

#include "absd.h"

/*________________________________________________________________________
*/
# ifdef AIX
#	define	__H_LOCALEDEF
# endif

# define	_XOPEN_SOURCE	500
/*________________________________________________________________________
*/
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ftw.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
/*________________________________________________________________________
*/
# define	DFL_LOGPATH		"/tmp/trem.log"

# define	HELPMESSAGE		"use : trem [-Ccdfghklnpsvwx] -M masterpath -m mimicpath\n"

# define	DFL_MAXLEAF		2097152 /*	32768	*/

# define	TREM_PRUNE			0x0001
# define	TREM_GRAFT			0x0002
# define	TREM_WEIGH			0x0004
# define	TREM_COMPD			0x0010

# ifndef	PATH_MAX
# define	PATH_MAX		2048
# endif

# define	MAX1PATH		PATH_MAX+1

# define	DFL_SYLSIZE		4096
/*________________________________________________________________________
*/
struct tremleaf {
	char *				tl_name ;
	const struct stat *	tl_mast ;
	const struct stat *	tl_mist ;
	struct FTW  *		tl_matb ;
	struct FTW  *		tl_mitb ;
	int					tl_flag ;
	int					tl_code ;
} ;

typedef		struct tremleaf		TREM_LEAF ;

# define	TREM_LEAFSIZE			sizeof(TREM_LEAF)
/*________________________________________________________________________
*/

#endif /* _TREM_H_ */

/*
 * vi:nu ts=4
 */

