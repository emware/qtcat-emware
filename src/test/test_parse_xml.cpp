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

#include <QXmlInputSource>
#include <QXmlSimpleReader>
#pragma endregion


#pragma region Application includes: Business Logic
//#include "mainwindow.h"
#include "testcode.h"
#include "defsandtools.h"
#include "whereisitxmlimportclass.h"
#pragma endregion

#pragma region Application includes: GUI
#pragma endregion


#define TEST_INPUT_XML_PATH "/mnt/data/projects/c_cpp/dvdcat/data/Movies_exp_gen_new_withFlag_sorted_FN.xml"
//"/mnt/data/projects/c_cpp/dvdcat/data/Movies_exp2_unix.xml"

class test_parse_xml : public QObject
{
     Q_OBJECT
	
	static const char *XML_CATALOG_PATH() { return TEST_INPUT_XML_PATH; };

	private slots:
		void initTestCase() {
		}

		void cleanupTestCase() {
		}

		void testImportXml()
		{
#if 0
			QStringList labels;
			labels << QObject::tr("Terms") << QObject::tr("Pages");
			QTreeWidget *treeWidget = new QTreeWidget;
			treeWidget->setHeaderLabels(labels);
			treeWidget->setWindowTitle(QObject::tr("SAX Handler"));
			treeWidget->show();
#endif

			QFile file(QString::fromUtf8(test_parse_xml::XML_CATALOG_PATH()));
			QXmlInputSource inputSource(&file);
			QXmlSimpleReader reader;
			WhereIsItXmlImportClass handler;
			reader.setContentHandler(&handler);
			reader.setErrorHandler(&handler);
			reader.parse(inputSource);
			if ( !handler.isParseSuccessful() )
			{
				QWARN(
					QString("Error while parsing input %1: %2")
						.arg( QString::fromUtf8(test_parse_xml::XML_CATALOG_PATH()) )
						.arg( handler.getParseError() )
						.toStdString().c_str()
				);
			}
			QVERIFY(handler.isParseSuccessful());

			printQS(QString("GROUPS = %1\nDISKS = %2\nFOLDERS = %3\nFILES = %4").arg(handler._diskGroupsNum).arg(handler._disks).arg(handler._folders).arg(handler._files));

		}
};

QTTESTUTIL_REGISTER_TEST(test_parse_xml);
#include "test_parse_xml.moc"


