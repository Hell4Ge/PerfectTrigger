#include <QCoreApplication>
#include <QtCore>
#include <QtXml>
#include <QVector>

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

class Reader
{
    qint32 games = Games(); // c++11
public:
    QVector<qint32> getStat(QString s)
    {
        QVector<qint32> v;

        QDomElement root = LoadContent();
        QDomNodeList items = root.elementsByTagName("Entry");

        if(items.count() > 0)
        {
            for(int i=0; i<items.count(); i++)
            {
                QDomNode itemnode = items.at(i);

                if(itemnode.isElement())
                {
                    QDomElement itemele = itemnode.toElement();
                    if(s=="K" || s=="k")
                        v.append(itemele.attribute("K").toInt());

                    if(s=="D" || s=="d")
                        v.append(itemele.attribute("D").toInt());

                    if(s=="A" || s=="a")
                        v.append(itemele.attribute("A").toInt());
                }
            }
        }
        else
            qDebug() << "There are no any elements";

        return v;
    }

    float getKDR(qint32 gameid)
    {
        if(games>0)
        {
            QVector<qint32> K = getStat("K");
            QVector<qint32> D = getStat("D");

            return (float)K.at(gameid-1) / (float)D.at(gameid-1);
        }
        else
            return 0.f;
    }

    float getAVGKDR()
    {
        float sum =0.f;

        if(games>0)
        {
            for(qint32 i=1; i<games; i++)
                sum += getKDR(i);

            return sum / (float)games;
        }
        else
            return 0;
    }

    float getMinKDR()
    {
        if(games>0)
        {
            float min = getKDR(1);
            for(qint32 i=2; i<games; i++)
            {
                if(getKDR(i)<min)
                    min=getKDR(i);
            }

            return min;
        }
        return 0.f;
    }

    float getMaxKDR()
    {
        if(games>0)
        {
            float max = getKDR(1);
            for(qint32 i=2; i<games; i++)
            {
                if(getKDR(i)<max)
                    max=getKDR(i);
            }

            return max;
        }
        return 0.f;
    }

    float getDF(qint32 gameid)
    {
        if(games>0)
        {
            qint8 KillMultipler = 2;
            qint8 DeathMultipler = -3;
            qint8 AssistMultipler = 1;

            float DF = 0.f;

            QVector<qint32> K = getStat("K");
            QVector<qint32> D = getStat("D");
            QVector<qint32> A = getStat("A");

            if(K.count() > 0)
                DF = K.at(gameid-1) * KillMultipler + D.at(gameid-1) * DeathMultipler + A.at(gameid-1) * AssistMultipler;

            return DF;
        }

        return 0.f;
    }

    float getAVGDF()
    {
        if(games>0)
        {
            float avg=0.f;
            for(qint32 i=1; i< games; i++)
                avg += getDF(i);

            return avg / (float)games;
        }

        return 0.f;
    }

    qint8 getMaxDF()
    {
        qint8 max=0;
        qint32 games = Games();

        if(games>0)
        {
            for(qint32 i=1; i< games; i++)
            {
                if(getDF(i) > max)
                    max = getDF(i);
            }
        }
        else
            return 0;

        return max;
    }

    qint8 getMinDF()
    {
        if(games > 0)
        {
            qint8 min=getDF(1);

            for(qint32 i=2; i< games; i++)
            {
                if(getDF(i) < min)
                    min = getDF(i);
            }

            return min;
        }

        return 0;
    }

    float getKDA(qint32 gameid)
    {
        if(games>0)
        {
            if(gameid > games)
                return 0.f;

            QVector<qint32> K = getStat("K");
            QVector<qint32> D = getStat("D");
            QVector<qint32> A = getStat("A");

            return ((float)K.at(gameid-1) + (float)A.at(gameid-1)) / (float)D.at(gameid-1);
        }

        return 0.f;
    }

    float getAVGKDA()
    {
        if(games>0)
        {
            float sum =0.f;

            for(qint32 i=1; i<games; i++)
                sum += getKDA(i);

            return sum / (float)games;
        }

        return 0.f;
    }

    float getMaxKDA()
    {
        if(games>0)
        {
            qint8 max=0;
            qint32 games = Games();

            for(qint32 i=1; i< games; i++)
            {
                if(getKDA(i) > max)
                    max = getKDA(i);
            }

            return max;
        }

        return 0.f;
    }

    float getMinKDA()
    {
        qint8 min=getKDA(1);
        qint32 games = Games();

        for(qint32 i=2; i< games; i++)
        {
            if(getDF(i) < min)
                min = getKDA(i);
        }

        return min;
    }

private:
    qint32 Games()
    {
        QVector<qint32> K = getStat("K");
        return K.count();
    }

    QDomElement LoadContent()
    {
        QDomElement n;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            doc.setContent(&file);
            n = doc.elementsByTagName("Root").at(0).toElement();
            file.close();
        }

        return n;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Writer wr;
    Reader rd;

    qDebug() << QString::number(rd.getAVGKDR());
    qDebug() << QString::number(rd.getKDR(3));
    qDebug() << QString::number(rd.getMinKDR());
    qDebug() << QString::number(rd.getMaxKDR());
    qDebug() << QString::number(rd.getDF(1));
    qDebug() << QString::number(rd.getAVGDF());
    qDebug() << QString::number(rd.getMinDF());
    qDebug() << QString::number(rd.getMaxDF());
    qDebug() << QString::number(rd.getKDA(1));
    qDebug() << QString::number(rd.getAVGKDA());

    return 0;
}
