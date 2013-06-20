#ifndef APPS_H_INCLUDED
#define APPS_H_INCLUDED

int setkb_main(int argc, char *argv[]);				// setkb.c
int shell_main(int argc, char *argv[]);				// shell.c
int simple_phil_main(int argc, char *argv[]);		// sfilo.c
int phil_main(int argc, char *argv[]);				// filo.c
int extra_phil_main(int argc, char *argv[]);		// xfilo.c
int atomic_phil_main(int argc, char *argv[]);		// afilo.c
int camino_main(int argc, char *argv[]);			// camino.c
int camino_ns_main(int argc, char *argv[]);			// camino_ns.c
int prodcons_main(int argc, char *argv[]);			// prodcons.c
int divz_main(int argc, char *argv[]);				// divz.c
int chkdsk_main(int argc, char *argv);				// chkdsk.c
int diskwrite_main(int argc, char *argv);			// diskwrite.c
int diskread_main(int argc, char *argv);			// diskread.c
int disksize_main(int argc, char *argv); 			// disksize.c

#endif
