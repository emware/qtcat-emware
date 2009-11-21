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
#include <iostream>
#include <cstdlib>
#include <cstring>


#pragma region System includes
#include <QObject>
#include <QtTest/QtTest>

#include "QtTestUtil/QtTestUtil.h"
/*#include <QCoreApplication>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QDateTime>
#include <QtGlobal>
*/
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#pragma endregion


#pragma region Application includes: Business Logic
#include "defsandtools.h"
/*#include "datamodule.h"
#include "dbnode.h"
#include "mediascanner.h"
#include "catalogclass.h"

#include "testcode.h"
#include "defsandtools.h"
#include "iconscdcat.h"
#include "iconmanager.h"
*/
#include "whereisitxmlimportclass.h"
/*
#include "deviceclass.h"
*/
#pragma endregion

#pragma region Application includes: GUI
#pragma endregion



#define home
#ifdef home
#define DB_FILE "data/home2.sqlite"
#else
#define DB_FILE "data/office2.sqlite"
#endif

int allFiles = 0;



#define TEST_INPUT_XML_PATH "/mnt/data/projects/c_cpp/dvdcat/data/Movies_exp_gen_new_withFlag_sorted_FN.xml"
//"/mnt/data/projects/c_cpp/dvdcat/data/Movies_exp2_unix.xml"

class test_xml_parser : public QObject
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

			QFile file(QString::fromUtf8(test_xml_parser::XML_CATALOG_PATH()));
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
						.arg( QString::fromUtf8(test_xml_parser::XML_CATALOG_PATH()) )
						.arg( handler.getParseError() )
						.toStdString().c_str()
				);
			}
			QVERIFY(handler.isParseSuccessful());

			printQS(QString("GROUPS = %1\nDISKS = %2\nFOLDERS = %3\nFILES = %4").arg(handler._diskGroupsNum).arg(handler._disks).arg(handler._folders).arg(handler._files));

		}
};

QTTESTUTIL_REGISTER_TEST(test_xml_parser);
#include "testcode.moc"




#if 0




void printDir(int i)
{
    //if (allFiles)
   // 	printQS("Completed " + QString::number((i / allFiles) * 100.0) +  " %");
    double p = (double)((double)i / allFiles) * 100;
    //printf("Completed %1 percent", p);
    printQS(QString("Completed %1 %").arg(p));
}


void test_diskClass()
{
    DeviceClass test;
    
    //printQS(QString("Test for /dev/hdc - %1").arg(test.checkOpticalDevice("/dev/hdc")));
    //printQS(QString("Test for /media/dvd - %1").arg(test.checkOpticalDevice("/media/dvd")));
    
    /*
    printQS(QString("Mount test for /media/dvd - %1").arg(test.checkIfMounted("/media/dvd")));
    printQS(QString("Mount test for /media/pqi_data - %1").arg(test.checkIfMounted("/media/pqi_data")));
    printQS(QString("Mount test for /dev/sdb1 - %1").arg(test.checkIfMounted("/dev/sdb1")));
    printQS(QString("Mount test for /dev/sda1 - %1").arg(test.checkIfMounted("/dev/sda1")));
    printQS(QString("Mount test for /mnt/tmpsata - %1").arg(test.checkIfMounted("/mnt/tmpsata")));
    printQS(QString("Mount test for /dev/hdb - %1").arg(test.  checkIfMounted("/dev/hdb")));
    */
    
    //printQS(QString("DVD Volume is %1").arg(test.getFileSystemForOptivalDev("/dev/hdb")));
    
    
    //test.getInfoForOptical("/dev/hdb");
    //test.getInfoForOptical("/home/victor/tmp/knoppix-test.iso");
	//test.getInfoForOptical("/mnt/work/temp/_Office/Software/Slax/slax6rc4.iso");
	//test.getInfoForOptical("/home/victor/tmp/hirens.iso");
	test.getInfoForOptical("/home/victor/tmp/udf.iso");
	if (test.ElTorito)
        printQS("Disk is Bootable");
    if (test.iso9660)
        printQS("Disk has iso9660 filesystem with Volume Label " + test.vdIso9660);
    if (test.Joliet)
        printQS("Disk has Joliet extension with Volume Label " + test.vdJoliet);

	if (test.udf)
		printQS("Disk has UDF with Volume Label " + test.vdUDF);

}





void test_fill_icons(){
    
}

void test_fill_data(){
    /*
	dbDVDCat *dvd = new dbDVDCat(DB_FILE);
	printQS(QDir::currentPath());
	
	CatalogClass *cat = new CatalogClass("DVDCAT");
	
	catalogNode cat_data;
	cat_data.catName = "MyDVDs";
	cat_data.catDescription = "All my DVDs except porn, pix and movies";
	cat_data.iconId = 3;
	
	int i = cat->createCatalog(cat_data);
	printQS("createCatalog = " + QString::number(i));
	
	if (cat->isExist("MyDVDs"))
	{
		cat->readCatalog("MyDVDs", cat_data);
		printQS("Exist!");
		printQS("Updating!");
		cat_data.numFiles = 100;
		cat_data.numDirs = 10;
		cat_data.numDVD = 3;
		cat_data.catSize = 1234567;
		cat_data.iconId = 1;
		cat_data.numCD = 4;
		cat_data.numNetDrives = 1;
		cat_data.numFlash = 50;
		cat_data.numHDD = 4;
		cat_data.numFDD = 2;
		cat_data.catFlags = 6;
		cat_data.catReadPass = "alabala";
		cat_data.catWritePass = "allall";
		cat_data.catDescription = "Description!";
		cat_data.numGroups = 6;
		cat->updateCatalog(cat_data);
	}
	else
		printQS("Not Exist!");
    
//      int root_el = cat->isRootElementExist();
    
    printQS(QString::number(cat_data.catId));
	dbNodeClass *dbn = new dbNodeClass("DVDCAT");
	dbNode data;
    //memset(data, 0, sizeof data);
    
	data.catId = cat_data.catId;
	data.parent = 0;
	data.nodeSize = 0;
	data.nodeAttr = 0;
	data.nodeFlags = 0;
	data.iconId = 0;
	
	
	data.nodeName = "Movies";
	dbn->createGroup(data);
	int moviesGroup = dbn->isExistGroup(data.catId, 0, data.nodeName);
	
	data.nodeName = "PicsBubi";
	dbn->createGroup(data);
	int bubiGroup = dbn->isExistGroup(data.catId, 0, data.nodeName);
	
	data.nodeName = "Serials";
	data.parent = moviesGroup;
	dbn->createDisk(data);
	int serials = dbn->isExistDisk(data.catId, moviesGroup, data.nodeName);
	int serialsNo = data.diskNo;
	
	data.nodeName = "Documentary";
	data.parent = moviesGroup;
	dbn->createDisk(data);
	int documentary = dbn->isExistDisk(data.catId, moviesGroup, data.nodeName);
	int documentaryNo = data.diskNo;
	
	data.nodeName = "BubiPrivate2007";
	data.parent = bubiGroup;
	dbn->createDisk(data);
	int bubiPics2007 = dbn->isExistDisk(data.catId, bubiGroup, data.nodeName);
	int bubiPics2007No = data.diskNo;
	
	data.nodeName = "BubiPrivate2006";
	data.parent = bubiGroup;
	dbn->createDisk(data);
	int bubiPics2006 = dbn->isExistDisk(data.catId, bubiGroup, data.nodeName);
	int bubiPics2006No = data.diskNo;
    
//      printQS("root is " + QString::number(root_el));
//      printQS("disk is " + QString::number(disk_el));
    
#ifdef home      
	MediaScanner *ms = new MediaScanner(dbn, data.catId, bubiPics2007, "/home/victor/secure/Photos/Bokoto/Private/2007");
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	ms->startScan(bubiPics2007No);
	allFiles = ms->getItemsNum();
	printQS("allFiles = " + QString::number(allFiles));
	ms->setDirProgressFunc(printDir);
	ms->setJustCount(false);
	ms->startScan(bubiPics2007No);

//	printQS(QString::number(ms->cntAll));

	printQS("Delete ms");
	delete ms;

	ms = new MediaScanner(dbn, data.catId, bubiPics2006, "/home/victor/secure/Photos/Bokoto/Private/2006");
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	ms->startScan(bubiPics2006No);
	allFiles = ms->getItemsNum();
	printQS("allFiles = " + QString::number(allFiles));
	ms->setDirProgressFunc(printDir);
	ms->setJustCount(false);
	ms->startScan(bubiPics2006No);

//	printQS(QString::number(ms->cntAll));

	printQS("Delete ms");
	delete ms;

	ms = new MediaScanner(dbn, data.catId, serials, "/mnt/movies/Serials");
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	ms->startScan(serialsNo);
	allFiles = ms->getItemsNum();
	printQS("allFiles = " + QString::number(allFiles));
	ms->setDirProgressFunc(printDir);
	ms->setJustCount(false);
	ms->startScan(serialsNo);

	ms = new MediaScanner(dbn, data.catId, documentary, "/mnt/movies/Documentary");
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	ms->startScan(documentaryNo);
	allFiles = ms->getItemsNum();
	printQS("allFiles = " + QString::number(allFiles));
	ms->setDirProgressFunc(printDir);
	ms->setJustCount(false);
	ms->startScan(documentaryNo);

//	printQS(QString::number(ms->cntAll));

	printQS("Delete ms");
	delete ms;

#else
	MediaScanner *ms = new MediaScanner("DVDCAT", cat->id, disk_el, "/home/victor/Documents/Picures/Bubulina");
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	ms->startScan();
	allFiles = ms->cntItems;
	printQS("allFiles = " + QString::number(allFiles));
	ms->setDirProgressFunc(printDir);
	ms->setJustCount(false);
	ms->startScan();
	
	printQS(QString::number(ms->cntAll));
	
	printQS("Delete ms");
	delete ms;
	
#endif
	printQS("Delete dbn");
	delete dbn;

	printQS("Delete cat");
	delete cat;
	printQS("Delete dvd");
	delete dvd;
	*/
}

void test_icons_save()
{
	init_icon_base();
	get_m_catalog_icon()->save("icons/icon_cdcat_m_catalog_icon.png", "PNG");
	get_m_unknown_icon()->save("icons/icon_cdcat_m_unknown_icon.png", "PNG");
	get_m_cd_icon()->save("icons/icon_cdcat_m_cd_icon.png", "PNG");
	get_m_dvd_icon()->save("icons/icon_cdcat_m_dvd_icon.png", "PNG");
	get_m_hdd_icon()->save("icons/icon_cdcat_m_hdd_icon.png", "PNG");
	get_m_net_icon()->save("icons/icon_cdcat_m_net_icon.png", "PNG");
	get_m_flash_icon()->save("icons/icon_cdcat_m_flash_icon.png", "PNG");
	get_m_other_icon()->save("icons/icon_cdcat_m_other_icon.png", "PNG");
	get_m_floppy_icon()->save("icons/icon_cdcat_m_floppy_icon.png", "PNG");
	get_v_folderlocked_icon()->save("icons/icon_cdcat_v_folderlocked_icon.png", "PNG");
	get_v_folderclosed_icon()->save("icons/icon_cdcat_v_folderclosed_icon.png", "PNG");
	get_v_folderopen_icon()->save("icons/icon_cdcat_v_folderopen_icon.png", "PNG");
	get_v_file_icon()->save("icons/icon_cdcat_v_file_icon.png", "PNG");
	get_v_back_icon()->save("icons/icon_cdcat_v_back_icon.png", "PNG");
	get_anim(0)->save("icons/icon_cdcat_a_0.png", "PNG");
	get_anim(1)->save("icons/icon_cdcat_a_1.png", "PNG");
	get_anim(2)->save("icons/icon_cdcat_a_2.png", "PNG");
	get_anim(3)->save("icons/icon_cdcat_a_3.png", "PNG");
	get_anim(4)->save("icons/icon_cdcat_a_4.png", "PNG"); 
	get_p_icon()->save("icons/icon_cdcat_p_icon.png", "PNG"); 
	get_t_open_icon()->save("icons/icon_cdcat_t_open_icon.png", "PNG"); 
	get_t_save_icon()->save("icons/icon_cdcat_t_save_icon.png", "PNG"); 
	get_t_saveas_icon()->save("icons/icon_cdcat_t_saveas_icon.png", "PNG"); 
	get_t_close_icon()->save("icons/icon_cdcat_t_close_icon.png", "PNG"); 
	get_t_add_icon()->save("icons/icon_cdcat_t_add_icon.png", "PNG"); 
	get_t_rescan_icon()->save("icons/icon_cdcat_t_rescan_icon.png", "PNG"); 
	get_t_delete_icon()->save("icons/icon_cdcat_t_delete_icon.png", "PNG"); 
	get_t_config_icon()->save("icons/icon_cdcat_t_config_icon.png", "PNG"); 
	get_t_help_icon()->save("icons/icon_cdcat_t_help_icon.png", "PNG"); 
	get_t_new_icon()->save("icons/icon_cdcat_t_new_icon.png", "PNG"); 
	get_t_find_icon()->save("icons/icon_cdcat_t_find_icon.png", "PNG"); 
	get_t_about_icon()->save("icons/icon_cdcat_t_about_icon.png", "PNG"); 
	get_t_add_export_icon()->save("icons/icon_cdcat_t_add_export_icon.png", "PNG");
	get_t_remove_export_icon()->save("icons/icon_cdcat_t_remove_export_icon.png", "PNG");
	get_t_comment_icon()->save("icons/icon_cdcat_t_comment_icon.png", "PNG");
	get_t_deletec_icon()->save("icons/icon_cdcat_t_deletec_icon.png", "PNG");
	get_t_showc_icon()->save("icons/icon_cdcat_t_showc_icon.png", "PNG");
	get_m_export_icon()->save("icons/icon_cdcat_m_export_icon.png", "PNG");
	get_m_import_icon()->save("icons/icon_cdcat_m_import_icon.png", "PNG");
	get_m_borrow_icon()->save("icons/icon_cdcat_m_borrow_icon.png", "PNG");
	get_t_sborrow_icon()->save("icons/icon_cdcat_t_sborrow_icon.png", "PNG");
	get_t_cborrow_icon()->save("icons/icon_cdcat_t_cborrow_icon.png", "PNG");
	get_t_colorconfig_icon()->save("icons/icon_cdcat_t_colorconfig_icon.png", "PNG");
	
}

void test_icons_to_db()
{
    QCatDataModuleClass *dm = new QCatDataModuleClass("/mnt/data/projects/c_cpp/dvdcat/data/home2.sqlite");
	
    dm->im->deleteAllIcons(0);
    
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
    
    /*
    
    im.addIconFromFile(0, CATALOG_ICON_ENABLED, "src/commonicons/catalog_enabled.png");
    im.addIconFromFile(0, CATALOG_ICON_DISABLED, "src/commonicons/catalog_disabled.png");
    im.addIconFromFile(0, DISK_GROUP_ICON, "src/commonicons/disk_group.png");
    im.addIconFromFile(0, DISK_OTHER_ICON, "src/commonicons/other_device.png");
    im.addIconFromFile(0, DISK_CD_ICON, "src/commonicons/cdrom.png");
    im.addIconFromFile(0, DISK_DVD_ICON, "src/commonicons/dvd.png");
    im.addIconFromFile(0, DISK_HDD_ICON, "src/commonicons/hdd.png");
    im.addIconFromFile(0, DISK_NET_ICON, "src/commonicons/net_drive.png");
    im.addIconFromFile(0, DISK_USB_ICON, "src/commonicons/usbpendrive.png");
    im.addIconFromFile(0, DISK_CARD_ICON, "src/commonicons/card.png");
    im.addIconFromFile(0, FOLDER_OPENED_ICON, "src/commonicons/folder_opened.png");
    im.addIconFromFile(0, FOLDER_CLOSED_ICON, "src/commonicons/folder_closed.png");
    im.addIconFromFile(0, BINARY_FILE_ICON, "src/commonicons/file_binary.png");
    im.addIconFromFile(0, TEXT_FILE_ICON, "src/commonicons/file_ascii.png");
    im.addIconFromFile(0, IMAGE_FILE_ICON, "src/commonicons/file_image.png");
    im.addIconFromFile(0, MOVIE_FILE_ICON, "src/commonicons/file_movie.png");

    
    */
/*    
    im.addIconFromFile(3, "icon0001", "src/images/blockdevice.png");
    im.addIconFromFile(3, "icon0001", "src/images/critical.png");
    im.addIconFromFile(3, "icon0001", "src/images/folder_blue.png");
    im.addIconFromFile(3, "icon0001", "src/images/folder_green.png");
*/
    delete dm;
}

#endif