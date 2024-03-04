//////////////////////////////////////////////////////////////////////////////
// breezeconfigwidget.h
// -------------------
//
// SPDX-FileCopyrightText: 2009 Hugo Pereira Da Costa <hugo.pereira@free.fr>
// SPDX-FileCopyrightText: 2021-2024 Paul A McAuley <kde@paulmcauley.com>
//
// SPDX-License-Identifier: MIT
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "breeze.h"
#include "breezeexceptionlistwidget.h"
#include "breezesettings.h"
#include "buttonbehaviour.h"
#include "buttoncolors.h"
#include "buttonsizing.h"
#include "loadpreset.h"
#include "shadowstyle.h"
#include "systemicongeneration.h"
#include "titlebaropacity.h"
#include "titlebarspacing.h"
#include "ui_breezeconfigurationui.h"
#include "windowoutlinestyle.h"

#include <KCModule>
#include <KPageWidget>
#include <KSharedConfig>

#include <QSharedPointer>
#include <QTimer>
#include <QWidget>

// needed to display images when qrc is statically linked
// must be in global namespace to work
void initKlassydecorationConfigQrc();
void cleanupKlassydecorationConfigQrc();

namespace Breeze
{

enum struct LockIconState {
    Unlocked,
    Locked,
    Bistate,
};

//_____________________________________________
class ConfigWidget : public KCModule
{
    Q_OBJECT

public:
    //* constructor
    explicit ConfigWidget(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    //* destructor
    virtual ~ConfigWidget();

    //* default
    void defaults() override;

    //* load configuration
    void load() override;

    //* save configuration
    void save() override;
    void saveMain(QString saveAsPresetName = QString());

    QIcon lockIcon(LockIconState state)
    {
        switch (state) {
        case LockIconState::Unlocked:
            return m_unlockedIcon;
        case LockIconState::Locked:
            return m_lockedIcon;
        case LockIconState::Bistate:
        default:
            return m_lockIcon;
        }
    }

Q_SIGNALS:
    void saved();

public Q_SLOTS:
    void generateSystemIcons();

protected Q_SLOTS:

    //* update changed state
    virtual void updateChanged();
    void setEnabledAnimationsSpeed();
    void updateIconsStackedWidgetVisible();
    void updateBackgroundShapeStackedWidgetVisible();
    void systemIconGenerationButtonClicked();
    void buttonSizingButtonClicked();
    void buttonColorsButtonClicked();
    void buttonBehaviourButtonClicked();
    void titleBarSpacingButtonClicked();
    void titleBarOpacityButtonClicked();
    void shadowStyleButtonClicked();
    void windowOutlineStyleButtonClicked();
    void presetsButtonClicked();
    void dialogChanged(bool changed);
    void kPageWidgetChanged(KPageWidgetItem *current, KPageWidgetItem *before);
    void updateWindowControlPreviewIcons();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    //* ui
    Ui_BreezeConfigurationUI m_ui;

    QPushButton *m_presetsButton;

    //* kconfiguration object
    KSharedConfig::Ptr m_configuration;

    //* presets kconfiguration object
    KSharedConfig::Ptr m_presetsConfiguration;

    //* internal exception
    InternalSettingsPtr m_internalSettings;

    //* changed state
    bool m_changed;

    //* defaults clicked
    bool m_defaultsPressed = false;

    bool m_processingDefaults = false;
    bool m_loading = false;

    //* dialogs behind button
    SystemIconGeneration *m_systemIconGenerationDialog;
    ButtonSizing *m_buttonSizingDialog;
    ButtonColors *m_buttonColorsDialog;
    ButtonBehaviour *m_buttonBehaviourDialog;
    TitleBarSpacing *m_titleBarSpacingDialog;
    TitleBarOpacity *m_titleBarOpacityDialog;
    ShadowStyle *m_shadowStyleDialog;
    WindowOutlineStyle *m_windowOutlineStyleDialog;
    LoadPreset *m_loadPresetDialog;

    KPageWidget *m_kPageWidget;

    QIcon m_unlockedIcon;
    QIcon m_lockedIcon;
    QIcon m_lockIcon; // bistate with both unlocked and locked

    bool isDefaults();
    QString presetGroupName(QString str);
    void writePreset(KCoreConfigSkeleton *skeleton, KConfig *config, const QString &groupName);

    void importBundledPresets();
    void updateIcons();
    void generateWindowControlPreviewIcon(QSize size, InternalSettings::EnumButtonIconStyle::type iconStyle);
};

}
