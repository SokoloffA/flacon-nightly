/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2017
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef COVERDIALOG_H
#define COVERDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QPointer>

class Disc;
class QAbstractButton;
class QListWidgetItem;

namespace Ui {
class CoverDialog;
}

class CoverDialog : public QDialog
{
    Q_OBJECT

public:
    static CoverDialog *createAndShow(Disc *disc, QWidget *parent = nullptr);
    ~CoverDialog() override;

    Disc *disc() const { return mDisc; }

public slots:
    void setDisc(Disc *disc);

private slots:
    void coverDoubleClicked(QListWidgetItem *item);
    void buttonClicked(QAbstractButton *button);

private:
    explicit CoverDialog(QWidget *parent = nullptr);

    void scan(const QString &startDir);

    Ui::CoverDialog *ui;
    QPointer<Disc>   mDisc;
    QIcon            mEmptyIcon;
};

#endif // COVERDIALOG_H
