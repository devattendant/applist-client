#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

bool Settings::darkTheme() {
    QSettings settings("schumi1331", "AppList");
    return settings.value("settings/darkTheme", false).toBool();
}

void Settings::setDarkTheme(bool newValue) {
    QSettings settings("schumi1331", "AppList");
    settings.setValue("settings/darkTheme", newValue);
}

bool Settings::aasFeed() {
    QSettings settings("schumi1331", "AppList");
    return settings.value("settings/aasFeed", true).toBool();
}

void Settings::setAasFeed(bool newValue) {
    QSettings settings("schumi1331", "AppList");
    settings.setValue("settings/aasFeed", newValue);
}
