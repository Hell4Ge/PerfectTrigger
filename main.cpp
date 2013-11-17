#include <QCoreApplication>
#include <QtCore>
#include <QtXml>

void printData();
QDomElement createStat(qint8 K, qint8 D, qint8 A);
QDomElement LoadContent();
bool AddStats(qint8 K, qint8 D, qint8 A);
bool makeFile();

QDomDocument doc;
QFile file("Data.xml");

class Writer
{
public:
    bool AddStats(qint8 K, qint8 D, qint8 A)
    {
        QDomElement content = LoadContent();

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if(content.isElement())
                content.appendChild(createStat(K,D,A));
            else
                qDebug() << "The root wasn't found";

            qDebug() << doc.toString();

            QTextStream stream(&file);
            stream << doc.toString();

            file.close();

            return true;
        }

        return false;
    }

private:
    QDomElement createStat(qint8 K, qint8 D, qint8 A)
    {
        QDomElement stat = doc.createElement("Entry");
        stat.setAttribute("Date", QDateTime::currentDateTime().toString());
        stat.setAttribute("K", QString::number(K));
        stat.setAttribute("D", QString::number(D));
        stat.setAttribute("A", QString::number(A));
        return stat;
    }


    QDomElement LoadContent()
    {
        makeFile();
        QDomElement n;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            doc.setContent(&file);
            n = doc.elementsByTagName("Root").at(0).toElement();
            file.close();
        }

        return n;
    }

    bool makeFile()
    {
        if(!file.exists() || file.size() == 0)
        {
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QDomElement root = doc.createElement("Root");
                doc.appendChild(root);

                QTextStream stream(&file);
                stream << doc.toString();

                file.close();
                return true;
            }
            else
                return false;
        }

        return true;
    }

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Writer wr;
    wr.AddStats(6,7,8);

    //makeFile();
    //AddStats(1,2,3);

    return 0;
}
