#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

class Peripherals {
public:
	Peripherals() {};
	virtual ~Peripherals() {};
	
	virtual int init() {return 0;};
	virtual int close() {return 0;};
};

#endif /* __PERIPHERALS_H__ */
