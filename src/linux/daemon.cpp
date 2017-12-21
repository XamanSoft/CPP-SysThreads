#include <CppSystemRT.hpp>

namespace CppSystemRT {
	
namespace Daemon {

static int pid_fd{-1};	
static char *pid_file_name{NULL};
static bool running{false};

void process(ThreadCreator* tc);

void _handleSignal(int sig);

int exec(ThreadCreator* tc) {
	// Process ID and Session ID
	pid_t pid, sid;
	int fd;
	
	/* Daemon will handle two signals */
	signal(SIGINT, _handleSignal);
	signal(SIGHUP, _handleSignal);

	// Forking the parent process
	// fork return the process id of the child process
	pid = fork();
	if (pid < 0) {
		return EXIT_FAILURE;
	}

	// setting the umask to 0 in order to have full access to the files generated by the daemon
	umask(0);

	// Creating a new SID for the child process
	// The child process must get a unique SID from the kernel in order to operate
	sid = setsid();
	if (sid < 0) {
		return EXIT_FAILURE;
	}

	// Changing the current working directory
	if ((chdir("/")) < 0) {
		return EXIT_FAILURE;
	}
	
	// Close all open file descriptors
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

	// Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2)
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");

	// calling process function
	process(tc);

	return EXIT_SUCCESS;
}

void process(ThreadCreator* tc) {
	if (tc == nullptr) return;
	std::unique_ptr<Thread> worker(tc->create());

	while (worker && running) {		
		worker->run();
		usleep(1000);
	}
}


void _handleSignal(int sig)
{
	if (sig == SIGINT) {
		//fprintf(log_stream, "Debug: stopping daemon ...\n");
		/* Unlock and close lockfile */
		if (pid_fd != -1) {
			lockf(pid_fd, F_ULOCK, 0);
			close(pid_fd);
		}
		/* Try to delete lockfile */
		if (pid_file_name != NULL) {
			unlink(pid_file_name);
		}
		running = false;
		/* Reset signal handling to default behavior */
		signal(SIGINT, SIG_DFL);
	} else if (sig == SIGHUP) {
		//fprintf(log_stream, "Debug: reloading daemon config file ...\n");
		//read_conf_file(1);
	} else if (sig == SIGCHLD) {
		//fprintf(log_stream, "Debug: received SIGCHLD signal\n");
	}
}

}

}
