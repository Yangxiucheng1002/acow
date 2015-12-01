/*
 * ACException.cpp
 *
 *  Created on: 2015年9月22日
 *      Author: qiang.liu
 */

#include "ACException.h"
#include <iostream>

namespace ac {

using namespace std;

ACException::ACException() {
	// TODO Auto-generated constructor stub

}

ACException::ACException(int _exceptionCode) {
	ACException();
	errCode = _exceptionCode;
	info();
}

ACException::~ACException() {
	// TODO Auto-generated destructor stub
}

int ACException::getID() {
	return errCode;
}

ACException::ACException(string errmsg) {
	ACException();
	errCode = EXP_UNDEFINED;
	this->errmessage = errmsg;
	info();
}

string ACException::info() {
	switch(errCode){
	case EXP_UNDEFINED: // 未指定错误码，一定已经指定了错误描述。
	break;

	case EXP_NULL_MAT:
		errmessage = "给定的mat为空。";
		break;
	}

	cout << errmessage << endl;
	return errmessage;
}

} /* namespace ac */
