/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2013
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

#ifndef ENCODERCONFIGPAGE_H
#define ENCODERCONFIGPAGE_H

#include <QWidget>
#include "profiles.h"

class QSlider;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Profile;

class EncoderConfigPage : public QWidget
{
    Q_OBJECT
public:
    explicit EncoderConfigPage(QWidget *parent = nullptr);
    virtual ~EncoderConfigPage();

    virtual void load(const Profile &profile) = 0;
    virtual void save(Profile *profile)       = 0;

    static QString losslessCompressionToolTip(int min, int max);
    static void    setLosslessToolTip(QSlider *widget);
    static void    setLosslessToolTip(QSpinBox *widget);

    static QString lossyCompressionToolTip(int min, int max);
    static void    setLossyToolTip(QSlider *widget);
    static void    setLossyToolTip(QSpinBox *widget);
    static void    setLossyToolTip(QDoubleSpinBox *widget);

    static void fillBitrateComboBox(QComboBox *comboBox, const QList<int> &bitrates);

    void loadWidget(const Profile &profile, const QString &key, QSlider *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QSlider *widget);

    void loadWidget(const Profile &profile, const QString &key, QLineEdit *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QLineEdit *widget);

    void loadWidget(const Profile &profile, const QString &key, QCheckBox *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QCheckBox *widget);

    void loadWidget(const Profile &profile, const QString &key, QGroupBox *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QGroupBox *widget);

    void loadWidget(const Profile &profile, const QString &key, QSpinBox *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QSpinBox *widget);

    void loadWidget(const Profile &profile, const QString &key, QDoubleSpinBox *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QDoubleSpinBox *widget);

    void loadWidget(const Profile &profile, const QString &key, QComboBox *widget) const;
    void saveWidget(Profile *profile, const QString &key, const QComboBox *widget);

    static QString toolTipCss();

private:
};

#endif // ENCODERCONFIGPAGE_H
