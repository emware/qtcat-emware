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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QHeaderView>
#include <QContextMenuEvent>

#include "configmanager.h"
#include "defsandtools.h"
#include "datamodule.h"
#include "rightpanel.h"
#include "dbnode.h"
#include "iconmanager.h"
#include "catalogtable.h"

RightPanel::RightPanel(QCatDataModuleClass *_dm, QWidget *parent)
    : dm(_dm), QFrame(parent)
{
    init();
}


RightPanel::~RightPanel()
{
    //TODO: Add other new created components
}

void RightPanel::init()
{
    // Set up GUI elements
    catalogTable = new CatalogTable(dm, this);
    showDescription = new QTextEdit;
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
    
    vertSplit = new QSplitter(Qt::Vertical);
    vertSplit->addWidget(catalogTable);
    vertSplit->addWidget(showDescription);
    vertSplit->setStretchFactor(0, 5);
    vertSplit->setStretchFactor(1, 1);
    layout->addWidget(vertSplit);

    connect(catalogTable, SIGNAL(goUp()), this, SIGNAL(goUp()));
    connect(catalogTable, SIGNAL(goDown(int)), this, SIGNAL(goDown(int)));
    connect(catalogTable, SIGNAL(newItemSelected(int)), this, SLOT(newItemSelectedSlot(int)));
    connect(catalogTable, SIGNAL(newItemSelected(QString)), this, SLOT(newItemSelectedSlot(QString)));
	connect(catalogTable, SIGNAL(canceledDrag()), this, SIGNAL(canceledDrag()));
    
}

void RightPanel::refreshItems(int newCatalog, int parentId)
{
    catalogTable->refreshItems(newCatalog, parentId);
}

QTreeWidget* RightPanel::getCatalogTable() const
{
	return catalogTable;     
}


void RightPanel::changeDataModule(QCatDataModuleClass * newDM)
{
    dm = newDM;
    catalogTable->changeDataModule(newDM);
}

void RightPanel::newItemSelectedSlot(int _id)
{
}

void RightPanel::newItemSelectedSlot(QString newDesc)
{
    showDescription->setText(newDesc);
}


