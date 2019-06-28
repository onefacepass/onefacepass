#include "form/quiwidget.h"
#include "form/quicreator.h"
#include "demo/appinit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));
    a.setWindowIcon(QIcon(":/main.ico"));

    QUIWidget::setCode();
    QUIWidget::setTranslator(":/image/qt_zh_CN.qm");
    QUIWidget::setTranslator(":/image/widgets.qm");
    AppInit::Instance()->start();

    QUIWidget qui;
    QUICreator *creator = new QUICreator;

    //设置主窗体
    qui.setMainWidget(creator);

    //设置标题
    qui.setTitle("一脸通 演示版");

    //设置标题文本居中
    qui.setAlignment(Qt::AlignCenter);

    //设置窗体可拖动大小
    qui.setSizeGripEnabled(true);

    //设置换肤下拉菜单可见
    qui.setVisible(QUIWidget::BtnMenu, true);


    //设置左上角图标-图形字体
//    qui.setIconMain(QChar(0xf099), 11);

    //设置左上角图标-图片文件
//    qui.setPixmap(QUIWidget::Lab_Ico, ":/logo.jpg");

    qui.show();

    return a.exec();
}
