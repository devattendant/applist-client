# Add more folders to ship with the application, here
folder_01.source = qml/AppList
folder_01.target = qml
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xA1331002

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices #TrustedUI

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
MOBILITY += feedback systeminfo publishsubscribe

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    appmanagement.cpp \
    #xqinstaller.cpp \
    #xqinstaller_p.cpp
    roleitemmodel.cpp \
    getlist.cpp \
    hswidget.cpp \
    downloader.cpp \
    installserverthread.cpp \
    news.cpp \
    update.cpp \
    language.cpp \
    updatemanagement.cpp \
    subscriber.cpp \
    settings.cpp \
    symbian.cpp \
    appitem.cpp \
    reviewitem.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    appmanagement.h \
    #xqinstaller.h \
    #xqinstaller_p.h \
    sisregistrysession.h \
    sisregistryentry.h \
    roleitemmodel.h \
    getlist.h \
    qhswidget.h \
    hswidget.h \
    downloader.h \
    installserverthread.h \
    news.h \
    update.h \
    language.h \
    updatemanagement.h \
    subscriber.h \
    settings.h \
    symbian.h \
    appitem.h \
    reviewitem.h

addFiles.pkg_postrules += "\"C:\\AppList.png\" - \"c:\\AppList.png\""
addFiles.pkg_postrules += "\"C:\\HSWidgetxSchumi1331AppList_A1331002_rel.dll\" - \"!:\\sys\\bin\\HSWidgetxSchumi1331AppList_A1331002_rel.dll\""
DEPLOYMENT += addFiles

# For lupdate
#evil_hack_to_fool_lupdate {
#    SOURCES += qml/AppList/*.qml
#}

TRANSLATIONS = applist_template.ts \
    applist_de.ts \
    applist_pl.ts \
    applist_hu.ts \
    applist_fr.ts \
    applist_it.ts \
    applist_ru.ts \
    applist_uk.ts \
    applist_pt.ts \
    applist_es.ts \
    applist_el.ts

#symbian:MMP_RULES += "DEBUGGABLE"

symbian {
    VERSION = 1.0.298

    DEFINES += APP_UID=\"$$TARGET.UID3\"
    DEFINES += APP_Version=\"$$VERSION\"

    ICON = icon.svg

    supported_platforms = \
      "[0x20022E6D],0,0,0,{\"S60ProductID\"}" \ # Symbian Anna
      "[0x2003A678],0,0,0,{\"S60ProductID\"}" # only Belle
    # remove default platforms
    default_deployment.pkg_prerules -= pkg_platform_dependencies
    # add our platforms
    supported_platforms_deployment.pkg_prerules += supported_platforms
    DEPLOYMENT += supported_platforms_deployment

    vendorinfo = "%{\"Sebastian (schumi1331)\"}" ":\"Sebastian (schumi1331)\""
    vendor_deploy.pkg_prerules += vendorinfo
    DEPLOYMENT += vendor_deploy

    LIBS += -lsisregistryclient

    #that library should be copied to SDK folder
    #LIBS += -lhswidgetpublisher
    #Libs for appswitcher
    LIBS += -lapgrfx -lcone -lws32 -lapparc -leuser
    #Libs for CFbsBimap
    LIBS += -lbitgdi -lfbscli -laknskins -laknskinsrv -leikcore
    #Libs for openDefaultBrowser
    LIBS += -lapmime
}

QT += network testlib

RESOURCES += \
    Qml.qrc
