/*
 *	version.h
 */

# ifndef _VERSION_H_

# define _VERSION_H_

extern const char * trem_program_string ;
extern const char * trem_version_string ;
extern const char * trem_release_string ;
extern const char * trem_relogio_string ;
extern const char * trem_summary_string ;
extern const char * trem_taglist_string ;
extern const char * trem_license_string ;
extern const char * trem_creator_string ;
extern const char * trem_contact_string ;
extern const char * trem_builder_string ;
extern const char * trem_compile_string ;

	/*  legacy  */

# define	SWNAME		trem_program_string
# define	SWVERS		trem_version_string
# define	SWDATE		trem_release_string
# define	SWTIME		trem_relogio_string
# define	SWDESC		trem_summary_string
# define	SWTAGS		trem_taglist_string
# define	SWCOPY		trem_license_string
# define	SWAUTH		trem_creator_string
# define	SWMAIL		trem_contact_string
# define	SWBLDR		trem_builder_string
# define	SWCOMP		trem_compile_string

# define	SWFORG		" " /* deprecated */

# endif /* _VERSION_H_ */

/*
 * vi:nu ts=8
 */
