contains(QT_CONFIG, freetype)|contains(QT_CONFIG, system-freetype) {
    include($$PWD/basic/basic.pri)
}

unix:!mac {
    CONFIG += qpa/genericunixfontdatabase
    include($$PWD/genericunix/genericunix.pri)
    contains(QT_CONFIG,fontconfig) {
        include($$PWD/fontconfig/fontconfig.pri)
    }
}

mac {
    include($$PWD/mac/coretext.pri)
}

