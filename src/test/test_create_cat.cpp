#include <QObject>
#include <QtTest/QtTest>

#include "QtTestUtil/QtTestUtil.h"

#include "mainwindow.h"
#include "testcode.h"

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

