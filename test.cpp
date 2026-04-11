#include <iostream>
# include <sys/socket.h>
#define CRLF "\r\n"

void	sendCodes(const int &fd, std::string code, const std::string &prefix, const::std::string &msg)
{
	std::string completeMsg = prefix + " " + code + " " + msg + CRLF;
	std::cout << completeMsg << std::endl;
	send(fd, completeMsg.c_str(), completeMsg.size(), 0);
}

int main()
{
		sendCodes(3, "331", ":server", "client.getNick() destChan.getName :No topic is set");
}
