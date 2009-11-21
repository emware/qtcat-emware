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
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QFileInfo>
#include <QIcon>

#include "defsandtools.h"
#include "iconmanager.h"



IconManager::IconManager(const QSqlDatabase &db)
{
	this->db = db;
	readGlobalIcons();
}


IconManager::~IconManager()
{
    globalIcons.clear();
}

void IconManager::addIconFromFile(int catId, const QString &iconName, const QString &fileName)
{
	QPixmap pix(fileName);
    QFileInfo fi(fileName);
    QString justFileName = fi.fileName();
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	pix.save(&buffer, "PNG");
	QString sql = "INSERT INTO Icons (catId, name, fileName, iconData) VALUES (?, ?, ?, ?)";
	QSqlQuery *q = new QSqlQuery(db);
	q->prepare(sql);
	q->addBindValue(catId);
	q->addBindValue(iconName);
	q->addBindValue(justFileName);
	q->addBindValue(bytes);
	q->exec();
	delete q;

}

void IconManager::addIconFromRes(int catId, const QString &iconName, const QString & resName)
{
//    QIcon _icon(resName);
    
    QPixmap pix = QIcon(resName).pixmap(48, 48);
    int lastPos = resName.lastIndexOf("/");
    QString fileName = resName.mid(lastPos + 1);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    //bytes << icon;
    //buffer << (buffer, icon);
    pix.save(&buffer, "PNG");
    QString sql = "INSERT INTO Icons (catId, name, fileName, iconData) VALUES (?, ?, ?, ?)";
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(sql);
    q->addBindValue(catId);
    q->addBindValue(iconName);
    q->addBindValue(fileName);
    q->addBindValue(bytes);
    q->exec();
    delete q;
    
}

void IconManager::updateIconFromFile(int catId, int iconId, const QString &iconName, const QString &fileName)
{
    QPixmap pix(fileName);
    QFileInfo fi(fileName);
    QString justFileName = fi.fileName();
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer, "PNG");
    QString sql = "UPDATE Icons SET name = ?, fileName = ?, iconData = ? WHERE catId = " + QString::number(catId) + " AND id = " + QString::number(iconId);
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(sql);
    q->addBindValue(iconName);
    q->addBindValue(fileName);
    q->addBindValue(bytes);
    q->exec();
    delete q;

}

void IconManager::updateIconName(int catId, int iconId, const QString &iconName)
{
    QString sql = "UPDATE Icons SET name = '" + iconName + "' WHERE catId = " + QString::number(catId) + " AND id = " + QString::number(iconId);
    QSqlQuery *q = new QSqlQuery(db);
    q->prepare(sql);
    q->exec();
    printQS(q->executedQuery());
    delete q;

}


QPixmap* IconManager::getPixmap(int catId, int iconId)
{
	QString sql = "SELECT id, iconData FROM icons WHERE id = " + QString::number(iconId) + " AND catid = " + QString::number(catId);
    QSqlQuery *q = new QSqlQuery(db);
    QPixmap *pix = 0;
    if (!q)
        return NULL;
    if (q->exec(sql))
    {
     	if (q->isActive())
        {
            q->first();
            QByteArray bytes = q->value(1).toByteArray();
            pix = new QPixmap;
            pix->loadFromData(bytes);
        }
    }
    delete q;
	return pix;
}

void IconManager::deleteAllIcons(int catId)
{
	QString sql = "DELETE FROM Icons WHERE catId = " + QString::number(catId);
    QSqlQuery *q = new QSqlQuery(db);
    // TODO: Fix this error not proof code!
    q->exec(sql);
    delete q;
        
}

void IconManager::deleteIcon(int catId, int iconId)
{
    QString sqlDelete = "DELETE FROM Icons WHERE catId = " + QString::number(catId) + " AND id = " + QString::number(iconId);
	QString sqlUpdate = "UPDATE Catalog_" + QString::number(catId) + " SET iconId = '0' WHERE iconId = " + QString::number(iconId);
	 
    QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return;
	if (catId)
		q->exec(sqlUpdate);
	q->exec(sqlDelete);
    delete q;
        
}

QPixmap* IconManager::getPixmap(int catId, const QString &iconName)
{
    QString sql = "SELECT id, iconData FROM icons WHERE catId = " + QString::number(catId) + " AND name = '" + iconName + "'";
    QSqlQuery *q = new QSqlQuery(db);
    QPixmap *pix = 0;
    if (!q)
        return NULL;
    if (q->exec(sql))
    {
        if (q->isActive())
        {
            q->first();
            QByteArray bytes = q->value(1).toByteArray();
            pix = new QPixmap;
            pix->loadFromData(bytes);
        }
    }
    delete q;
    return pix;
}

QPixmap* IconManager::getDefaultPixmap(int itemType, int diskFullType)
{
    QPixmap *pix = 0;
    QString defIconName;
    int diskBasicType = mediaTypesArray[findIndexFromFullDiskType(diskFullType)].basicType;
    switch (itemType)
    {
        case ROOT_ELEMENT:
            if (diskFullType)
            	defIconName = CATALOG_ICON_ENABLED;
            else
                defIconName = CATALOG_ICON_DISABLED;
            break;
        case GROUP_ELEMENT:
            defIconName = DISK_GROUP_ICON;
            break;
        case DISK_ELEMENT:
            
            switch (diskBasicType){
                case MT_OTHER:   
                    defIconName = DISK_OTHER_ICON;
                    break;
                case MT_CDROM:   
                    defIconName = DISK_CD_ICON;
                    break;
                case MT_DVDROM:   
                    defIconName = DISK_DVD_ICON;
                    break;
                case MT_HDD:   
                    defIconName = DISK_HDD_ICON;
                    break;
                case MT_NET:   
                    defIconName = DISK_NET_ICON;
                    break;
                case MT_USB:   
                    defIconName = DISK_USB_ICON;
                    break;
                case MT_CARD:   
                    defIconName = DISK_CARD_ICON;
                    break;
            }
            break;
        case FOLDER_ELEMENT:
            if (diskFullType)
                defIconName = FOLDER_OPENED_ICON;
            else
            	defIconName = FOLDER_CLOSED_ICON;
            break;
        case FILE_ELEMENT:
            defIconName = BINARY_FILE_ICON;
            break;
        case UPDIR_ELEMENT:
            defIconName = UPDIR_ICON;
            break;
    }
    
    //pix = getPixmap(0, defIconName);
    pix = globalIcons[defIconName];
    QPixmap *newPix = new QPixmap(*pix);
    return newPix;
}

int IconManager::getIconId(int catId, const QString &iconName)
{
    int result = -1;
    QString sql = "SELECT id FROM Icons WHERE catId = " + QString::number(catId) + " AND name = '" + iconName + "'";
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return result;
    if (q->exec(sql))
    {
    	if (q->isActive())
        {
         	q->first();
            result = q->value(0).toInt();   
        }
    }
    delete q;
    return result;
}

void IconManager::saveIcon(int catId, int iconId, const QString &fileName)
{
    QPixmap *pix = getPixmap(catId, iconId);
    pix->save(fileName);
    delete pix;
}

void IconManager::readGlobalIcons()
{
	QString sql = "SELECT name, iconData FROM Icons WHERE catId = 0";
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return;
    if (q->exec(sql))
    {
        if (q->isActive())
        {
            while (q->next())
            {
                QPixmap *pix = 0;
                QByteArray bytes = q->value(1).toByteArray();
                pix = new QPixmap;
                pix->loadFromData(bytes);
                globalIcons[q->value(0).toString()] = pix;
            }
        }
    }
    delete q;
    
}

void IconManager::fillDefaultIcons()
{
	/*
	addIconFromFile(0, CATALOG_ICON_ENABLED, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/catalog_enabled.png");
	addIconFromFile(0, CATALOG_ICON_DISABLED, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/catalog_disabled.png");
	addIconFromFile(0, DISK_GROUP_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/disk_group.png");
	addIconFromFile(0, DISK_OTHER_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/other_device.png");
	addIconFromFile(0, DISK_CD_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/cdrom.png");
	addIconFromFile(0, DISK_DVD_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/dvd.png");
	addIconFromFile(0, DISK_HDD_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/hdd.png");
	addIconFromFile(0, DISK_NET_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/net_drive.png");
	addIconFromFile(0, DISK_USB_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/usbpendrive.png");
	addIconFromFile(0, DISK_CARD_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/card.png");
	addIconFromFile(0, FOLDER_OPENED_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/folder_opened.png");
	addIconFromFile(0, FOLDER_CLOSED_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/folder_closed.png");
	addIconFromFile(0, BINARY_FILE_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/file_binary.png");
	addIconFromFile(0, TEXT_FILE_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/file_ascii.png");
	addIconFromFile(0, IMAGE_FILE_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/file_image.png");
	addIconFromFile(0, MOVIE_FILE_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/file_movie.png");
	addIconFromFile(0, UPDIR_ICON, "/mnt/programs/projects/c_cpp/qcat/icons/db_icons/updir.png");
	*/
	
	addIconFromRes(0, CATALOG_ICON_ENABLED, ":/icons/db_icons/catalog_enabled.png");
	addIconFromRes(0, CATALOG_ICON_DISABLED, ":/icons/db_icons/catalog_disabled.png");
	addIconFromRes(0, DISK_GROUP_ICON, ":/icons/db_icons/disk_group.png");
	addIconFromRes(0, DISK_OTHER_ICON, ":/icons/db_icons/other_device.png");
	addIconFromRes(0, DISK_CD_ICON, ":/icons/db_icons/cdrom.png");
	addIconFromRes(0, DISK_DVD_ICON, ":/icons/db_icons/dvd.png");
	addIconFromRes(0, DISK_HDD_ICON, ":/icons/db_icons/hdd.png");
	addIconFromRes(0, DISK_NET_ICON, ":/icons/db_icons/net_drive.png");
	addIconFromRes(0, DISK_USB_ICON, ":/icons/db_icons/usbpendrive.png");
	addIconFromRes(0, DISK_CARD_ICON, ":/icons/db_icons/card.png");
	addIconFromRes(0, FOLDER_OPENED_ICON, ":/icons/db_icons/folder_opened.png");
	addIconFromRes(0, FOLDER_CLOSED_ICON, ":/icons/db_icons/folder_closed.png");
	addIconFromRes(0, BINARY_FILE_ICON, ":/icons/db_icons/file_binary.png");
	addIconFromRes(0, TEXT_FILE_ICON, ":/icons/db_icons/file_ascii.png");
	addIconFromRes(0, IMAGE_FILE_ICON, ":/icons/db_icons/file_image.png");
	addIconFromRes(0, MOVIE_FILE_ICON, ":/icons/db_icons/file_movie.png");
	addIconFromRes(0, UPDIR_ICON, ":/icons/db_icons/updir.png");

	/*
	dm->im->addIconFromFile(0, CATALOG_ICON_ENABLED, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/catalog_enabled.png");
	dm->im->addIconFromFile(0, CATALOG_ICON_DISABLED, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/catalog_disabled.png");
	dm->im->addIconFromFile(0, DISK_GROUP_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/disk_group.png");
	dm->im->addIconFromFile(0, DISK_OTHER_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/other_device.png");
	dm->im->addIconFromFile(0, DISK_CD_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/cdrom.png");
	dm->im->addIconFromFile(0, DISK_DVD_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/dvd.png");
	dm->im->addIconFromFile(0, DISK_HDD_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/hdd.png");
	dm->im->addIconFromFile(0, DISK_NET_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/net_drive.png");
	dm->im->addIconFromFile(0, DISK_USB_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/usbpendrive.png");
	dm->im->addIconFromFile(0, DISK_CARD_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/card.png");
	dm->im->addIconFromFile(0, FOLDER_OPENED_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/folder_opened.png");
	dm->im->addIconFromFile(0, FOLDER_CLOSED_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/folder_closed.png");
	dm->im->addIconFromFile(0, BINARY_FILE_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/file_binary.png");
	dm->im->addIconFromFile(0, TEXT_FILE_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/file_ascii.png");
	dm->im->addIconFromFile(0, IMAGE_FILE_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/file_image.png");
	dm->im->addIconFromFile(0, MOVIE_FILE_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/file_movie.png");
	dm->im->addIconFromFile(0, UPDIR_ICON, "/mnt/data/projects/c_cpp/dvdcat/src/commonicons/updir.png");
	*/
}

