/*
 * Copyright (c) 2012, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "apps/oskar_MainWindow.h"
#include "widgets/oskar_About.h"
#include "widgets/oskar_BinaryLocations.h"
#include "widgets/oskar_CudaInfoDisplay.h"
#include "widgets/oskar_DocumentationDisplay.h"
#include "widgets/oskar_RunDialog.h"
#include "widgets/oskar_SettingsDelegate.h"
#include "widgets/oskar_SettingsItem.h"
#include "widgets/oskar_SettingsModelApps.h"
#include "widgets/oskar_SettingsView.h"

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QKeySequence>
#include <QtCore/QModelIndex>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtGui/QCloseEvent>

oskar_MainWindow::oskar_MainWindow(QWidget* parent)
: QMainWindow(parent)
{
    // Set the window title.
    mainTitle_ = QString("OSKAR (%1)").arg(OSKAR_VERSION_STR);

    // Create the central widget and main layout.
    setWindowTitle(mainTitle_);
    setWindowIcon(QIcon(":/icons/oskar.ico"));
    widget_ = new QWidget(this);
    setCentralWidget(widget_);
    QVBoxLayout* v_layout = new QVBoxLayout(widget_);
    QFormLayout* formLayout = new QFormLayout;

    // Create and set up the settings model.
    model_ = new oskar_SettingsModelApps(widget_);
    modelProxy_ = new oskar_SettingsModelFilter(widget_);
    modelProxy_->setSourceModel(model_);

    // Create the search box.
    QLineEdit* filterBox = new QLineEdit(widget_);
    formLayout->addRow("Settings View Filter", filterBox);
    connect(filterBox, SIGNAL(textChanged(QString)),
            modelProxy_, SLOT(setFilterText(QString)));
    v_layout->addLayout(formLayout);

    // Create and set up the settings view.
    view_ = new oskar_SettingsView(widget_);
    oskar_SettingsDelegate* delegate = new oskar_SettingsDelegate(view_,
            widget_);
    view_->setModel(modelProxy_);
    view_->setItemDelegate(delegate);
    view_->resizeColumnToContents(0);
    v_layout->addWidget(view_);

    // Create and set up the actions.
    QAction* actOpen = new QAction("&Open...", this);
    QAction* actSaveAs = new QAction("&Save As...", this);
    QAction* actExit = new QAction("E&xit", this);
    QAction* actBinLocations = new QAction("&Binary Locations...", this);
    actHideUnset_ = new QAction("&Hide Unset Items", this);
    actHideUnset_->setCheckable(true);
    QAction* actShowFirstLevel = new QAction("Show &First Level", this);
    QAction* actExpandAll = new QAction("&Expand All", this);
    QAction* actCollapseAll = new QAction("&Collapse All", this);
    QAction* actRunInterferometer = new QAction("Run &Interferometer", this);
    QAction* actRunBeamPattern = new QAction("Run &Beam Pattern", this);
    QAction* actRunImager = new QAction("Run I&mager", this);
    QAction* actHelpDoc = new QAction("Documentation...", this);
    QAction* actCudaInfo = new QAction("CUDA System Info...", this);
    QAction* actAbout = new QAction("&About OSKAR...", this);
    connect(actOpen, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(actSaveAs, SIGNAL(triggered()), this, SLOT(saveSettingsAs()));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actBinLocations, SIGNAL(triggered()), this, SLOT(binLocations()));
    connect(actHideUnset_, SIGNAL(toggled(bool)),
            this, SLOT(setHideIfUnset(bool)));
    connect(actShowFirstLevel, SIGNAL(triggered()),
            view_, SLOT(showFirstLevel()));
    connect(actExpandAll, SIGNAL(triggered()), view_, SLOT(expandSettingsTree()));
    connect(actCollapseAll, SIGNAL(triggered()), view_, SLOT(collapseAll()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actCudaInfo, SIGNAL(triggered()), this, SLOT(cudaInfo()));
    connect(actRunInterferometer, SIGNAL(triggered()),
            this, SLOT(runInterferometer()));
    connect(actRunBeamPattern, SIGNAL(triggered()),
            this, SLOT(runBeamPattern()));
    connect(actRunImager, SIGNAL(triggered()), this, SLOT(runImager()));
    connect(actHelpDoc, SIGNAL(triggered()), this, SLOT(helpDoc()));

    // Set up keyboard shortcuts.
    actOpen->setShortcut(QKeySequence::Open);
    actExit->setShortcut(QKeySequence::Quit);
    actHideUnset_->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_H));
    actShowFirstLevel->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    actExpandAll->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
    actCollapseAll->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));

    // Create the menus.
    menubar_ = new QMenuBar(this);
    setMenuBar(menubar_);
    menuFile_ = new QMenu("&File", menubar_);
    QMenu* menuEdit = new QMenu("&Edit", menubar_);
    QMenu* menuView = new QMenu("&View", menubar_);
    QMenu* menuRun = new QMenu("&Run", menubar_);
    QMenu* menuHelp = new QMenu("&Help", menubar_);
    menubar_->addAction(menuFile_->menuAction());
    menubar_->addAction(menuEdit->menuAction());
    menubar_->addAction(menuView->menuAction());
    menubar_->addAction(menuRun->menuAction());
    menubar_->addAction(menuHelp->menuAction());
    menuFile_->addAction(actOpen);
    menuFile_->addAction(actSaveAs);
    createRecentFileActions();
    updateRecentFileActions();
    menuFile_->addSeparator();
    menuFile_->addAction(actExit);
    menuEdit->addAction(actBinLocations);
    menuView->addAction(actHideUnset_);
    menuView->addSeparator();
    menuView->addAction(actShowFirstLevel);
    menuView->addAction(actExpandAll);
    menuView->addAction(actCollapseAll);
    menuRun->addAction(actRunInterferometer);
    menuRun->addAction(actRunBeamPattern);
    menuRun->addAction(actRunImager);
    menuHelp->addAction(actHelpDoc);
    menuHelp->addSeparator();
    menuHelp->addAction(actCudaInfo);
    menuHelp->addAction(actAbout);

    // Create the toolbar.
    QToolBar* toolbar = new QToolBar(this);
    toolbar->setObjectName("Run");
    toolbar->addAction(actRunInterferometer);
    toolbar->addAction(actRunBeamPattern);
    toolbar->addAction(actRunImager);
    addToolBar(Qt::TopToolBarArea, toolbar);

    // Load the settings.
    QSettings settings;
    restoreGeometry(settings.value("main_window/geometry").toByteArray());
    restoreState(settings.value("main_window/state").toByteArray());

    // Set up binary path names.
    binary_interferometer_ = settings.value("binaries/interferometer",
            "oskar_sim_interferometer").toString();
    binary_beam_pattern_ = settings.value("binaries/beam_pattern",
            "oskar_sim_beam_pattern").toString();
    binary_imager_ = settings.value("binaries/imager",
            "oskar_imager").toString();
    binary_cuda_info_ = settings.value("binaries/cuda_info",
            "oskar_cuda_system_info").toString();

    // First, restore the expanded items.
    view_->restoreExpanded();

    // Optionally hide unset items.
    actHideUnset_->setChecked(settings.value("main_window/hide_unset_items").toBool());

    // Restore the scroll bar position.
    // A single-shot timer is used to do this after the main event loop starts.
    QTimer::singleShot(0, view_, SLOT(restorePosition()));
}

void oskar_MainWindow::openSettings(QString filename)
{
    if (settingsFile_.isEmpty() && model_->isModified())
    {
        int ret = QMessageBox::warning(this, "OSKAR",
                "Opening a new file will discard any current unsaved modifications.\n"
                "Do you want to proceed?",
                QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    // Check if the supplied filename is empty, and prompt to open file if so.
    if (filename.isEmpty())
    {
        filename = QFileDialog::getOpenFileName(this, "Open Settings",
                settingsFile_);
    }

    // Open the file if one was selected.
    if (!filename.isEmpty())
    {
        view_->saveExpanded();
        settingsFile_ = filename;
        model_->loadSettingsFile(filename);
        setWindowTitle(mainTitle_ + " [" + filename + "]");
        updateRecentFileList();
        view_->restoreExpanded();
    }
}

void oskar_MainWindow::saveSettingsAs(QString filename)
{
    // Check if the supplied filename is empty, and prompt to save file if so.
    if (filename.isEmpty())
    {
        filename = QFileDialog::getSaveFileName(this, "Save Settings",
                settingsFile_);
    }

    // Save the file if one was selected.
    if (!filename.isEmpty())
    {
        // Remove any existing file with this name.
        if (QFile::exists(filename))
        {
            if (!QFile::remove(filename))
            {
                QMessageBox::critical(this, mainTitle_,
                        QString("Could not overwrite file at %1").arg(filename),
                        QMessageBox::Ok);
                return;
            }
        }

        // Try to open the file for writing.
        QFile file(filename);
        bool result = file.open(QFile::ReadWrite);
        file.close();
        if (!result) return;

        view_->saveExpanded();
        settingsFile_ = filename;
        model_->saveSettingsFile(filename);
        setWindowTitle(mainTitle_ + " [" + filename + "]");
        updateRecentFileList();
        view_->restoreExpanded();
    }
}

// =========================================================  Protected methods.

void oskar_MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the tree view.
    view_->saveExpanded();
    view_->savePosition();

    QSettings settings;
    settings.setValue("main_window/geometry", saveGeometry());
    settings.setValue("main_window/state", saveState());
    settings.setValue("main_window/hide_unset_items", actHideUnset_->isChecked());
    settings.setValue("binaries/interferometer", binary_interferometer_);
    settings.setValue("binaries/beam_pattern", binary_beam_pattern_);
    settings.setValue("binaries/imager", binary_imager_);
    settings.setValue("binaries/cuda_info", binary_cuda_info_);

    if (settingsFile_.isEmpty() && model_->isModified())
    {
        int ret = QMessageBox::warning(this, "OSKAR",
                "Exiting OSKAR will discard any unsaved modifications.\n"
                "Do you want to proceed?",
                QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Save, QMessageBox::Ok);
        if (ret == QMessageBox::Ok)
        {
            event->accept();
        }
        else if (ret == QMessageBox::Save)
        {
            saveSettingsAs();

            // Check if the save failed for any reason.
            if (settingsFile_.isEmpty())
                event->ignore();
            else
                event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

// =========================================================  Private slots.

void oskar_MainWindow::about()
{
    oskar_About aboutDialog(this);
    aboutDialog.exec();
}

void oskar_MainWindow::binLocations()
{
    oskar_BinaryLocations binaryLocations(this);
    binaryLocations.setBeamPattern(binary_beam_pattern_);
    binaryLocations.setCudaSystemInfo(binary_cuda_info_);
    binaryLocations.setImager(binary_imager_);
    binaryLocations.setInterferometer(binary_interferometer_);
    if (binaryLocations.exec() == QDialog::Accepted)
    {
        binary_beam_pattern_ = binaryLocations.beamPattern();
        binary_cuda_info_ = binaryLocations.cudaSystemInfo();
        binary_imager_ = binaryLocations.imager();
        binary_interferometer_ = binaryLocations.interferometer();
    }
}

void oskar_MainWindow::cudaInfo()
{
    // Check that the binary actually exists.
    QProcess process;
    process.start(binary_cuda_info_);
    if (!process.waitForStarted())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(mainTitle_);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("The CUDA system info binary could not be found.");
        msgBox.setInformativeText("Please edit the binary location and try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        binLocations();
        return;
    }

    oskar_CudaInfoDisplay infoDisplay(binary_cuda_info_, this);
    infoDisplay.exec();
}

void oskar_MainWindow::helpDoc()
{
    oskar_DocumentationDisplay helpDisplay(this);
    helpDisplay.exec();
}

void oskar_MainWindow::runBeamPattern()
{
    run_binary_ = binary_beam_pattern_;
    runButton();
}

void oskar_MainWindow::runInterferometer()
{
    run_binary_ = binary_interferometer_;
    runButton();
}

void oskar_MainWindow::runImager()
{
    run_binary_ = binary_imager_;
    runButton();
}

void oskar_MainWindow::setHideIfUnset(bool value)
{
    view_->saveExpanded();
    modelProxy_->setHideIfUnset(value);
    view_->restoreExpanded();
    view_->update();
}

void oskar_MainWindow::openRecentFile()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act)
    {
        QString filename = act->data().toString();

        if (QFile::exists(filename))
        {
            // If the file exists, then open it.
            openSettings(filename);
        }
        else
        {
            // If the file doesn't exist, display a warning message.
            QMessageBox::critical(this, mainTitle_,
                    QString("File %1 not found").arg(filename),
                    QMessageBox::Ok);

            // Remove it from the list.
            QSettings settings;
            QStringList files = settings.value("recent_files/files").toStringList();
            files.removeAll(filename);
            settings.setValue("recent_files/files", files);
            updateRecentFileActions();
        }
    }
}


// =========================================================  Private methods.

void oskar_MainWindow::runButton()
{
    // Save settings if they are not already saved.
    if (settingsFile_.isEmpty())
    {
        // Get the name of the new settings file (return if empty).
        saveSettingsAs(QString());
        if (settingsFile_.isEmpty())
            return;
    }

    // Check that the selected binary actually exists.
    QProcess process;
    process.start(run_binary_);
    if (!process.waitForStarted())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(mainTitle_);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("The selected binary could not be found.");
        msgBox.setInformativeText("Please edit the binary location and try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        binLocations();
        return;
    }

    // Get the (list of) output file names.
    QStringList outputFiles;
    QStringList keys = model_->data(QModelIndex(),
            oskar_SettingsModel::OutputKeysRole).toStringList();
    for (int i = 0; i < keys.size(); ++i)
    {
        const oskar_SettingsItem* it = model_->getItem(keys[i]);
        QString file = it->value().toString();
        outputFiles.append(file);
    }

    // Run simulation recursively.
    oskar_RunDialog dialog(model_, this);
    dialog.start(run_binary_, settingsFile_, outputFiles);
    dialog.exec();

    // Restore the output files.
    for (int i = 0; i < keys.size(); ++i)
    {
        model_->setValue(keys[i], outputFiles[i]);
    }
}

void oskar_MainWindow::createRecentFileActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFiles_[i] = new QAction(this);
        recentFiles_[i]->setVisible(false);
        connect(recentFiles_[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
    separator_ = menuFile_->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        menuFile_->addAction(recentFiles_[i]);
    }
}


void oskar_MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recent_files/files").toStringList();

    int num_files = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < num_files; ++i)
    {
        QFileInfo info(files[i]);
        QString txt = QString("&%1 %2  [%3]").arg(i+1).arg(info.fileName()).
                arg(info.absolutePath());
        recentFiles_[i]->setText(txt);
        recentFiles_[i]->setData(files[i]);
        recentFiles_[i]->setVisible(true);
    }
    for (int i = num_files; i < MaxRecentFiles; ++i)
    {
        recentFiles_[i]->setVisible(false);
    }
    separator_->setVisible(num_files > 0);
}


void oskar_MainWindow::updateRecentFileList()
{
    QSettings settings;
    QStringList files = settings.value("recent_files/files").toStringList();
    QFileInfo file(settingsFile_);
    files.removeAll(file.absoluteFilePath());
    files.prepend(file.absoluteFilePath());
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recent_files/files", files);

    updateRecentFileActions();
}

