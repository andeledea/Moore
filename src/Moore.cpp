#include "Moore.h"
#include "Asse.h"

void Moore::setCurrentPosition(float x, float y, float z)
{
	xt = std::thread(Xaxis.setPosition, x);
	yt = std::thread(Yaxis.setPosition, y);
	zt = std::thread(Zaxis.setPosition, z);

	xt.join();
	yt.join();
	zt.join();
}

void Moore::setAxes(Asse x, Asse y, Asse z)
{
	Xaxis = x;
	Yaxis = y;
	Zaxis = z;
}

void Moore::startProcess()
{
	 t = std::thread(&Moore::process, this);
}

void Moore::stopProcess()
{
	t.detach();
}

void Moore::process()
{
	while (true)
	{
		currentPos.x = Xaxis.getPosition();
		currentPos.y = Yaxis.getPosition();
		currentPos.z = Zaxis.getPosition();
	}
}
