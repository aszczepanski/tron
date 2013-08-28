#ifndef APPLICATION_H
#define APPLICATION_H

namespace application
{

class Application
{
public:
	Application();
	void run();
private:
	void clientApplication();
	void serverApplication();
};

}

#endif // APPLICATION_H
