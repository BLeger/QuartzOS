#ifndef QZ_DRIVER_H
#define QZ_DRIVER_H

class Driver
{
public:
	Driver() {};
	~Driver() {};

	virtual void activate() = 0;
	virtual int reset() = 0;
	virtual void deactivate() = 0;
};

class DriverManager
{
public:
	Driver* drivers[265]; // Needs refactor => private
	int numDrivers;

public:
	DriverManager();
	void addDriver(Driver*);

	void activateAll();

};

#endif // !QZ_DRIVER_H
