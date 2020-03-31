
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
 |  This file is part of the 'TREM' ("tree mimic") software             |
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

#include "trem.h"

/*________________________________________________________________________
*/

int	commitflag = 0 ;				/*	execute changes				*/
int	contentflag = 0 ;				/*	compare contents			*/
int	diffflag = 1 ;					/*	run diff					*/
int	mockflag = 1 ;					/*	preview only				*/
int	modeflag = 0 ;					/*	uneven modes				*/
int	freshflag = 0 ;					/*	update newer				*/
int	graftflag = 0 ;					/*	graft only					*/
int	helpflag = 0 ;					/*	syntax help					*/
int	knackflag = 0 ;					/*	crafted artistry			*/
int	lsflag = 0 ;					/*	run ls						*/
int	matchflag = 0 ;					/*	match pattern/regex			*/
int	pruneflag = 0 ;					/*	prune only					*/
int	systemflag = 0 ;				/*	system commands				*/
int	usageflag = 0 ;					/*	usage help					*/
int	verboseflag = 0 ;				/*	detailed report				*/
int	versionflag = 0 ;				/*	detailed report				*/
int	weighflag = 0 ;					/*	weigh only					*/
int	zestflag = 0 ;					/*	taste before				*/

int grd = 0 ;						/*	global result descriptor	*/
int maxdepth = 127 ;				/*	NOFILES - 10%				*/
int maxleaf = DFL_MAXLEAF ;

int lenmasterfull = 0 ;
int lenmimicfull = 0 ;

int graftcount = 0 ;
int graftdircount = 0 ;
int graftfilcount = 0 ;
int graftsylcount = 0 ;
int graftanycount = 0 ;

int prunecount = 0 ;
int prunedircount = 0 ;
int prunefilcount = 0 ;
int prunesylcount = 0 ;
int pruneanycount = 0 ;

int leafcount = 0 ;
int copycount = 0 ;
int permcount = 0 ;

int tmstate = 0 ;

int sylsiz = DFL_SYLSIZE ;

off_t graftbytes = 0 ;
off_t prunebytes = 0 ;
off_t weighbytes = 0 ;

char * mastertop = NULL ;
char * mimictop = NULL ;
char * logfile = NULL ;

char workfull [ MAX1PATH ] ;

char masterfull [ MAX1PATH ] ;
char mimicfull [ MAX1PATH ] ;

char masterleaf [ MAX1PATH ] ;
char mimicleaf [ MAX1PATH ] ;

char propbuff [ MAX1PATH ] ;
char pathbuff [ MAX1PATH ] ;
char epocbuff [1024] ;
char banbuf [ 128 ] ;
char cmdbuf [ 4 * PATH_MAX ] ;
char sylbuf [ DFL_SYLSIZE ] ;

FILE * lfp = NULL ;

time_t sotloc ;
time_t eotloc ;

TREM_LEAF * trem_bush = NULL ;

struct stat * tempstap = NULL ;
/*________________________________________________________________________
*/
void tmlog ( char * tag , char * txt ) {
	printf ( "%s %s\n" , tag , txt ) ;
	if ( lfp != NULL )
		fprintf ( lfp , "%s %s\n" , tag , txt ) ;
}

int tmfatal ( int eno , char * tag , char * obj ) {
	fprintf ( lfp == NULL ? stderr : lfp , "tm: error %d on %s ( %s ) \n" , eno , tag , obj ) ;
	exit ( eno ) ;
	return grd ;
}

TREM_LEAF * tmquery ( const char * name ) {
	TREM_LEAF * tlp ;
	int i ;

	for ( i = 0 , tlp = trem_bush ; i < leafcount ; ++i , ++tlp )
		if ( 0 == strcmp ( name , tlp->tl_name ) )
			return tlp ;
	return NULL ;
}

struct stat * statdup ( const struct stat * stap ) {
	struct stat * tsp ;

	tsp = (struct stat *) malloc ( sizeof(struct stat) ) ;

	if ( tsp == NULL )
		tmfatal ( errno , "malloc" , "stat" ) ;

	memcpy ( tsp , stap , sizeof(struct stat) ) ;

	return tsp ;
}

struct FTW * ftwdup ( struct FTW * ftwp ) {
	struct FTW * tfp ;

	tfp = (struct FTW *) malloc ( sizeof(struct FTW) ) ;

	if ( tfp == NULL )
		tmfatal ( errno , "malloc" , "FTW" ) ;

	memcpy ( tfp , ftwp , sizeof(struct FTW) ) ;

	return tfp ;
}

TREM_LEAF * tmplant ( const char * name , const struct stat * stap , struct FTW * ftwp ) {

	TREM_LEAF * tlp ;

	tlp = tmquery ( name ) ;

	if ( tlp == NULL ) {

		tlp = trem_bush + leafcount ;
		++leafcount ;

		tlp->tl_name = strdup ( name ) ;
		tlp->tl_flag |= tmstate ;

		if ( tmstate == TREM_PRUNE ) {
			tlp->tl_mist = statdup ( stap ) ;
			tlp->tl_mitb = ftwdup ( ftwp ) ;
		} else {
			tlp->tl_mast = statdup ( stap ) ;
			tlp->tl_matb = ftwdup ( ftwp ) ;
		}

	} else {

		if ( tmstate == TREM_GRAFT ) {
			tlp->tl_flag |= tmstate ;
			tlp->tl_mast = statdup ( stap )  ;
			tlp->tl_matb = ftwdup ( ftwp ) ;
		} else {
			tmfatal ( -1 , "loop" , (char *)name ) ;
		}

	}

	return tlp ;
}

int tremplow ( char * name ) {

	char * sp ;

	_STR_CPY_ ( pathbuff , name ) ;
	sp = strrchr ( pathbuff , '/' ) ;

	if ( sp != NULL ) {
		*(sp+1) = '\0' ;
	}

	if ( access ( pathbuff , F_OK ) != 0 ) {
		if ( systemflag ) {
			sprintf ( cmdbuf , "mkdir -p  \"%s\"" , pathbuff ) ;
			if ( verboseflag >= 3 ) {
				tmlog ( "+" , cmdbuf ) ;
			}
			if ( mockflag ) {
				return 0 ;
			} else {
				grd = system ( cmdbuf ) ;
			}
		}
	}

	return grd ;
}

int tmdup ( const char * name , char * dupname , const struct stat * stap , int objtyp ) {

	char * tmpdup = masterleaf ;
	char * cmd = "ls -ld" ;
	char dqt = '"' ;
	int rd ;

	if ( tremplow ( dupname ) != 0 ) {
		return grd ;
	}

	++graftcount ; graftbytes += stap->st_size ;

	_STR_CPY_ ( &masterleaf[lenmasterfull] , name+1 ) ;

	if ( objtyp == FTW_F ) {
		cmd = "cp -p" ;
		++graftfilcount ;
	} else {
		if ( objtyp == FTW_D ) {
			cmd = "mkdir" ;
			++graftdircount ;
			tmpdup = " " ;
			dqt = ' ' ;
		} else {
			if ( objtyp == FTW_SL ) {
				rd = readlink ( tmpdup , sylbuf , sylsiz ) ;
				if ( rd > 0 ) {
					cmd = "ln -s" ;
					++graftsylcount ;
					if ( rd < DFL_SYLSIZE ) {
						sylbuf[rd] = '\0' ;
					} else {
						sylbuf[DFL_SYLSIZE - 1] = '\0' ;
					}
					tmpdup = sylbuf ;
				} else {
					cmd = "ls -ld" ;
					dqt = '?' ;
				}
			} else {
				cmd = "ls -ld" ;
				++graftanycount ;
			}
		}
	}

	if ( systemflag ) {
		sprintf ( cmdbuf , "%s %c%s%c \"%s\"" , cmd , dqt , tmpdup , dqt , dupname ) ;
		tmlog ( "+" , cmdbuf ) ;
		if ( mockflag ) {
			return 0 ;
		} else {
			grd = system ( cmdbuf ) ;
		}
	} else {
		printf ( "+++ create \"%s\"" , dupname ) ;
	}

	return grd ;
}

int tmdel ( const char * name , const struct stat * stap , int objtyp ) {

	char * cmd = "rm -i" ;

	prunebytes += stap->st_size ;
	++prunecount ;

	if ( objtyp == FTW_F ) {
		cmd = "rm -f" ;
		++prunefilcount ;
	} else if ( objtyp == FTW_D || objtyp == FTW_DP ) {
		cmd = "rmdir" ;
		++prunedircount ;
	} else if ( objtyp == FTW_SL ) {
		cmd = "unlink" ;
		++prunesylcount ;
	} else {
		cmd = "unlink" ;
		++pruneanycount ;
	}

	_STR_CPY_ ( &mimicleaf[lenmimicfull] , name+1 ) ;

	if ( systemflag ) {
		sprintf ( cmdbuf , "%s \"%s\"" , cmd , mimicleaf ) ;
		tmlog ( "~" , cmdbuf ) ;
		if ( mockflag ) {
			return 0 ;
		} else {
			grd = system ( cmdbuf ) ;
		}
	} else {
		printf ( "--- delete \"%s\"" , mimicleaf ) ;
	}

	return grd ;
}

int ftwprune ( const char * namepath , const struct stat * statbuff , int objtyp , struct FTW * ftwbuf ) {

	_STR_CPY_ ( &masterleaf[lenmasterfull] , namepath+1 ) ;

		if ( access ( masterleaf , F_OK ) == 0 ) {
			if ( weighflag ) {
				tmplant ( namepath , statbuff , ftwbuf ) ;
			}
		} else {
			if ( pruneflag ) {
				tmdel ( namepath , statbuff , objtyp ) ;
			}
		}

	return 0 ;
}

int ftwgraft ( const char * namepath , const struct stat * statbuff , int objtyp , struct FTW * ftwbuf ) {

	_STR_CPY_ ( &mimicleaf[lenmimicfull] , namepath+1 ) ;

		if ( access ( mimicleaf , F_OK ) == 0 ) {
			if ( weighflag ) {
				tmplant ( namepath , statbuff , ftwbuf ) ;
			}
		} else {
			if ( graftflag ) {
				tmdup ( namepath , mimicleaf , statbuff , objtyp ) ;
			}
		}

	return 0 ;
}

int tmgwd ( char * buf , int siz ) {

	if ( getcwd ( buf , siz ) == NULL )
		tmfatal ( errno , "getcwd" , "void" ) ;

	return grd ;
}

int tmcwd ( char * whereto ) {

	if ( chdir (whereto) != 0 )
		tmfatal ( errno , "chdir" , whereto ) ;

	return grd ;
}

int tmquit ( void ) {

	time ( &eotloc ) ;
/*
	tmlog ( "e_n_d @" , ctime ( &eotloc ) ) ;
*/

	if ( lfp != NULL )
		fclose (lfp) ;

	return grd ;
}

int tmhelp ( void ) {
	tmlog ("!", HELPMESSAGE) ;
	tmquit () ;
	exit (1) ;
	return grd ;
}

void tmint ( int signo ) {
	signal (SIGINT, SIG_IGN) ;
	tmlog ("signal =", "INT") ;
	tmquit () ;
	exit (signo) ;
}

int tminit ( void ) {

	logfile = strdup (DFL_LOGPATH) ;
	lfp = fopen ( logfile , "a" ) ;

	if ( versionflag ) {
		sprintf ( banbuf , "%s %s%s %s (%s) %s\n" , SWNAME , SWVERS,SWFORG , SWDATE , SWCOPY , SWAUTH ) ;
		tmlog ( "\n*" , banbuf ) ;
		exit (0) ;
	}

	time ( &sotloc ) ;
/*
	tmlog ( "start @" , ctime ( &sotloc ) ) ;
*/

/*________________________________________________________________________
*/

	if ( usageflag )
		++helpflag ;

	if ( mastertop == NULL || mimictop == NULL )
		++helpflag ;

	if ( helpflag )
		tmhelp () ;

	if ( ! ( pruneflag || graftflag || weighflag ) ) {
		pruneflag = graftflag = weighflag = 1 ;
	}

	if ( ! ( knackflag || systemflag ) )
		++systemflag ;

	if ( ! ( freshflag || contentflag ) )
		++freshflag ;

	if ( freshflag )
		contentflag = 0 ;

	if ( contentflag )
		freshflag = 0 ;

	if ( commitflag )
		mockflag = 0 ;

/*__________________________________________________________
*/

	tmgwd (workfull, MAX1PATH) ;

	tmcwd (mastertop) ;
	tmgwd (masterfull, MAX1PATH) ;
	lenmasterfull = strlen (masterfull) ;

	if ( verboseflag >= 3 )
		tmlog ("master =", masterfull) ;

	tmcwd (workfull) ;

	tmcwd (mimictop) ;
	tmgwd (mimicfull, MAX1PATH) ;
	lenmimicfull = strlen (mimicfull) ;

	if ( verboseflag >= 3 )
		tmlog ("mimic =", mimicfull) ;

	_STR_CPY_ ( mimicleaf , mimicfull ) ;
	_STR_CPY_ ( masterleaf , masterfull ) ;

/*__________________________________________________________
*/

	trem_bush = (TREM_LEAF *) malloc ( maxleaf * TREM_LEAFSIZE ) ;

	if ( trem_bush == NULL )
		tmfatal ( errno , "malloc" , "leaf" ) ;

	signal (SIGINT, tmint) ;

	return grd ;
}

char * tmepoch ( time_t tloc ) {
	struct tm * tp = localtime (&tloc) ;
	sprintf (epocbuff, "%04d/%02d/%02d+%02d:%02d:%02d", 1900 + tp->tm_year, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec) ;
	return epocbuff ;
}

char * tmprops ( TREM_LEAF * tlp ) {
	sprintf ( propbuff , "%06lo %9ld %s %s", (long)tlp->tl_mast->st_mode, (long)tlp->tl_mast->st_size, tmepoch(tlp->tl_mast->st_mtime), tlp->tl_name ) ;

	return propbuff ;
}

int tmcopy ( TREM_LEAF * tlp ) {

	_STR_CPY_ ( &masterleaf[lenmasterfull] , tlp->tl_name+1 ) ;
	_STR_CPY_ ( &mimicleaf[lenmimicfull] , tlp->tl_name+1 ) ;

	weighbytes += ( tlp->tl_mast->st_size - tlp->tl_mist->st_size ) ;

	++copycount ;

	if ( lsflag ) {
		sprintf ( cmdbuf , "ls -lrtd \"%s\" \"%s\"" , masterleaf , mimicleaf ) ;
		tmlog ( ">" , cmdbuf ) ;
		grd = system ( cmdbuf ) ;
	}

	if ( systemflag ) {
		sprintf ( cmdbuf , "cp -p \"%s\" \"%s\"" , masterleaf , mimicleaf ) ;
		tmlog ( ">" , cmdbuf ) ;
		if ( mockflag )
			return 0 ;
		else
			grd = system ( cmdbuf ) ;
	}

	return grd ;
}

int tmperm ( TREM_LEAF * tlp ) {

	_STR_CPY_ ( &masterleaf[lenmasterfull] , tlp->tl_name+1 ) ;
	_STR_CPY_ ( &mimicleaf[lenmimicfull] , tlp->tl_name+1 ) ;

	++permcount ;

	if ( systemflag ) {
		sprintf ( cmdbuf , "chmod `stat -c %%a \"%s\"` \"%s\"" , masterleaf , mimicleaf ) ;
		tmlog ( ">" , cmdbuf ) ;
		if ( mockflag )
			return 0 ;
		else
			grd = system ( cmdbuf ) ;
	}

	return grd ;
}

#if 0
int tmtouch ( TREM_LEAF * tlpa , TREM_LEAF * tlpb ) {

	if ( systemflag ) {
		sprintf ( cmdbuf , "touch -r \"%s\" \"%s\"" , tlpa->tl_name , tlpb->tl_name ) ;
		tmlog ( ">" , cmdbuf ) ;
		if ( mockflag )
			return 0 ;
		else
			grd = system ( cmdbuf ) ;
	}

	return grd ;
}
#endif

int gistcmp ( TREM_LEAF * tlp ) {

	_STR_CPY_ ( &masterleaf[lenmasterfull] , tlp->tl_name+1 ) ;
	_STR_CPY_ ( &mimicleaf[lenmimicfull] , tlp->tl_name+1 ) ;

	if ( systemflag ) {
		sprintf ( cmdbuf , "cmp \"%s\" \"%s\"" , masterleaf , mimicleaf ) ;
		tmlog ( "?" , cmdbuf ) ;
		if ( mockflag ) {
			return 0 ;
		} else {
			grd = system ( cmdbuf ) ;
		}
	}

	return grd ;
}

void tmrep ( char * what , int count , off_t bytes ) {
	static char repbuf [128] ;

	sprintf ( repbuf , ": %lld bytes in %d files\n" , (long long)bytes , count ) ;
	tmlog ( what , repbuf ) ;
}

void tmweigh ( void ) {
	TREM_LEAF * tlp ;
	int i ;

	if ( ! weighflag ) {
		return ;
	}

	if ( verboseflag >= 1 ) {
		tmlog ( "\nphase III" , "weigh\n" ) ;
	}

	tmstate = TREM_WEIGH ;

	for ( i = 0 , tlp = trem_bush ; i < leafcount ; ++i , ++tlp ) {

		/* update older */
		if ( freshflag ) {
/*			if ( tlp->tl_mast == NULL || tlp->tl_mist == NULL ) {		*/
/*				printf ("*** incomplete(%s)\n", tlp->tl_name) ;			*/
/*			}															*/
			if ( ( tlp->tl_mast->st_mode & S_IFMT ) == S_IFDIR ) {
				continue ;
			}
			if ( ( tlp->tl_mast->st_mode & S_IFMT ) == S_IFLNK ) {
				continue ;
			}
			if ( tlp->tl_mast->st_mtime > tlp->tl_mist->st_mtime ) {
				if ( verboseflag >= 3 ) {
					tmlog ( ">newer =" , tmprops ( tlp ) ) ;
				}
				tmcopy ( tlp ) ;
			}
		}

		/* uneven modes */
		if ( modeflag ) {
			if ( tlp->tl_mast->st_mode != tlp->tl_mist->st_mode ) {
				if ( verboseflag >= 3 ) {
					tmlog ( "#modes =" , tmprops ( tlp ) ) ;
				}
				tmperm ( tlp ) ;
			}
		}

		/* update uneven */
		if ( contentflag ) {
			if ( ( tlp->tl_mast->st_size != tlp->tl_mist->st_size ) || ( gistcmp ( tlp ) != 0 ) ) {
				if ( verboseflag >= 3 ) {
					tmlog ( "#stuff =" , tmprops ( tlp ) ) ;
				}
				tmcopy ( tlp ) ;
			}
		}
	}

	if ( verboseflag >= 1 ) {
		tmrep ( "\nphase III weigh" , copycount , weighbytes ) ;
	}
}

void tmgraft ( void ) {

	if ( graftflag ) {
		if ( verboseflag >= 1 ) {
			tmlog ( "\nphase II" , "graft\n" ) ;
		}
	} else {
		if ( ! weighflag ) {
			return ;
		}
	}

	tmstate = TREM_GRAFT ;

	tmcwd (workfull) ;
	tmcwd (masterfull) ;

	grd += nftw (".", ftwgraft, maxdepth, FTW_PHYS) ;

	if ( graftflag )
		if ( verboseflag >= 1 )
			tmrep ( "\nphase II graft" , graftcount , graftbytes ) ;
}

void tmprune ( void ) {

	if ( pruneflag ) {
		if ( verboseflag >= 1 ) {
			tmlog ( "\nphase I" , "prune\n" ) ;
		}
	} else {
		if ( ! weighflag ) {
			return ;
		}
	}

	tmstate = TREM_PRUNE ;

	tmcwd (workfull) ;
	tmcwd (mimicfull) ;

	grd += nftw (".", ftwprune, maxdepth, FTW_PHYS|FTW_DEPTH) ;

	if ( pruneflag )
		if ( verboseflag >= 1 )
			tmrep ( "\nphase I prune" , prunecount , prunebytes ) ;
}

int tm ( void ) {

	tminit () ;

	tmprune () ;

	tmgraft () ;

	if ( pruneflag && graftflag )
		if ( verboseflag >= 2 )
			tmrep ( "\nresidual" , graftcount - prunecount , graftbytes - prunebytes ) ;

	tmweigh () ;

	tmquit () ;

	return grd ;
}
/*________________________________________________________________________
*/
int main ( int argc , char * * argv ) {
	if ( --argc ) {
		while ( *++argv ) {
			if ( 0 == strcmp ( *argv , "-?" ) ) {
				++helpflag ;
			} else if ( 0 == strcmp ( *argv , "-a" ) ) {
				++modeflag ;
			} else if ( 0 == strcmp ( *argv , "-d" ) ) {
				++diffflag ;
			} else if ( 0 == strcmp ( *argv , "-h" ) ) {
				++helpflag ;
			} else if ( 0 == strcmp ( *argv , "-n" ) ) {
				++mockflag ;
			} else if ( 0 == strcmp ( *argv , "-k" ) ) {
				++knackflag ;
			} else if ( 0 == strcmp ( *argv , "-l" ) ) {
				++lsflag ;
			} else if ( 0 == strcmp ( *argv , "-x" ) ) {
				++matchflag ;
			} else if ( 0 == strcmp ( *argv , "-s" ) ) {
				++systemflag ;
			} else if ( 0 == strcmp ( *argv , "-p" ) ) {
				++pruneflag ;
			} else if ( 0 == strcmp ( *argv , "-g" ) ) {
				++graftflag ;
			} else if ( 0 == strcmp ( *argv , "-w" ) ) {
				++weighflag ;
			} else if ( 0 == strcmp ( *argv , "-c" ) ) {
				++contentflag ;
			} else if ( 0 == strcmp ( *argv , "-f" ) ) {
				++freshflag ;
			} else if ( 0 == strcmp ( *argv , "-V" ) ) {
				++versionflag ;
			} else if ( 0 == strcmp ( *argv , "-v" ) ) {
				++verboseflag ;
			} else if ( 0 == strcmp ( *argv , "-C" ) ) {
				++commitflag ;
			} else if ( 0 == strcmp ( *argv , "-M" ) ) {
				mastertop = strdup ( *++argv ) ;
			} else if ( 0 == strcmp ( *argv , "-m" ) ) {
				mimictop = strdup ( *++argv ) ;
			} else if ( 0 == strcmp ( *argv , "-z" ) ) {
				++zestflag ;
			} else {
				++usageflag ;
			}
		}
	}
	tm () ;
	return grd ;
}
/*________________________________________________________________________
*/

/*
 * vi:nu ts=4
 */
