/*
 * ACException.h
 *
 *  Created on: 2015年9月22日
 *      Author: qiang.liu
 */
#include <string>

#ifndef ACEXCEPTION_H_
#define ACEXCEPTION_H_

//未指定代码的错误
#ifndef EXP_UNDEFINED
#define EXP_UNDEFINED 0
#endif

//MAT值为空。
#ifndef EXP_NULL_MAT
#define EXP_NULL_MAT 1
#endif

//只接受单通道MAT数据
#ifndef EXP_ONLY_ONE_CHANNEL_ACCEPT
#define EXP_ONLY_ONE_CHANNEL_ACCEPT 2
#endif

namespace ac {

using std::string;

class ACException {

private:
	int errCode;
	string errmessage;

	ACException();

public:
	ACException(int _exceptionCode);
	ACException(string errmsg);
	virtual ~ACException();

	int getID();
	string info();
};

} /* namespace ac */

#endif /* ACEXCEPTION_H_ */
