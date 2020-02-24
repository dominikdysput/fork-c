#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void Print(char*argv1, char*argv2)
{

	if (argv2 != NULL)
	{
		printf("%d %s %s\n", getpid(), argv2, argv1);
	}
	else
	{
		printf("%d %s\n", getpid(), argv2);
	}
}
int Rounding(int value)
{
	int sh = 1;
	while (sh <= value)
	{
		sh = sh * 2;
		if ((sh * 2) > value)
			break;
	}
	return sh;
}
int IsPowerOfTwo(int value)
{
	if (value == 0)
		return 0;
	while (value != 1)
	{
		if (value % 2 != 0)
			return 0;
		value = value / 2;
	}
	return value;
}
char *Save(char *argv1, char *argv2)
{
	char *temp;
	if (argv2 == NULL)
	{
		asprintf(&temp, "%s ", argv1);
	}
	else
	{
		asprintf(&temp, "%s %s", argv2, argv1);
	}
	return temp;
}

int main(int argc, char **argv)
{
	//brak argumentu
	if (argc == 1)
	{
		printf("Brak argumentu \n");
		return -1;
	}
	size_t a = strlen(argv[1]);
	char *help_tab;
	if (!IsPowerOfTwo(a))
	{
		a = Rounding(a);
		help_tab = malloc((a + 1) * sizeof(char));
		strcpy(help_tab, argv[1]);
		help_tab[a] = '\0';
	}
	else
	{
		help_tab = malloc((a + 1) * sizeof(char));
		strcpy(help_tab, argv[1]);
		help_tab[a] = '\0';
	}
	if ((argc == 2) && (a == 1))
	{
		free(help_tab);
		help_tab = NULL;
		pid_t help_id;
		help_id = fork();
		if (help_id == 0)
		{
			printf("%d %s\n", getpid(), argv[1]);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
	char*help_tab_1 = malloc((a / 2 + 1) * sizeof(char));
	char*help_tab_2 = malloc((a / 2 + 1) * sizeof(char));
	strncpy(help_tab_1, help_tab, a / 2);
	strncpy(help_tab_2, help_tab + a / 2, a / 2);
	help_tab_1[a / 2] = '\0';
	help_tab_2[a / 2] = '\0';
	if (a == 1)
	{
		Print(help_tab, argv[2]);
		free(help_tab_1);
		free(help_tab_2);
		free(help_tab);
		help_tab_1 = NULL;
		help_tab_2 = NULL;
		help_tab = NULL;
		return 0;
	}
	else
	{
		pid_t ppid_1, ppid_2;
		ppid_1 = fork();
		if (ppid_1 == 0)
		{
			char* zmienna = NULL;
			zmienna = Save(help_tab, argv[2]);
			execlp("./a.out", "a.out", help_tab_1, zmienna, NULL);
			free(zmienna);
			zmienna = NULL;
			exit(0);
		}
		else
		{
			ppid_2 = fork();
			wait(0);
			if (ppid_2 == 0)
			{
				char *zmienna_a = NULL;
				zmienna_a = Save(help_tab, argv[2]);
				execlp("./a.out", "a.out", help_tab_2, zmienna_a, NULL);
				free(zmienna_a);
				zmienna_a = NULL;
				exit(0);
			}
		}
		free(help_tab_1);
		help_tab_1 = NULL;
		free(help_tab_2);
		help_tab_2 = NULL;
		wait(0);
		if (argv[2] == NULL)
		{
			printf("%d %s\n", getpid(), help_tab);
			free(help_tab);
			help_tab = NULL;
		}
		else
		{
			free(help_tab_1);
			help_tab_1 = NULL;
			free(help_tab_2);
			help_tab_2 = NULL;
			Print(help_tab, argv[2]);
			free(help_tab);
			help_tab = NULL;
		}
		return 0;
	}
}