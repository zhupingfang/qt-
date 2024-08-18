//
// Created by zhupi on 2024/8/18.
//

#ifndef TEST_H
#define TEST_H

#define QT_NO_KEYWORDS
#include <QObject>
#include <QDebug>


class MyPythonTest : public QObject
{
	Q_OBJECT
public:
	void TestPython();

Q_SIGNALS:
	void DataChangeSg();
};


#endif //TEST_H
