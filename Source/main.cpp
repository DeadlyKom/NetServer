
#include <Server.h>

int main(int argc, char* argv[])
{
	std::vector<std::string> Args;
	
	if (argc == 1)
	{
		std::cout << "no arguments!" << std::endl;
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			Args.push_back(argv[i]);
		}
	}
	return FServer::Get().Launch(Args);
}
