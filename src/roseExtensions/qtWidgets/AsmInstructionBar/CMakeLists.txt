
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############


########### install files ###############




#original Makefile.am contents follow:

##------------ AsmInstructionBar Makefile.am -----------
#include $(top_srcdir)/src/roseExtensions/qtWidgets/Make.inc 
#
#if ROSE_WITH_ROSEQT
#
#noinst_LTLIBRARIES = libAsmInstructionBar.la 
#
#
#
#libAsmInstructionBar_la_SOURCES = AsmInstructionsBar.cpp
#
#nodist_libAsmInstructionBar_la_SOURCES = moc_AsmInstructionsBar.cxx
#
#
#
## Target for installing header 
#headerTargetdir    =  $(roseQtWidgetsHeaderInstallPath)/
#headerTarget_HEADERS = AsmInstructionsBar.h
#
#
#endif
#
#EXTRA_DIST = 
#
