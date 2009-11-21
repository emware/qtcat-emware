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
#ifndef CATALOGREPORTS_H
#define CATALOGREPORTS_H

/**
	@author Victor Nike <victor.nike@gmail.com>
*/
#include <QString>
#include <QSqlDatabase>


struct ReportStruct 
{
	enum ReportTypes
	{
		BlankReport,
		CatalogReport, 
  		GroupReport,
		DiskReport,
  		FolderReport,
		FileReport
	};
	ReportTypes reportType;	
	QString name;
	QString description;
	int iconId;
	quint64 size;
	long int numFiles;
	long int numDirs;
	int groups;
	int disks;
	int numCDs;
	int numDVDs;
	int numNetDrives;
	int numFlash;
	int numHDD;
	int numOther;
	int dateTime;
	int attributes;
	QString owner;
	QString grp;
	int flags;
	int diskNo;
};

class CatalogReports{
	public:
		CatalogReports(const QSqlDatabase &db);
	
		~CatalogReports();

		void getFileReport(int catId, int id, ReportStruct &report);
		void getFolderReport(int catId, int id, ReportStruct &report);
		void getDiskReportById(int catId, int diskId, ReportStruct &report);
		void getDiskReportByDiskNo(int catId, int diskNo, ReportStruct &report);
		void getGroupReport(int catId, int id, ReportStruct &report);
		void getCatalogReport(int catId, ReportStruct &report);
		
        
		void updateCatalog(int catId);
		void updateCatalogDescription(int catId , const QString &newDescription);
		void updateCatalogIcon(int catId , int newIconId);
		void initGivenData(ReportStruct &data);
    private:
        QSqlDatabase db;
};

#endif
