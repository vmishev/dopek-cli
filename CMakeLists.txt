cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core target: no UI, no Q_OBJECT, no resources
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


add_executable(app
  main.cpp
  core/models/counterparty.h
  core/services/result.h
  core/models/invoice.h
  core/models/invoice_line.h
  core/models/posting.h
  core/repos/counterparty_repo.h core/repos/counterparty_repo.cpp
  core/repos/invoice_repo.h core/repos/invoice_repo.cpp
  core/repos/invoice_line_repo.h core/repos/invoice_line_repo.cpp
  core/services/counterparty_service.h core/services/counterparty_service.cpp
  core/services/invoice_service.h core/services/invoice_service.cpp
  core/services/invoice_line_service.h core/services/invoice_line_service.cpp
  core/repos/posting_repo.h core/repos/posting_repo.cpp

  core/services/counterparty_service.h core/services/counterparty_service.cpp
  core/services/posting_service.h core/services/posting_service.cpp
  core/repos/project_repo.h core/repos/project_repo.cpp
  core/services/project_service.h core/services/project_service.cpp
  core/models/project.h
  core/models/material.h
  core/models/inventory.h
  core/repos/material_repo.h core/repos/material_repo.cpp
  core/services/material_service.h core/services/material_service.cpp
  core/repos/inventory_repo.h core/repos/inventory_repo.cpp
  core/services/inventory_service.h core/services/inventory_service.cpp


)

target_link_libraries(app PRIVATE Qt${QT_VERSION_MAJOR}::Core)
#scv

#core/models/counterparty.h
#core/models/invoice.h
#core/models/invoice_line.h
#core/models/posting.h

#core/repos/counterparty_repo.h
#core/repos/counterparty_repo.cpp
#core/repos/invoice_repo.h
#core/repos/invoice_repo.cpp
#core/repos/invoice_line_repo.h
#core/repos/invoice_line_repo.cpp

#core/services/result.h
#core/services/counterparty_service.h
#core/services/counterparty_service.cpp
# core/services/invoice_service.h
## core/services/invoice_service.cpp
## core/services/invoice_line_service.h
## core/services/invoice_line_service.cpp
