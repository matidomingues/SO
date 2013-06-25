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
int chkdsk_main(int argc, char *argv[]);			// chkdsk.c
int ls_main(int argc, char *argv[]);				// ls.c
int mkdir_main(int argc, char *argv[]);				// mkdir.c
int rm_main(int argc, char *argv[]);				// rm.c
int cat(int argc, char *argv[]);					// fs.c
int cd_main(int argc, char *argv[]);				// cd.c
int edit(int argc, char *argv[]);					// fs.c
int print_sectors(int argc, char *argv[]);			// fs.c
int format_main(int argc, char *argv[]);			// format.c
int touch_main(int argc, char *argv[]);				// touch.c
int pwd_main(int argc, char *argv[]);				// pwd.c
int detect_cpu(int argc, char *argv[]);				// cpuid.c

#endif
