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
#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QProgressBar;
class QLabel;
class QVBoxLayout;

class ProgressWidget : public QWidget
{
	Q_OBJECT
	public:
		ProgressWidget(int progressNum, QWidget *parent = 0);
	
		~ProgressWidget();
		
		void setProgressBarTitle(int num, const QString newTitle);
		void setProgressBarMinMax(int num, int _min, int _max);
		void stepProgressBar(int num);
		void setProgressValue(int num, int newValue);
		
	private:
		QList<QProgressBar*> progBars;
		QList<QLabel*> progBarsTitles;
		QVBoxLayout *layout;

};

#endif
