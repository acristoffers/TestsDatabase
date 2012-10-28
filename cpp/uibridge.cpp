/* (c) 2012 Álan Crístoffer */

#include "uibridge.h"

UIBridge* UIBridge::_self = NULL;
QWebFrame* UIBridge::_frame = NULL;

UIBridge* UIBridge::instance(QWebFrame *frame)
{
    if (!_self)
        _self = new UIBridge;

    if (frame != 0)
        _frame = frame;

    return _self;
}

UIBridge::UIBridge(QObject *parent) :
    QObject(parent)
{
    current_lang = 0;
    _db = 0;
    //_db = new DataBase( QDir::homePath() + "/db.qdb");
}

void UIBridge::show_error(QString title, QString message)
{
    QMessageBox::critical(0, title, message);
}

QString UIBridge::get_languages()
{
    QString translations_folder = resolved_path("translations");

    QMap<QString,QString> langs;
    QDirIterator qmIt(translations_folder, QStringList() << "*.qm", QDir::Files);
    while(qmIt.hasNext()) {
        qmIt.next();
        QFileInfo finfo = qmIt.fileInfo();
        QTranslator translator;
        translator.load(finfo.baseName(), translations_folder);
        langs.insert(finfo.baseName(), translator.translate("UIBridge", "__LANGUAGE__"));
    }

    QString json = "{";

    QMapIterator<QString,QString> i(langs);
    while (i.hasNext()) {
        i.next();
        if (json != "{") json += ",";
        json += "\"" + i.value() + "\":\"" + i.key() + "\"";
    }

    json += "}";

    return json;
}

void UIBridge::open_db(QString path)
{
    if ( _db ) {
        _db->close();
        _db->deleteLater();
    }
    _db = new DataBase(path);
}

void UIBridge::choose_and_open_db()
{
    QString filename = QFileDialog::getOpenFileName(0, tr("Open file"), QDir::homePath(), "*.qdb");
    if( filename.isEmpty() ) return;

    open_db(filename);
    checkDB();
}

void UIBridge::create_and_open_db()
{
    QString filename = QFileDialog::getSaveFileName(0, tr("Create new file"), QDir::homePath(), "*.qdb");
    if( filename.isEmpty() ) return;

    if ( _db ) _db->deleteLater();

    if( QFileInfo(filename).exists() ) {
        QFile f(filename);
        f.open(QFile::ReadWrite);
        f.remove();
    }

    open_db(filename);
    checkDB();
}

void UIBridge::category_new(QString name, int parent)
{
    if( !name.isEmpty() )
        _db->category_add(name, parent);
}

QStringList UIBridge::choose_and_encode_image()
{
    QString formats = "(";
    foreach(QString f, QImageReader::supportedImageFormats())
        formats += " *." + f;
    formats += ")";

    QStringList filenames = QFileDialog::getOpenFileNames(0, tr("Select images"), QDir::homePath(), tr("Image Files") + formats);

    QStringList base64;
    foreach(QString filename, filenames) {
        QImage image(filename);

        if( image.size().width() > 400 )
            image = image.scaledToWidth(400);

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG", 100);
        base64 << byteArray.toBase64();
    }
    return base64;
}

void UIBridge::set_language(QString lang)
{
    if ( current_lang ) {
        QCoreApplication::removeTranslator(current_lang);
        current_lang->deleteLater();
    }

    current_lang = new QTranslator();
    current_lang->load(lang, resolved_path("translations"));

    QCoreApplication::installTranslator(current_lang);

    _frame->page()->view()->setWindowTitle(tr("Test Database"));
}

void UIBridge::print(QString type, QString html)
{
    QString filename;
    if ( type == "pdf" ) {
        filename = QFileDialog::getSaveFileName(0, tr("Print to file"), QDir::homePath(), "*.pdf");
    } else if ( type == "html" ) {
        filename = QFileDialog::getSaveFileName(0, tr("Print to file"), QDir::homePath(), "*.html");
    }

    if( filename.isEmpty() ) return;

    if( QFileInfo(filename).exists() ) {
        QFile f(filename);
        f.open(QFile::ReadWrite);
        f.remove();
    }

    html.prepend("<html><head><style type='text/css'>.page-break{page-break-after:always}body{font-family:Arial!important;font-size:12px!important}ol{font-weight:bold}li{font-weight:normal}.print-question{page-break-inside:avoid}.print-question-body{margin:12pt}.print-header{margin-bottom:24pt}.print-hide{display:none}</style></head><body>");
    html.append("</body></html>");

    if ( type == "pdf" ) {
        QWebView view;
        view.page()->mainFrame()->setHtml(html);

        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);

        printer.setOutputFileName(filename);

        view.page()->mainFrame()->print(&printer);
    } else if ( type == "html" ) {
        QFile f(filename);
        f.open(QFile::WriteOnly);
        QTextStream out(&f);
        out << html;
    }
}

// don't need to be called, just to be present.
void UIBridge::htmlTranslations()
{
    // Used to get the language this translation file provides.
    tr("__LANGUAGE__");

    // in JS
    tr("Categories");
    tr("Questions");
    tr("Root");
    tr("Reference: ");
    tr("Difficulty: ");
    tr("delete");

    // in HTML
    tr("Test Database");
    tr("About");
    tr("About Qt");
    tr("File");
    tr("New");
    tr("Open");
    tr("Show");
    tr("Categories/Questions");
    tr("Tests");
    tr("Settings");
    tr("Language");
    tr("");
    tr("New Question");
    tr("New Test");
    tr("Edit Category");
    tr("Name");
    tr("Save");
    tr("Merge");
    tr("Delete");
    tr("Add Question");
    tr("Edit Question");
    tr("Title");
    tr("Dificulty");
    tr("Reference");
    tr("Answers");
    tr("Edit");
    tr("Create Test");
    tr("Number of questions");
    tr("Page header");
    tr("Categories");
    tr("Generate");
    tr("Attention!");
    tr("Are you sure you want to merge categories?");
    tr("The selected category will be deleted and it's tree moved to the category selected as parent.");
    tr("This operation can't be undone.");
    tr("Cancel");
    tr("Are you sure you want to delete the selected category and all its subitems?");
    tr("If you want to delete the category and keep its subitems use merge instead.");
    tr("Add category");
    tr("Name:");
    tr("No right answer selected");
    tr("Looks like you forgot to select the right answer...");
    tr("Ok");
    tr("Delete question");
    tr("Are you sure you want to delete this question?");
    tr("Create a database with questions and easily print tests.");
    tr("Close");
    tr("No title selected");
    tr("You need to choose a title first.");
}
