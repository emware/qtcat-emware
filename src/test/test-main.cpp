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
#include <QtGui/QApplication>
#include "QtTestUtil/TestRegistry.h"
#pragma endregion

#include "test-common.h"

#include "mainwindow.h"

// Runs all tests registered with the QtTestUtil registry.
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QStringList options;
	options << "skipsettings";

    MainWindow mainWindow(options);
	TestWidgetReferences globalRefs;
	qWin = &mainWindow;
	mainWindow.show();




	return QtTestUtil::TestRegistry::getInstance()->runTests(argc, argv);
}
