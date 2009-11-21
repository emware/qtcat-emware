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
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QApplication>

#include "progresswidget.h"

ProgressWidget::ProgressWidget(int progressNum, QWidget *parent)
 : QWidget(parent)
{
	layout = new QVBoxLayout;
    layout->setMargin(0);
	for (int i = 0; i < progressNum; i++)
	{
		QProgressBar *_bar = new QProgressBar;
		QLabel *_label = new QLabel;
        _label->setAlignment(Qt::AlignCenter);
		progBars.append(_bar);
		progBarsTitles.append(_label);
		QVBoxLayout *_vb = new QVBoxLayout;
		_vb->addWidget(_label);
		_vb->addWidget(_bar);
        _vb->setMargin(0);
		layout->addLayout(_vb);
	}
	setLayout(layout);
}


ProgressWidget::~ProgressWidget()
{
	// Delete collections	
}

void ProgressWidget::setProgressBarTitle(int num, const QString newTitle)
{
	progBarsTitles.at(num)->setText(newTitle);
    progBarsTitles.at(num)->repaint();
}

void ProgressWidget::setProgressBarMinMax(int num, int _min, int _max)
{
    progBars.at(num)->setValue(0);
	progBars.at(num)->setMaximum(_max);
	progBars.at(num)->setMinimum(_min);
}

void ProgressWidget::stepProgressBar(int num)
{
	int currValue = progBars.at(num)->value();
	currValue++;
	progBars.at(num)->setValue(currValue);
    qApp->processEvents();
}

void ProgressWidget::setProgressValue(int num, int newValue)
{
	progBars.at(num)->setValue(newValue);
}

