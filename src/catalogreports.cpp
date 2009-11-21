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

#include <QSqlQuery>
#include <QVariant>

#include "defsandtools.h"
#include "catalogreports.h"
#include "datamodule.h"

CatalogReports::CatalogReports(const QSqlDatabase &db)
{
    this->db = db;
}

CatalogReports::~CatalogReports()
{
}

void CatalogReports::initGivenData(ReportStruct &data)
{
	data.reportType = ReportStruct::BlankReport;
	data.name = "";
	data.description = "";
	data.owner = "";
	data.grp = "";
	data.size = Q_INT64_C(0);
	data.numFiles = 0;
	data.numDirs = 0;
	data.groups = 0;
	data.disks = 0;
	data.numCDs = 0;
	data.numDVDs = 0;
	data.numNetDrives = 0;
	data.numFlash = 0;
	data.numHDD = 0;
	data.numOther = 0;
	data.dateTime = 0;
	data.attributes = 0;
	data.flags = 0;
	data.diskNo = 0;
} 

void CatalogReports::getCatalogReport(int catId, ReportStruct &report)
{
	QString sql = "SELECT name, description, iconId, catSize, numFiles, numDirs, numGroups, numCD, numDVD, numNetDrives, numFlash, numHDD, numFDD, flags, dateTime FROM Catalogs Where id = " + QString::number(catId);
	QSqlQuery *q = new QSqlQuery(db);
	initGivenData(report);
	if (!q)
		return;
	if (q->exec(sql))
	{
		q->first();
		report.reportType 	= ReportStruct::CatalogReport;
		report.name 		= q->value(0).toString();
		report.description 	= q->value(1).toString();
		report.iconId 		= q->value(2).toInt();	
		report.size			= q->value(3).toLongLong();	
		report.numFiles 	= q->value(4).toInt();	
		report.numDirs 		= q->value(5).toInt();	
		report.groups 		= q->value(6).toInt();	
		report.numCDs 		= q->value(7).toInt();	
		report.numDVDs 		= q->value(8).toInt();	
		report.numNetDrives = q->value(9).toInt();	
		report.numFlash 	= q->value(10).toInt();	
		report.numHDD 		= q->value(11).toInt();	
		report.numOther 	= q->value(12).toInt();	
		report.flags 		= q->value(13).toInt();	
        report.dateTime		= q->value(14).toInt();
		report.disks 		= report.numCDs + report.numDVDs + report.numNetDrives + report.numFlash + report.numHDD + report.numOther;
	}
	delete q;
}

void CatalogReports::getDiskReportById(int catId, int diskId, ReportStruct &report)
{
	QString sql = "SELECT diskNo FROM Catalog_" + QString::number(catId) + " WHERE id = " + QString::number(diskId);
    QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return;
	initGivenData(report);
    bool _tmp = false;
	if (q->exec(sql))
	{
		_tmp = q->first();
        if (_tmp)
			getDiskReportByDiskNo(catId, q->value(0).toInt(), report);
	}
    if (!_tmp)
    {
        sql = "SELECT diskNo FROM Files_" + QString::number(catId) + " WHERE id = " + QString::number(diskId);
        if (q->exec(sql))
        {
         	if (q->first())
                getDiskReportByDiskNo(catId, q->value(0).toInt(), report);
        }
    }
	delete q;

}

void CatalogReports::getDiskReportByDiskNo(int catId, int diskNo, ReportStruct &report)
{
	QString sql1 = "SELECT name, size, dateTime, iconId, description, attributes, owner, grp, flag, diskNo FROM Catalog_" + QString::number(catId) + " WHERE itemType = " + QString::number(DISK_ELEMENT) + " AND diskNo = " + QString::number(diskNo);
	
	QString sql2 = "SELECT sum(size) FROM Files_" + QString::number(catId) + " WHERE diskNo = " + QString::number(diskNo);
	
	QString sql3 = "SELECT count(id) FROM Catalog_" + QString::number(catId) + " WHERE itemType = " + QString::number(FOLDER_ELEMENT) + " AND diskNo = " + QString::number(diskNo);
	
	QString sql4 = "SELECT count(id) FROM Files_" + QString::number(catId) + " WHERE diskNo = " + QString::number(diskNo);
	
	printQS(sql1);
	printQS(sql2);
	printQS(sql3);
	printQS(sql4);
    QSqlQuery *q = new QSqlQuery(db);
	initGivenData(report);
	if (!q)
		return;
	if (q->exec(sql1))
	{
		
		q->first();
		report.reportType 	= ReportStruct::DiskReport;
		report.name 		= q->value(0).toString();
/*		if (!q->value(1).toString().isEmpty())
			report.name 		+= "." + q->value(1).toString();*/
		report.description 	= q->value(4).toString();
		report.iconId 		= q->value(3).toInt();	
		report.size			= q->value(1).toLongLong();	
		report.dateTime		= q->value(2).toInt();
		report.attributes	= q->value(5).toInt();
		report.owner		= q->value(6).toString();
		report.grp			= q->value(7).toString();
		report.flags 		= q->value(8).toInt();	
		report.diskNo		= q->value(9).toInt();
	}
	if (q->exec(sql2))
	{
		q->first();
		report.size = q->value(0).toLongLong();	
	}else{
		report.reportType = ReportStruct::BlankReport;
		delete q;
		return;
	}
	if (q->exec(sql3))
	{
		q->first();
		report.numDirs = q->value(0).toInt();	
	}else{
		report.reportType = ReportStruct::BlankReport;
		delete q;
		return;
	}
	if (q->exec(sql4))
	{
		q->first();
		report.numFiles = q->value(0).toInt();	
	}else{
		report.reportType = ReportStruct::BlankReport;
		delete q;
		return;
	}
	
	delete q;
	
}

void CatalogReports::getGroupReport(int catId, int id, ReportStruct &report)
{
//	ReportStruct localReport;
	QString sqlChilds = "SELECT id, name, size, dateTime, itemType, iconId, description, attributes, owner, grp, flag, diskNo FROM Catalog_" + QString::number(catId) + " WHERE parent = " + QString::number(id);
	
	QString sqlParent = "SELECT id, name, dateTime, itemType, iconId, description, attributes, owner, grp, flag FROM Catalog_" + QString::number(catId) + " WHERE id = " + QString::number(id);
    QSqlQuery *q = new QSqlQuery(db);
	//initGivenData(localReport);
	//printQS(sqlChilds);
	if (!q)
		return;
	if (q->exec(sqlChilds))
	{
		while (q->next())
		{
			if (q->value(4).toInt() == GROUP_ELEMENT)
			{
				++report.groups;
				getGroupReport(catId, q->value(0).toInt(), report);
			}else{
				int diskType = getBasicDiskType(q->value(10).toInt());
				switch (diskType)
				{
                    case MT_OTHER:
                        report.numOther++;
                        break;
                    case MT_CDROM:
						report.numCDs++;
						break;
					case MT_DVDROM:
						report.numDVDs++;
						break;
					case MT_HDD:
						report.numHDD++;
						break;
					case MT_NET:
						report.numNetDrives++;
						break;
					case MT_USB:
                    case MT_CARD:
						report.numFlash++;
						break;
                    default:
                        report.numOther++;
                        break;
				}
				ReportStruct diskRpt;
				getDiskReportByDiskNo(catId, q->value(11).toInt(), diskRpt);
				report.numFiles += diskRpt.numFiles;
				report.numDirs += diskRpt.numDirs;
				report.size += diskRpt.size;
				report.disks++;
			}
		}
	}
	if (q->exec(sqlParent))
	{
		q->first();
		report.name = q->value(1).toString();
		report.iconId = q->value(4).toInt();
		report.description = q->value(5).toString();
		report.dateTime = q->value(2).toInt();
	}
	delete q;	
}

void CatalogReports::getFolderReport(int catId, int id, ReportStruct &report)
{
	int currItemType = 0;
    QString sqlChilds1 = "SELECT id, name, size, dateTime, itemType, iconId, description, attributes, owner, grp, flag, diskNo FROM Catalog_" + QString::number(catId) + " WHERE parent = " + QString::number(id);
    QString sqlChilds2 = "SELECT id, name, size, dateTime, itemType, iconId, description, attributes, owner, grp, flag, diskNo FROM Files_" + QString::number(catId) + " WHERE parent = " + QString::number(id);
	
	QString sqlParent = "SELECT id, name, dateTime, itemType, iconId, description, attributes, owner, grp, flag FROM Catalog_" + QString::number(catId) + " WHERE id = " + QString::number(id);
	
	
    QSqlQuery *q = new QSqlQuery(db);
	//printQS(sqlChilds);
	if (!q)
		return;
	
    if (q->exec(sqlChilds2))
    {
     	++report.numFiles;
        report.size += q->value(2).toLongLong();   
    }
    
	if (q->exec(sqlChilds1))
	{
		while (q->next())
		{
			if (q->value(4).toInt() == FOLDER_ELEMENT)
			{
				++report.numDirs;
				getFolderReport(catId, q->value(0).toInt(), report);
			}
            /*
            else{
				++report.numFiles;
				report.size += q->value(2).toLongLong();
			}
            */
		}
	}
	if (q->exec(sqlParent))
	{
		q->first();
		currItemType = q->value(3).toInt();
//		QString ext = q->value(2).toString();
		report.name = q->value(1).toString();
		/*if (!ext.isEmpty())
			report.name += "." + ext;*/
		report.iconId = q->value(4).toInt();
		report.description = q->value(5).toString();
		report.dateTime = q->value(2).toInt();
        report.attributes = q->value(6).toInt();
        report.owner = q->value(7).toInt();
        report.grp = q->value(8).toInt();
        report.flags = q->value(9).toInt();
    }
	delete q;	
	
}

void CatalogReports::getFileReport(int catId, int id, ReportStruct &report)
{
	int currItemType = 0;
	
	QString sql = "SELECT id, name, size, dateTime, itemType, iconId, description, attributes, owner, grp, flag FROM Files_" + QString::number(catId) + " WHERE id = " + QString::number(id);
	
	
    QSqlQuery *q = new QSqlQuery(db);
	//printQS(sqlChilds);
	if (!q)
		return;
	
	if (q->exec(sql))
	{
		q->first();
		currItemType = q->value(4).toInt();
		//QString ext = q->value(2).toString();
		report.name = q->value(1).toString();
		/*if (!ext.isEmpty())
			report.name += "." + ext;*/
		report.iconId = q->value(5).toInt();
		report.description = q->value(6).toString();
		report.dateTime = q->value(3).toInt();
		report.size = q->value(2).toLongLong();
		report.attributes = q->value(7).toInt();
	}
	delete q;	
	
}

void CatalogReports::updateCatalog(int catId)
{
	// Count files
	QString sqlCountFiles = "SELECT count(id) FROM Files_" + QString::number(catId);
	QString sqlCountDirs = "SELECT count(id) FROM Catalog_" + QString::number(catId) + " WHERE itemType = " + QString::number(FOLDER_ELEMENT);
	QString sqlCountGroups = "SELECT count(id) FROM Catalog_" + QString::number(catId) + " WHERE itemType = " + QString::number(GROUP_ELEMENT);
	QString sqlSumSize = "SELECT sum(size) FROM Files_" + QString::number(catId);
	QString sqlGetAllDisks = "SELECT id, attributes, flag, diskNo FROM Catalog_" + QString::number(catId) + " WHERE itemType = " + QString::number(DISK_ELEMENT);
	
    QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return;
	ReportStruct rpt;
	initGivenData(rpt);
	if (q->exec(sqlCountFiles))
	{
		q->first();
		rpt.numFiles = q->value(0).toInt();
	}else{
		// TODO: do something
	}
	if (q->exec(sqlCountDirs))
	{
		q->first();
		rpt.numDirs = q->value(0).toInt();
	}else{
		// TODO: do something
	}
	if (q->exec(sqlCountGroups))
	{
		q->first();
		rpt.groups = q->value(0).toInt();
	}else{
		// TODO: do something
	}
	if (q->exec(sqlSumSize))
	{
		q->first();
		rpt.size = q->value(0).toLongLong();
	}else{
		// TODO: do something
	}
	
	if (q->exec(sqlGetAllDisks))
	{
		while (q->next())
		{
			int currDiskType = getBasicDiskType(q->value(2).toInt());
			switch (currDiskType)
			{
                case MT_OTHER:
                    rpt.numOther++;
                    break;
                case MT_CDROM:
					rpt.numCDs++;
					break;
				case MT_DVDROM:
					rpt.numDVDs++;
					break;
				case MT_HDD:
					rpt.numHDD++;
					break;
				case MT_NET:
					rpt.numNetDrives++;
					break;
				case MT_USB:
                case MT_CARD:
					rpt.numFlash++;
					break;
                default:
                    rpt.numOther++;
                    break;
			}
			
		}
	}else{
		// TODO: do something
	}
	
	QString sqlUpdate = "UPDATE Catalogs SET catSize = " + QString::number(rpt.size) + ", "
			+ " numFiles = " + QString::number(rpt.numFiles) + ", "
			+ " numDirs = " + QString::number(rpt.numDirs) + ", "
			+ " numGroups = " + QString::number(rpt.groups) + ", "
			+ " numCD = " + QString::number(rpt.numCDs) + ", "
			+ " numDVD = " + QString::number(rpt.numDVDs) + ", "
			+ " numNetDrives = " + QString::number(rpt.numNetDrives) + ", "
			+ " numFlash = " + QString::number(rpt.numFlash) + ", "
			+ " numHDD = " + QString::number(rpt.numHDD) + ", "
			+ " numFDD = " + QString::number(rpt.numOther) + " WHERE id = " + QString::number(catId);
	printQS(sqlUpdate);
	if (!q->exec(sqlUpdate))
	{
		printQS("Fail update Catalog!");
	}
}

void CatalogReports::updateCatalogDescription(int catId , const QString &newDescription)
{
	QString sqlUpdate = "UPDATE Catalogs SET description = '" + newDescription + "' WHERE id = " + QString::number(catId);
	printQS(sqlUpdate);
    QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return;
	if (!q->exec(sqlUpdate))
	{
		printQS("Fail update Catalog!");
	}
	
	
}

void CatalogReports::updateCatalogIcon(int catId , int newIconId)
{
	QString sqlUpdate = "UPDATE Catalogs SET iconId = " + QString::number(newIconId) + " WHERE id = " + QString::number(catId);
	printQS(sqlUpdate);
    QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return;
	if (!q->exec(sqlUpdate))
	{
		printQS("Fail update Catalog!");
	}
}











