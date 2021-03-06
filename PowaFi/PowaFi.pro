TEMPLATE = aux
TARGET = PowaFi

RESOURCES += PowaFi.qrc

QML_FILES += $$files(*.qml,true) \
             $$files(*.js,true)

CONF_FILES +=  PowaFi.apparmor \
               PowaFi.desktop \
               PowaFi.png

AP_TEST_FILES += tests/autopilot/run \
                 $$files(tests/*.py,true)

OTHER_FILES += $${CONF_FILES} \
               $${QML_FILES} \
               $${AP_TEST_FILES}

#specify where the qml/js files are installed to
qml_files.path = /PowaFi
qml_files.files += $${QML_FILES}

#specify where the config files are installed to
config_files.path = /PowaFi
config_files.files += $${CONF_FILES}

INSTALLS+=config_files qml_files

