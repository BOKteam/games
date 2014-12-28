/*
 * Tool.h
 *
 *  Created on: Oct 11, 2012
 *      Author: root
 */

#ifndef TOOL_H_
#define TOOL_H_

#include "AndroidAI.h"

class Tool
{
	BYTE m_cbCardData[FULL_COUNT];
	static Tool* m_Instance;

	Tool();
public:
	static Tool* Singleton();
	int GetCardID(BYTE Num);
};
#endif /* TOOL_H_ */
