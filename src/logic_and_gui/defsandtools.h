/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nikiforov@bsc.bg   *
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
#ifndef DEFSANDTOOLS_H
#define DEFSANDTOOLS_H


#define APP_VER "0.5"
#define APP_NAME "QCat"
//#include <sys/types.h>


/**
	@author Victor Nike <victor.nike@gmail.com>
 */

// Item types
#define CATALOG_ELEMENT		0
#define ROOT_ELEMENT 		0
#define GROUP_ELEMENT 		1
#define DISK_ELEMENT 		2
#define FOLDER_ELEMENT 		3
#define FILE_ELEMENT 		4
#define ARCHIVE_ELEMENT 	5
#define UPDIR_ELEMENT		255

// Catalog Flags
#define	READ_PROTECTED		1
#define	WRITE_PROTECTED		2
#define	CATALOG_OPENED		4

// DB related
#define DBTYPE 				"QSQLITE"
#define CONN_NAME 			"DVDCAT"

// Catalogs and Groups Columns
#define COL_CATALOG_NAME			0x001
#define COL_CATALOG_TYPE			0x002
#define COL_CATALOG_CAPACITY		0x004
#define COL_CATALOG_LAST_UPDATE		0x008
#define COL_CATALOG_DESCRIPTION		0x010
#define COL_CATALOG_DISK_NO			0x020
#define COL_CATALOG_DISK_LOCATION	0x040
#define COL_CATALOG_FLAGS			0x080
#define COL_CATALOG_DEFAULT			0x07f

//Disk and Folder Columns
#define COL_DISK_NAME				0x001
#define COL_DISK_EXT				0x002
#define COL_DISK_SIZE				0x004
#define COL_DISK_DATE				0x008
#define COL_DISK_TIME				0x010
#define COL_DISK_ATTRIBUTES			0x020
#define COL_DISK_DESCRIPTION		0x040
#define COL_DISK_OWNER				0x080
#define COL_DISK_GROUP				0x100
#define COL_DISK_FLAGS				0x200
#define COL_DISK_DISK_NO			0x400
#define COL_DISK_DEFAULT			0x47f

// Define Media Types
#define MT_OTHER	0
#define MT_CDROM	1
#define MT_DVDROM	2
#define MT_HDD		3
#define MT_NET		4
#define MT_USB		5
#define MT_CARD		6



#define OTHER_TYPE			0x0000
// Define CD-ROM Types
#define CD_CDROM			0x0001
#define	CD_CDR				0x0002
#define CD_CDRW				0x0004

// Define DVD-ROM Types
#define	DVD_DVDROM			0x0001
#define	DVD_DVD_MINUS_R		0x0002
#define DVD_DVD_PLUS_R		0x0004
#define DVD_DVD_MINUS_RW	0x0008
#define DVD_DVD_PLUS_RW		0x0010
#define	DVD_DVD_RAM			0x0020
#define	DVD_DVD_MINUS_R_DL	0x0040
#define	DVD_DVD_PLUS_R_DL	0x0080
#define	DVD_DVD_MINUS_RW_DL	0x0100
#define	DVD_DVD_PLUS_RW_DL	0x0200
#define DVD_HD_DVD_ROM		0x0400
#define DVD_HD_DVD_R		0x0800
#define DVD_HD_DVD_RAM		0x1000
#define DVD_BD_ROM			0x2000
#define DVD_BD_R			0x4000
#define DVD_BD_RE			0x8000

#define USB_FLASH_KEY		0x0001
#define USB_SDCARD			0x0002
#define USB_SDCARD_MINI		0x0004
#define USB_SDCARD_MICRO	0x0008
#define USB_SDCARD_HC		0x0010
#define USB_SDCARD_HC_MINI	0x0020
#define USB_SDCARD_HC_MICRO	0x0040
#define USB_MMC				0x0080
#define USB_COMPACTFLASH	0x0100
#define USB_SMART_MEDIA		0x0200
#define USB_MS				0x0400
#define USB_MS_PRO			0x0800
#define USB_MS_PRO_DUO		0x1000
#define USB_MS_MICRO		0x2000
#define USB_XD_CARD			0x4000
// Define File Systems

#define FS_EXT2				0x0001
#define FS_EXT3				0x0002
#define FS_EXT4				0x0004
#define FS_Reiser3			0x0008
#define FS_Reiser4			0x0010

// Define Standart Icon Names in Icons table in DB 48x48
#define CATALOG_ICON_ENABLED		"CatIconEnabled"
#define CATALOG_ICON_DISABLED		"CatIconDisabled"
#define DISK_GROUP_ICON				"DiskGroupIcon"
#define DISK_OTHER_ICON				"DiskIcon_Other"
#define DISK_CD_ICON				"DiskIcon_CD"
#define	DISK_DVD_ICON				"DiskIcon_DVD"
#define DISK_HDD_ICON				"DiskIcon_HDD"
#define DISK_NET_ICON				"DiskIcon_NET"
#define DISK_USB_ICON				"DiskIcon_USB"
#define DISK_CARD_ICON				"DiskIcon_Card"
#define FOLDER_OPENED_ICON			"FolderOpenedIcon"
#define FOLDER_CLOSED_ICON			"FolderClosedIcon"
#define BINARY_FILE_ICON			"FileIcon_Bin"
#define TEXT_FILE_ICON				"FileIcon_Ascii"
#define IMAGE_FILE_ICON				"FileIcon_Image"
#define MOVIE_FILE_ICON				"FileIcon_Movie"  
#define UPDIR_ICON					"UpDirIcon"  

#define	PROCESS_JUST_COUNT		1
#define PROCESS_GROUPS			2
#define PROCESS_DISKS			3
#define PROCESS_FOLDERS			4
#define PROCESS_FILES			5

#include <QString>

struct dbNode{
    long int id;
    int catId;
    int parent;
	int nodeType;
	QString nodeName;
    qlonglong nodeSize;
    int dateTime;
	int otherNum;
    int iconId;
    QString nodeDesc;
    int nodeAttr;
    QString nodeOwner;
    QString nodeGrp;
    int nodeFlags;
    QString nodeMimeType;
    int diskNo;
    
};

struct catalogNode{
    int catId;
    QString catName;
    QString catDescription;
    int iconId;
    qlonglong catSize;
    int numFiles;
    int numDirs;
    int numGroups;
    int numCD;
    int numDVD;
    int numNetDrives;
    int numFlash;
    int numHDD;
    int numFDD;
    int catFlags;
    QString catReadPass;
    QString catWritePass;
    int dateTime;
    
    bool isReadProtected;
    bool isReadUnlocked;
    bool isWriteProtected;
    bool isWriteUnlocked;
    bool isOpened;
};

struct structScanPlaces{
	QString name;
	QString path;
	int itemType;
	bool useMount;
};

struct MediaTypesStruct {
	char iconFile[50];
	char mediaName[30];
	int basicType;
	int fullType;
};

struct vFileName {
  	QString fileName;
    QString fileExt;
    QString fileMimeType;
    bool hasExt;
    bool isExtAtomic;
    bool isHidden;
    QString defIcon;
      
};

#define MEDIA_TYPES_COUNT 39

const MediaTypesStruct mediaTypesArray[] = {
    {":icons/media/icon_cdcat_m_other_icon.png", "Other", MT_OTHER, 0},  // 0

    // CDROM - 1 - 9
    {":icons/media/icon_cdcat_m_cd_icon.png", "CD-ROM", MT_CDROM, 1}, //1
    {":icons/media/icon_cdcat_m_cd_icon.png", "CD-R", MT_CDROM, 2}, //2
    {":icons/media/icon_cdcat_m_cd_icon.png", "CD-RW", MT_CDROM, 3}, //3
    {":icons/media/icon_cdcat_m_cd_icon.png", "VCD", MT_CDROM, 4}, //4
    {":icons/media/icon_cdcat_m_cd_icon.png", "SVCD", MT_CDROM, 5}, //5
		//DVD - 10 - 24
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD-ROM", MT_DVDROM, 10}, //6
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD-R", MT_DVDROM, 11}, //7 
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD+R", MT_DVDROM, 12}, //8
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD-RW", MT_DVDROM, 13}, //9
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD+RW", MT_DVDROM, 14}, //10
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD RAM", MT_DVDROM, 15}, //11
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD-R DL", MT_DVDROM, 16}, //12
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD+R DL", MT_DVDROM, 17}, //13
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD-RW DL", MT_DVDROM, 18}, //14
    {":icons/media/icon_cdcat_m_dvd_icon.png", "DVD+RW DL", MT_DVDROM, 19}, //15
 	// HDDVD 25 - 34
    {":icons/media/icon_cdcat_m_dvd_icon.png", "HDDVD-ROM", MT_DVDROM, 25}, //16
    {":icons/media/icon_cdcat_m_dvd_icon.png", "HDDVD-R", MT_DVDROM, 26}, //17
    {":icons/media/icon_cdcat_m_dvd_icon.png", "HDDVD-RAM", MT_DVDROM, 27}, //18
    // BL DVD 35 - 49
    {":icons/media/icon_cdcat_m_dvd_icon.png", "BlueRay DVD-ROM", MT_DVDROM, 35}, //19
    {":icons/media/icon_cdcat_m_dvd_icon.png", "BlueRay DVD-R", MT_DVDROM, 36}, //20
    {":icons/media/icon_cdcat_m_dvd_icon.png", "BlueRay DVD-RE", MT_DVDROM, 37}, //21
		// HDD - 50 - 59
    {":icons/media/icon_cdcat_m_hdd_icon.png", "HDD", MT_HDD, 50}, // 22
 		// Net Drv - 60 - 69
    {":icons/media/icon_cdcat_m_net_icon.png", "netDrive", MT_NET, 60}, //23
		//USB - 70 - 79
    {":icons/media/usbpendrive_unmount.png", "USB Flash", MT_USB, 70},
  		// Cards - 80 - 99
    {":icons/media/sd_mmc_unmount.png", "SDCard", MT_CARD, 80}, 
    {":icons/media/sd_mmc_unmount.png", "SDCard Mini", MT_CARD, 81}, 
    {":icons/media/sd_mmc_unmount.png", "SDCard Micro", MT_CARD, 82}, 
    {":icons/media/sd_mmc_unmount.png", "SDHC Card", MT_CARD, 83}, 
    {":icons/media/sd_mmc_unmount.png", "SDHC Mini Card", MT_CARD, 84}, 
    {":icons/media/sd_mmc_unmount.png", "SDHC Micro Card", MT_CARD, 85}, 
    {":icons/media/sd_mmc_unmount.png", "MMC Card", MT_CARD, 86}, 
    {":icons/media/compact_flash_unmount.png", "CompactFlash Card", MT_CARD, 87}, 
    {":icons/media/smart_media_unmount.png", "SmartMedia Card", MT_CARD, 88}, 
    {":icons/media/memory_stick_unmount.png", "MemoryStick", MT_CARD, 89}, 
    {":icons/media/memory_stick_unmount.png", "MemoryStick Pro", MT_CARD, 90}, 
    {":icons/media/memory_stick_unmount.png", "MemoryStick Pro Duo", MT_CARD, 91}, 
    {":icons/media/memory_stick_unmount.png", "MemoryStick Micro", MT_CARD, 92}, 
    {":icons/media/icon_cdcat_m_flash_icon.png", "xD Card", MT_CARD, 93}
		
};


void printQS(const QString::QString toPrn);
QString getMediaTypeIconName(int index);

/**
 * Gets Basic Media Type from dbFlags 
 */
int getBasicDiskType(int dbFlags);

/**
 * Get Basic Media Type from Index - Usually QComboBox
 */
int getBasicDiskTypeFromIndex(int index);

/**
 * Get Full Media Type from Index - Usually QComboBox
 */
int getFullDiskTypeFromIndex(int index);

/**
 * Return oldDbFlags combined with Basic Disk Type
 */
int setBasicDiskType(int newBasicDiskType, int dbFlags);

/**
 * Return index in mediaTypesArray. Search fullType. Usually come from attributes
 */
int findIndexFromFullDiskType(int fullDiskType);

inline bool between(int value, int min, int max);

const QString permsToQString(int m);
void parseFileName(const QString &fullFileName, vFileName &data);

int getDiskFullTypeFromWhereIsIt(const QString &_dt);


#endif
