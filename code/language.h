#ifndef LANGUAGE_H
#define LANGUAGE_H

/*

 * This class manages the basic language settings and options in AppList.
 * We have two StringLists containing all languages as long and short form.
 * In the constructor we load the translation of the UI if available.
 * addLanguage() and removeLanguage() are adding/removing languages to the list of languages
 * the user understands and that will be passed when requesting app lists & categories.
 * The selected languages are store in the _myLanguages StringList and the
 * "settings/languages" key as a QSettings value.

 * Additionally we are managing the categories and category name translations here.
 * We have two types - headCategories (Applications, Games, and Themes) and the subcategories.
 * The method isHeadCategory(int id) returns if a category is a headCategory.
 * This is done this way as QPair/struct is not recognized by QML.

 */
#include <QtDeclarative>
#include <QCoreApplication>
#include <QObject>
#include <QTranslator>
#include <QSettings>
#include <QSystemInfo>

class Language : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList languageCodes READ languageCodes NOTIFY myLanguagesChanged)
    Q_PROPERTY(QString subtitleString READ subtitleString NOTIFY myLanguagesChanged)
    Q_PROPERTY(QString emptyString READ emptyString)
public:
    Language(QObject *parent = 0);

    Q_INVOKABLE void addLanguage(QString languageCode);
    Q_INVOKABLE void removeLanguage(QString languageCode);

    Q_INVOKABLE QString getLanguageName(int __id) { return _allLanguages.at(__id); }
    Q_INVOKABLE bool getSelected(QString languageCode) { return _myLanguages.contains(languageCode); }

    QStringList languageCodes() { return _languageCodes; }      // Returns language list for the selection listView
    QString subtitleString();                                   // Display the selected languages in the settings
    QString languagesForUrl();                                  // Returns language string passed for requests
    QString currentLanguage() { return _currentLanguage; }      // Returns the current language

    QString emptyString() { return ""; }                        // Deprecated
signals:
    void myLanguagesChanged();
private:
    QTranslator *translator;
    QStringList _allLanguages, _languageCodes, _myLanguages;
    QString _currentLanguage;

/* Categories */
public:
    Q_INVOKABLE QStringList categories() { return _categories; }
    Q_INVOKABLE bool isHeadCategory(int __id);
    Q_INVOKABLE QString categoryName(int __id);
private:
    QStringList _categories;

    void initCategories();  //Populate _categories (called in constructor)
};

#endif // LANGUAGE_H
