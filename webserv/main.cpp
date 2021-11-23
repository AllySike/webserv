#include "MultiClientChat.h"

int main()
{
	int ret;
	MultiClientChat server("0.0.0.0", 54000);

	ret = server.init_listener();
	if (ret)
		return (ret);
	ret = server.run_listener();
	if (ret)
		return (ret);
	system("pause");
	return (0);
}