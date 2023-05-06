/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2022
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

#ifndef ALACCONFIGPAGE_H
#define ALACCONFIGPAGE_H

#include <QWidget>
#include "../encoderconfigpage.h"

namespace Ui {
class AlacConfigPage;
}

class AlacConfigPage : public EncoderConfigPage
{
    Q_OBJECT

public:
    explicit AlacConfigPage(QWidget *parent = nullptr);
    ~AlacConfigPage();

    void load(const Profile &profile) override;
    void save(Profile *profile) override;

private:
    Ui::AlacConfigPage *ui;
};

#endif // ALACCONFIGPAGE_H
