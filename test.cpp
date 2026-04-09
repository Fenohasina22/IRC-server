#include <iostream>
# include <sys/socket.h>
#define CRLN "\r\n"

void	sendCodes(const int &fd, std::string code, const std::string &prefix, const::std::string &msg)
{
	std::string completeMsg = prefix + " " + code + " " + msg + CRLN;
	std::cout << completeMsg << std::endl;
	send(fd, completeMsg.c_str(), completeMsg.size(), 0);
}

int main()
{
		sendCodes(3, "401",":server", "sender destination :No such nick/channel");
}
