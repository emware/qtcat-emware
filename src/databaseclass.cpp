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
#include <QWidget>
#include <QtGui>
#include <QSqlDatabase>

#include "databaseclass.h"

VDataBaseClass::VDataBaseClass(QString fileName, QObject *p)
{
    dbFileName = fileName;
    _db = QSqlDatabase::addDatabase(DBDRIVER);
    _db.setDatabaseName(dbFileName);
    if (!_db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                      "This example needs SQLite support. Please read "
                                      "the Qt SQL driver documentation for information how "
                                      "to build it.\n\n"
                                      "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }
}


VDataBaseClass::~VDataBaseClass()
{
    _db.close();
}


