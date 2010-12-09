#include "bbpreferences.h"

#include "bbsettings.h"
#include "bbdebug.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

BBPreferences::BBPreferences()
{
    BBDEBUG;

    resize(640, 0);
    setWindowTitle(QString(BBPACKAGE " - %1").arg("Preferences"));

    QVBoxLayout *box = new QVBoxLayout();
    setLayout(box);

    {
        QHBoxLayout *layout = new QHBoxLayout();
        box->addLayout(layout);

        {
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(QPixmap::fromImage(QImage(":images/icon.png")));
            layout->addWidget(label);
        }

        {
            QLabel *label = new QLabel(tr("Preferences"));
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label);

            QFont font;
            font.setBold(true);
            font.setPointSize(font.pointSize() * 2);
            label->setFont(font);
        }

        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    }

    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Panel);
    box->addWidget(frame);

    QGridLayout *layout = new QGridLayout();
    frame->setLayout(layout);

    int row(0);

    {
        QLabel *label = new QLabel(tr("Directory:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_directoryWidget = new QLineEdit();
        m_directoryWidget->setText(BBSettings::instance()->directory());
        box->addWidget(m_directoryWidget);

        QPushButton *button = new QPushButton(tr("Browse"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onDirectorySearchClicked()));
    }

    row++;
    {
        QLabel *label = new QLabel(tr("Subversion:"));
        layout->addWidget(label, row, 0);

        QHBoxLayout *box = new QHBoxLayout();
        layout->addLayout(box, row, 1);

        m_svnWidget = new QLineEdit();
        m_svnWidget->setText(BBSettings::instance()->svn());
        box->addWidget(m_svnWidget);

        QPushButton *button = new QPushButton(tr("Browser"));
        box->addWidget(button);
        connect(button,
                SIGNAL(clicked()),
                SLOT(onSVNSearchClicked()));
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    box->addLayout(buttonLayout);

    buttonLayout->addWidget(new QWidget(), 1, 0);

    {
        QPushButton *button = new QPushButton(tr("&Cancel"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(reject()));
    }

    {
        QPushButton *button = new QPushButton(tr("&Save"));
        buttonLayout->addWidget(button, 0, 0);
        connect(button, SIGNAL(clicked()), SLOT(save()));
    }

    setFocus();
}

BBPreferences::~BBPreferences()
{
    BBDEBUG;
}

void BBPreferences::save()
{
    BBDEBUG;

    if (m_svnWidget->text().isEmpty() ||
        m_directoryWidget->text().isEmpty()) {
        QMessageBox::warning(this,
                             QString(BBPACKAGE " - %1").arg("Preferences"),
                             tr("Please, all the fields are required."));
        return;
    }

    BBSettings::instance()->setSvn(m_svnWidget->text());
    BBSettings::instance()->setDirectory(m_directoryWidget->text());

    accept();
}

void BBPreferences::onSVNSearchClicked()
{
    BBDEBUG;

    if (m_svnDialog.isNull()) {
        m_svnDialog = new QFileDialog(this);
        m_svnDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_svnDialog->setFileMode(QFileDialog::ExistingFile);
        m_svnDialog->setWindowTitle(tr("Select the 'subversion' tool."));
        m_svnDialog->setModal(true);

        connect(m_svnDialog,
                SIGNAL(fileSelected(QString)),
                m_svnWidget,
                SLOT(setText(QString)));
    }

    m_svnDialog->show();
}

void BBPreferences::onDirectorySearchClicked()
{
    BBDEBUG;

    if (m_directoryDialog.isNull()) {
        m_directoryDialog = new QFileDialog(this);
        m_directoryDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_directoryDialog->setFileMode(QFileDialog::Directory);
        m_directoryDialog->setWindowTitle(tr("Select the subversion directory."));
        m_directoryDialog->setModal(true);

        connect(m_directoryDialog,
                SIGNAL(fileSelected(QString)),
                m_directoryWidget,
                SLOT(setText(QString)));
    }

    m_directoryDialog->show();
}
