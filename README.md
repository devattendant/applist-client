# AppList for Symbian
An app store developed for the Symbian platform after Nokia/Microsoft closed the Nokia Store/Ovi Store in late 2013. This repository contains the client code that was used to browse through, download and update apps on the phone.

It was developed by the 18-year-old me in 2013-2015, so please bear with me about the code quality.

#### Notes:

AppList has been discontinued, so this is __just an archive repository__. If you want to keep developing it for whatever reason in 2025 (_or have any other usage for that in 2025_), feel free to drop me a message to add a link to your store. If any file required to compile the code is missing, don't hesitate to message me or open an issue.

#### Further reading:
- [Introducing AppList (beta) - and a Symbian Store for 2014 and the future](http://www.allaboutsymbian.com/news/item/19470_Introducing_AppList_beta-and_a.php)
- [AppList for Symbian is being discontinued & open sourced](https://devattendant.de/blog/tech/applist-for-symbian-is-being-discontinued-open-sourced/)

## API/Backend
If you want to build a functional version of the app, you need to setup a backend for that.
I have no plans to release mine, as it wouldn't be safe anyway being based on very old PHP and it's for sure missing a lot of best practices of 2025.

For your own backend, basically you need to create two "dynamic" and two static files:

#### `applist.php`
This is the main file that returns all data of available apps as a GET request, depending on any category, search or whatever you're browsing in. Have a look at [getlist.cpp](code/getlist.cpp) and [UpdateManagement::loadList()](code/updatemanagement.cpp) to see all possible params.

Have a look into [resources/applist.xml](resources/applist.xml) for an example output. All fields are mandatory, but they can be empty (verify code to be sure).

#### `applist-download.php`
This is a very simple file, which tracks downloads for stat reasons before really starting the file download (see [Downloader::startDownload()](code/downloader.cpp)). It is a GET request and returns 0 on success and 1 on failure (e.g. database connection error, etc).

#### `changelog.xml`
A static file returning a full list of the content of previous updates, displayed in [AboutPage.qml](code/qml/AppList/AboutPage.qml). See [resources/changelog.xml](resources/changelog.xml) for an example.

#### `version.xml`
A static file notifying of updates for the client itself, always called when launching the app. See [resources/version.xml](resources/version.xml) for an example.

## Thanks to:
- Anand Bibek: Pull to refresh
- Bhavin: Beta testing
- Fabian (@huellif): Symbian C++ and beta testing
- Kush: Beta testing
- István: Beta testing
- Kush: Beta testing
- Lily: Beta testing
- Lyubomir (@batezippi): Beta testing
- Magissia: Hosting
- Microsoft: Idea how a good Store web UI may look like
- Moritz (@MoritzJT): Beta testing
- Nokia: The Symbian platform till now and my 808 PureView
- Oleg (@aquarius_rus): SISContents
- pisarz1958 and Motaz: Homescreen widget tutorial and dll improvements
- Simone Grandi: Belle icons from his theme \"Evolve+ FP2 Edition\"
- Slava; Ilya and Anton: SVGB decoder and the Qt port SVGB2SVG
- Stephen Elop: Killing a beautiful software and operating system
- Steve Litchfield: His curated App Store, feedback, promoting and beta testing
- All these guys in the Qt forums, stackoverflow and the web
- All developers and companies who developed or are developing for Symbian
- You for reading this and using AppList