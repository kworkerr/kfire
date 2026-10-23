#define _DEFAULT_SOURCE

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

static volatile sig_atomic_t interrupted = 0;

void print_help(void)
{
    printf("kfire - Simple fire written in C.\n\n");
    printf("--intensity N   flame intensity (default: 3.0)\n");
    printf("--color N       fire color, from 1 to 8\n");
}

static void handle_sigint(int sig)
{
	(void)sig;
	interrupted = 1;
}

int main(int argc, char **argv)
{
	int colors[] = {
		0, 52, 88, 124, 160,
		196, 202, 208, 214, 231
	};

	const char chars[] = " .:-=+*#%@";

	double intensity = 3.0;
	char *end;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--intensity") == 0) {
			if (i + 1 >= argc) {	
				fprintf(stderr, 
						"kfire: '--intensity' requires a value\n");
				return 1;
			}

			char *value = argv[++i];
			
			errno = 0;
			intensity = strtod(value, &end);
		
			if (*end != '\0' || errno == ERANGE || !isfinite(intensity) || intensity <= 0.0) {
				fprintf(stderr,
						"kfire: intensity must be a number greater than 0\n");
				return 1;
			}
		}


		else if (strcmp(argv[i], "--color") == 0) {
			if (i + 1 >= argc) {	
				fprintf(stderr, 
						"kfire: '--color' requires a value\n");
				return 1;
			}

			char *value = argv[++i];
			
			errno = 0;
			int color = strtol(value, &end, 10);
		
			if (*end != '\0' || errno == ERANGE || color < 1 || color > 8 ) {
				fprintf(stderr,
						"kfire: color must be a number between 1 and 8\n");
				return 1;
			}

		if (color == 1)
			memcpy(colors, (int[]){232, 233, 234, 236, 238, 240, 244, 248, 252, 255}, sizeof colors);
		else if (color == 2)
			memcpy(colors, (int[]){0, 52, 88, 124, 160, 196, 203, 210, 217, 231}, sizeof colors);
		else if (color == 3)
			memcpy(colors, (int[]){0, 22, 28, 34, 40, 46, 82, 120, 157, 231}, sizeof colors);
		else if (color == 4)
			memcpy(colors, (int[]){0, 58, 94, 130, 166, 202, 208, 214, 220, 231}, sizeof colors);
		else if (color == 5)
			memcpy(colors, (int[]){0, 17, 18, 19, 20, 21, 27, 33, 39, 195}, sizeof colors);
		else if (color == 6)
			memcpy(colors, (int[]){0, 53, 89, 125, 161, 165, 201, 207, 213, 225}, sizeof colors);
		else if (color == 7)
			memcpy(colors, (int[]){0, 23, 30, 36, 37, 43, 44, 87, 123, 195}, sizeof colors);
		else if (color == 8)
			memcpy(colors, (int[]){232, 233, 235, 237, 239, 242, 245, 248, 252, 255}, sizeof colors);
		}

		else {
			print_help();
			return 1;
		}
	}

	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		perror("cfire: ioctl");
		return 1;
	}

	int H = ws.ws_row;
	int W = ws.ws_col;

	if (W < 1 || H < 2)
	{
		fprintf(stderr, "kfire: terminal is too small\n");
		return 1;
	}

	double (*fire)[W] = calloc((size_t)H, sizeof *fire);
	
	if (fire == NULL)
	{
		perror("kfire: calloc");
		return 1;
	}
	
	srand(time(NULL));
	
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		perror("kfire: signal");
		free(fire);
		return 1;
	}

	printf("\033[2J\033[H\033[?25l\033[?7l");
	fflush(stdout);

	while (!interrupted)
	{	
		printf("\033[H");
		
		for (int x = 0; x < ws.ws_col; x++)
			fire[H - 1][x] = 9;

		for (int y = H - 2; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				double cooling = fmod(rand(), 3.0) * 3.0 / intensity;

				double heat = fire[y + 1][x] - cooling;

				if (heat < 0.0)
					heat = 0.0;
				
				fire[y][x] = heat;
			}
		}


		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				int heat = (int)fire[y][x];
				
				if (heat < 0)
					heat = 0;
				else if (heat > 9)
					heat = 0;

				printf("\033[38;5;%dm%c", 
						colors[heat], chars[heat]);
			}

			if (y < H - 1)
				putchar('\n');
		}
		
		fflush(stdout);
		usleep(100000);
	}

    printf("\033[0m\033[?7h\033[?25h\n");
	fflush(stdout);
	
	free(fire);
	
	return 0;
}
