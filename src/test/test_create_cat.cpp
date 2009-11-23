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
#include "test-common.h"
#include "testcode.h"
#include "configmanager.h"
#pragma endregion

#pragma region Application includes: GUI
#include <QtGui/QInputDialog>
#include <QtGui/QLineEdit>
#include "mainwindow.h"
#pragma endregion


#define DB_FILE "data/home2.sqlite"

void test_createDB();

class test_create_cat : public QObject
{
     Q_OBJECT
	
	private:
		QString sampleDBfilename;
		QString sampleDBpath;
		QTemporaryFile *tmpfile;

	private slots:
		void initTestCase() {
#if 0
			sampleDBfilename = "qcat_ut-" + QUuid::createUuid().toString() + ".qcat";
			sampleDBpath = QDir::home().absolutePath() + "/" + sampleDBfilename;
#else
			tmpfile = new QTemporaryFile(QDir::tempPath() + "/qcat_ut-XXXXXX.qcat");
			QVERIFY(tmpfile != 0);
			QVERIFY(tmpfile->open() != false);
			sampleDBpath = tmpfile->fileName();
			sampleDBfilename = QFileInfo(sampleDBpath).fileName();
#endif
		}

		void cleanupTestCase() {
			if (tmpfile)
			{
				tmpfile->close();
				// normally unlinks the database, unless it was not properly closed.
				delete tmpfile;
				QTest::qWait(800);
				QVERIFY(QFile::exists(sampleDBpath) == false);
			}
		}


		void testCreateNamedCatalogs() {
			QStringList params;

			// Check window title and enable Debug mode
			QTestEventList list0;
			list0.addDelay(2000);
			list0.addKeyClick(Qt::Key_F12, Qt::ShiftModifier, 200);
			list0.simulate(qWin);
			QCOMPARE(qWin->windowTitle(), tr(APPNAME));

			// Create New catalog in Debug mode
			params.clear();
			params << sampleDBpath;
			qWin->setDebugParams(params);

			QTestEventList list1;
			list1.addKeyClick(Qt::Key_N, Qt::ControlModifier, 500);
			list1.simulate(qWin);
			QCOMPARE(qWin->windowTitle(), tr("%1[*] - %2").arg(sampleDBfilename).arg(tr(APPNAME)));

			// TODO: perform various tests here

			// Close catalog
			QTestEventList list99;
			list99.addDelay(2000);
			list99.addKeyClick(Qt::Key_W, Qt::ControlModifier, 2500);
			list99.simulate(qWin);
			QCOMPARE(qWin->windowTitle(), tr(APPNAME));

		}

		void test_createDB(){
			//QCatDataModuleClass::createDB(DB_FILE, true);
		}
};


QTTESTUTIL_REGISTER_TEST(test_create_cat);
#include "test_create_cat.moc"

