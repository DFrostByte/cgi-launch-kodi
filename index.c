#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static const char *path_kodi_ = "/usr/lib/kodi/kodi.bin";
static const char *filename_kodi_ = "kodi.bin";

#define UID_PI 1000
#define GID_PI 1000

static int
_is_running (const char *program)
{
	pid_t pid;
	int exit_status;

	pid = vfork();

	if (pid == -1)
		return 0;
	else if (pid == 0)
	{
		freopen ("/dev/null", "r", stdin);
		freopen ("/dev/null", "w", stdout);
		freopen ("/dev/null", "w", stderr);

		execl("/usr/bin/pgrep", "pgrep", program, NULL);

		_exit(EXIT_FAILURE);
	}

	waitpid(pid, &exit_status, 0);

	if (exit_status) /* abnormal exit */
		return 0;

	return 1;
}


static int
_kodi_start ()
{
	pid_t pid;
	int status = 0;

	pid = vfork();

	if (pid == -1)
		return 0;
	else if (pid == 0)
	{
		chdir ("/tmp");

		setsid ();
		setgid (GID_PI);
		setuid (UID_PI);

		putenv ("HOME=/home/pi");
		putenv ("USER=pi");

		freopen ("/dev/null", "r", stdin);
		freopen ("/dev/null", "w", stdout);
		freopen ("/dev/null", "w", stderr);

		execl(path_kodi_, filename_kodi_, "--standalone", NULL);

		_exit(1);
	}

	waitpid (pid, &status, WNOHANG);

	if (status)
		return 0;

	return 1;
}

int
main (int argc, char *argv[])
{
	puts ("Content-type: text/html\r\n\r\n"
	      "<!DOCTYPE html>\n"
		  "<html>\n"
	      "<head>\n"
		  "<title>RPi: TV (Kodi)</title>\n"
		  "</head>\n\n"
		  "<body>\n"
		  "<article>\n"
		  "<h1>RPi: TV (Kodi)</h1>");

	if (_is_running ("kodi"))
	{
		puts ("<p><em>Kodi</em> is <strong>already running</strong>. "
		      "Switch TV channel to view.</p>\n");
	}
	else
	{
		if (_kodi_start ())
			puts ("<p><em>Kodi</em> has been <strong>started</strong>. "
			      "TV should now switch automatically.</p>\n");
		else
			puts ("<p><em>Kodi</em> <strong>failed to start</strong>.</p>\n");
	}

	puts ("</article>\n"
	      "</body>\n"
	      "</html>\n");

	return 0;
}





