/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nike@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#pragma region System includes
#include <QObject>
#include <QtTest/QtTest>

#include "QtTestUtil/QtTestUtil.h"
#pragma endregion


#pragma region Application includes: Business Logic
#include "testcode.h"
#pragma endregion

#pragma region Application includes: GUI
#include "mainwindow.h"
#pragma endregion


#define home
#ifdef home
#define DB_FILE "data/home2.sqlite"
#else
#define DB_FILE "data/office2.sqlite"
#endif


void test_createDB();

class test_create_cat : public QObject
{
     Q_OBJECT
	
	private slots:
		void initTestCase() {
		}

		void cleanupTestCase() {
		}


		void testCreateNamedCatalogs_data()
		{
			QTest::addColumn<QTestEventList>("events");
			QTest::addColumn<QString>("expected");

			// add the event for a key click on the widget
			QTestEventList list1;
			list1.addKeyClick('a');
			QTest::newRow("char") << list1 << "a";

			QTestEventList list2;
			list2.addKeyClick('a');
			list2.addKeyClick(Qt::Key_Backspace);
			QTest::newRow("there and back again") << list2 << "";
		}


		void testCreateNamedCatalogs() {
			test_createDB();
			QCOMPARE(1, 1); // Dummy test
		}
};

void test_createDB(){
//	QCatDataModuleClass::createDB(DB_FILE, true);
}


QTTESTUTIL_REGISTER_TEST(test_create_cat);
#include "test_create_cat.moc"

