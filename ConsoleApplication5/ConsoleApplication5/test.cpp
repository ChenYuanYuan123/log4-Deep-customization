#define _LOG4CPP_

#include<iostream>
#include "log4cpp/Logger_CYY.hh"

using namespace std;

int main(void) {
	//Mylog& log = Mylog::getInstance();
	//log.setPriority(ERROR);
	LOGGER()->Error_f("Hello  Log!!!");
	return 0;
}