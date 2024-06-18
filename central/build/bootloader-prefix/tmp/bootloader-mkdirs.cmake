# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/User/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/tmp"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/src"
  "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/User/Central-Monitoramento/central/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
